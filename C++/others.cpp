#include <iostream>
using namespace std;

class ConstTest // 类中const变量需要在类内声明时初始化或在初始化列表中初始化
{
private:
    const int val = 1;

public:
    ConstTest() : val(2){};
    ConstTest(int x) : val(x){};

    int getValue()
    {
        return val;
    }
    int getValue() const
    {
        return val;
    }

    void myPrint()
    {
        ConstTest c1;
        cout << c1.getValue() << endl;
        ConstTest c2(3);
        cout << c2.getValue() << endl;
    }
};

class InlineVirtual // 虚函数可以是内联函数吗？
{
private:
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
        inline void who() // 不写inline时隐式内联
        {
            cout << "I am Derived\n";
        }
    };

public:
    void myPrint()
    {
        // 此处的虚函数 who()，是通过类（Base）的具体对象（b）来调用的，编译期间就能确定了，所以它可以是内联的，但最终是否内联取决于编译器。
        Base b;
        b.who(); // I am Base

        Base *ptr_b = new Base();
        ptr_b->who(); // I am Base

        // 此处的虚函数是通过指针调用的，呈现多态性，需要在运行时期间才能确定，所以不能为内联。
        Base *ptr = new Derived();
        ptr->who(); // I am Derived

        delete ptr_b;
        ptr_b = nullptr;
        // 因为Base有虚析构函数（virtual ~Base() {}），所以 delete 时，会先调用派生类（Derived）析构函数，再调用基类（Base）析构函数，防止内存泄漏。
        delete ptr;
        ptr = nullptr;
    }
};

class PragmaTest // 设定结构体、联合以及类成员变量以 n 字节方式对齐
{
private:
#pragma pack(push) // 保存对齐状态
#pragma pack(4)    // 设定为 4 字节对齐
    class test1
    {
        char c1;
        double d;
        char c2;
        int i;
    };

    class test2
    {
        double d;
        char c1;
        char c2;
        int i;
    };
#pragma pack(pop) // 恢复对齐状态

#pragma pack(push) // 保存对齐状态
#pragma pack(8)    // 设定为 8 字节对齐
    class test3
    {
        char c1;
        double d;
        char c2;
        int i;
    };

    class test4
    {
        double d;
        char c1;
        char c2;
        int i;
    };
#pragma pack(pop) // 恢复对齐状态

public:
    void myPrint()
    {
        cout << sizeof(test1) << endl;
        cout << sizeof(test2) << endl;
        cout << sizeof(test3) << endl;
        cout << sizeof(test4) << endl;
    }
};

class BitMode // 位域
{
private:
    class test1
    {
        unsigned short nWeekDay : 3;
        unsigned short nMonthDay : 6;
        unsigned short : 0; // 宽度为0的未命名位域强制将下一个位域与下一个 类型 边界对齐，其中 type 是成员的类型。
        unsigned short nMonth : 5;
        unsigned short nYear : 8;
    };

    class test2
    {
        unsigned int nWeekDay : 3;
        unsigned int nMonthDay : 6;
        unsigned short : 0;
        unsigned int nMonth : 5;
        unsigned int nYear : 8;
    };

    class test3
    {
        int nWeekDay : 3;
        int nMonthDay : 6;
        unsigned short : 0;
        int nMonth : 5;
        int nYear : 8;
    };

public:
    void myPrint()
    {
        cout << sizeof(test1) << endl;
        cout << sizeof(test2) << endl;
        cout << sizeof(test3) << endl;
    }
};

class StructTest // struct和typedef struct
{
private:
    struct Test1
    {
        int val = 1;
        void printVal()
        {
            cout << val << endl;
        }
    };
    typedef struct Test2
    {
        int val = 2;
        void printVal()
        {
            cout << val << endl;
        }
    } T2;

    void Test1() // 当定义了和结构体Test1同名的函数后，Test1只能表示函数，要使用结构体时需要在前面加上"struct"
    {
        cout << "function Test1" << endl;
    }
    void Test2()
    {
        cout << "function Test2" << endl;
    }
    // void T2() // T2已经被定义成一个“struct Test2”的别名了，不能再定义为函数名
    // {
    //     cout << "function T1" << endl;
    // } // error: 'void Struct::T2()' conflicts with a previous declaration

public:
    void myPrint()
    {
        Test1(); // function Test1
        struct Test1 test1;
        test1.printVal(); // 1
        T2 t2;
        t2.printVal(); // 2
    }
};

class ExplicitTest
{
private:
    struct A
    {
        A(int) {}
        operator bool() const { return true; }
    };

