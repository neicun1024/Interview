// 线程安全、内存安全的懒汉式单例
#include <iostream>
#include <mutex>
#include <memory>
using namespace std;

class Singleton{
public:
    typedef shared_ptr<Singleton> Ptr;  // 这个的访问级别必须是public，不然main函数中不能用
    ~Singleton(){
        cout<<"destructor called!"<<endl;
    };
    Singleton(const Singleton&)=delete;
    Singleton& operator=(const Singleton&)=delete;

    static Ptr getInstance()
    {
        if(m_instance==nullptr){
            lock_guard<mutex> lock(m);
            if(m_instance==nullptr){
                m_instance = Ptr(new Singleton());
            }
        }
        return m_instance;
    }

private:
    static Ptr m_instance;  // 由于Ptr在public中定义，所以public得放在private前面
    static mutex m;
    Singleton(){
        cout<<"constructor called!"<<endl;
    };
};

// 初始化两个静态成员变量
Singleton::Ptr Singleton::m_instance = nullptr;
mutex Singleton::m;

int main(){
    Singleton::Ptr m_instance = Singleton::getInstance();
    Singleton::Ptr m_instance2 = Singleton::getInstance();

    return 0;
}