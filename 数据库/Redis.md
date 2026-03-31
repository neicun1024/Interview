# 一. Redis介绍

## 1.1 引言

>1. 由于用户量增大，请求数量也随之增大，数据压力过大
>2. 多台服务器之间，数据不同步
>3. 多太服务器之间的锁，不存在互斥性

![20220530204310](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220530204310.png)

## 1.2 NoSQL

NoSQL（Not Only SQL）：非关系数据库
1. key-value：Redis。。。
2. 文档型：ElasticSearch，Solr，MongoDB。。。
3. 面向列：Hbase，Cassandra。。。
4. 图形化：Neo4j。。。

除了非关系型数据库都是关系型数据库。

NoSQL只是一种概念，泛指非关系型数据库，和关系型数据库做一个区分

## 1.3 Redis介绍

有一位意大利人，在开发一款LLOOGG的统计页面，因为MySQL的性能不好，就自己研发了一款非关系型数据库，并命名为Redis。

- Redis（Remote Dictionary Server）即远程字典服务
- Redis是由C语言编写的，是一款基于key-value的NoSQL
- 基于内存存储数据的，提供多种持久化机制，性能可以达到110000次/s读取数据以及81000次/s写入数据
- 提供主从，哨兵以及集群的搭建方式，可以更方便地横向扩展和垂直扩展

# 二. Redis安装

## 2.1  安装Redis

1. 采用Docker安装，准备一个YAML文件：
```yml
version: '3'
services:
    redis: 
        image: daocloud.io/library/redis:5.0.7
        restart: always
        container_name: redis
        environment: 
            - TZ=Asia/Shanghai
        ports:
            - 6379:6379
```
2. 创建`docker-compose.yml`文件，并写入上面的配置
3. 执行`docker-compose up -d`，如果出现错误：`docker: Error response from daemon: Ports are not available: listen tcp 0.0.0.0:4449: bind: An attempt was made to access a socket in a way forbidden by its access permissions.`就在命令行输入`netcfg -d`并重启电脑
4. 执行`docker ps`查看当前启动的docker容器

### 2.2 连接Redis

1. 执行`docker exec -it 99 bash`来进入容器，其中`99`表示该容器的标志符，就是`CONTAINER ID`的前两位
2. 使用`redis-cli`连接Redis
3. 可以简单输入两条命令`set name zhangsan`和`get name`，可以看到`'zhangsan'`被存入了Redis数据库
4. 使用图形化界面连接Redis，从[GitHub](https://github.com/lework/RedisDesktopManager-Windows/releases)下载并安装，连接到Redis服务器，可以看到db0中有`'zhangsan'`


# 三. Redis常用命令

## 3.1 Redis存储数据的结构

常用的5种数据结构
- key-string：一个key对应一个值，是最常用的，一般用于存储一个值
- key-hash：一个key对应一个map，存储一个对象数据的
- key-list：一个key对应一个列表，使用list结构实现栈和队列结构
- key-set：一个key对应一个无序的集合，提供交集、差集和并集的操作
- key-zset：一个key对应一个有序的集合，适合排行榜，积分存储等操作

![20220601161158](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220601161158.png)

另外三种数据结构
- HyperLogLog：用于计算近似值
- GEO：用于存储地理位置，即经纬度
- BIT：一般存储的也是一个字符串，存储的是一个byte[]

## 3.2 key-string常用命令



# 知识总结

Redis是一种内存型KV数据库。支持多种数据类型，包括string、list、hash、set、zset等。

## 核心特点
1. 内存存储：延迟低
2. 单线程架构：避免多线程上下文切换和锁竞争，提高效率
3. 支持持久化：通过RDB快照和AOF日志实现数据持久化
4. 丰富的数据结构：支持string、hash、list、set、zset、HyperLogLog（基数统计）、Geo（地理位置）、Bitmaps（位操作）等
5. 高可用和分布式：通过哨兵实现故障转移，通过集群实现水平扩展
6. IO多路复用：使用epoll监听多个客户端连接，高效处理网络IO
7. 高级特性：支持发布订阅、Lua脚本、事务、过期键删除等

## 如何持久化
1. RDB（Redis Database）快照持久化：
默认的持久化方式，通过save指令设置时间间隔，过了时间间隔就会fork子进程，负责将内存中的数据快照序列化到临时RDB文件中，子进程完成后，会用这个临时文件替换旧的RDB文件，完成持久化。
优点：恢复速度快，数据存储紧凑，文件体积小；
缺点：两次快照之间的数据可能会丢失，save命令会导致主进程被阻塞，bgsave命令会导致fork子进程期间会阻塞；
2. AOF（Append Only File）追加文件持久化：
用追加写的方式记录所有对内存进行修改的写操作，先写到AOF缓冲区，然后根据配置决定间隔多久将缓冲区的内容同步到磁盘。
优点：数据安全性高，丢失的数据很少（根据配置），文件内容为日志形式，易于理解和修复；
缺点：文件体积比RDB大，恢复速度相对较慢，每次写入都要追加日志，性能略低于RDB；

## 分布式锁
利用redis可以实现分布式锁，通过SETNX+EXPIRE的方式，set if not exists用于尝试获取锁，用EXPIRE设置过期时间，防止死锁。

## io多路复用
Redis的核心命令执行是由单个主线程处理的，但是可以同时监听和处理多个客户端的连接，linux中使用epoll。

## Redis主从复制
从节点发送slaveof命令连接到主节点，并定期发送ping信号保持通信，主节点将所有数据发送给从节点，保证主从数据一致性。

## Redis哨兵模式
redis可以监控主从节点是否正常运行，并在出问题时通过API脚本通知管理员或其他应用程序，哨兵模式还支持自动故障转移，当主节点不能正常工作时，哨兵会开始一次自动故障转移操作。

## 过期策略
1. 惰性删除：访问键时检查是否过期，若过期则删除
2. 定期删除：定期扫描，删除过期键
3. 主动删除：内存不足时，根据淘汰策略删除键

## 淘汰策略
1. noeviction：默认策略，内存不足时不淘汰数据，也无法写入新数据
2. allkeys-lru / lfu / random：从所有键中使用LRU算法/LFU算法/随机淘汰
3. volatile-lru / lfu / random：从设置了过期时间的键中使用LRU算法/LFU算法/随机淘汰
4. volatile-ttl：从设置了过期时间的键中优先淘汰TTL最小的

## 哈希槽
用于实现redis集群，将16834个槽分配到不同节点，实现数据分配。

## 集群方案
哈希槽+主从复制+哨兵模式（自动故障转移）

## 事务机制
只能保证部分原子性，要么全都执行，要么全都不执行，有命令失败也不会回滚

## 为什么Redis使用哨兵模式而不是Raft
Raft是强一致性复制协议，偏CP，而Redis追求低延时高吞吐，默认允许最终一致性和少量丢写，偏AP。Redis不需要保证多数派提交，不会回滚。