    struct B
    {
        explicit B(int) {}
        explicit operator bool() const { return true; }
    };

    void doA(A a) {}

    void doB(B b) {}

public:
    // void myPrint()
    // {
    //     A a1(1);     // OK：直接初始化
    //     A a2 = 1;    // OK：复制初始化
    //     A a3{1};     // OK：直接列表初始化
    //     A a4 = {1};  // OK：复制列表初始化
    //     A a5 = (A)1; // OK：允许 static_cast 的显式转换
    //     doA(1);      // OK：允许从 int 到 A 的隐式转换
    //     if(a1);                          // OK：使用转换函数 A::operator bool() 的从 A 到 bool 的隐式转换
    //     bool a6(a1);                     // OK：使用转换函数 A::operator bool() 的从 A 到 bool 的隐式转换
    //     bool a7 = a1;                    // OK：使用转换函数 A::operator bool() 的从 A 到 bool 的隐式转换
    //     bool a8 = static_cast<bool>(a1); // OK ：static_cast 进行直接初始化

    //     B b1(1);     // OK：直接初始化
    //     B b2 = 1;    // 错误：被 explicit 修饰构造函数的对象不可以复制初始化
    //     B b3{1};     // OK：直接列表初始化
    //     B b4 = {1};  // 错误：被 explicit 修饰构造函数的对象不可以复制列表初始化
    //     B b5 = (B)1; // OK：允许 static_cast 的显式转换
    //     doB(1);      // 错误：被 explicit 修饰构造函数的对象不可以从 int 到 B 的隐式转换
    //     if(b1);                          // OK：被 explicit 修饰转换函数 B::operator bool() 的对象可以从 B 到 bool 的按语境转换
    //     bool b6(b1);                     // OK：被 explicit 修饰转换函数 B::operator bool() 的对象可以从 B 到 bool 的按语境转换
    //     bool b7 = b1;                    // 错误：被 explicit 修饰转换函数 B::operator bool() 的对象不可以隐式转换
    //     bool b8 = static_cast<bool>(b1); // OK：static_cast 进行直接初始化
    // }
};

class RemoveReference // remove_reference的作用是解除引用
{
public:
    void myPrint()
    {
        int a[] = {1, 2, 3};
        decltype(*a) b = a[0];
        remove_reference<decltype(*a)>::type c = a[0];
        a[0] = 4;
        cout << b << endl;
        cout << c << endl;
    }
};

class StaticCast // static_cast
{
private:
    class Base
    {
    };

    class Derived : public Base
    {
    };

public:
    void myPrint()
    {
        // 基本类型数据转换举例
        char a = 'a';
        int b = static_cast<int>(a); //正确，将char型数据转换成int型数据

        double *c = new double;
        void *d = static_cast<void *>(c); //正确，将double指针转换成void指针

        int e = 10;
        const int f = static_cast<const int>(e); //正确，将int型数据转换成const int型数据

        // const int g = 20;
        // int *h = static_cast<int *>(&g); //编译错误，static_cast不能转换掉g的const属性

        cout << typeid(a).name() << endl;
        cout << typeid(b).name() << endl;
        cout << typeid(c).name() << endl;
        cout << typeid(d).name() << endl;
        cout << typeid(e).name() << endl;
        cout << typeid(f).name() << endl;
        // cout << typeid(g).name() << endl;
        // cout << typeid(h).name() << endl;

        // 类的上行和下行转换举例
        Base *pB = new Base();
        cout << typeid(pB).name() << endl;
        if (Derived *pD = static_cast<Derived *>(pB))
        {
            cout << typeid(pD).name() << endl;
        } //下行转换是不安全的(坚决抵制这种方法)

        Derived *pD = new Derived();
        cout << typeid(pD).name() << endl;
        if (Base *pB = static_cast<Base *>(pD))
        {
            cout << typeid(pB).name() << endl;
        } //上行转换是安全的

        cout << typeid(pB).name() << endl;
        cout << typeid(pD).name() << endl;
    }
};

class DynamicCast // dynamic_cast
{
};

int main()
{
    // ConstTest constTest;
    // constTest.myPrint();

    // InlineVirtual inlineVirtual;
    // inlineVirtual.myPrint();

    // PragmaTest pragmaTest;
    // pragmaTest.myPrint();

    // BitMode bitMode;
    // bitMode.myPrint();

    // StructTest structTest;
    // structTest.myPrint();

    // ExplicitTest explicitTest;
    // explicitTest.myPrint();

    // RemoveReference removeReference;
    // removeReference.myPrint();

    StaticCast staticCast;
    staticCast.myPrint();

    return 0;
}