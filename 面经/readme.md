# 这里记录了我的面试被问到的问题

## 网易C++开发工程师

### 一面

- new和malloc的区别，malloc的内存分配方式
- 智能指针（给了一段代码，看这段代码有什么问题，然后解释每句话的计数值是多少）
- STL中比如vector和map的数据结构和内存申请方式，红黑树为什么快
- 内联函数的优缺点
- 单例是如何实现的
- 解释一下多态，举一下实例
- 线程同步的几种方式
- 选一个项目进行介绍，并讲讲遇到了什么问题，是怎么解决的
- 有没有记录的习惯，比如CSDN、博客，这个习惯带来了怎样的影响

### 二面

- 自我介绍
- 手撕两道代码：反转链表、数组中和最大的连续子串
- 给了三行代码，问两个变量的结果，考察连续赋值、前缀加、后缀加、或（“||”）


## 字节视频架构实习生

### 一面

- 自我介绍
- 用户态和内核态分别是什么，怎么互相转换，有什么指令或操作引起转换
    - 通过系统调用、中断、异常可以由用户态切换到内核态
    - 系统调用：进程控制、进程通信、文件操作、设备操作、信息维护、安全
- 进程的几种状态，如何切换
    - 就绪状态：通过调度算法获得CPU时间，转换为运行状态
    - 运行状态：在分配给进程的CPU时间片用完后会转为就绪状态
    - 阻塞状态：当运行状态时要进行I/O操作会进入阻塞状态
- 造成死锁的原因（必要条件）
    - 互斥、占有和等待、不可抢占、环路等待
- TCP如何实现安全传输，除了三次握手、四次挥手外，有什么机制保证了传输的安全
    - 超时重传、滑动窗口、流量控制、拥塞控制
    - 拥塞控制：慢开始、拥塞避免、快重传、快恢复
- Http报文的结构，状态响应码
    - 请求报文：第一行包含请求方法、URL、协议版本，接下来多行是请求首部Header，每个首部都有一个首部名称以及对应的值，一个空行用来分隔首部和内容主体Body，最后是请求的内容主体
    - 响应报文：第一行包含协议版本、状态码和描述，后面和请求报文一样
- Https的传输机制
    - 加密，认证，完整性保护
- 哈希表，以及如何解决冲突
- 排序中稳定是什么意思，有哪些排序算法是稳定的
- 数据库比如mysql，客户端发起一条指令之后服务器是怎么做的
- 分布式系统的CAP
    - 一致性、可用性、分区容忍性
- 虚函数、虚函数表、虚表指针
- 解释一下内联函数，它和宏定义有什么区别
- 内存分区顺序，各个分区的作用，堆和栈的区别
- 最后手撕一道代码：最长回文子串

### 二面

- 自我介绍，问了项目
- 排序中什么是稳定，如何修改冒泡排序使它变成不稳定的
- TCP和UDP的区别
- TCP的三次握手
- TCP滑动窗口有什么作用
- 如果服务器和带宽的资源无限，TCP的连接数的上限是多少
- 链路层如何根据目标IP地址找到目标的MAC地址
- 给定一个数据库表，根据要求写sql语句，以及看这个表是否符合第三范式，为什么不符合，要如何修改
- 任务的先后顺序用有向图表示，有向图用什么数据结构实现？拓扑排序
- 接收海量数据时如何记录的第k小的数
- 手撕代码：实现sqrt函数，给定target和r，求x使得|x－sqrt(target)|<r
- 部门介绍
  - 部门主要负责维护App的视频投稿的整个生命周期，包括上传、处理、发布、播放等等
  - 面我的这个团队主要负责大规模视频处理平台，维护视频计算集群，包括管理和调度等，以及视频如何存储到某个位置
  - 使用golang语言

### 三面

- 自我介绍
- 对字节跳动的了解
- 开发抖音APP需要注意哪些点
- 诸如此类比较开放性的问题


## 字节AML系统实习生

### 一面

- 自我介绍，问了一下项目
- TCP相关：实现了什么，有什么特点，底层调用了什么
- 手撕代码：写一个LRUCache类，有int capacity表示容量，有int get(int key)，void put(int key, int value)两个函数
- new和malloc和区别
- move()函数的作用，右值的作用
- SIMT（**Single instruction, multiple threads**）有了解吗？不了解。。
- CPU在执行时是执行进程还是线程，如果CPU有两个内核可以同时执行两个线程吗？
- GPU有了解吗？不了解。

