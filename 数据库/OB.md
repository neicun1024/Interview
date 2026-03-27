# 这里存放了OceanBasea相关的项目/知识点

## delete_insert

### 介绍
oceanbase用的LSMTree架构，采用追加写的方式，所以更新的速度很快，但是分层的设计会降低读性能，因为一个主键可能在每层都会出现，要拿到一个完整的行可能需要从每层拿到对应的行然后拼接起来，这就导致了很多微块无法走batch路径，尤其是列存基线数据，走单行和走batch的性能差别很大，并且无法使用下压filter，所以我们设计了delete_insert表模式，将update拆成delete+insert，并且delete会记全行，这样就能在执行归并之前就对微块进行过滤，并且能根据delete行去跳基线，提高batch能力。

### 痛点
1. 点查路径可能需要读多层sstable后fuse；
2. 需要fuse出完整的行后调用sql层的filter，过程中存在大量无用的投影，需要深拷贝；
3. 基线微块中只要有一行有主键交叉，整个微块都无法走batch路径，也无法用skip index；

### 解决
1. delete_insert表模式下，点查读到insert行后可直接返回；
2. 微块没有nop，可下推谓词，且只需浅拷贝；
3. 基线不参与败者树，根据增量fuse后得到的delete行确认基线扫描边界，之前的行都可以走batch路径；

### delete_insert在update时需要先查询，是否性能会变很差

partial_update表模式在update时也需要读旧行，因为需要在clog日志中写前像，前像主要用于：CDC（变更数据捕获需要构造完整的before/after change event，类似mysql binlog row格式）、唯一索引维护（update涉及唯一索引列时，备副本回放需要前像来定位并删除旧索引条目）、数据校验（回放时可用前像做一致性校验）。

### delete_insert性能测试

内部测试，使用48c128g规格，一亿条数据在基线中，并在不同比例的随机update下，对比partial-update表模式和delete_insert表模式的查询性能，单表table scan差不多能提升一个数量级，基线使用skip index可提升2～3个数量级。影响提升大小的主要因素是需要跳基线的次数，以及增量越多提升越大；
线上复杂查询以及物化视图之类的导致性能提升没这么大，可以根据table scan的比例去差不多计算能提升多少，线上某个业务差不多能提升90%；

### delete_insert的优化点

1. 微块按行数切分RowGroup，各个CG的微块按相同行数对齐，查询时能按批推进下压谓词的计算和投影过程；
2. 通过delete bitmap让基线不用频繁跳过delete行，必须先实现RowGroup；

## 增量skip index介绍

在增量数据的微块上生成skip inde聚合信息，主要是min_max，null_count等，查询时如果有相关的filter就可以直接利用skip index来跳过整个微块的IO，然后由于skip index聚合信息长度是受限的，所以设计了一个根据列类型自适应生成skip index的策略。
此外，如果微块中存在跨微块多版本行，使用skip index可能会有正确性问题，这种微块无法使用skip index跳过，所以另外还优化了写微块逻辑，来减少跨微块多版本行。

## filter排序

每个filter的过滤率不同，将过滤率高的filter放在前面执行，后面的filter可能就不需要执行了，从而减少查询耗时。主要难点是filter排序以及收集过滤代价本身也有开销，比如本来后面的filter不需要执行了，但是为了收集每个filter的过滤代价，需要额外执行后面的filter，如何平衡重排序带来的收益和开销就是一个难点了。

## LSM-Tree

相比于B+树的原地更新，LSM-Tree采用追加写，有更快的写入速度，并且采用分层的结构，memtable的数据量达到阈值后会转储到L0层，L0层达到阈值后会再合并到下一层。RocksDB的memtable采用跳表，sstable采用有序的键值对，通过sequence number实现多版本，而oceanbase的memtable采用B+树和版本链，sstable的数据也能实现MVCC；

## 为什么含有跨微块多版本行的微块不能通过skip index过滤

因为当时遇到过一个副本迁移相关的BUG。副本迁移会进行迁移和回放（迁移是将位点前的sstable直接复制过来，回放是将位点后的clog回放，写到memtable后转储），由于位点可能不准，可能会导致多版本行在minor和mini中重复（minor中直接迁移复制过来，mini中的数据由clog回放到memtable后转储生成），比如mini中是insert1->delete1->insert2->delete2，minor中是insert2->delete2（insert2->delete2重复了）。此外，在delete_insert表模式在败者树逻辑中，每个微块中相同rowkey的多版本链，只会输出一行，并对多个迭代器的该rowkey进行fuse。在BUG发生时，代码中还没有is_insert_filtered_和is_insert_filtered_标记。如果filter能过滤insert1和delete2，不能过滤insert2，那么mini不会吐出任何行，minor会吐出insert2，导致fuse结果会吐出insert2，发生正确性问题。如果filter能过滤insert1，不能过滤delete2和insert2，那么mini会吐出delete2，minor会吐出insert2和delete2，但由于insert2和delete2提交版本相同，所以折叠后仍然会吐出insert2，导致正确性问题。正因为担心有类似的隐患，所以选择了最稳妥的方式，跳过这些微块。并且在优化了写微块逻辑，尽量减少跨微块多版本行。

## 最大的困难和挑战

