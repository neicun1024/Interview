# include<iostream>
using namespace std;

int main()
{
    int i = 1;
    printf("%d,%d\n", ++i, ++i);    //3,3
    printf("%d,%d\n", ++i, i++);    //5,3
    printf("%d,%d\n", i++, i++);    //6,5
    printf("%d,%d\n", i++, ++i);    //8,9 

    cout<<endl;
    i = 1;
    cout<<++i<<","<<++i<<endl;      //2,3
    cout<<++i<<","<<i++<<endl;      //4,4
    cout<<i++<<","<<i++<<endl;      //5,6
    cout<<i++<<","<<++i<<endl;      //7,9
    system("pause");
    return 0;
}