### 二面
- 讲一下项目
- 手撕代码：写一个优先队列（这个写了好久。。）
- unique_ptr和shared_ptr有什么区别
- 我想了解一些什么？我问了部门是干什么的，面试官就回答了一两句话，感觉凉了呀。。


## 字节抖音直播-支付业务实习生

### 一面

- 自我介绍，问了项目
- TCP的四次挥手的过程
- TCP如何检测拥塞，如何进行处理
- 手撕代码：给定n个数，求最小的k个数，要求时间复杂度：O(n)，空间复杂度：常数
- 手撕代码：最长公共子序列，输出该最长公共子序列
- 有什么问题要问？


## 腾讯游戏引擎实习生

### 一面

- 个人主要有哪些技术
- 做的项目用的是哪个版本的C++
- 解释一下多态
    - 多态分为编译期多态和运行期多态
    - 编译期多态：通过模板具现化和函数重载解析实现，以不同的模板参数具现化导致调用不同的函数，就是编译期多态
    - 运行期多态：不同继承类的对象，对同一消息作出不同响应，基类的指针指向或绑定到派生类的对象，使得基类指针呈现不同的表现方式。将基类的函数定义为虚函数，在派生类中重写该函数，运行时会根据对象的实际类型来调用相应的函数
- 模板的实例化发生在什么时候？
    - 当首次为每个类型调用函数模板时，编译器会创建一个实例化，每个实例化是专用于该类型的模板化函数版本
    - 在使用类模板时，只有当代码中使用了类模板的一个实例的名字，而去上下文环境要求必须存在类的定义时，这个类模板才被实例化
        - 声明一个类模板的指针和引用，不会引起类模板的实例化，因为没有必要知道该类的定义，当检查这个指针或引用所指的对象时，会引起类模板的实例化
        - 定义类的对象时需要该类的定义，因此类模板会被实例化
        - 在使用sizeof()时，它是计算对象的大小，编译器必须根据类型将其实例化出来
        - new表达式要求类模板被实例化
        - 引用类模板的成员会导致类模板被实例化
- 函数的重载和返回值有关系吗？
    - 没有关系。函数的重载是指同一个可访问区内被声明几个具有不同参数列（参数的类型、个数、顺序）的同名函数，根据参数列表确定调用哪个函数，重载不关心函数返回类型
- 解释一下虚函数、纯虚函数、虚函数表、虚表指针这四个概念
    - 虚函数：
        - 被virtual关键字修饰的成员函数就是虚函数
    - 纯虚函数：
        - 在类中声明虚函数时加上"=0"
        - 含有纯虚函数的类称为抽象类，不能实例化对象，类中只有接口，没有具体的实现方法
        - 继承纯虚函数的派生类，如果没有完全实现基类函数，依然是抽象类
    - 虚函数表和虚表指针：
        - 虚函数通过虚函数表来实现
        - 虚函数的地址保存在虚函数表中，在类的对象所在的内存空间中，保存了指向虚函数表的指针（称为虚表指针），虚表指针指向虚函数表
        - 虚函数表解决了基类和派生类的继承问题和类中成员函数的覆盖问题，当用基类的指针来操作一个派生类的时候，这张虚函数表就指明了实际应该调用的函数
        - 虚函数表在编译阶段建立，程序的编译过程会将虚函数的地址放在虚函数表中
        - 虚函数表和类绑定，虚表指针和对象绑定
- 虚函数表的位置？
    - 虚函数表是全局共享的元素，即全局仅有一个
    - 虚函数表类似一个数组，类对象中存储vptr指针，指向虚函数表，即虚函数表不是函数，不是程序代码，不可能存储在代码段
    - 虚函数表存储虚函数的地址，即虚函数表的元素是指向类成员函数的指针，而类中虚函数的个数在编译期可以确定，即虚函数表的大小可以确定，即大小是在编译时期确定的，不必动态分配内存空间存储虚函数表，所以不在堆中
    - 根据以上特征，虚函数表类似于类中静态成员变量，静态成员变量也是全局共享、大小确定
    - 所以我推测虚函数表和静态成员变量一样存放在全局数据区
