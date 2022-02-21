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
        void printVal(){
            cout << val << endl;
        }
    };
    typedef struct Test2
    {
        int val = 2;
        void printVal(){
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

    return 0;
}