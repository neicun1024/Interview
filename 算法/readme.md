## 记录了各种算法的规范流程

### 回溯
1. 画出递归树，找到状态变量(回溯函数的参数)，这一步非常重要※
2. 根据题意，确立结束条件
3. 找准选择列表(与函数参数相关),与第一步紧密关联※
4. 判断是否需要剪枝
5. 作出选择，递归调用，进入下一层
6. 撤销选择

[回溯算法详解](https://leetcode-cn.com/problems/subsets/solution/c-zong-jie-liao-hui-su-wen-ti-lei-xing-dai-ni-gao-/)

### 