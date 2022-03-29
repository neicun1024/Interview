## 主要记录了C++方面的内容

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

[How world one write object-oriented code in C?](https://stackoverflow.com/a/351745)
* 封装：使用函数指针把属性与方法封装到结构体中
* 继承：结构体嵌套
* 多态：父类与子类的函数指针不同


### friend 友元类和友元函数
* 能访问私有成员
* 破坏封装性
* 友元关系不可传递
* 友元关系的单向性
* 友元声明的形式及数量不受限制


### static_cast
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

注意：*static_cast不能转换掉expression的const、volatile、或者__unaligned属性*

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
- dynamic_cast<type*>(expression)
    - type必须是一个类类型且必须是一个有效的指针

- dynamic_cast<type&>(expression)
    - type必须是一个类类型且必须是一个左值

- dynamic_cast<type&&>(expression)
    - type必须是一个类类型且必须是一个右值

expression的类型必须符合以下三个条件中的任何一个：
1. expression的类型是目标type的公有派生类
2. expression的类型是目标type的公有基类
3. expression的类型是目标type的类型

如果一条dynamic_cast语句的转换目标是指针类型并且失败了，则结果为0。如果转换目标是引用类型并且失败了，则dynamic_cast运算符将抛出一个std::bad_cast异常（该异常定义在typeinfo标准库头文件中）。expression也可以是一个空指针，结果是所需类型的空指针。

dynamic_cast主要用于类层次间的上行转换和下行转换，还可以用于类之间的交叉转换（cross cast）。

* 在类层次间进行上行转换时，dynamic_cast和static_cast的效果是一样的；
* 在进行下行转换时，dynamic_cast具有类型检查的功能，比static_cast更安全；
dynamic_cast是唯一无法由旧式语法执行的动作，也是唯一可能耗费重大运行成本的转型动作

#### 指针类型

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

#### 引用类型

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
* C++规定，对象的成员变量的初始化动作发生在进入构造函数本体之前，比如在进入InitializerList的构造函数本体之前，会先初始化a和b，在进入InitializerList的构造函数本体后， a=3 和 b=4 会调用对应的构造函数来创建临时对象，并将值赋给a和b，随后将临时对象析构；
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


### 函数调用过程

[函数调用过程-简要](https://www.cnblogs.com/biyeymyhjob/archive/2012/07/20/2601204.html)

[函数调用过程-详细](https://cloud.tencent.com/developer/article/1814613)

#### 函数调用涉及到的寄存器
- EIP：指令指针，指向下一条即将执行的指令的地址
- EBP：基址指针，指向栈底
- ESP：栈指针，指向栈顶
- EBX：基址寄存器
- ESI：源变址寄存器
- EDI：目的变址寄存器

#### 过程
![20220307193704](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220307193704.png)
1. 函数调用
![20220307200307](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220307200307.png)
   1. 将参数从右往左入栈
2. 保护现场
   ![20220307193814](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220307193814.png)
   1. 将函数的返回地址入栈（编译器隐式地执行**push eip**指令）
   2. 将ebp入栈（因为每个函数都有自己的栈区域，所以栈基址也是不一样的，现在进入了一个中断函数，函数执行过程中也需要ebp寄存器）
   3. 将栈顶地址作为该函数的栈基址（将栈底指针指向栈顶指针所指的地址），确定被调用的函数的栈区域（ebp为栈底，esp为栈顶）
   4. 将栈顶指针往上移动48h来将两个函数的栈区域隔开一段距离，该间隔区域的大小固定为40h，即64Byte，另外8h为预留出的存储局部变量的区域
   ![20220307200138](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220307200138.png)
   1. 将EBX、ESI、EDI入栈
   2. 将留出的48h的内存区域赋值为0xCCCCCCCCh
3. 执行子函数
   ![20220307193836](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220307193836.png)
   1. 根据ebp来计算被调用的函数中的局部变量的内存地址，并存入相应位置
4. 恢复现场
   ![20220307201041](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220307201041.png)
   1. 三条出栈指令，将EDI、ESI、EBX出栈
   2. 将esp的值赋给esp（让esp跳过了一段区域，就是间隔区域和局部数据区域，因为函数已经退出，所以这两个区域没有用了）
   3. 将ebp出栈
   4. 将函数的返回地址出栈（编译器隐式地执行**pop eip**指令）
   5. 将栈顶指针esp回到函数调用前的位置（让栈顶指针往下移动12Byte，即三个参数）


### 内联函数

#### 原理
- 内联函数不是在调用时发生控制转移关系，而是在编译阶段将函数体嵌入到每一个调用该函数的语句块中，编译器会将程序中出现内联函数的调用表达式用内联函数的函数体来替换
- 普通函数是将程序执行转移到被调用函数所存放的内存地址，当函数执行完后，返回到执行此函数前的地方。转移操作需要保护现场，被调函数执行完后，再恢复现场，该过程需要较大的资源开销

#### 特征
- 

#### 和宏定义的区别

1. 内联函数是在编译时展开，而宏在编译预处理时展开；在编译的时候，内联函数直接被嵌入到目标代码中去，而宏只是一个简单的文本替换
2. 内联函数是真正的函数，和普通函数调用的方法一样，在调用点处直接展开，避免了函数的参数压栈操作，减少了调用的开销。而宏定义编写较为复杂，常需要增加一些括号来避免歧义
3. 宏定义只进行文本替换，不会对参数的类型、语句能否正常编译等进行检查。而内联函数是真正的函数，会对参数的类型、函数体内的语句编写是否正确等进行检查

#### 优点
1. 内联函数同宏函数一样将在被调用处进行代码展开，省去了参数压栈、栈帧开辟与回收，结果返回等，从而提高程序运行速度
2. 内联函数相比宏函数来说，在代码展开时，会做安全检查或自动类型转换（同普通函数），而宏定义则不会
3. 在类声明中定义的函数，除了虚函数的其他函数都会自动隐式地当成内联函数，因此内联函数可以访问类的成员变量，宏定义则不能
4. 内联函数在运行时可调试，而宏定义不可以

#### 缺点
1. 代码膨胀。内联是以代码膨胀（复制）为代价，消除函数调用带来的开销。如果执行函数体内代码的时间，相比于函数调用的开销较大，那么效率的收获会很少。另一方面，每一处内联函数的调用都要复制代码，将使程序的总代码量增大，消耗更多的内存空间
2. **inline 函数无法随着函数库升级而升级。inline函数的改变需要重新编译，不像 non-inline 可以直接链接**
3. **是否内联，程序员不可控。内联函数只是对编译器的建议，是否对函数内联，决定权在于编译器**


### ++i和i++的区别
- 对于基本类型
- 对于自定义类型


### lambda表达式
lambda表达式是C++11中最重要也最常用的特性之一，用于定义匿名函数，并且可以捕获一定范围内的变量

语法形式： \[capture](params)opt->ret{body;};

其中capture是捕获列表，params是参数表，opt是函数选项，ret是返回值类型，body是函数体；

#### 优点
- 声明式编程风格：就地匿名定义目标函数或函数对象，不需要额外写一个命名函数或者函数对象。以更直接的方式去写程序，好的可读性和可维护性
- 简洁：不需要额外再写一个函数或者函数对象，避免了代码膨胀和功能分散
- 在需要的时间和地点实现功能闭包，使程序更灵活


#### lambda表达式的各个部分
- 捕获列表
- 参数列表（选）
- mutable（选）
- 异常属性（选）
- 返回类型（选）
- lambda body

#### 使用lambda表达式捕获列表
- [] 不捕获任何变量
- [&] 捕获外部作用域中所有变量，并作为引用在函数体中使用（按引用捕获）
- [=] 捕获外部作用于中所有变量，并作为副本在函数体中使用（按值捕获）
- [=, &foo] 按值捕获外部作用于中所有变量，并按引用捕获foo变量
- [bar] 按值捕获bar变量，同时不捕获其它变量
- [this] 捕获当前类中的this指针，让lambda表达式拥有和当前类成员函数同样的访问权限。如果已经使用了&或=，就默认添加此选项，捕获this的目的是可以在lambda表达式中使用当前类的成员函数和成员变量

```
class A
{
    public:
    int i_ = 0;
    void func(int x, int y)
    {
        auto x1 = []{ return i_; };                    // error，没有捕获外部变量
        auto x2 = [=]{ return i_ + x + y; };           // OK，捕获所有外部变量
        auto x3 = [&]{ return i_ + x + y; };           // OK，捕获所有外部变量
        auto x4 = [this]{ return i_; };                // OK，捕获this指针
        auto x5 = [this]{ return i_ + x + y; };        // error，没有捕获x、y
        auto x6 = [this, x, y]{ return i_ + x + y; };  // OK，捕获this指针、x、y
        auto x7 = [this]{ return i_++; };              // OK，捕获this指针，并修改成员的值
    }
};

int a = 0, b = 1;
auto f1 = []{ return a; };               // error，没有捕获外部变量
auto f2 = [&]{ return a++; };            // OK，捕获所有外部变量，并对a执行自加运算
auto f3 = [=]{ return a; };              // OK，捕获所有外部变量，并返回a
auto f4 = [=]{ return a++; };            // error，a是以复制方式捕获的，无法修改
auto f5 = [a]{ return a + b; };          // error，没有捕获变量b
auto f6 = [a, &b]{ return a + (b++); };  // OK，捕获a和b的引用，并对b做自加运算
auto f7 = [=, &b]{ return a + (b++); };  // OK，捕获所有外部变量和b的引用，并对b做自加运算
```

默认状态下 lambda 表达式无法修改通过复制方式捕获的外部变量。如果希望修改这些变量的话，我们需要使用引用方式进行捕获。

一个容易出错的细节是关于 lambda 表达式的延迟调用的：
```
int a = 0;
auto f = [=]{ return a; };      // 按值捕获外部变量
a += 1;                         // a被修改了
std::cout << f() << std::endl;  // 输出？
```
在这个例子中，lambda 表达式按值捕获了所有外部变量。在捕获的一瞬间，a 的值就已经被复制到f中了。之后 a 被修改，但此时 f 中存储的 a 仍然还是捕获时的值，因此，最终输出结果是 0。

从上面的例子中我们知道，按值捕获得到的外部变量值是在 lambda 表达式定义时的值。此时所有外部变量均被复制了一份存储在 lambda 表达式变量中。此时虽然修改 lambda 表达式中的这些外部变量并不会真正影响到外部，我们却仍然无法修改它们。

如果希望去修改按值捕获的外部变量应当显式指明 lambda 表达式为 mutable
```
int a = 0;
auto f1 = [=]{ return a++; };             // error，修改按值捕获的外部变量
auto f2 = [=]() mutable { return a++; };  // OK，mutable
```

#### lambda表达式的类型

lambda表达式的类型在C++11中被称为**闭包类型**，它是一个特殊的，匿名的类类型。

我们可以认为它是一个带有operator()的类，即仿函数。因此我们可以使用std::function和std::bind来存储和操作lambda表达式：
```
std::function<int(int)>  f1 = [](int a){ return a; };
std::function<int(void)> f2 = std::bind([](int a){ return a; }, 123);
```
对于没有捕获任何变量的 lambda 表达式，还可以被转换成一个普通的函数指针：
```
using func_t = int(*)(int);
func_t f = [](int a){ return a; };
f(123);
```
lambda表达式可以说是就地定义仿函数闭包的“语法糖”。它的捕获列表捕获住的任何外部变量，最终均会变为闭包类型的成员变量。而一个使用了成员变量的类的 operator()，如果能直接被转换为普通的函数指针，那么 lambda表达式本身的 this 指针就丢失掉了。而没有捕获任何外部变量的 lambda表达式则不存在这个问题。

这里也可以很自然地解释为何按值捕获无法修改捕获的外部变量。因为按照 C++ 标准，lambda表达式的 operator() 默认是 const 的。一个 const 成员函数是无法修改成员变量的值的。而 mutable 的作用，就在于取消 operator() 的 const。

需要注意的是，没有捕获变量的 lambda表达式可以直接转换为函数指针，而捕获变量的 lambda 达式则不能转换为函数指针。看看下面的代码：
```
typedef void(*Ptr)(int*);
Ptr p = [](int* p){delete p;};  // 正确，没有状态的lambda（没有捕获）的lambda表达式可以直接转换为函数指针
Ptr p1 = [&](int* p){delete p;};  // 错误，有状态的lambda不能直接转换为函数指针
```
上面第二行代码能编译通过，而第三行代码不能编译通过，因为第三行的代码捕获了变量，不能直接转换为函数指针。


### GDB工作原理和内核实现
[参考](https://zhuanlan.zhihu.com/p/336922639)

gdb主要功能的实现依赖于一个系统函数ptrace，通过man手册可以了解到，ptrace可以让父进程观察和控制其子进程的检查、执行，改变其寄存器和内存的内容，主要应用于打断点（也是gdb的主要功能）和打印系统调用轨迹。

#### 一、ptrace函数
函数的原型如下：
```
#include <sys/ptrace.h>
long ptrace(enum __ptrace_request request, pid_t pid,
           void *addr, void *data);
```

ptrace系统调用的request主要选项
- PTRACE_TRACEME：
  - 表示本进程将被其父进程跟踪，交付给这个进程的所有信号（除SIGKILL之外），都将使其停止，父进程将通过wait()获知这一情况。
- PTRACE_ATTACH
  - attach到一个指定的进程，使其成为当前进程跟踪的子进程，子进程的行为等同于它进行了一次PTRACE_TRACEME操作。
- PTRACE_CONT
  - 继续运行之前停止的子进程。可同时向子进程交付指定的信号。


#### 二、gdb使用ptrace的基本流程
1. gdb调试一个新进程：通过fork函数创建一个新进程，在子进程中执行ptrace(PTRACE_TRACEME, 0, 0, 0)函数，然后通过execv()调用准备调试的程序。
2. attach到已运行进程：将pid传递给gdb，然后执行ptrace(PTRACE_ATTACH, pid, 0, 0)。

在使用参数为PTRACE_TRACEME或PTRACE_ATTACH的ptrace系统调用建立调试关系之后，交付给目标程序的任何信号（除SIGKILL之外）都将被gdb先行截获，gdb因此有机会对信号进行相应处理，并根据信号的属性决定在继续目标程序运行时是否将之前截获的信号实际交付给目标程序。

#### 三、gdb使用的内核机制
断点的功能是通过内核信号实现的，以x86为例，内核向某个地址打入断点，实际上就是往该地址写入断点指令INT 3，即0xCC。目标程序运行到这条指令之后就会触发SIGTRAP信号，gdb捕获到这个信号，根据目标程序当前停止位置查询gdb维护的断点链表，若发现在该地址确实存在断点，则可判定为断点命中。

内核是通过如下调用进入内核态的：

SYSCALL_DEFINE4(ptrace, long, request, long, pid, long, addr, long, data)

根据不同的request调用不同函数，基本都是判断当前进程task中ptrace选项，走security_ptrace函数，在linux security模块中，然后汇编。


### [单例是如何实现的](https://www.cnblogs.com/sunchaothu/p/10389842.html)

- 必要条件：
    - 私有化构造函数，防止外界创建单例类的对象
    - 线程安全
    - 禁止赋值和拷贝
    - 提供一个公开的静态方法，用于获取类的唯一实例
- 方法1：懒汉式
    - 通过调用静态成员函数来获取静态成员变量，第一次调用时才初始化这个静态成员变量，这叫做延迟初始化
    - 存在线程安全和内存泄露的问题
- 方法2：线程安全、内存安全的懒汉式
    - 加锁实现线程安全，共享指针实现内存安全
- 方法3：局部静态变量
    - 最推荐的方式


### [如何在多个文件中共享全局变量？](https://www.learncpp.com/cpp-tutorial/sharing-global-constants-across-multiple-files-using-inline-variables/)

在头文件中定义内联变量，在其他需要这个全局变量的文件中include这个头文件

