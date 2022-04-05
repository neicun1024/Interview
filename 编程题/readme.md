# 这里存放了我刷过的大厂往年机试题

## 【2021】阿里巴巴编程题（4星）

### 1. 子集
![20220314153603](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220314153603.png)

```
# include<bits/stdc++.h>
using namespace std;

void update(vector<int> &end, int x){
    if(end.size()==0){
        end.push_back(x);
        return;
    }
    int l = 0, r = end.size()-1;
    while(l<=r){
        int mid = l + (r-l)/2;
        if(x==end[mid]){
            return;
        }
        else if(x<end[mid]){
            r = mid - 1;
        }
        else{
            l = mid + 1;
        }
    }
    if(l==end.size()){
        end.push_back(x);
    }
    else{
        end[l] = x;
    }
}


int main(){
    int T;
    cin>>T;
    while(T--){
        int n;
        cin>>n;
        vector<pair<int,int>> v(n);
        for(int i=0; i<n; ++i){
            cin>>v[i].first;
        }
        for(int i=0; i<n; ++i){
            cin>>v[i].second;
        }
        sort(v.begin(), v.end(), [](const pair<int,int> &a, const pair<int,int> &b)->bool{
            return a.first==b.first?a.second>b.second:a.first<b.first;
        });
        
        vector<int> end;
        for(int i=0; i<n; ++i){
            update(end, v[i].second);
        }
        cout<<end.size()<<endl;
    }
}
```

### 2. 小强爱数学
![20220314153701](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220314153701.png)

```
# include<bits/stdc++.h>
using namespace std;

const int mod = 1e9+7;

int main(){
    int T;
    cin>>T;
    while(T--){
        long long A, B, n;
        cin>>A>>B>>n;
        vector<long long> dp(n+1, 0);
        dp[0] = 2;
        dp[1] = A;
        for(int i=2; i<=n; ++i){
            dp[i] = (A*dp[i-1] - B*dp[i-2])%mod;
            if(dp[i]<0){
                dp[i] += mod;
            }
        }
        cout<<dp[n]<<endl;
    }
}
```

### 3. 二叉树
![20220315194600](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220315194600.png)

```
# include<bits/stdc++.h>
using namespace std;

const int mod = 1e9+7;

int main(){
    int n, m;
    cin>>n>>m;
    vector<vector<long long>> dp(n+1, vector<long long> (m+1, 0));
    for(int j=0; j<=m; ++j){
        dp[0][j] = dp[1][j] = 1;
    }
    for(int i=2; i<=n; ++i){
        dp[i][0] = dp[i][1] = 0;
    }
    for(int i=2; i<=n; ++i){
        for(int j=2; j<=m; ++j){
            for(int k=0; k<i; ++k){
                dp[i][j] += dp[k][j-1]*dp[i-1-k][j-1];
                dp[i][j] %= mod;
            }
        }
    }
    cout<<dp[n][m]<<endl;
}
```

### 5. 知识竞赛
![20220314153752](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220314153752.png)

```
# include<bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin>>n;
    vector<pair<int,int>> v(n);
    for(int i=0; i<n; ++i){
        cin>>v[i].first>>v[i].second;
    }
    sort(v.begin(), v.end(), [](const pair<int,int> &a, const pair<int,int> &b)->bool{
        return min(a.first, a.second) > min(b.first, b.second);
    });
    cout<<min(v[0].first+v[1].first, v[0].second+v[1].second)/2<<endl;
    return 0;
}
```


### 6.树上最短链
![20220314154737](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220314154737.png)

```
# include<bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin>>n;
    vector<int> rank(n+1);
    for(int i=1; i<=n; ++i){
        cin>>rank[i];
    }
    vector<vector<int>> adj(n+1, vector<int> {});
    for(int i=0; i<n-1; ++i){
        int a, b;
        cin>>a>>b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    
    int ret = INT_MAX;
    for(int i=1; i<=n; ++i){
        int start = i;
        vector<bool> vis(n+1, false);
        queue<int> q;
        q.push(start);
        vis[start] = true;
        int step = 1;
        while(!q.empty()){
            int size = q.size();
            bool flag = false;
            while(size--){
                int cur = q.front();
                q.pop();
                for(int k=0; k<adj[cur].size(); ++k){
                    int des = adj[cur][k];
                    if(vis[des]){
                        continue;
                    }
                    vis[des] = true;
                    if(rank[start]==rank[des]){
                        flag = true;
                        ret = min(ret, step);
                        break;
                    }
                    q.push(des);
                }
                if(flag){
                    break;
                }
            }
            if(flag){
                break;
            }
            step++;
        }
        if(ret==1){
            break;
        }
    }
    if(ret==INT_MAX){
        cout<<-1<<endl;
    }
    else{
        cout<<ret<<endl;
    }
    return 0;
}
```

