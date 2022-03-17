### 【2021】阿里巴巴编程题（4星）

#### 1. 子集
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

#### 2. 小强爱数学
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

#### 3. 二叉树
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

#### 5. 知识竞赛
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


#### 6.树上最短链
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

#### 7. 牛牛们吃糖果
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

#### 9. 合法连续子段
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

