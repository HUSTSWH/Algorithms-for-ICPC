#include <cstdio>
#include <cstring>
#include <cmath>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <list>
#include <string>
#include <queue>
#include <stack>
#include <map>
#include <complex>
using namespace std;

const int maxn = 1e5+9;
int maxbit = 17;
vector<int> edge[maxn];
vector<pair<int, int> > query_lca[maxn];
int fa[maxn], arr[maxn], tag[maxn];
int layer[maxn];
// int da[maxn], rk[maxn], val[maxn], top[maxn], tpos;

class Trie
{
  public:
    friend int insert(int p, const int &val);
    friend int query(int pl, int pr, const int &val);
  private:
    int son[2], sum;
};
template<typename T, size_t size>
class memorypool
{
  public:
    int alloc() { return pos++; }
    void init() { memset(data, 0, sizeof(data)); pos = 0; }
    void clear() { memset(data, 0, pos*sizeof(data[0])); pos=0; }
    T& operator[](const size_t &t) { return data[t]; }
  private:
    T data[size];
    int pos;
};
memorypool<Trie, maxn*23> pool;
int insert(int p, const int &val)
{
    int node=pool.alloc();
    int ans = node;
    pool[node] = pool[p];
    pool[node].sum++;
    for(int i=0; i<maxbit; i++) {
        int bit=((val>>(maxbit-i-1))&1);
        pool[node].son[bit]=pool.alloc();
        node=pool[node].son[bit];
        p=pool[p].son[bit];
        pool[node] = pool[p];
        pool[node].sum++;
    }
    return ans;
}
int query(int pl, int pr, const int &val)
{
    int ans = 0;
    for(int i=0; i<maxbit; i++) {
        int bit=((val>>(maxbit-i-1))&1);
        if(pool[pool[pr].son[bit^1]].sum-pool[pool[pl].son[bit^1]].sum) {
            pr = pool[pr].son[bit^1];
            pl = pool[pl].son[bit^1];
            ans += 1<<(maxbit-i-1);
        } else {
            pr = pool[pr].son[bit];
            pl = pool[pl].son[bit];
        }
    }
    return ans;
}

struct QUERY
{
    int a,b,val,lca;
}q[maxn];

int find_set(int n)
{
    int ans = n;
    while(ans != tag[ans]) ans = tag[ans];
    int m=n;
    while(n!=ans) {
        m = tag[n];
        tag[n] = ans;
        n = m;
    }
    return ans;
}

void dfs(const int &n)
{
    tag[n] = n;
    layer[n] = insert(layer[fa[n]], arr[n]);
    for(unsigned i=0; i<query_lca[n].size(); i++)
        if(tag[query_lca[n][i].first])
            q[query_lca[n][i].second].lca=find_set(query_lca[n][i].first);
    for(unsigned i=0; i<edge[n].size(); i++) {
        const int &y = edge[n][i];
        if(y == fa[n]) continue;
        fa[y] = n;
        dfs(y);
        tag[y] = n;
    }
}

int main()
{
    ios::sync_with_stdio(false); cin.tie(0);
    int n,m;
    pool.init();
    while(scanf("%d %d", &n, &m) != EOF) {
        for(int i=1; i<=n; i++)
            scanf("%d", arr+i);
        int max_value = *max_element(arr, arr+n);
        for(int i=1; i<n; i++) {
            int a,b;
            scanf("%d %d", &a, &b);
            edge[a].push_back(b);
            edge[b].push_back(a);
        }
        for(int i=0; i<m; i++) {
            int a,b,c;
            scanf("%d %d %d", &a, &b, &c);
            query_lca[a].push_back(make_pair(b,i));
            query_lca[b].push_back(make_pair(a,i));
            q[i].a = a;
            q[i].b = b;
            q[i].val = c;
            max_value = max(max_value, c);
        }
        maxbit = 8*sizeof(int)-__builtin_clz(max_value);
        if(maxbit==0) maxbit++;
        fa[1] = 0;
        pool.alloc();
        layer[0] = pool.alloc();
        dfs(1);
        for(int i=0; i<m; i++)
            printf("%d\n", max(query(layer[q[i].lca], layer[q[i].a], q[i].val), query(layer[fa[q[i].lca]], layer[q[i].b], q[i].val)));
        pool.clear();
        for(int i=1; i<=n; i++) {
            edge[i].clear();
            query_lca[i].clear();
        }
        memset(tag, 0, (n+1)*sizeof(tag[0]));
    }
    return 0;
}