### 7. 牛牛们吃糖果
![20220314161624](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220314161624.png)

```
# include<bits/stdc++.h>
using namespace std;

int main(){
    int n, m;
    cin>>n>>m;
    vector<int> a(n+1);
    for(int i=1; i<=n; ++i){
        cin>>a[i];
    }
    int K;
    cin>>K;
    vector<pair<int, int>> v;
    vector<bool> vis(n+1, false);
    for(int k=0; k<K; ++k){
        int i, j;
        cin>>i>>j;
        vis[i] = vis[j] = true;
        v.push_back({a[i]+a[j], 2});
    }
    for(int i=1; i<=n; ++i){
        if(!vis[i]){
            v.push_back({a[i], 1});
        }
    }
    
    vector<vector<int>> dp(v.size()+1, vector<int> (m+1, 0));
    for(int i=1; i<=v.size(); ++i){
        for(int j=1; j<=m; ++j){
            if(j>=v[i-1].first){
                dp[i][j] = max(dp[i-1][j-v[i-1].first]+v[i-1].second, dp[i-1][j]);
            }
            else{
                dp[i][j] = dp[i-1][j];
            }
        }
    }
    cout<<dp[v.size()][m]<<endl;
    return 0;
}
```

### 9. 合法连续子段
![20220314163219](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220314163219.png)

```
# include<bits/stdc++.h>
using namespace std;

int main(){
    int n, m;
    cin>>n>>m;
    vector<int> a(n);
    for(int i=0; i<n; ++i){
        cin>>a[i];
    }
    unordered_map<int, int> cnt;
    long long ret = 0;
    int i=0, j=0;
    for(; j<n; ++j){
        cnt[a[j]]++;
        while(cnt[a[j]]>=m){
            ret += (n-j);
            cnt[a[i]]--;
            ++i;
        }
    }
    cout<<ret<<endl;
    return 0;
}
```
这道题一开始一直出错，最后看题解才知道是结果用int存不下。。




## 拼多多2021笔试真题集

### 1. 多多的数字组合
![20220320144145](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220320144145.png)

```
# include<bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin>>n;
    if(n<10){
        cout<<n<<endl;
        return 0;
    }
    else if(n>45){
        cout<<-1<<endl;
        return 0;
    }
    int res = 0;
    int base = 0;
    int cur = 9;
    while(n>cur){
        res += cur * pow(10, base);
        n -= cur;
        cur--;
        base++;
    }
    if(n){
        res += n * pow(10, base);
    }
    cout<<res<<endl;
    return 0;
}
```

### 2. 多多的字符变换
![20220320143743](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220320143743.png)

```
# include<bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin>>n;
    string a, b;
    cin>>a>>b;
    vector<int> count(26, 0);
    for(int i=0; i<n; ++i){
        count[a[i]-'a']++;
        count[b[i]-'a']--;
    }
    vector<int> dif_a, dif_b;
    for(int i=0; i<26; ++i){
        if(count[i]==0){
            continue;
        }
        else if(count[i]>0){
            for(int j=0; j<count[i]; ++j){
                dif_a.push_back(i);
            }
        }
        else{
            for(int j=0; j<-count[i]; ++j){
                dif_b.push_back(i);
            }
        }
    }
    int res = 0;
    for(int i=0; i<dif_a.size(); ++i){
        res += abs(dif_a[i]-dif_b[i]);
    }
    cout<<res<<endl;
    return 0;
}
```


### 3. 多多的求和计算
![20220320143556](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220320143556.png)

```
# include<bits/stdc++.h>
using namespace std;

int main(){
    int n, m;
    cin>>n>>m;
    vector<int> a(n);
    for(int i=0; i<n; ++i){
        int x;
        cin>>x;
        a[i] = x % m;
    }
    unordered_map<int,int> ma;
    ma[0] = 1;
    int pre = 0, cur = 0;
    for(int i=1; i<=n; ++i){
        cur = pre + a[i-1];
        cur %= m;
        ma[cur]++;
        pre = cur;
    }
    long long res = 0;
    for(unordered_map<int,int>::iterator iter=ma.begin(); iter!=ma.end(); ++iter){
        long long num = iter->second;
        res += (num*(num-1)/2);
    }
    cout<<res<<endl;
    return 0;
}
```
unordered_map的key为前缀和的模，value为等于key的模的数量

### 4. 多多的骰子组合
![20220320144117](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220320144117.png)

