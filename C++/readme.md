## 主要记录了之前没复习到的内容


### 指针
1. 类中的this指针是右值

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