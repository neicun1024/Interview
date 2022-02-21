## 主要记录了之前没复习到的内容

### 指针

* 类中的this指针是右值

### assert()

断言，是宏，而非函数。可以使用NDEBUG来关闭assert，但是需要在源代码的开头，在include<assert.h>之前
```
#define NDEBUG          // 加上这行，则 assert 不可用
#include <assert.h>

assert( p != NULL );    // assert 不可用
```

### #pragma pack(n)

设定结构体、联合(Union)以及类成员变量以n字节方式对齐
```
#pragma pack(push)  // 保存对齐状态
#pragma pack(4)     // 设定为4字节对齐

struct test
{
    char m1;
    double m4;
    int m3;
};

#pragma pack(pop)   // 恢复对齐状态
```

### extern "C"
* 被extern限定的函数或变量是extern类型的
* 被extern "C"修饰的变量和函数是按照C语言方式编译和链接的
extern "C"的作用是让C++编译器将extern "C"声明的代码当做C语言代码处理，可以避免C++因符号修饰导致代码不能和C语言库中的符号进行连接的问题
```
#ifdef __cplusplus
extern "C" {
#endif

void *memset(void *, int, size_t);

#ifdef __cplusplus
}
#endif
```

### struct和typedef struct
若定义了与struct同名的函数后，该struct只代表函数，不代表结构体

### struct和class的区别
1. 默认的继承访问权限不同。struct是public的，class是private的
2. 默认的数据访问权限不同。struct是public的，class是private的

### union
联合（union）是一种节省空间的特殊的类，一个 union 可以有多个数据成员，但是在任意时刻只有一个数据成员可以有值。当某个成员被赋值后其他成员变为未定义状态。联合有如下特点：
* 默认访问控制符为 public
* 可以含有构造函数、析构函数
* 不能含有引用类型的成员
* 不能继承自其他类，不能作为基类
* 不能含有虚函数
* 匿名 union 在定义所在作用域可直接访问 union 成员
* 匿名 union 不能包含 protected 成员或 private 成员
* 全局匿名联合必须是静态（static）的
```
#include<iostream>

union UnionTest {
    UnionTest() : i(10) {};
    int i;
    double d;
};

static union {
    int i;
    double d;
};

int main() {
    UnionTest u;

    union {
        int i;
        double d;
    };

    std::cout << u.i << std::endl;  // 输出 UnionTest 联合的 10

    ::i = 20;
    std::cout << ::i << std::endl;  // 输出全局静态匿名联合的 20

    i = 30;
    std::cout << i << std::endl;    // 输出局部匿名联合的 30

    return 0;
}
```

### 用C实现C++的面向对象特性（封装、继承、多态）
* 封装：使用函数指针把属性与方法封装到结构体中
* 继承：结构体嵌套
* 多态：父类与子类的函数指针不同
