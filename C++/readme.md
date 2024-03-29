# 主要记录了C++方面的内容

## 指针
* 类中的this指针是右值

## assert()
断言，是宏，而非函数。可以使用NDEBUG来关闭assert，但是需要在源代码的开头，在include<assert.h>之前
```
#define NDEBUG          // 加上这行，则 assert 不可用
#include <assert.h>

assert( p != NULL );    // assert 不可用
```

## #pragma pack(n)
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

## extern "C"
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

## struct和typedef struct
若定义了与struct同名的函数后，该struct只代表函数，不代表结构体

## struct和class的区别
1. struct一般用于描述一个数据结构集合，而class是对一个对象数据的封装；
2. 默认的继承访问权限不同。struct是public的，class是private的；
3. 默认的数据访问权限不同。struct是public的，class是private的
4. class关键字可以用于定义模板参数，struct不行；

## union
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


## 用C实现C++的面向对象特性（封装、继承、多态）

[How world one write object-oriented code in C?](https://stackoverflow.com/a/351745)
* 封装：使用函数指针把属性与方法封装到结构体中
* 继承：结构体嵌套
* 多态：父类与子类的函数指针不同


## friend 友元类和友元函数
* 能访问私有成员
* 破坏封装性
* 友元关系不可传递
* 友元关系的单向性
* 友元声明的形式及数量不受限制


## static_cast
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

## dynamic_cast
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


## const_cast

主要有如下几种用法：
1. 将常量指针转换为非常量指针
2. 将常量引用转换为非常量引用
3. 添加const属性

注意：常量对象（或基本类型）不可以被转换为非常量对象（或基本类型）


## reinterpret_cast

允许将任意指针转换到其他指针类型，也允许做任意整数类型和任意指针类型之间的转换。转换时，执行的是逐个比特复制的操作。

主要有如下几种用法：
1. 从指针类型到一个足够大的整数类型
2. 从整数类型或者枚举类型到指针类型
3. 从一个指向函数的指针到另一个不同类型的指向函数的指针
4. 从一个指向对象的指针到另一个不同类型的指向对象的指针
5. 从一个指向类函数成员的指针到另一个指向不同类型的函数成员的指针
6. 从一个指向类数据成员的指针到另一个指向不同类型的数据成员的指针


### 指针类型

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

### 引用类型

因为不存在所谓空引用，所以引用类型的dynamic_cast转换与指针类型不同，在引用转换失败时，会抛出std::bad_cast异常，该异常定义在头文件typeinfo中。
```
Base b;
const Base &rb = b;
try
{
    const Derived &rd = dynamic_cast<const Derived &>(rb);  // 下行转换
    cout << typeid(rd).name() << endl;
}
catch (std::bad_cast)   //处理类型转换失败的情况
{
    cout << "bad_cast!" << endl;
}
// 结果输出"bad_cast!"
```

*尽量少使用转型操作，尤其是dynamic_cast，耗时较高，会导致性能的下降，尽量使用其他方法替代。*


## 异常处理
程序的错误大致可分为三种，分别是语法错误、逻辑错误和运行时错误
* 语法错误：在编译和链接阶段就能发现，只有 100% 符合语法规则的代码才能生成可执行程序。语法错误是最容易发现、最容易定位、最容易排除的错误，程序员最不需要担心的就是这种错误
* 逻辑错误：编写的代码思路有问题，不能够达到最终的目标，这种错误可以通过调试来解决
* 运行时错误：指程序在运行期间发生的错误，例如除数为 0、内存分配失败、数组越界、文件不存在等。C++ 异常（Exception）机制就是为解决运行时错误而引入的


## [左值引用和右值引用](https://www.jianshu.com/p/d19fc8447eaa)
右值引用可实现转移语义（Move Sementics）和[完美转发](https://blog.csdn.net/xiangbaohui/article/details/103673177)（Perfect Forwarding），它的主要目的有两个方面：
* 消除两个对象交互时不必要的对象拷贝，节省运算存储资源，提高效率
* 能够更简洁明确地定义泛型函数

引用折叠
* type& &、type& &&、type&& & 可折叠成 type&
* type&& && 可折叠成 type&&


## 成员初始化列表
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

## 多态的分类及实现
* 重载多态（编译期）：函数重现、运算符重载
* 子类型多态（运行期）：虚函数
* 参数多态性（编译器）：类模板、函数模板
* 强制多态（编译期/运行期）：基本类型转换、自定义类型转换

### 动态多态
动态绑定：当使用基类的引用或指针调用一个虚函数时将发生动态绑定

**注意**：
* 可以将派生类的对象赋值给基类的指针或引用，反之不可
* 普通函数（非类成员函数）不能是虚函数
* 静态函数（static）不能是虚函数
* 构造函数不能是虚函数（因为在调用构造函数时，虚表指针并没有在对象的内存空间中，必须要构造函数调用完成后才会形成虚表指针）
* 内联函数不能是表现多态性时的虚函数

## 虚表指针初始化

在初始化列表前初始化。

### 无继承时
1. 分配内存
2. 初始化列表之前赋值虚表指针
3. 列表初始化
4. 执行构造函数体

### 有继承时
1. 分配内存
2. 基类构造过程（按照无继承来）
3. 初始化子类虚表指针
4. 子类列表初始化
5. 执行子类构造函数体

## 虚继承

## 虚继承和虚函数

## 模板类、成员模板、虚函数
* 模板类中可以使用虚函数
* 一个类（无论是普通类还是类模板）的成员模板（本身是模板的成员函数）不能是虚函数

## 抽象类、接口类、聚合类
* 抽象类：含有纯虚函数的类
* 接口类：仅含有纯虚函数的抽象类
* 聚合类：用户可以直接访问其成员，并且具有特殊的初始化语法形式。满足如下特点：
    * 所有成员都是 public
    * 没有定义任何构造函数
    * 没有类内初始化
    * 没有基类，也没有 virtual 函数

## 内存分配和管理

### malloc、calloc、realloc、alloca
* malloc：申请指定字节数的内存。申请到的内存中的初始值不确定
* calloc：为指定长度的对象，分配能容纳其指定个数的内存。申请到的内存的每一位（bit）都初始化为 0
* realloc：更改以前分配的内存长度（增加或减少）。当增加长度时，可能需将以前分配区的内容移到另一个足够大的区域，而新增区域内的初始值则不确定
* alloca：在栈上申请内存。程序在出栈的时候，会自动释放内存。但是需要注意的是，alloca 不具可移植性, 而且在没有传统堆栈的机器上很难实现。alloca 不宜使用在必须广泛移植的程序中。C99 中支持变长数组 (VLA)，可以用来替代 alloca

### placement new
placement new 允许我们向 new 传递额外的地址参数，从而在预先指定的内存区域创建对象
```
new (place_address) type
new (place_address) type (initializers)
new (place_address) type [size]
new (place_address) type [size] { braced initializer list }
```
* place_address 是个指针
* initializers 提供一个（可能为空的）以逗号分隔的初始值列表

## delete this 合法吗？
合法，但是：
1. 必须保证 this 对象是通过 new（不是 new[]、不是 placement new、不是栈上、不是全局、不是其他对象成员）分配的
2. 必须保证调用 delete this 的成员函数是最后一个调用 this 的成员函数
3. 必须保证成员函数的 delete this 后面没有调用 this 了
4. 必须保证 delete this 后没有人使用了

## 如何定义一个只能在堆上（栈上）生成对象的类？
### 只能在堆上
* 方法1：将析构函数设置为私有

    原因：
    * 非静态对象建立在栈上，是由编译器分配和释放内存空间，编译器为对象分配内存空间时，会对类的非静态函数进行检查，即编译器会检查析构函数的访问性。当析构函数设为私有时，编译器创建的对象就无法通过访问析构函数来释放对象的内存空间，因此，编译器不会在栈上为对象分配内存。

    存在的问题：
    * 用 new 创建的对象，通常会使用 delete 释放该对象的内存空间，但此时类的外部无法调用析构函数，因此类内必须定义一个 destory() 函数，用来释放 new 创建的对象
    * 无法解决继承问题，因为如果这个类作为基类，析构函数要设置成 virtual，然后在派生类中重写该函数，来实现多态。但此时，析构函数是私有的，派生类中无法访问
* 方法2：构造函数设置为 protected，并提供一个 public 的静态函数来完成构造，而不是在类的外部使用 new 构造；将析构函数设置为 protected

    原因：
    类似于单例模式，也保证了在派生类中能够访问析构函数。通过调用 create() 函数在堆上创建对象。

### 只能在栈上
* 方法：将 operator new() 设置为私有

    原因：
    * 当对象建立在堆上时，是采用 new 的方式进行建立，其底层会调用 operator new() 函数，因此只要对该函数加以限制，就能够防止对象建立在堆上。


## 函数调用过程

[函数调用过程-简要](https://www.cnblogs.com/biyeymyhjob/archive/2012/07/20/2601204.html)

[函数调用过程-详细](https://cloud.tencent.com/developer/article/1814613)

### 函数调用涉及到的寄存器
- EIP：指令指针，指向下一条即将执行的指令的地址
- EBP：基址指针，指向栈底
- ESP：栈指针，指向栈顶
- EBX：基址寄存器
- ESI：源变址寄存器
- EDI：目的变址寄存器

### 过程
![20220307193704](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220307193704.png)
1. 函数调用
![20220307200307](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220307200307.png)
   1. 将参数从右往左入栈
2. 保护现场
   ![20220307193814](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220307193814.png)
   1. 将函数的返回地址入栈（编译器隐式地执行**push eip**指令）
   2. 将ebp入栈（因为每个函数都有自己的栈区域，所以栈基址也是不一样的，现在进入了一个中断函数，函数执行过程中也需要ebp寄存器）
   3. 将栈顶地址作为该函数的栈基址（将栈底指针指向栈顶指针所指的地址），确定被调用的函数的栈区域（ebp为栈底，esp为栈顶）
   4. 将栈顶指针往上移动48h来将两个函数的栈区域隔开一段距离，该间隔区域的大小固定为40h，即64Byte，另外8h为预留出的存储局部变量的区域
   ![20220307200138](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220307200138.png)
   1. 将EBX、ESI、EDI入栈
   2. 将留出的48h的内存区域赋值为0xCCCCCCCCh
3. 执行子函数
   ![20220307193836](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220307193836.png)
   1. 根据ebp来计算被调用的函数中的局部变量的内存地址，并存入相应位置
4. 恢复现场
   ![20220307201041](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220307201041.png)
   1. 三条出栈指令，将EDI、ESI、EBX出栈
   2. 将ebp的值赋给esp（让esp跳过了一段区域，就是间隔区域和局部数据区域，因为函数已经退出，所以这两个区域没有用了）
   3. 将ebp出栈
   4. 将函数的返回地址出栈（编译器隐式地执行**pop eip**指令）
   5. 将栈顶指针esp回到函数调用前的位置（让栈顶指针往下移动12Byte，即三个参数）


### [C语言函数参数压栈顺序为什么是从右往左](https://blog.csdn.net/jiange_zh/article/details/47381597?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1.pc_relevant_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1.pc_relevant_default&utm_relevant_index=2)

C语言支持不定参函数，也就是函数的参数数量可变。比如printf()函数，原型是`printf(const char* format, ...)`，它是一个不定参函数。我们在实际使用中是通过format中的%占位符来确定参数的个数的。

如果从左往右压栈，那么函数调用时，format最先进栈，之后是各个参数进栈。此时，format上面压着未知个数的参数，想知道参数的个数，必须找到format，而想找到format，则必须知道参数的个数。。

所以参数必须从右往左入栈，这样就能找到format，取得后面的参数个数。


## 内联函数

### 原理
- 内联函数不是在调用时发生控制转移关系，而是在编译阶段将函数体嵌入到每一个调用该函数的语句块中，编译器会将程序中出现内联函数的调用表达式用内联函数的函数体来替换
- 普通函数是将程序执行转移到被调用函数所存放的内存地址，当函数执行完后，返回到执行此函数前的地方。转移操作需要保护现场，被调函数执行完后，再恢复现场，该过程需要较大的资源开销

### 和宏定义的区别
1. 内联函数是在编译时展开，而宏在编译预处理时展开；在编译的时候，内联函数直接被嵌入到目标代码中去，而宏只是一个简单的文本替换
2. 内联函数是真正的函数，和普通函数调用的方法一样，在调用点处直接展开，避免了函数的参数压栈操作，减少了调用的开销。而宏定义编写较为复杂，常需要增加一些括号来避免歧义
3. 宏定义只进行文本替换，不会对参数的类型、语句能否正常编译等进行检查。而内联函数是真正的函数，会对参数的类型、函数体内的语句编写是否正确等进行检查

### 优点
1. 内联函数同宏函数一样将在被调用处进行代码展开，省去了参数压栈、栈帧开辟与回收，结果返回等，从而提高程序运行速度
2. 内联函数相比宏函数来说，在代码展开时，会做安全检查或自动类型转换（同普通函数），而宏定义则不会
3. 在类声明中定义的函数，除了虚函数的其他函数都会自动隐式地当成内联函数，因此内联函数可以访问类的成员变量，宏定义则不能
4. 内联函数在运行时可调试，而宏定义不可以

### 缺点
1. 代码膨胀。内联是以代码膨胀（复制）为代价，消除函数调用带来的开销。如果执行函数体内代码的时间，相比于函数调用的开销较大，那么效率的收获会很少。另一方面，每一处内联函数的调用都要复制代码，将使程序的总代码量增大，消耗更多的内存空间
2. **inline 函数无法随着函数库升级而升级。inline函数的改变需要重新编译，不像 non-inline 可以直接链接**
3. **是否内联，程序员不可控。内联函数只是对编译器的建议，是否对函数内联，决定权在于编译器**

## 类中的内联函数
1. 类中的函数会自动转换为内联函数（内联函数只是对编译器的建议，是否对函数内联，决定权在于编译器），在类内定义为隐式内联，在类内声明、类外定义需要显示内联，定义时写上inline
```
// 声明1（加 inline，建议使用）
inline int functionName(int first, int second,...);

// 声明2（不加 inline）
int functionName(int first, int second,...);

// 定义
inline int functionName(int first, int second,...) {/****/};

// 类内定义，隐式内联
class A {
    int doA() { return 0; }         // 隐式内联
}

// 类外定义，需要显式内联
class A {
    int doA();
}
inline int A::doA() { return 0; }   // 需要显式内联
```

## 虚函数可以是内联函数吗？
1. 虚函数可以是内联函数，内联是可以修饰虚函数的，但是当虚函数表现多态性的时候不能内联
2. 内联函数是在编译期建议编译器内联，而虚函数的多态性在运行期，编译器无法知道运行期调用哪个代码，因此虚函数表现为多态性时不可以内联
3. inline virtual唯一可以内联的时候是：编译器知道所调用的对象是哪个类（如Base::who()），这只有在编译期具有实际对象而不是对象的指针或引用时才会发生
```
#include <iostream>  
using namespace std;
class Base
{
public:
	inline virtual void who()
	{
		cout << "I am Base\n";
	}
	virtual ~Base() {}
};
class Derived : public Base
{
public:
	inline void who()  // 不写inline时隐式内联
	{
		cout << "I am Derived\n";
	}
};

int main()
{
	// 此处的虚函数 who()，是通过类（Base）的具体对象（b）来调用的，编译期间就能确定了，所以它可以是内联的，但最终是否内联取决于编译器。 
	Base b;
	b.who();

	// 此处的虚函数是通过指针调用的，呈现多态性，需要在运行时期间才能确定，所以不能为内联。  
	Base *ptr = new Derived();
	ptr->who();

	// 因为Base有虚析构函数（virtual ~Base() {}），所以 delete 时，会先调用派生类（Derived）析构函数，再调用基类（Base）析构函数，防止内存泄漏。
	delete ptr;
	ptr = nullptr;

	system("pause");
	return 0;
} 
```

## ++i和i++的区别
1. i++返回原来的值，++i返回加一后的值；
2. i++不能作为左值，++i可以作为左值；

在汇编代码上，i++相比++i多了一条指令：在对i执行自加操作之前，先保存i的当前值留作稍后使用。

不同的编译器的结果可能不同，而且现代的编译器能够根据上下文编译C语言代码，如果编译器没有必要保存i的当前值，就会把这一步优化掉。

- 对于基本类型：i++和++i差别很小
- 对于自定义类型：i++比++i多了一次构造函数的调用


## lambda表达式
lambda表达式是C++11中最重要也最常用的特性之一，用于定义匿名函数，并且可以捕获一定范围内的变量

语法形式： \[capture](params) mutable exception->ret{body};

其中capture是捕获列表，params是参数表，mutable用来说明是否可以修改捕获的变量，exception是异常设定，ret是返回值类型，body是函数体；

### 优点
- 声明式编程风格：就地匿名定义目标函数或函数对象，不需要额外写一个命名函数或者函数对象。以更直接的方式去写程序，好的可读性和可维护性
- 简洁：不需要额外再写一个函数或者函数对象，避免了代码膨胀和功能分散
- 在需要的时间和地点实现功能闭包，使程序更灵活


### lambda表达式的各个部分
- 捕获列表
- 参数列表（选）
- mutable（选）
- 异常属性（选）
- 返回类型（选）
- lambda body

### 使用lambda表达式捕获列表
- [] 不捕获任何变量
- [&] 捕获外部作用域中所有变量，并作为引用在函数体中使用（按引用捕获）
- [=] 捕获外部作用域中所有变量，并作为副本在函数体中使用（按值捕获）
- [=, &foo] 按值捕获外部作用域中所有变量，并按引用捕获foo变量
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

### lambda表达式的类型

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

需要注意的是，没有捕获变量的 lambda表达式可以直接转换为函数指针，而捕获变量的lambda表达式则不能转换为函数指针。看看下面的代码：
```
typedef void(*Ptr)(int*);
Ptr p = [](int* p){delete p;};  // 正确，没有状态的lambda（没有捕获）的lambda表达式可以直接转换为函数指针
Ptr p1 = [&](int* p){delete p;};  // 错误，有状态的lambda不能直接转换为函数指针
```
上面第二行代码能编译通过，而第三行代码不能编译通过，因为第三行的代码捕获了变量，不能直接转换为函数指针。


## GDB工作原理和内核实现
[参考](https://zhuanlan.zhihu.com/p/336922639)

gdb主要功能的实现依赖于一个系统函数ptrace，通过man手册可以了解到，ptrace可以让父进程观察和控制其子进程的检查、执行，改变其寄存器和内存的内容，主要应用于打断点（也是gdb的主要功能）和打印系统调用轨迹。

### 一、ptrace函数
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


### 二、gdb使用ptrace的基本流程
1. gdb调试一个新进程：通过fork函数创建一个新进程，在子进程中执行ptrace(PTRACE_TRACEME, 0, 0, 0)函数，然后通过execv()调用准备调试的程序。
2. attach到已运行进程：将pid传递给gdb，然后执行ptrace(PTRACE_ATTACH, pid, 0, 0)。

在使用参数为PTRACE_TRACEME或PTRACE_ATTACH的ptrace系统调用建立调试关系之后，交付给目标程序的任何信号（除SIGKILL之外）都将被gdb先行截获，gdb因此有机会对信号进行相应处理，并根据信号的属性决定在继续目标程序运行时是否将之前截获的信号实际交付给目标程序。

### 三、gdb使用的内核机制
断点的功能是通过内核信号实现的，以x86为例，内核向某个地址打入断点，实际上就是往该地址写入断点指令INT 3，即0xCC。目标程序运行到这条指令之后就会触发SIGTRAP信号，gdb捕获到这个信号，根据目标程序当前停止位置查询gdb维护的断点链表，若发现在该地址确实存在断点，则可判定为断点命中。

内核是通过如下调用进入内核态的：

SYSCALL_DEFINE4(ptrace, long, request, long, pid, long, addr, long, data)

根据不同的request调用不同函数，基本都是判断当前进程task中ptrace选项，走security_ptrace函数，在linux security模块中，然后汇编。


## [单例是如何实现的](https://www.cnblogs.com/sunchaothu/p/10389842.html)

- 必要条件：
    - 私有化构造函数，防止外界创建单例类的对象
    - 线程安全
    - 禁止赋值和拷贝
    - 提供一个公开的静态方法，用于获取类的唯一实例
- 方法1：懒汉式
    - 通过调用静态成员函数来获取静态成员变量，第一次调用时才初始化这个静态成员变量，这叫做延迟初始化
    - 存在线程安全和内存泄露的问题
- 方法2：线程安全、内存安全的懒汉式
    - 加锁实现线程安全，智能指针实现内存安全
- 方法3：局部静态变量
    - 最推荐的方式


## [如何在多个文件中共享全局变量？](https://www.learncpp.com/cpp-tutorial/sharing-global-constants-across-multiple-files-using-inline-variables/)

在头文件中定义内联变量，在其他需要这个全局变量的文件中include这个头文件


## 回调函数（callback）

### 使用回调函数的场景
有这样一个程序，由两行组成：
```
res = request();    // 调用某个函数，获取结果res
handle(res);        // 处理获取到的结果res
```
这就是函数的同步调用，必须等待request()函数执行完，才能调用handle()函数进行处理。

而使用异步回调的话，程序会变成这样：
```
request(handle);    // 将handle()函数作为参数传给request()函数
```
我们不需要关心request()函数什么时候获取结果res，只需要将获取结果res后该怎么做告诉request就行了。

### 什么是回调函数
![20220405210131](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220405210131.png)

当程序跑起来时，一般情况下，应用程序（application program）会时常通过API调用库里所预先备好的函数。但是有些库函数（library function）却要求应用先传给它一个函数，好在合适的时候调用，以完成目标任务。这个被传入的、后又被调用的函数就称为回调函数（callback function）。

### 回调的类型
1. 同步回调：也叫阻塞式回调。当我们调用某个函数A并以参数的形式传入回调函数后，在A返回之前回调函数会被执行，也就是说我们的主程序会等待回调函数执行完成。
   
![20220405210900](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220405210900.png)

2. 异步回调
   
![20220405211620](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220405211620.png)

### 异步回调的优点
1. 在同步模式下，服务调用方会因等待的服务执行而暂停，导致整个线程被阻塞，不适合并发连接的场景，而异步回调解决了这个问题
2. 程序更加灵活


## [malloc底层实现原理](https://blog.csdn.net/mmshixing/article/details/51679571)
[C++动态内存分配](https://blog.csdn.net/qq_25065595/article/details/107453890)

![20220407183918](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220407183918.png)

这是32位系统，寻址空间是4G，linux系统下0-3G是用户模式，3-4G是内核模式。而在用户模式下又分为代码段、.data段、.bss段、堆、栈。

heap段有个重要的标志：program break。Linux维护一个break指针，这个指针指向堆空间的某个地址。从堆起始地址到break之间的地址空间为映射好的，可以供进程访问；而从break往上，是未映射的地址空间，如果访问这段空间则程序会报错。我们用malloc进行内存分配就是从break往上进行的。

进程所面对的虚拟内存地址空间，只有按页映射到物理内存地址，才能真正使用。受物理存储容量限制，整个堆虚拟内存空间不可能全部映射到实际的物理内存。Linux对堆的管理示意如下：

![20220407184911](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220407184911.png)

获取了break地址，也就是内存申请的初始地址，下面是malloc的整体实现方案：

malloc 函数的实质是它有一个将可用的内存块连接为一个长长的列表的所谓空闲链表。 调用 malloc（）函数时，它沿着连接表寻找一个大到足以满足用户请求所需要的内存块。 然后，将该内存块一分为二（一块的大小与用户申请的大小相等，另一块的大小就是剩下来的字节）。 接下来，将分配给用户的那块内存存储区域传给用户，并将剩下的那块（如果有的话）返回到连接表上。

调用 free 函数时，它将用户释放的内存块连接到空闲链表上。 到最后，空闲链会被切成很多的小内存片段，如果这时用户申请一个大的内存片段， 那么空闲链表上可能没有可以满足用户要求的片段了。于是，malloc（）函数请求延时，并开始在空闲链表上检查各内存片段，对它们进行内存整理，将相邻的小空闲块合并成较大的内存块。

### 1. malloc分配内存前的初始化：
malloc_init 是初始化内存分配程序的函数。 它完成以下三个目的：将分配程序标识为已经初始化，找到操作系统中最后一个有效的内存地址，然后建立起指向需要管理的内存的指针。

这里需要用到三个全局变量:
- int has_initialized = 0; /* 初始化标记 */
- void *managed_memory_start; /* 管理内存起始地址 */
- void *last_valid_address; /* 操作系统的最后一个有效地址*/

被映射的内存边界（操作系统最后一个有效地址）常被称为系统中断点或者当前中断点。为了指出当前系统中断点，必须使用 sbrk(0) 函数。 sbrk 函数根据参数中给出的字节数移动当前系统中断点，然后返回新的系统中断点。 使用参数 0 只是返回当前中断点。 这里给出 malloc（）初始化代码，它将找到当前中断点并初始化所需的变量：

Linux通过brk和sbrk系统调用操作break指针。两个系统调用的原型如下：
```
int brk(void *addr);
void *sbrk(intptr_t increment);
```

- brk将break指针直接设置为某个地址，执行成功时返回0，否则返回-1并设置errno为ENOMEM；
- sbrk将break从当前位置移动increment所指定的增量，成功时返回break移动之前所指向的地址，否则返回(void *)-1。如果将increment设置为0，则可以获得当前break的地址。

### 2. 下为malloc_init()代码：可以看到使用sbrk(0)来获得break地址。
```
#include <unistd.h> /*sbrk 函数所在的头文件 */
void malloc_init()
{
    last_valid_address = sbrk(0); /* 用 sbrk 函数在操作系统中
    取得最后一个有效地址 */
    managed_memory_start = last_valid_address; /* 将 最 后 一 个
    有效地址作为管理内存的起始地址 */
    has_initialized = 1; /* 初始化成功标记 */
}
```

### 3. 内存块的获取

所要申请的内存是由多个内存块构成的链表。

1. 内存块的大致结构：每个块由meta区和数据区组成，meta区记录数据块的元信息（数据区大小、空闲标志位、指针等等），数据区是真实分配的内存区域，并且数据区的第一个字节地址即为malloc返回的地址。
```
typedef struct s_block *t_block;
struct s_block {
    size_t size;    /* 数据区大小 */
    t_block next;   /* 指向下个块的指针 */
    int free;       /* 是否是空闲块 */
    int padding;    /* 填充4字节，保证meta块长度为8的倍数 */
    char data[1];   /* 这是一个虚拟字段，表示数据块的第一个字节，长度不应计入meta */
};
```

现在，为了完全地管理内存，我们需要能够追踪要分配和回收哪些内存。在对内存块进行了 free 调用之后，我们需要做的是诸如将它们标记为未被使用的等事情，并且，在调用 malloc 时，我们要能够定位未被使用的内存块。因此， malloc 返回的每块内存的起始处首先要有这个结构：
```
struct mem_control_block
{	
	int is_available;//是否空闲
	int size; //内存块大小
};
```

2. 寻找合适的block

现在考虑如何在block链中查找合适的block。一般来说有两种查找算法：
- First fit：从头开始，使用第一个数据区大小大于要求size的块所谓此次分配的块
- Best fit：从头开始，遍历所有块，使用数据区大小大于size且差值最小的块作为此次分配的块

两种方法各有千秋，best fit具有较高的内存使用率（payload较高），而first fit具有更好的运行效率。

find_block从frist_block开始，查找第一个符合要求的block并返回block起始地址，如果找不到这返回NULL。这里在遍历时会更新一个叫last的指针，这个指针始终指向当前遍历的block。这是为了如果找不到合适的block而开辟新block使用的。

3. 如果现有block都不能满足size的要求，则需要在链表最后开辟一个新的block。

下为利用sbrk()创建新的block示意代码：
```
#define BLOCK_SIZE 24 /* 由于存在虚拟的data字段，sizeof不能正确计算meta长度，这里手工设置 */
 
t_block extend_heap(t_block last, size_t s) {
    t_block b;
    b = sbrk(0);
    if(sbrk(BLOCK_SIZE + s) == (void *)-1){
        return NULL;
    }
    b->size = s;
    b->next = NULL;
    if(last){
        last->next = b;
    }
    b->free = 0;
    return b;
}
```

### 4. 内存分配，下为malloc的实现源码
```
/**内存控制块数据结构，用于管理所有的内存块
* is_available: 标志着该块是否可用。1表示可用，0表示不可用
* size: 该块的大小
**/
struct mem_control_block {
    int is_available;
    int size;
};
 
/**在实现malloc时要用到linux下的全局变量
*managed_memory_start：该指针指向进程的堆底，也就是堆中的第一个内存块
*last_valid_address：该指针指向进程的堆顶，也就是堆中最后一个内存块的末地址
**/
void *managed_memory_start;
void *last_valid_address;
 
/**malloc()功能是动态的分配一块满足参数要求的内存块
*numbytes：该参数表明要申请多大的内存空间
*返回值：函数执行结束后将返回满足参数要求的内存块首地址，要是没有分配成功则返回NULL
**/
void *malloc(size_t numbytes) {
    //游标，指向当前的内存块
    void *current_location;
    //保存当前内存块的内存控制结构
    struct mem_control_block *current_location_mcb;
    //保存满足条件的内存块的地址用于函数返回
    void *memory_location;
    memory_location = NULL;
    //计算内存块的实际大小，也就是函数参数指定的大小+内存控制块的大小
    numbytes = numbytes + sizeof(struct mem_control_block);
    //利用全局变量得到堆中的第一个内存块的地址
    current_location = managed_memory_start;
 
    //对堆中的内存块进行遍历，找合适的内存块
    while (current_location != last_valid_address) //检查是否遍历到堆顶了
    {
        //取得当前内存块的内存控制结构
        current_location_mcb = (struct mem_control_block*)current_location;
        //判断该块是否可用
        if (current_location_mcb->is_available)
            //检查该块大小是否满足
            if (current_location_mcb->size >= numbytes)
            {
                //满足的块将其标志为不可用
                current_location_mcb->is_available = 0;
                //得到该块的地址，结束遍历
                memory_location = current_location;
                break;
            }
        //取得下一个内存块
        current_location = current_location + current_location_mcb->size;
    }
 
    //在堆中已有的内存块中没有找到满足条件的内存块时执行下面的函数
    if (!memory_location)
    {
        //向操作系统申请新的内存块
        if (sbrk(numbytes) == -1)
            return NULL;//申请失败，说明系统没有可用内存
        memory_location = last_valid_address;
        last_valid_address = last_valid_address + numbytes;
        current_location_mcb = (struct mem_control_block)memory_location;
        current_location_mcb->is_available = 0;
        current_location_mcb->size = numbytes;
    }
    //到此已经得到所要的内存块，现在要做的是越过内存控制块返回内存块的首地址
    memory_location = memory_location + sizeof(struct mem_control_block);
    return memory_location;
}
```

### 5. free的实现源码
```
/**free()功能是将参数指向的内存块进行释放
*firstbyte：要释放的内存块首地址
*返回值：空
**/
void free(void *firstbyte)
{
    struct mem_control_block *mcb;
    //取得该块的内存控制块的首地址
    mcb = firstbyte - sizeof(struct mem_control_block);
    //将该块标志设为可用
    mcb->is_available = 1;
    return;
}
```


## [内存池](https://zhuanlan.zhihu.com/p/64719710)

### 为什么需要内存池？

C++程序默认的内存管理（new，delete，malloc，free）会频繁地在堆上分配和释放内存，导致性能损失，产生大量的内存碎片，降低内存的利用率。默认的内存管理因为被设计的比较通用，所以在性能上并不能做到极致。

因此，很多时候需要根据业务需求设计专用内存管理器，便于针对特定数据结构和使用场合的内存管理，比如：内存池。

### 内存池原理

内存池的思想是，在真正使用内存之前，预先申请分配一定数量、大小预设的内存块留作备用。当有新的内存需求时，就从内存池中分出一部分内存块，若内存块不够再继续申请新的内存，当内存释放后就回归到内存块留作后续的复用，使得内存使用效率得到提升，一般也不会产生不可控制的内存碎片。

### 内存池设计

算法原理：
1. 预申请一个内存区chunk，将内存中按照对象大小划分成多个内存块block
2. 维持一个空闲内存块链表，通过指针相连，标记头指针为第一个空闲块
3. 每次新申请一个对象的空间，则将该内存块从空闲链表中去除，更新空闲链表头指针
4. 每次释放一个对象的空间，则重新将该内存块加到空闲链表头
5. 如果一个内存区占满了，则新开辟一个内存区，维持一个内存区的链表，同指针相连，头指针指向最新的内存区，新的内存块从该区内重新划分和申请

![20220514155756](https://raw.githubusercontent.com/neicun1024/PicBed/main/images_for_markdown/20220514155756.png)


## [静态链接和动态链接](https://blog.csdn.net/kang___xi/article/details/80210717)

## [虚继承实现原理（虚基类表指针与虚基类表）](https://blog.csdn.net/longlovefilm/article/details/80558879)

## 为什么移动构造函数比拷贝构造函数快

https://blog.51cto.com/u_15127568/3455515

https://blog.csdn.net/qq_36991505/article/details/105193717

```
class Test
{
public:
    int *p;
    Test(const Test& t) // 拷贝构造函数
    {
        p = new int(*(t.p));
    }
    Test(Test &&t)  // 移动构造函数
    {
        p = t.p;
        t.p = nullptr;
    }
}
```

从上面的代码中可以看出，拷贝构造函数是深拷贝，对于传进来的参数中的每个内存空间，都需要重新申请内存，而移动构造函数事浅拷贝，直接将传进来的参数中的内存空间的所有权转移给当前对象。


## volatile的作用

[参考1](https://www.cnblogs.com/xing901022/p/7840684.html)

[参考2](https://www.cnblogs.com/yc_sunniwell/archive/2010/07/14/1777432.html)



## [仿函数](https://blog.csdn.net/k346k346/article/details/82818801)

### 什么是仿函数？
仿函数是一个class或struct，但是实现的是函数的功能；通过重载 `operator()` 运算符来实现；

### 仿函数的作用？
普通函数A作为函数B的参数传递时，扩展性很差，当修改了函数A的参数数量时，还需要修改函数B的参数；而使用仿函数可以解决这个问题；