```
# include<bits/stdc++.h>>
using namespace std;

int main(){
    int n;
    cin>>n;
    unordered_map<int, int> m;
    while(n--){
        vector<int> v(6);
        for(int i=0; i<6; ++i){
            cin>>v[i];
        }
        bool neg = false;
        for(int i=0; i<6; i+=2){
            if(v[i]<v[i+1]){
                neg = !neg;
            }
        }
        vector<pair<int,int>> vv;
        for(int i=0; i<6; i+=2){
            vv.push_back({v[i]+v[i+1], i/2});
        }
        sort(vv.begin(), vv.end(), [v](const pair<int,int> &p1, const pair<int,int> &p2)->bool{
            if(p1.first==p2.first){
                int a = min(v[p1.second*2], v[p1.second*2+1]);
                int b = min(v[p2.second*2], v[p2.second*2+1]);
                return a<b?true:false;
            }
            else{
                return p1.first<p2.first?true:false;
            }
        });
        if(vv[0].second==0 && vv[1].second==1 || vv[0].second==1 && vv[1].second==2 || vv[0].second==2 && vv[1].second==0){
            neg = !neg;
        }
        int key = 0;
        for(int i=0; i<3; ++i){
            int a = min(v[vv[i].second*2], v[vv[i].second*2+1]);
            int b = max(v[vv[i].second*2], v[vv[i].second*2+1]);
            key = key * 10 + a;
            key = key * 10 + b;
        }
        if(neg){
            key = -key;
        }
        m[key]++;
    }
    vector<int> res;
    for(unordered_map<int, int>::iterator iter=m.begin(); iter!=m.end(); ++iter){
        res.push_back(iter->second);
    }
    sort(res.rbegin(), res.rend());
    cout<<res.size()<<endl;
    for(int i=0; i<res.size(); ++i){
        cout<<res[i]<<" ";
    }
    return 0;
}
```


## 拼多多2020校招部分编程题合集

### 1. 多多的魔术盒子
![20220320165354](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220320165354.png)

```
# include<bits/stdc++.h>
using namespace std;
int main(){
    int T;
    cin>>T;
    while(T--){
        int x;
        cin>>x;
        int res = 0;
        while(x){
            x /= 2;
            res++;
        }
        cout<<res<<endl;
    }
    return 0;
}
```
二分的思想，从中间的数开始减，使得两边需要减少的次数相同


### 2. 多多的排列函数
![20220320165444](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220320165444.png)

```
# include<bits/stdc++.h>
using namespace std;
int main(){
    int T;
    cin>>T;
    while(T--){
        int n;
        cin>>n;
        int res_min, res_max;
        if(n*(n+1)/2%2==0){
            res_min = 0;
        }
        else{
            res_min = 1;
        }
        if((n-1)*n/2%2==0){
            res_max = n;
        }
        else{
            res_max = n-1;
        }
        cout<<res_min<<" "<<res_max<<endl;
    }
    return 0;
}
```
找规律，最小值由1到n求和决定，最大值由1-到n-1求和决定


### 3. 多多的电子字典（没做出来）
![20220320175207](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220320175207.png)


### 4. 骰子期望
![20220320165706](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220320165706.png)

```
# include<bits/stdc++.h>
using namespace std;
int main(){
    int n;
    cin>>n;
    vector<int> num(n);
    for(int i=0; i<n; ++i){
        cin>>num[i];
    }
    sort(num.begin(), num.end());
    
    
    double ratio = 1;
    for(int i=0; i<n; ++i){
        ratio /= num[i];
    }
    vector<double> prefix(n+1, 1);
    for(int i=1; i<=n; ++i){
        prefix[i] = prefix[i-1] * num[i-1];
    }
    
    double res = 0;
    int start = 0;
    for(int max_num=1; max_num<=num[n-1]; ++max_num){
        while(num[start]<max_num)++start;
        int len = n - start;
        res = res + (max_num * ratio * ((pow(max_num, len) - pow(max_num-1, len))) * prefix[start]);
    }
    cout<<fixed<<setprecision(2)<<res<<endl;
    return 0;
}
```
这题属实麻烦。。


### 5. 二维表第k大数(8/10 组用例通过 运行超时)
![20220320172351](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220320172351.png)