- 函数参数存在栈上还是堆上？入栈顺序是从左往右还是从右往左？
    - 栈上，从右往左
    - 每个参数都有自己的地址，但可变长参数无法确认地址，并且函数参数的个数也不确定
    - 如果从左往右传参，栈顶看到的是最后一个参数，由于不知道参数的数量，所以无法确定首参数的地址
- 快速排序是稳定的吗？什么情况下它的复杂度会达到O(n^2)？
    - 不稳定
    - 当数组原来就是有序的时候，复杂度会达到O(n^2)
- 快速排序有什么优化方法？（我只回答了锚点的随机选取）
    - 锚点：随机选取、三数取中
    - 当待排序序列的长度分割到一定大小后，使用插入排序
    - 在一次分割结束后，把与锚点值相等的元素聚在锚点周围，减少递归时数组的长度，从而减少迭代次数
    - 通过尾递归优化来减少堆栈深度
- 当定义了map[0] = “a”，map[1] = ”b”，如果程序直接调用map[2]，会发生什么？程序会直接结束还是警告？
    - value为int类型会返回默认值0，并将 **map[2]=0** 插入
    - value为string类型会返回默认值""，并将 **map[2]=""** 插入
    - value为char类型程序会直接结束，捕获不到异常
- map底层实现？红黑树和平衡搜索树、搜索树的区别？
- 堆和栈的区别？
- 解释一下内存泄漏和野指针？如何避免？如何定义野指针的“指向不确定的地址”？
    - 内存泄漏：动态申请的内存空间没有正常释放，但是也不能继续使用，比如指向这块空间的指针指向了其它地方，导致没有指针指向这块空间，无法访问也无法释放
    - 悬空指针：若指针指向一块内存空间，当这块内存空间被释放后，该指针仍然指向这块内存空间，则称它为悬空指针
    - 野指针：不确定其指向的指针，未初始化的指针，任何指针刚被创建时不会自动成为NULL指针，它的默认值是随机的
    - *无论是野指针还是悬空指针，都是指向无效内存区域的指针（这里的无效指的是不安全不可控）* 见[野(wild)指针和悬空(dangling)指针](https://www.cnblogs.com/idorax/p/6475941.html)
- 程序不断造成内存泄漏，系统最终会报异常吗？
    - 不会报异常，程序会直接崩溃
- shared_ptr中引用计数在什么时候修改？
- 为什么需要有weak_ptr？
    - 为了解决循环引用
- 解释一下虚拟内存和物理内存？
    - 物理内存：真正的内存，比如8G、16G的内存条，由于物理内存是有限的，当有多个进程要执行的时候，都要分配4G的内存，可能不够用，没有分配到资源的进程只能等待
    - 虚拟内存：虚拟内存使用了空分复用技术，它将物理内存抽象为地址空间，每个进程都有各自的地址空间。地址空间的页被映射到物理内存，地址空间的页并不需要全部在物理内存中，当使用到一个没有在物理内存的页时，执行页面置换算法，将该页置换到内存中
- 在windows上运行一个32位程序，系统会给它分配多少内存？其中有多少给用户，多少给系统？
    - 4G内存，windous中2G给用户，2G给系统，linux中3G给用户，1G给系统
- 学过线性代数吗？如何判断一个点P在三角形ABC内？
    - 内角和法：如果PA、PB、PC，两两之间的夹角和为180度，则P在三角形内
    - 同向法：如果P在三角形内，那么P和A相对于直线BC在同侧，P和B相对于直线AC在同侧，线段C相对于直线AB在同侧，用叉乘来判断是否同侧
    - 重心法：向量AP可以表示为x*AB+y*AC，如果x、y、x+y都属于[0,1]，那么P在三角形内
- 有玩什么网游吗？Dota2中你释放了技能，其它玩家是如何感受到的？服务器和玩家的连接是TCP还是UDP？
    - UDP
- TCP的缺点，相比UDP有什么缺点？
    - 慢、效率低、占用系统资源高、易被攻击

### 二面

- 主要问了本科CPU这个比赛，我基本忘了。。
- std::move()的作用，const右值
- 内存对齐
- 二维数组按行遍历和按列遍历的快慢区别？
    - cache
- 一个数组，如何构造一个深度最小的二叉搜索树
- 快排中，当有很多数相同的时候，会造成什么影响，有什么解决方法？


## 阿里云原生AI实习生

### 一面
- 电话闲聊，问的都是比较基础的东西，进程和线程、数组和链表、tcp/ip四层模型等等
- 团队方向：在部门的技术底座之上进行平台的搭建，主要给模型提供平台，替用户搭建服务器的环境，也涉及到优化模型推理方面的工作，语言不仅限于C++、Python，还有Go等，一起建设这个平台，作为实习生去做模型的训练部分或是推理的部分


## 腾讯视频云实习生

### 一面
- 问了项目
- 做了一道编码相关的简单题，题是很简单，面试官问我有什么优化的地方，我从时间复杂度和空间复杂度进行分析，面试官是从代码素养的方向考虑的，比如封装成函数、添加注释等
- 有什么问题要问


## 阿里CRO实习生

### 一面
- 电话面，先聊项目
- 指针和引用有什么区别
- 面向对象的三个特性
- 虚函数是如何实现的
- new和malloc的区别
- STL中vector和list的区别
- 进程之间的通信方式
- 当发生内存泄露时，有什么办法去检测发生内存泄露的位置
- 数据库中索引有什么用，是怎么实现的
- 在网页上输入一个网址然后访问网页，中间发生了什么
- 有什么问题要问
  - 作为实习生主要去做什么工作？
  - 团队工作：类似于GPU调度，比如夜间GPU相对空闲的时候可以用来进行安全检测等其他事

### 二面
- 电话面，主要问了项目
- 问了编解码器的项目以及CPU的比赛
- 如何提高数据的计算速度？
    - 我回答了并行计算、cache、根据特定场景定义特殊的数据结构等方式（这方面并没有基础，是我临时想的），但是面试官不是很满意，应该有什么我没想到的好方法吧
- 一个初高中的问题：小明的考试通过率是60%，考了10次，通过的概率是多少？
    - 1 - (1-40%)<sup>10</sup>
- 最后面试官说了他们部门做的方向，然后说和编解码器这一块不是很match，如果有可能的话后面会再联系我，感觉是劝退我了。。（竟然过了）

### 三面（Boss终面）
- 视频面，主要问了项目
- 对项目的拓展：在直播过程中需要对直播的内容进行实时检测和审核，对此有什么方案？
- 部门介绍：CRO技术部（CRO：首席风险官），主要做风控
- 我去的这个团队主要做算法工程
    - 偏底层的有：CPU、GPU调度、流量分配
    - 偏上层的有：数据处理、算法的编译、部署、评测
    - 更上层的有：样本处理、知识处理等
- 实习生主要了解整个项目怎么运作、设计，了解技术在大公司怎么应用等等，具体还是得等入职后分配

#### HR面
- 视频面，自我介绍，问了项目
- 对部门的了解
- 选择部门时的考虑因素
- 还有一些问题忘了，因为是面完过了将近一个月才记录的


## 美团数据库实习生

### 一面
- 简单介绍了一下部门，是数据库方向的，和操作系统内核有关
- 自我介绍
- 原子操作是如何实现的
- 数组和链表有什么区别，遍历的时间复杂度
- 如何对收到的消息进行计数
- 设计一个数据结构，记录一个部门里的员工近两天是否有打卡，或者指定两天，还得查询某一天有多少员工打卡
- 操作系统如何进行IO操作，如何进行文件读写


### 网易雷火云平台研发工程师（精英实习）

### 一面
- 毕业时间？实习时间？
- 自我介绍
- CPU比赛团队有几个人，主要负责什么？
- 编解码器这个项目遇到过的困难
- 对linux有了解吗？
    - 如何查看端口状态
      - netstat
    - 如何对文件的某处内容进行替换
    - 协程
    - cgroup、namespace
- Python中的全局解释锁
- 对Docker和K8s有了解吗
- 手撕代码：利用快排思想求数组中第k大的数
- 部门介绍：主要做底层云支撑，实习生和正式工做一样的事情，实习生不会做历史包袱很重的事，比如读之前的文档，而是会做一两个相对独立的模块；部门涉及的工作有：云平台网关、监控、边缘计算云边协同、调度优化等等


## 字节抖音社区安全实习生

### 一面

- 自我介绍
- 内存是如何分区的，有没有什么内存是在程序结束后不释放的
- 全局变量和静态全局变量有什么区别，函数中的变量和静态变量有什么区别？
- 数据库ACID，什么是原子性，什么是一致性，mysql中有哪些日志？
- TCP流量控制和拥塞控制，如果接收方先给发送方发送一个包来设置发送方的滑动窗口大小为0，然后再发送一个包来设置发送方的滑动窗口大小为2，但是这个包丢失了，会发生什么？
  - [TCP窗口机制和流量控制](https://www.cnblogs.com/-wenli/p/13089310.html)
  - 当发送方收到了接收方发来的零窗口通知，就启动持续计时器，如果持续计时器超时，就发送窗口探测报文，向接收方询问自己的窗口大小是否要更改
- 手撕代码：
    - 给定一个数组和k，问是否存在子数组的和刚好是k的倍数
        - [LeetCode原题](https://leetcode-cn.com/problems/continuous-subarray-sum/)
        - 方法1：前缀和
        - 方法2：求余优化
    - 设计一个数据结构，实现insert、remove、getRandom（随机返回一个数），要求时间复杂度都为O（1）
        - [LeetCode原题](https://leetcode-cn.com/problems/insert-delete-getrandom-o1/)

### 二面

- 自我介绍
- 进程和线程的区别，线程切换的开销主要在什么地方
- 数据库ACID，什么是原子性，原子性是怎么实现的
- 数据库的隔离级别
- 在网页上输入一个网址，到渲染出这个网页，中间经历了哪些过程
- https的非对称密钥和对称密钥
- http1.0、1.1、2.0之间的变化和区别，长连接和短连接的区别，短连接相比长连接多的开销在哪
- 有了解什么设计模式吗？单例是如何实现的？
- 手撕代码：求字符串的最长不重复子串


## 拼多多
**注意：拼多多的面试界面不允许切出去，切出去5秒以上算作弊，所以最好提前打印好简历**

### 一面

- 自我介绍
- 数据库
    - mysql有了解吗，MySQL常用的两种存储引擎分别是什么，介绍一下InnoDB
    - B+树和B树有什么区别，为什么用B+树不用B树
    - 除了B+树以外，还有哪些数据结构用来存放索引
    - 数据库的三级封锁协议
    - 数据库中有哪些锁，操作系统中有哪些锁，什么是自旋锁，它有什么优缺点
- Python
    - Python中的负索引
    - Python中的线程池有了解吗
    - 我说其实我对C++更熟悉一些。。然后就没问Python了
- Java
    - Java的线程池了解吗，有哪些变量
    - JVM了解吗
    - 公平锁和非公平锁
    - 线程有什么方法实现同步，不使用锁（比如两个线程如何对同一个变量+1）
        - CAS（Compare And Swap）比较并交换
    - ThreadLocal
    - 消息队列有了解吗，kafka了解吗
    - Spring了解吗
    - HashMap和HashTable的区别
- 计算机网络
    - TCP三次握手和四次挥手
    - 为什么需要第三次握手
- 操作系统
    - 进程通信的方式
    - 线程同步的方式
    - 进程有哪些状态
    - 进程控制块PCB
- C++
    - 内存分区
    - 重载和覆盖
    - 当两个函数的函数名和参数都相同，只有返回值不同，会发生什么
        - 编译不通过
    - 宏定义和const的区别
    - 内联函数
    - template
    - 当动态申请内存时，内存不够该怎么处理
    - 编译过程，解释一下链接
- linux
  - 如何查看磁盘使用情况
    - df
  - 如何创建目录
    - mkdir
- 手撕代码：给定一个整数，判断是否是回文串
- 部门介绍：中台，电商，服务端维护，另外也有一些业务框架，基础架构，主要用Java

### 二面（技术面）

- 自我介绍，问项目
- 手撕代码：
  - 两个矩阵是否相交，如果相交则输出相交的矩阵坐标
  - 给定n,m，问n个节点的m叉树有多少种结构

### 三面（技术主管面）

- 自我介绍，问项目
- 对拼多多工作时间的看法
- 对拼多多砍一刀的负面言论的看法
- 选择工作会考虑的因素，排个序
- 自我评价，有什么优点或缺点
- 有什么兴趣爱好

### HR面
- 自我介绍
- 父母的工作单位
- 在学校的成绩排名如何
- 有在面试其它公司吗
- 有没有亲戚朋友在拼多多上班
- 实习的时间
- 是否单身
- 为什么想来拼多多


## 网易云音乐

### 一面

- 自我介绍，问项目
- 进程、线程、协程的区别
- 协程如何切换，涉及到哪些寄存器
- 函数调用的压栈过程，为什么从右往左压栈
- 客户端向服务端发送请求后，服务端是怎么做的
- 智能指针是如何实现的，什么是循环引用
- malloc的底层实现，对于频繁地申请内存有什么优化方式
- 数据库索引的实现方式，什么是全文索引
- 数据库的隔离级别
- 分布式系统了解吗
    - 回答了一下Zookeeper是如何实现顺序一致性的
- 手撕代码：二叉树的层序遍历
- 有什么问题要问？
- 补充提问：你觉得你做这两篇论文相比别人做了项目来说有什么优势吗？


## 七牛云IOS客户端开发

### 一面

- 自我介绍，问项目
- IOS基础
- H264相关
- 音频相关
- C++基础
- 有什么自己觉得比较不错的项目
- 反问


## Momenta统研发实习生-C++

### 一面

- 面试官介绍了自己的部门和团队方向
- 自我介绍
- 手撕代码：一道简单的回溯：用1、2、5、10进行排列组合来使和为10，每个数可重复使用
- 线程同步问题和进程通信问题
- 类对象的private变量的指针返回到了类外，能否对这个变量进行修改？如果将这个指针传递到了另一个进程，能否进行修改？
- 虚拟内存地址空间如何映射到物理内存
- 如果你带领一个团队，团队的某些成员有些突发情况不得不请假一段时间，你会如何处理

### 二面

- 面试官介绍了自己的部门和团队方向
- 自我介绍
- 有写过多线程的代码吗
- 有写过网络通信相关的代码吗
- 介绍一下Http
- 问了俩项目
- 反问
- 部门业务涵盖：
  - 无人驾驶网约车平台
  - 自动驾驶系统，有个C++模块还需要做
  - 为各个车厂设计HMI模块，需要进行上车测试


## 腾讯后台开发QQ技术中台

### 一面（电话面）

- 没有自我介绍
- static变量的默认值是多少
- 不断调用malloc会把内存消耗完吗
- 用malloc分配1M的内存，物理内存分配了多少
- write和fwrite的区别
- C++中class和struct的区别
- TCP socket返回值为0表示什么
- UDP报文最大是多少
- 介绍一下第一个项目
- 一栋50层高的大楼大概有多少台阶
  - 2 * 每层台阶数 * 层数
- 没有反问


## 虹软CameraTuning

### 一面

- 自我介绍
- 问了下项目
- 栈内存和堆内存的区别 
- 野指针和空指针
- 反问
- 业务涵盖：
  - 根据芯片厂商的SDK，调整Camera使得画质最好
  - 图像ISP方面


## 理想汽车

### 一面

- 自我介绍
- 主要问了项目
- 反问
- 这是一个22年1月新成立的团队，有大量hc

### 二面


## 云和恩墨

### 一面

- 自我介绍
- 面试官直接问我都了解什么东西，然后再具体深入地问，包括但不限于以下问题
- 操作系统：
  - 进程和线程的区别、进程切换和线程切换的区别、IPC、有哪些socket、进程同步、常见的锁
- 哈希表的实现、哈希冲突、使用再哈希法的时候，如果给定数据量比数组空间大该怎么办
- C和C++的区别
- 数据库：
  - B+树、ACID、并发一致性、隔离级别、MVCC
- 将多个有序数组合并成一个有序数组
- 有向图的遍历
- 如何求两个数组的交集、差集    
  - [两个无序数组求公共交集](https://blog.csdn.net/t46414704152abc/article/details/77341368)
- 反问

### 二面

- 自我介绍
- 问了CPU这个项目
    - 为什么采用五级流水线，流水线级数可以再多一些吗
- 手撕代码：
    - 写了个最基本的二分查找，问传参为什么用引用和const
    - 二维数组求和，问有什么优化的地方？比如int换成unsigned int，i++换成++i，能否外层循环先遍历列
- 反问
- 部门职责：
    - 公司的数据库：MogDB，一主多副结构
    - 北京：SQL引擎的执行优化、kernel
    - 南京：Oracle兼容，即以Oracle为标杆，看用户具体需要哪些功能需求，然后往自己的数据库上加
    - 成都：存储引擎的下半部分
    - 还有西安等工作地点，工作地点可以按照需求自己选择