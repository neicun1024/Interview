// �̰߳�ȫ���ڴ氲ȫ������ʽ����
#include <iostream>
#include <mutex>
#include <memory>
using namespace std;

class Singleton{
public:
    typedef shared_ptr<Singleton> Ptr;  // ����ķ��ʼ��������public����Ȼmain�����в�����
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
    static Ptr m_instance;  // ����Ptr��public�ж��壬����public�÷���privateǰ��
    static mutex m;
    Singleton(){
        cout<<"constructor called!"<<endl;
    };
};

// ��ʼ��������̬��Ա����
Singleton::Ptr Singleton::m_instance = nullptr;
mutex Singleton::m;

int main(){
    Singleton::Ptr m_instance = Singleton::getInstance();
    Singleton::Ptr m_instance2 = Singleton::getInstance();

    return 0;
}