/*************
 * Poj 1742
 * 点分治模板
 * 1.求直径（函数：getd）
 * 2.处理当前点的情况（函数：solve）
 * 3.分治
 * 时间复杂度： O(nlogn*P)(P为处理操作的复杂度)
 *************/
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <functional>
using namespace std;
typedef long long LL;
typedef unsigned long long ULL;
const LL modn = 1e9+7;
const int maxn = 1e5+7;

vector<pair<int, int> > edge[maxn];
int dep[maxn], fa[maxn];
bool vis[maxn];
int n,m;

void getd(int x, int& ans)
{
    int l = edge[x].size();
    if(dep[x]>dep[ans]) ans = x;
    for(int i=0; i<l; i++) {
        const int &y = edge[x][i].first;
        if(vis[y] || fa[x]==y) continue;
        fa[y] = x;
        dep[y] = dep[x]+1;
        getd(y, ans);
    }
}

int dist[maxn], dpos;
void getdist(int x, int d)
{
    dist[dpos++] = d;
    int l = edge[x].size();
    for(int i=0; i<l; i++) {
        const int &y = edge[x][i].first;
        if(vis[y] || fa[x]==y) continue;
        fa[y] = x;
        getdist(y, d+edge[x][i].second);
    }
}

int solve(int node, int d)
{
    int ans = 0;
    dpos = 0;
    fa[node] = 0;
    getdist(node, d);
    sort(dist, dist+dpos);
    int i=0, j=dpos-1;
    while(i<j) {
        while(dist[i]+dist[j]>m) j--;
        if(i>=j) break;
        ans += (j-i);
        i++;
    }
    return ans;
}

void divide(int root, int& ans)
{
    dep[root] = 0;
    fa[root] = 0;
    getd(root, root);
    dep[root] = 0;
    fa[root] = 0;
    getd(root, root);
    int diam = dep[root];
    for(diam/=2; diam; diam--)
        root = fa[root];
    vis[root] = true;
    dist[root] = 0;
    ans += solve(root, 0);
    int l = edge[root].size();
    for(int i=0; i<l; i++) {
        const int &y = edge[root][i].first;
        if(vis[y]) continue;
        ans -= solve(y, edge[root][i].second);
    }
    for(int i=0; i<l; i++) {
        const int &y = edge[root][i].first;
        if(vis[y]) continue;
        divide(y, ans);
    }
}

int main()
{
    while(scanf("%d %d", &n, &m)!=EOF && n+m) {
        memset(vis, 0, sizeof(vis));
        for(int i=1; i<n; i++) {
            int a,b,c;
            scanf("%d %d %d", &a, &b, &c);
            edge[a].push_back(make_pair(b,c));
            edge[b].push_back(make_pair(a,c));
        }
        int ans = 0;
        divide(1, ans);
        printf("%d\n", ans);
        for(int i=1; i<=n; i++)
            edge[i].clear();
    }
    return 0;
}