```
# include<bits/stdc++.h>
using namespace std;

void up(vector<int> &v, int idx){
    while(idx>1 && v[idx]<v[idx/2]){
        swap(v[idx], v[idx/2]);
        idx /= 2;
    }
}

void down(vector<int> &v, int idx){
    while(idx<=(v.size()-1)/2){
        int tmp = idx*2;
        if(idx*2+1<=v.size()-1 && v[idx*2+1]<v[idx*2]){
            tmp = idx*2+1;
        }
        if(v[idx]>v[tmp]){
            swap(v[idx], v[tmp]);
            idx = tmp;
        }
        else{
            return;
        }
    }
}

int main(){
    int n,m,k;
    cin>>n>>m>>k;
    vector<int> v(k+1);
    int cnt = 0;
    for(int i=n; i>=1; --i){
        for(int j=m; j>=1; --j){
            if(cnt<k){
                v[cnt+1] = i*j;
                up(v, cnt+1);
                cnt++;
            }
            else if(i*j>v[1]){
                v[1] = i*j;
                down(v, 1);
            }
        }
    }
    cout<<v[1];
    return 0;
}
```
emmm，看了题解，还是得用二分啊。。


## 网易2021校招笔试-C++开发工程师（提前批）

### 1. 平分物品

![20220327143909](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220327143909.png)

```
# include<bits/stdc++.h>
using namespace std;

void backtrace(vector<int> &v, vector<int> &choose, int sum, int a, int b, int &ans, int idx){
    if(a==b){
        ans = min(ans, sum-a-b);
    }
    if(idx>=v.size()){
        return;
    }
    choose[idx] = 0;
    backtrace(v, choose, sum, a, b, ans, idx+1);
    if(a+v[idx]<=sum/2){
        choose[idx] = 1;
        backtrace(v, choose, sum, a+v[idx], b, ans, idx+1);
    }
    if(b+v[idx]<=sum/2){
        choose[idx] = 2;
        backtrace(v, choose, sum, a, b+v[idx], ans, idx+1);
    }
}

int main(){
    int T;
    cin>>T;
    while(T--){
        int n;
        cin>>n;
        vector<int> v(n);
        int sum = 0;
        for(int i=0; i<n; ++i){
            cin>>v[i];
            sum += v[i];
        }
        vector<int> choose(n);
        int ans = INT_MAX;
        backtrace(v, choose, sum, 0, 0, ans, 0);
        cout<<ans<<endl;
    }
}
```

### 2. 买票问题(8/20 组用例通过)

![20220327143820](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220327143820.png)

```
# include<bits/stdc++.h>
using namespace std;

int main(){
    int T;
    cin>>T;
    while(T--){
        int n;
        cin>>n;
        vector<int> a(n);
        int sum = 0;
        for(int i=0; i<n; ++i){
            cin>>a[i];
            sum += a[i];
        }
        vector<int> b(n);
        for(int i=1; i<n; ++i){
            cin>>b[i];
        }
        vector<int> dp(n, 0);
        for(int i=0; i<n; ++i){
            if(i==0){
                dp[i] = a[i];
            }
            else if(i==1){
                dp[i] = min(dp[i-1]+a[i], b[i]);
            }
            dp[i] = min(dp[i-1]+a[i], dp[i-2]+b[i]);
        }
        sum = min(sum, dp[n-1]);
        int h, m, s;
        s = sum%60;
        sum /= 60;
        m = sum%60;
        sum /= 60;
        h = sum%24 + 8;
        string str;
        if(h+sum>=12){
            h -= 12;
            str = "pm";
        }
        else{
            str = "am";
        }
        cout<<setw(2)<<setfill('0')<<h<<":"<<setw(2)<<m<<":"<<setw(2)<<s<<" "<<str<<endl;
    }
}
```

### 3. 小易爱回文

![20220327144015](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220327144015.png)

```
# include<bits/stdc++.h>
using namespace std;

int main(){
    string s;
    cin>>s;
    int len = s.size();
    int max_len = 0;
    vector<vector<bool>> dp(len, vector<bool> (len, false));
    for(int i=1; i<=len; ++i){
        for(int j=i-1; j<len; ++j){
            if(i==1){
                dp[j][j] = true;
                if(j==len-1){
                    max_len = i;
                }
            }
            else if(i==2){
                if(s[j-1]==s[j]){
                    dp[j-1][j] = true;
                    if(j==len-1){
                        max_len = i;
                    }
                }
            }
            else {
                if(s[j-i+1]==s[j] && dp[j-i+2][j-1]){
                    dp[j-i+1][j] = true;
                    if(j==len-1){
                        max_len = i;
                    }
                }
            }
        }
    }
    for(int i=len-max_len-1; i>=0; --i){
        s += s[i];
    }
    cout<<s<<endl;
}
```

### 4. 学术认可

![20220327145232](https://raw.githubusercontent.com/neicun1024/Interview/main/images_for_markdown/20220327145232.png)