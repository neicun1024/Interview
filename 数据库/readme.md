# 这里记录了数据库相关的内容

## 事务

### 概念
事务指的是满足 ACID 特性的一组操作，可以通过 Commit 提交一个事务，也可以使用 Rollback 进行回滚

### ACID
- A：原子性（Atomicity）
  - 事务被视为不可分割的最小单元，事务的所有操作要么全部提交成功，要么全部失败回滚。
  - 回滚可以用回滚日志（Undo Log）来实现，回滚日志记录着数据的旧值，在回滚时将数据恢复到旧值。
- C：一致性（Consistency）
  - 数据库在事务执行前后都保持一致性状态。在一致性状态下，所有事务对同一个数据的读取结果都是相同的。
- I：隔离性（Isolation）
  - 一个事务所做的修改在最终提交以前，对其它事务是不可见的。
- D：持久性（Durability）
  - 一旦事务提交，则其所做的修改将会永远保存到数据库中。即使系统发生崩溃，事务执行的结果也不能丢失。
  - 系统发生崩溃可以用重做日志（Redo Log）进行恢复，从而实现持久性。与回滚日志记录数据的逻辑修改不同，重做日志记录的是数据页的物理修改。
  
  ![20220317144945](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220317144945.png)

  - 只有满足一致性，事务的执行结果才是正确的
  - 在无并发的情况下，事务串行执行，隔离性一定能够满足。此时只要能满足原子性，就一定能满足一致性
  - 在并发的情况下，多个事务并行执行，事务不仅要满足原子性，还需要满足隔离性，才能满足一致性
  - 事务满足持久化是为了能应对系统崩溃的情况

## 并发一致性问题和解决方法

| 问题 | 说明 | 举例 | 解决方法 | 封锁协议 | 隔离级别 |
| ---- | ---- | ---- | ---- | ---- | ---- | 
| 丢失修改 | 一个事务的更新操作被另外一个事务的更新操作替换 | T1和T2两个事务都对一个数据进行修改，T1先修改并提交生效，T2随后修改，T2的修改覆盖了T1的修改 | 加写锁（X锁），使得两个写操作不能同时进行（防止了先写，再写） | 一级封锁协议 | 未提交读（事务中的修改，即使没有提交，对其他事务也是可见的）
| 读脏数据 | 在不同的事务下，当前事务可以读到另外事务未提交的数据 | T1修改了一个数据但未提交，T2随后读取这个数据，如果T1撤销了这次修改，那么T2读取的数据是脏数据 | 加读锁（S锁），使得读写操作不能同时进行（防止了先写，再读） | 二级封锁协议 | 提交读（一个事务所做的修改在提交之前对其它事务是不可见的）
| 不可重复读 | 一个事务内多次读取同一数据集合，在这一事务还未结束前，另一事务也访问了该同一数据集合并做了修改，由于第二个事务的修改，第一个事务的两次读取的数据可能不一致 | T2读取一个数据，T1对该数据做了修改，如果T2再次读取这个数据，此时读取的结果和第一次读取的结果不同 | 在二级封锁协议的基础上，读操作结束后不立刻释放读锁，直到事务结束再释放（防止了先读、再写、再读） | 三级封锁协议 | 可重复读（保证在同一个事务中多次读取一个数据的结果是一样的）
| 幻影读 | 本质也属于不可重复读的情况 | T1读取某个范围的数据，T2在这个范围内插入新的数据，T1再次读取这个范围的数据，此时读取的结果和第一次读取的结果不同 | 加间隙锁（锁定索引之间的间隙）| / | /


## 写锁（X锁）和读锁（S锁）

- 互斥锁（Exclusive），简写为X锁，又称写锁
- 共享锁（Shared），简写为S锁，又称读锁

![20220317142038](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220317142038.png)

- 一个事务对数据对象加了X锁，就可以对其进行读取和更新，加锁期间其它事务不能对该对象加任何锁
- 一个事务对数据对象加了S锁，就可以对其进行读取操作，但是不能进行更新操作，加锁期间其它事务能对该对象加S锁，但不能加X锁


## 行级锁、表级锁、意向锁

- 对表中的某一行（某一条记录）加锁，就是行级锁
- 对表中的每一行加锁，就是表级锁
- 当事务想对表加表级锁时，需要先检测表的每一行是否已经有行级锁，如果都没有行级锁，才能加上表级锁，这个检测过程是非常耗时的，意向锁就是用于解决这个问题
- 当表中的任意一行加了行级锁，这个表就加了意向锁，那么当事务想对表加表级锁时，只需要检查是否存在意向锁，节省了很多检测时间

![20220317142458](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220317142458.png)

