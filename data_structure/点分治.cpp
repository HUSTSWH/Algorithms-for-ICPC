/*************
 * HDU 5909
 * 点分治模板
 * 1.求直径（函数：getdiam）
 * 2.处理当前点的情况（函数：solve）
 * 3.分治
 * 时间复杂度： O(nlogn*P)(P为处理操作的复杂度)
 *************/
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <functional>
#include <limits>
#include <vector>
#include <list>
#include <string>
#include <queue>
#include <stack>
#include <map>
#include <complex>
using namespace std;
typedef long long LL;
typedef unsigned long long ULL;
#define zeros(a,n) memset(a,0,(n)*sizeof(a[0]))
const int modn = 1e9+7;
const int maxn = 1024+9;

vector<int> edge[maxn];
int dp[maxn][maxn];
int val[maxn];
int ans[maxn];
bool vis[maxn];
int n, m;

inline int nor(int x) { return x<0?x+modn:x<modn?x:x-modn; }
int getdiam(int x, bool on)
{
    static int q[maxn], fa[maxn];
    int qf=0, qe=0;
    fa[x] = 0;
    q[qe++] = x;
    while(qf<qe) {
        x = q[qf++];
        int l = edge[x].size();
        for(int i=0; i<l; i++) {
            const int &y = edge[x][i];
            if(vis[y] || fa[x]==y) continue;
            fa[y] = x;
            q[qe++] = y;
        }
    }
    if(on) {
        int cnt = 0;
        for(int i=x; i; i=fa[i]) cnt++;
        for(int i=cnt>>1; i; i--) x = fa[x];
    }
    return x;
}
void solve(int x, int f=0)
{
    const int &v = val[x];
    for(int i=0; i<m; i++)
        dp[x][i] = dp[f][i^v];

    int l = edge[x].size();
    for(int i=0; i<l; i++) {
        const int &y = edge[x][i];
        if(vis[y] || f==y) continue;
        solve(y, x);
        for(int i=0; i<m; i++)
            dp[x][i] = nor(dp[x][i] + dp[y][i]);
    }
}
void divide(int x)
{
    x = getdiam(x, 0);
    x = getdiam(x, 1);
    vis[x] = true;
    solve(x);
    int l = edge[x].size();
    for(int i=0; i<l; i++)
        if(!vis[edge[x][i]])
            divide(edge[x][i]);
}

int main()
{
    int T;
    scanf("%d", &T);
    for(int cas=1; cas<=T; cas++) {
        scanf("%d %d", &n, &m);
        for(int i=1; i<=n; i++)
            scanf("%d", val+i);
        for(int i=1; i<n; i++) {
            int a, b;
            scanf("%d %d", &a, &b);
            edge[a].push_back(b);
            edge[b].push_back(a);
        }
        memset(dp[0], 0, sizeof(dp[0]));
        dp[0][0] = 1;
        zeros(vis, n+1);
        divide(1);
        zeros(ans, m);
        for(int i=0; i<m; i++)
            for(int j=1; j<=n; j++)
                ans[i] = nor(ans[i] + dp[j][i]);
        for(int i=0; i<m; i++)
            printf("%d%c", ans[i], " \n"[i+1==m]);
        for(int i=1; i<=n; i++)
            edge[i].clear();
    }
    return 0;
}
