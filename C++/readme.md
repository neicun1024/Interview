## 主要记录了之前没复习到C++方面的内容

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

### static_cas
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

举例，Base为**包含至少一个虚函数**的基类，Derived是Base的公有派生类，如果有一个指向Base的指针bp，我们可以在运行时将它转换成指向Derived的指针，代码如下：
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
        const Derived &d = dynamic_cast<const Base &>(b); // 这里会报错，try catch无法捕获到，在下一条《异常处理》中会讲原因
        //使用b引用的Derived对象
    }
        catch(std::bad_cast){
        //处理类型转换失败的情况
    }
}
```

*尽量少使用转型操作，尤其是dynamic_cast，耗时较高，会导致性能的下降，尽量使用其他方法替代。*

### 异常处理
程序的错误大致可分为三种，分别是语法错误、逻辑错误和运行时错误
* 语法错误：在编译和链接阶段就能发现，只有 100% 符合语法规则的代码才能生成可执行程序。语法错误是最容易发现、最容易定位、最容易排除的错误，程序员最不需要担心的就是这种错误
* 逻辑错误：编写的代码思路有问题，不能够达到最终的目标，这种错误可以通过调试来解决
* 运行时错误：指程序在运行期间发生的错误，例如除数为 0、内存分配失败、数组越界、文件不存在等。C++ 异常（Exception）机制就是为解决运行时错误而引入的

### 左值引用和右值引用
右值引用可实现转移语义（Move Sementics）和精确传递（Perfect Forwarding），它的主要目的有两个方面：
* 消除两个对象交互时不必要的对象拷贝，节省运算存储资源，提高效率
* 能够更简洁明确地定义泛型函数

引用折叠
* type& &、type& &&、type&& & 可折叠成 type&
* type&& && 可折叠成 type&&

### 成员初始化列表
* C++规定，对象的成员变量的初始化动作发生在进入构造函数本体之前，比如在进入InitializerList的构造函数本体之前，会先初始化a和b，在进入InitializerList的构造函数本体后，**a=3;**和**b=4;**会调用对应的构造函数来创建临时对象，并将值赋给a和b，随后将临时对象析构；
* 如果InitializerList的构造函数的初始化列表中没有给成员变量初始化，那么会调用其默认构造函数，比如InitializerList的构造函数的初始化列表没有初始化a，所以会调用a的默认构造函数，而初始化了b，所以会调用b对应的构造函数（*这里如果不给b初始化会报错，因为我将B的默认构造函数delete了*）；
* 直接在成员初始化列表中赋值而不是在InitializerList的构造函数本体中赋值，能减少一次构造函数的调用，从而减少开销

```
class InitializerList
{
private:
    class A
    {
    private:
        int val;

    public:
        A()
        {
            cout << "default constructor of A" << endl;
            myPrint();
        };
        A(int tmp)
        {
            val = tmp;
            cout << "constructor of A" << endl;
            myPrint();
        }
        ~A()
        {
            cout << "destructor of A" << endl;
        };

        void myPrint()
        {
            cout << "val = " << val << endl;
        }
    };

    class B
    {
    private:
        int val;

    public:
        B() = delete;
        B(int tmp)
        {
            val = tmp;
            cout << "constructor of B" << endl;
            myPrint();
        }
        ~B()
        {
            cout << "destructor of B" << endl;
        };

        void myPrint()
        {
            cout << "val = " << val << endl;
        }
    };

    A a;
    B b;

public:
    InitializerList() : b(2) // 初始化列表里没初始化a，所以会调用a的默认构造函数
    {
        a = 3;
        b = 4;
        cout << "constructor of InitializerList" << endl;
        a.myPrint();
        b.myPrint();
    }

    ~InitializerList()
    {
        cout << "destructor of InitializerList" << endl;
    }
};

