# 这里存放了计算机网络相关知识

## 网络编程——TCP编程流程
[参考网址](https://blog.csdn.net/Eunice_fan1207/article/details/83903148)

### 服务器：
socket、bind、listen、accept、recv、send、close

1. socket创建网络套接字
    ```
    #include<sys/socket.h>
    #include<sys/types.h>
    int socket(int domain,int type,int protocol);
    //成功返回一个socket文件描述符，失败返回-1
    ```
    - domain：指定系统使用的底层协议族，对于TCP/IP协议族而言，参数设置为AF_INET/PF_INET（用于IPV4）或者PF_INET6（用于IPV6）；对于UNIX本地域协议族而言，参数设置为PF_UNIX等，我们基本设置为ipv4即AF_INET
    - type：指定服务类型，主要有SOCK_STREAM服务（流式服务）和SOCK_DGRAM服务（数据包服务），对于TCP/IP协议族，SOCK_STRAEM表示传输层使用TCP协议，SOCK_DGRAM表示使用UDP协议
    - protocol：在前两个参数构成的协议集合下，再选择一个具体的协议，几乎都会设置为0，表示使用默认协议

    ps：socket连接是全双工的
2. bind绑定socket与地址（IP+端口），也称命名socket
    - 对于上面创建了socket指定了使用那个地址族，但是并未指定使用该地址族中的那个具体的socket地址，对于服务器来说，只有命名了socket，将服务器的ip及其端口和socket绑定，才能将其发布出去，客户端才能知道如何连接它。相当于用socket实现进程与进程的通信
    - 为什么客户端不需要这个bind操作？
        
        因为服务器是一个大型的pc，它往往是固定的ip与其端口，它不能一直变化，那么客户端就没有办法请求它（比如百度的服务器，它的ip地址和端口就是固定绑定滴）。但是呢，对于客户端来说就没有必要绑定其socket与其固定的ip和端口，不会有其他客户端会一直请求访问你这个客户端，客户端会使用操作系统自动分配的地址，即采用匿名方式。另外一点，因为客户端总是发起链接的一方，所以服务器能获得客户端的信息，所以在客户端这一方可以不使用bind函数。
3. 监听socket
4. 接受连接
5. 数据读写
6. 发起连接（客户端）
7. 关闭连接


## 如果服务器和带宽的资源无限，TCP的连接数的上限是多少?

- [参考](https://mp.weixin.qq.com/s/X6c_H5_4OInR8nFQVn7IMA)
- 应该是端口号的范围，端口用16位数字表示，范围是0-65535，其中0-1024保留给预定义的服务，而剩下的端口中可使用的端口范围是有具体限制的，可以在 / etc/sysctl.conf 中修改


## TCP流量控制和拥塞控制，如果接收方先给发送方发送一个包来设置发送方的滑动窗口大小为0，然后再发送一个包来设置发送方的滑动窗口大小为2，但是这个包丢失了，会发生什么？

- [TCP窗口机制和流量控制](https://www.cnblogs.com/-wenli/p/13089310.html)
- 当发送方收到了接收方发来的零窗口通知，就启动持续计时器，如果持续计时器超时，就发送窗口探测报文，向接收方询问自己的窗口大小是否要更改