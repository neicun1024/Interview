# include<iostream>
using namespace std;

int main(){
    int a = 1;
    int& b = a;
    cout<<a<<" "<<b<<endl;
    cout<<&a<<" "<<&b<<endl;
    return 0;
}