# 通过一个简单的银行转账程序，来学习多线程、死锁、RAII等概念

[参考资料](https://paul.pub/cpp-concurrency/)

- bank_transfer.cpp 中使用mutex来加锁，使用scoped_lock来避免死锁
- bank_transfer_wait_notify.cpp 中使用 `wait` 和 `notify` 来实现类似生产者-消费者的方式


### RAII

RAII全称是Resource Acquisition Is Initialization，资源获取即初始化。

将mutex对象和lock_guard等对象绑定，当lock_gurad对象销毁时，mutex对象也会自动释放，从而简化加锁解锁流程。

- 将每个资源封装入一个类，其中：
    - 构造函数请求资源，并建立所有类不变式，或在它无法完成时抛出异常，
    - 析构函数释放资源并决不抛出异常；
- 始终经由 RAII 类的实例使用满足要求的资源，该资源
    - 自身拥有自动存储期或临时生存期，或
    - 具有与自动或临时对象的生存期绑定的生存期

### [lock_guard和unique_lock的区别](https://blog.csdn.net/boiled_water123/article/details/101021602)

unique_lock是对lock_guard的扩展，允许在生命周期内再调用lock和unlock来加解锁以切换锁的状态。

根据linux下条件变量的机制，condition_variable在wait成员函数内部会先调用参数unique_lock的unlock临时解锁，让出锁的拥有权（以让其它线程获得该锁使用权加锁，改变条件，解锁），然后自己等待notify信号，等到之后，再调用参数unique_lock的lock加锁，处理相关逻辑，最后unique_lock对象销毁时自动解锁。