delete_insert 是一个长链路的内核改动，涉及DML 写入、转储/合并以及查询路径。难点不仅是把功能做出来，更在于跨模块语义对齐，以及压测中疑难bug的定位。比如如何正确折叠多版本行，涉及到MVCC，比如修改败者树归并逻辑，根据delete行确定基线batch扫描边界，比如refresh table逻辑改动等等。然后bug排查上，我遇到过一个比较棘手的4377问题：在执行update时会先查询要更新的行，拿到rowkey后会用点查的方式校验一下和刚刚拿到的行是否一致，不一致就会报错4377。这个问题里，update扫描阶段读出来的行是insert行，而点查查出来的行是update行。我先从日志中拿到rowkey，然后dump出这个表对应的clog日志，根据这个rowkey找到了一条insert和一条update，然后进一步dump出宏块数据，确定insert行在基线中，update行在增量中，推测可能是增量已经读完了，基线还在读。再找4377之前是否有相关日志，发现这个查询是多range扫描场景，在扫某个range的时候，增量已经扫完了但是基线越过了这个range，导致update扫描只看到了基线的insert。最后检查代码确认是基线的range设置有问题，修复后还补充了mysqltest。

## 高光时刻

优化SQL监控视图：当时这个feature没人做，我就主动要了。一方面是我觉得这个feature很有价值，能提高诊断能力，另一方面是在修复和增加统计信息的过程中，能帮助学习查询链路中的很多模块，所以做了这个feature。过程中还发现了一个大bug，是关于skip index相关的，聚合信息中的string前缀的比较逻辑有问题，有个大于小于符号写错了，然后告诉了代码owner。第二天leader跟我说，你怎么回事，好像发现了一个大BUG。然后最后这个feature做完之后我还画了张查询流程图，标注了每个统计信息的收集位置，这个图也是帮助到了很多非内核同学。

## MVCC：（以RR可重复读隔离级别为例）

MySQL/InnoDB 在事务内第一次一致性读时创建 ReadView，它包含活跃事务集合信息（min/max + 活跃事务列表等），读时拿行版本上的事务ID与 ReadView 规则判断可见性，不可见则沿 undo 找更老版本。
OceanBase 在事务内第一次需要一致性读时获取一个 snapshot_version（读 SCN/读版本号），它来自事务层维护的可读提交版本水位线；读时主要用 commit_version <= snapshot_version 做可见性判断，必要时结合事务提交状态判断。


## OB列存副本（Columnstore Replica，简称 C 副本）

列存副本是OB中一种可选的、专用的只读型副本类型，其核心特征是：在同一个日志流上，用户表的基线数据以列式存储格式持久化，从而显著优化OLAP性能。不参与 Paxos 日志投票和选举，仅支持弱读。

## OB相比mysql在分布式上的优势

OB是原生分布式架构，支持高可扩展性、高可用性和高性能，mysql依赖中间件来实现分布式。

## OB的memtable

包含BTree和HashTable，存放的都是rowkey到ObMvccRow到映射。ObMvccRow是多版本链表。在执行insert时，先创建新的ObMvccRow链表，并在Hash表中插入rowkey到ObMvccRow的映射，并在BTree中插入新节点；在执行update/delete时，先根据Hash表找到ObMvccRow，然后在链表头插入新节点，BTree无需修改。

## OB的DML完整写入流程

分为4个阶段，先写Memtable，再注册Callback，然后异步生成Redo Log并提交到clog，最后clog持久化后会回调确认。事务还没提交时就可以写memtable以及将redo落盘，因为事务提交时会写一条TX_COMMIT_LOG，没有这条记录，之前的redo都会丢弃。提前把redo落盘可以减少提交延迟，释放Memtable内存压力，支持转储。
阶段一：Memtable写入（DML执行线程，同步）：首先在Hash表中定位ObMvccRow，然后在ObMvccRow到链表头插入新的事务节点，最后确保BTree中有对应rowkey；
阶段二：注册Callback（DML执行线程，同步）：创建ObMvccRowCallback并挂入callback_list，ObMvccRowCallback是丽娜姐Memtable数据和clog到桥梁，一旦append_callback成功，该callback随时可能被日志线程访问；
阶段三：Redo Log生成并提交clog：当数据量达到阈值或发生转储或事务提交时，会遍历callback_list，将redo日志写入redo buffer，通过paxos协议同步到多数派副本并落盘；
阶段四：clog持久化回调：clog成功后回调以更新memtable的SCN并标记日志已提交。

## OB如何解决写放大

1.  调大flush粒度，减少小文件数量
2.  合适的compaction触发时机，避免写入稍微多就疯狂触发，也避免长期不合并最终一次大爆发合并；
3.  选择局部或分层合并来减少无效搬运，很多文件基本不需要改动却被全量合并了，对overlap范围小的情况，倾向于做更局部的合并；


## OB如何解决读放大

1.  bloom filter/索引/MVCC减少需要扫描的sstable数量；
2.  合理的compaction策略来减少版本链长度；
3.  缓存；

## OB如何解决空间放大

1.  加速compaction；
2.  垃圾回收，回收不可见版本；
3.  合理的数据压缩和编码；

## 写放大、读放大、空间放大三者之间的关系

写放大主要由 LSM 的合并（compaction）引起；当合并繁忙、出现积压时，旧版本清理不及时，导致空间放大。合并积压会使新旧 SSTable 共存更久、层次更深，从而增加读放大。读放大带来的 CPU/IO 压力会进一步挤占资源，降低合并推进效率，形成恶性循环。

## batch为什么快

1. 能用谓词下推，聚合等操作，可以使用向量化
2. 只需要浅拷不需要深拷

## OB为什么不用StarRocks那种delete bitmap

OB如果要实现StarRocks那种delete bitmap，记clog、合并、序列化、备份恢复、迁移的时候都要考虑，并且delete bitmap需要支持快照度，对当前系统改动非常大。

## delete_insert表模式为什么不能只插入insert，并且记录delete标

因为内部有检查，不允许insert-insert这样的行，可以修改内部逻辑但是没必要；此外，CDC需要还原update行，也需要delete_insert这样的行；