- 意向锁分为IX锁和IS锁，分别代表写和读
- 一个事务在获得某个数据行对象的 S 锁之前，必须先获得表的 IS 锁或者更强的锁
- 一个事务在获得某个数据行对象的 X 锁之前，必须先获得表的 IX 锁
- 任意 IS/IX 锁之间都是兼容的
- IS/IX 锁和行级的 S/X 锁兼容，和表级的 S/X 锁不兼容


## 两段锁协议

- 加锁和解锁分为两个阶段进行
- 串行调度：在某个调度中，一个事务的所有调度都结束后，才进行另一个事务的调度。
- 可串行化调度：通过并发控制，使得并发执行的事务结果与某个串行执行的事务结果相同。串行执行的事务互不干扰，不会出现并发一致性问题。
- 冲突可串行化调度：如果某个调度可以通过交换非冲突操作后转换为串行调度，那么这个调度就是冲突可串行化调度。[参考](https://geek-docs.com/dbms/dbms-tutorial/dbms-conflict-serializability.html)。
- 事务遵循两段锁协议是保证可串行化调度的充分条件。例如以下操作满足两段锁协议，它是可串行化调度。
    ```
    lock-x(A)...lock-s(B)...lock-s(C)...unlock(A)...unlock(C)...unlock(B)
    ```
    但不是必要条件，例如以下操作不满足两段锁协议，但它还是可串行化调度。
    ```
    lock-x(A)...unlock(A)...lock-s(B)...unlock(B)...lock-s(C)...unlock(C)
    ```


## 隔离级别：可串行化

- 在上面的《并发一致性问题和解决方法》表格中，提到了三种隔离级别，而可串行化是一种更严格的隔离级别
- 可串行化：强制事务串行执行，这样多个事务互不干扰，不会出现并发一致性问题
- 该隔离级别需要加锁实现，因为要使用加锁机制保证同一时间只有一个事务执行，也就是保证事务串行执行

![20220317143952](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220317143952.png)


## MVCC（多版本并发控制）

- 多版本并发控制（Multi-Version Concurrency Control, MVCC）是 MySQL 的 InnoDB 存储引擎实现隔离级别的一种具体方式
- 用于实现提交读和可重复读这两种隔离级别
- 未提交读隔离级别总是读取最新的数据行，要求很低，无需使用 MVCC
- 可串行化隔离级别需要对所有读取的行都加锁，单纯使用 MVCC 无法实现

### 基本思想

- 在封锁一节中提到，加锁能解决多个事务同时执行时出现的并发一致性问题。在实际场景中读操作往往多于写操作，因此又引入了读写锁来避免不必要的加锁操作，例如读和读没有互斥关系。读写锁中读和写操作仍然是互斥的
- MVCC 利用了多版本的思想，**写操作更新最新的版本快照，而读操作去读旧版本快照**，没有互斥关系，这一点和 CopyOnWrite 类似
- 在 MVCC 中事务的修改操作（DELETE、INSERT、UPDATE）会为数据行新增一个版本快照
- 脏读和不可重复读最根本的原因是事务读取到其它事务未提交的修改。在事务进行读取操作时，为了解决脏读和不可重复读问题，MVCC 规定只能读取已经提交的快照。当然一个事务可以读取自身未提交的快照，这不算是脏读

### 版本号

- 系统版本号 SYS_ID：是一个递增的数字，每开始一个新的事务，系统版本号就会自动递增
- 事务版本号 TRX_ID ：事务开始时的系统版本号

### Undo日志

MVCC 的多版本指的是多个版本的快照，快照存储在 Undo 日志中，该日志通过回滚指针 ROLL_PTR 把一个数据行的所有快照连接起来。

例如在 MySQL 创建一个表 t，包含主键 id 和一个字段 x。我们先插入一个数据行，然后对该数据行执行两次更新操作。
```
INSERT INTO t(id, x) VALUES(1, "a");
UPDATE t SET x="b" WHERE id=1;
UPDATE t SET x="c" WHERE id=1;
```
因为没有使用 *START TRANSACTION* 将上面的操作当成一个事务来执行，根据 MySQL 的 AUTOCOMMIT 机制，每个操作都会被当成一个事务来执行，所以上面的操作总共涉及到三个事务。快照中除了记录事务版本号 TRX_ID 和操作之外，还记录了一个 bit 的 DEL 字段，用于标记是否被删除。

![20220317145749](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220317145749.png)

INSERT、UPDATE、DELETE 操作会创建一个日志，并将事务版本号 TRX_ID 写入。DELETE 可以看成是一个特殊的 UPDATE，还会额外将 DEL 字段设置为 1。

### ReadView

MVCC 维护了一个 ReadView 结构，主要包含了当前系统未提交的事务列表 TRX_IDs {TRX_ID_1, TRX_ID_2, ...}，还有该列表的最小值 TRX_ID_MIN 和 TRX_ID_MAX。

![20220317152441](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220317152441.png)

在进行 SELECT 操作时，根据数据行快照的 TRX_ID 与 TRX_ID_MIN 和 TRX_ID_MAX 之间的关系，从而判断数据行快照是否可以使用：
- TRX_ID < TRX_ID_MIN，表示该数据行快照是在当前所有未提交事务之前进行更改的，因此可以使用
- TRX_ID > TRX_ID_MAX，表示该数据行快照是在事务启动之后被更改的，因此不可使用
- TRX_ID_MIN <= TRX_ID <= TRX_ID_MAX，需要根据隔离级别再进行判断：
  - 提交读：如果 TRX_ID 在 TRX_IDs 列表中，表示该数据行快照对应的事务还未提交，则该快照不可使用。否则表示已经提交，可以使用
  - 可重复读：都不可以使用。因为如果可以使用的话，那么其它事务也可以读到这个数据行快照并进行修改，那么当前事务再去读这个数据行得到的值就会发生改变，也就是出现了不可重复读问题

在数据行快照不可使用的情况下，需要沿着 Undo Log 的回滚指针 ROLL_PTR 找到下一个快照，再进行上面的判断。


### 快照读与当前读

1. 快照读
MVCC 的 SELECT 操作是快照中的数据，不需要进行加锁操作。
```
SELECT * FROM table ...;
```

2. 当前读
MVCC 其它会对数据库进行修改的操作（INSERT、UPDATE、DELETE）需要进行加锁操作，从而读取最新的数据。可以看到 MVCC 并不是完全不用加锁，而只是避免了 SELECT 的加锁操作。
```
INSERT;
UPDATE;
DELETE;
```
在进行 SELECT 操作时，可以强制指定进行加锁操作。以下第一个语句需要加 S 锁，第二个需要加 X 锁。
```
SELECT * FROM table WHERE ? lock in share mode;
SELECT * FROM table WHERE ? for update;
```


## Next-Key Locks
Next-Key Locks 是 MySQL 的 InnoDB 存储引擎的一种锁实现。

MVCC 不能解决幻影读问题，Next-Key Locks 就是为了解决这个问题而存在的。在可重复读（REPEATABLE READ）隔离级别下，使用 MVCC + Next-Key Locks 可以解决幻读问题。

### Record Locks
锁定一个记录上的索引，而不是记录本身。

如果表没有设置索引，InnoDB 会自动在主键上创建隐藏的聚簇索引，因此 Record Locks 依然可以使用。

### Gap Locks
锁定索引之间的间隙，但是不包含索引本身。例如当一个事务执行以下语句，其它事务就不能在 t.c 中插入 15。
```
SELECT c FROM t WHERE c BETWEEN 10 and 20 FOR UPDATE;
```

### Next-Key Locks
它是 Record Locks 和 Gap Locks 的结合，不仅锁定一个记录上的索引，也锁定索引之间的间隙。它锁定一个前开后闭区间，例如一个索引包含以下值：10, 11, 13, and 20，那么就需要锁定以下区间：
```
(-∞, 10]
(10, 11]
(11, 13]
(13, 20]
(20, +∞)
```


## [一条SQL语句在MySQL中是如何执行的？](https://z.itpub.net/article/detail/43FB56B82F2178BB326272D693D6858B)


## [MySQL中有哪些日志文件](https://www.jianshu.com/p/edda26a5124f)

### 1. 序言
![20220405104138](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220405104138.png)

上面是MySQL简化版的体系结构图，可以看到自顶向下主要分成了链接池、SQL接口、解析器、查询优化器、缓存、执行引擎、系统文件和日志文件。系统文件和日志文件共同组成了系统文件层，是Mysql实现数据持久化、MVVC、主从同步等功能的重要一层。

Mysql中存在很多不同的日志，例如：错误日志、通用查询日志、二进制日志（Binlog）、Undo/Redo Log、中继日志（Relay Log）、慢查询日志等。

### 2. 错误日志
记录MySQL 运行过程中较为严重的警告和错误信息，以及MySQL每次启动和关闭的详细信息，默认是开启的，可以通过`show variables like '%log_error%'`查看。

![20220405103729](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220405103729.png)

其中log_error是定义是否启用错误日志的功能和错误日志的存储位置，可以修改/etc/my.cnf配置文件，添加参数`log_error=/home/keduw/mysql/mysql.err`指定错误日志的路径。

### 3. 通用查询日志
用来记录用户的所有操作，包括启动和关闭 MySQL 服务、更新语句和查询语句等，默认情况下是关闭，可以通过`show variables like '%general%'`查看。

![20220405104302](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220405104302.png)

一般不开启，会占用大量的磁盘空间。

### 4. 二进制日志
记录了对MySQL数据库执行的更改操作，并且记录了语句的发生时间、执行时长；但是它不记录select、show等不修改数据库的SQL。默认是关闭，可以通过`show variables like '%log_bin%'`查看是否开启，`show binary logs`查看日志文件，`show variables like '%binlog%'`查看参数配置。

![20220405104400](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220405104400.png)

Binlog有两个重要的使用场景：数据库恢复和主从复制，当需要使用对应功能需要开启Binlog日志。可以修改my.cnf或my.ini配置文件，在[mysqld]下面增加log_bin=mysql_bin_log启动。

主从复制的时候，在主库中开启Binlog功能，这样主库就可以把Binlog传递给从库，从库拿到Binlog后实现数据恢复达到主从数据一致性。数据恢复，可以通过mysqlbinlog工具来恢复数据。

### 5. Undo/Redo日志
Undo/Redo Log并不属于Mysql Server的日志，而是属于执行引擎InnoDB的日志。

1. Undo日志
Undo意为撤销或取消，以撤销操作为目的，返回指定某个状态的操作，Undo Log在事务开始前产生。事务在提交时，并不会立刻删除日志，Innodb会将该事务对应的Undo Log放入到删除列表中，通过后台线程进行回收处理。

数据库事务开始之前，会将要修改的记录存放到Undo Log里，当事务回滚时，可以利用Undo Log，撤销未提交的事务。因此依赖Undo Log可以实现事务的原子性和MVCC（多版本并发控制）。

原子性：事务处理过程中，如果出现了错误或者用户执行了ROLLBACK语句，MySQL可以利用Undo Log中的备份将数据恢复到事务开始之前的状态。

MVCC：事务未提交之前，Undo Log保存了未提交之前的版本数据，Undo Log中的数据可作为数据旧版本快照供其他并发事务进行快照读。事务A手动开启事务，执行更新操作，首先会把更新命中的数据备份到 Undo Buffer 中。事务B手动开启事务，执行查询操作，会读取 Undo 日志数据返回，进行快照读。

![20220405105408](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220405105408.png)

1. Redo日志
Redo意为重做，以恢复操作为目的。Redo Log 是为了实现事务的持久性而出现的产物，在事务执行的过程中如果发生异常（比如：数据库崩溃），在重启MySQL服务的时候，根据Redo Log进行重，进而恢复事务的状态。

Undo Log是在事务开启的时候产生，而Redo Log是在事务执行的过程产生。在事务提交时会将产生Redo Log写入Log Buffer，但并不是随着事务的提交就立刻写入磁盘，而是等到事务操作的脏页写入到磁盘之后，Redo Log占用的空间就可以重用（被覆盖写入）。

Redo Log文件内容是以顺序循环的方式写入文件，写满时则回溯到第一个文件，进行覆盖写。

![20220405105457](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220405105457.png)

如上图所示，Redo Log采用双指针进行维护。Write Pos是当前记录的位置，一边写一边后移，写到最后一个文件末尾后就回到0号文件开头；CheckPoint是当前要擦除的位置，也是往后推移并且循环的，擦除记录前要把记录更新到数据文件；

Write Pos和CheckPoint之间还空着的部分，可以用来记录新的操作。如果Write Pos追上CheckPoint，表示写满，这时候不能再执行新的更新，会停下来先擦掉一些记录。如图：W区则是可以写入的区域，R区则是需要刷盘的内容。

### 6. 中继日志
中继日志主要是MySQL主从同步的时候会用到，下图是主从同步的原理图：

![20220405105548](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220405105548.png)

主从同步主要分成三步：
1. 主库将数据库的变更操作记录到Binlog日志文件中
2. 从库读取主库中的Binlog日志文件信息写入到从库的Relay Log中继日志中
3. 从库读取中继日志信息在从库中进行Replay，更新从库数据信息

可以看到Relay Log中继日志在从库中担当类似中转的作用。可能会觉得从库读取主库中的Binlog日志为什么不直接执行，而是先写入到Relay Log后面由读出来。

看似多余，其实不然。其实我觉得这里还是出于性能和异常的考虑，Replay的操作要比直接文件写入慢得多，毕竟中间还要经过执行引擎的处理，而且如果从库出现异常，有Relay Log做持久化也可以确保从库恢复的时候数据的完整性。

### 7. 慢查询日志
记录所有执行时间超时的查询SQL，默认是10秒，方便于查询缓慢的定位和分析。可以通过`show variables like '%slow_query%'`查看是否开启和日志的位置，`show variables like '%long_query_time%'`查看慢查询的阈值。

![20220405105746](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220405105746.png)

## MySQL存储引擎InnoDB


## MySQL存储引擎与Myisam


## InnoDB与Myisam的区别