int main()
{
    InitializerList initializerList;
    return 0;
｝
```

以上代码的运行结果为
```
default constructor of A
val = 36
constructor of B
val = 2
constructor of A
val = 3
destructor of A
constructor of B
val = 4
destructor of B
constructor of InitializerList
val = 3
val = 4
destructor of InitializerList
destructor of B
destructor of A
```

有些场合必须要用初始化列表：
* 常量成员，因为常量只能初始化不能赋值，所以必须放在初始化列表里面
* 引用类型， 引用必须在定义时初始化，并且不能重新赋值，所以也要写在初始化列表里面
* 没有默认构造函数的类，比如上面例子中的b，通过使用初始化列表来避免调用默认构造函数

### 多态的分类及实现
* 重载多态（编译期）：函数重现、运算符重载
* 子类型多态（运行期）：虚函数
* 参数多态性（编译器）：类模板、函数模板
* 强制多态（编译期/运行期）：基本类型转换、自定义类型转换

#### 动态多态
动态绑定：当使用基类的引用或指针调用一个虚函数时将发生动态绑定

**注意**：
* 可以将派生类的对象赋值给基类的指针或引用，反之不可
* 普通函数（非类成员函数）不能是虚函数
* 静态函数（static）不能是虚函数
* 构造函数不能是虚函数（因为在调用构造函数时，虚表指针并没有在对象的内存空间中，必须要构造函数调用完成后才会形成虚表指针）
* 内联函数不能是表现多态性时的虚函数

### 虚继承

### 虚继承和虚函数

### 模板类、成员模板、虚函数
* 模板类中可以使用虚函数
* 一个类（无论是普通类还是类模板）的成员模板（本身是模板的成员函数）不能是虚函数

### 抽象类、接口类、聚合类
* 抽象类：含有纯虚函数的类
* 接口类：仅含有纯虚函数的抽象类
* 聚合类：用户可以直接访问其成员，并且具有特殊的初始化语法形式。满足如下特点：
    * 所有成员都是 public
    * 没有定义任何构造函数
    * 没有类内初始化
    * 没有基类，也没有 virtual 函数

### 内存分配和管理

#### malloc、calloc、realloc、alloca
* malloc：申请指定字节数的内存。申请到的内存中的初始值不确定
* calloc：为指定长度的对象，分配能容纳其指定个数的内存。申请到的内存的每一位（bit）都初始化为 0
* realloc：更改以前分配的内存长度（增加或减少）。当增加长度时，可能需将以前分配区的内容移到另一个足够大的区域，而新增区域内的初始值则不确定
* alloca：在栈上申请内存。程序在出栈的时候，会自动释放内存。但是需要注意的是，alloca 不具可移植性, 而且在没有传统堆栈的机器上很难实现。alloca 不宜使用在必须广泛移植的程序中。C99 中支持变长数组 (VLA)，可以用来替代 alloca

#### placement new
placement new 允许我们向 new 传递额外的地址参数，从而在预先指定的内存区域创建对象
```
new (place_address) type
new (place_address) type (initializers)
new (place_address) type [size]
new (place_address) type [size] { braced initializer list }
```
* place_address 是个指针
* initializers 提供一个（可能为空的）以逗号分隔的初始值列表

### delete this 合法吗？
合法，但是：
1. 必须保证 this 对象是通过 new（不是 new[]、不是 placement new、不是栈上、不是全局、不是其他对象成员）分配的
2. 必须保证调用 delete this 的成员函数是最后一个调用 this 的成员函数
3. 必须保证成员函数的 delete this 后面没有调用 this 了
4. 必须保证 delete this 后没有人使用了

### 如何定义一个只能在堆上（栈上）生成对象的类？
#### 只能在堆上
* 方法1：将析构函数设置为私有

    原因：
    * 非静态对象建立在栈上，是由编译器分配和释放内存空间，编译器为对象分配内存空间时，会对类的非静态函数进行检查，即编译器会检查析构函数的访问性。当析构函数设为私有时，编译器创建的对象就无法通过访问析构函数来释放对象的内存空间，因此，编译器不会在栈上为对象分配内存。

    存在的问题：
    * 用 new 创建的对象，通常会使用 delete 释放该对象的内存空间，但此时类的外部无法调用析构函数，因此类内必须定义一个 destory() 函数，用来释放 new 创建的对象
    * 无法解决继承问题，因为如果这个类作为基类，析构函数要设置成 virtual，然后在派生类中重写该函数，来实现多态。但此时，析构函数是私有的，派生类中无法访问
* 方法2：构造函数设置为 protected，并提供一个 public 的静态函数来完成构造，而不是在类的外部使用 new 构造；将析构函数设置为 protected

    原因：
    类似于单例模式，也保证了在派生类中能够访问析构函数。通过调用 create() 函数在堆上创建对象。

#### 只能在栈上
* 方法：将 operator new() 设置为私有
    原因：
    * 当对象建立在堆上时，是采用 new 的方式进行建立，其底层会调用 operator new() 函数，因此只要对该函数加以限制，就能够防止对象建立在堆上。

