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
[How world one write object-oriented code in C?](https://stackoverflow.com/a/351745)

### friend 友元类和友元函数
* 能访问私有成员
* 破坏封装性
* 友元关系不可传递
* 友元关系的单向性
* 友元声明的形式及数量不受限制

### static_cas和dynamic_cast
转换方式：
static_cast<new_type>(expression)

其中，new_type为目标数据类型，expression为原始数据类型变量或者表达式。
static_cast相当于传统的C语言里的强制转换，该运算符把expression转换为new_type类型，用来强迫隐式转换如non-const对象转为const对象，编译时检查，用于非多态的转换，可以转换指针及其他，但没有运行时类型检查来保证转换的安全性。

主要有如下几种用法：
1. 用于类层次结构中基类和派生类之间指针或引用的转换
* 进行上行转换（把派生类的指针或引用转换成基类表示）是安全的；
* 进行下行转换（把基类的指针或引用转换成派生类表示）是不安全的，因为没有动态类型检查；
2. 用于基本数据类型之间的转换，如把int转换成char，把int转换成enum
3. 把空指针转换成目标类型的空指针
4. 把任何类型的表达式转换成void类型
注意：static_cast不能转换掉expression的const、volatile、或者__unaligned属性

基本类型数据转换举例
```
char a = 'a';
int b = static_cast<int>(a);//正确，将char型数据转换成int型数据

double *c = new double;
void *d = static_cast<void*>(c);//正确，将double指针转换成void指针

int e = 10;
const int f = static_cast<const int>(e);//正确，将int型数据转换成const int型数据

const int g = 20;
int *h = static_cast<int*>(&g);//编译错误，static_cast不能转换掉g的const属性
```

类的上行和下行转换举例
```
class Base
{};

class Derived : public Base
{}

Base* pB = new Base();
if(Derived* pD = static_cast<Derived*>(pB))
{}//下行转换是不安全的(坚决抵制这种方法)

Derived* pD = new Derived();
if(Base* pB = static_cast<Base*>(pD))
{}//上行转换是安全的
```

### dynamic_cast
转换方式：
dynamic_cast<type*>(expression)
type必须是一个类类型且必须是一个有效的指针
dynamic_cast<type&>(expression)
type必须是一个类类型且必须是一个左值
dynamic_cast<type&&>(expression)
type必须是一个类类型且必须是一个右值

expression的类型必须符合以下三个条件中的任何一个：
1. expression的类型是目标type的公有派生类
2. expression的类型是目标type的公有基类
3. expression的类型是目标type的类型

如果一条dynamic_cast语句的转换目标是指针类型并且失败了，则结果为0。如果转换目标是引用类型并且失败了，则dynamic_cast运算符将抛出一个std::bad_cast异常（该异常定义在typeinfo标准库头文件中）。expression也可以是一个空指针，结果是所需类型的空指针。

dynamic_cast主要用于类层次间的上行转换和下行转换，还可以用于类之间的交叉转换（cross cast）。

* 在类层次间进行上行转换时，dynamic_cast和static_cast的效果是一样的；
* 在进行下行转换时，dynamic_cast具有类型检查的功能，比static_cast更安全；
dynamic_cast是唯一无法由旧式语法执行的动作，也是唯一可能耗费重大运行成本的转型动作

* 指针类型

举例，Base为包含至少一个虚函数的基类，Derived是Base的公有派生类，如果有一个指向Base的指针bp，我们可以在运行时将它转换成指向Derived的指针，代码如下：
```
if(Derived *dp = dynamic_cast<Derived *>(bp)){
  //使用dp指向的Derived对象  
}
else{
  //使用bp指向的Base对象  
}
```
值得注意的是，在上述代码中，if语句中定义了dp，这样做的好处是可以在一个操作中同时完成类型转换和条件检查两项任务。

* 引用类型

因为不存在所谓空引用，所以引用类型的dynamic_cast转换与指针类型不同，在引用转换失败时，会抛出std::bad_cast异常，该异常定义在头文件typeinfo中。
```
void f(const Base &b){
    try{
        const Derived &d = dynamic_cast<const Base &>(b);  
        //使用b引用的Derived对象
    }
        catch(std::bad_cast){
        //处理类型转换失败的情况
    }
}
```

*尽量少使用转型操作，尤其是dynamic_cast，耗时较高，会导致性能的下降，尽量使用其他方法替代。*



