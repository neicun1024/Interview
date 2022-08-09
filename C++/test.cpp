# include<bits/stdc++.h>
using namespace std;

int main(){
    int i = 2;
    i = (i++) + (++i);
    cout<<i<<endl;
    i = i++;
    cout<<i<<endl;
    return 0;
}