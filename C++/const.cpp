#include <iostream>
using namespace std;

class A
{
private:
    const int val=1;

public:
    A(): val(2){};
    A(int x): val(x) {};

    int getValue(){
        return val;
    }
    int getValue()const{
        return val;
    }
};


int main(){
    A a1;
    cout<<a1.getValue()<<endl;
    A a2(3);
    cout<<a2.getValue()<<endl;
    return 0;
}