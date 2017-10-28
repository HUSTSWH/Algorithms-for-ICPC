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
const LL modn = 1e9+7;
const int maxn = 5e4+9;

vector<int> edge[maxn];
struct node
{
    int val, fa, son, size, top, rank, dep;
}p[maxn];

namespace BIT {
    const int maxb = maxn;
    int bit[maxb], bpos;
    void init(int n)
    {
        memset(bit, 0, (bpos+1)*sizeof(bit[0]));
        bpos = n;
    }
    void addnum(int index, int val)
    {
        for(int i=index+1; i<=bpos; i += (i&-i))
            bit[i] += val;
    }
    inline void add(int l, int r, int val)
    {
        addnum(l, val);
        addnum(r, -val);
    }
    int getsum(int index)
    {
        int ans=0;
        for(int i=index+1; i; i -= (i&-i))
            ans += bit[i];
        return ans;
    }
};

namespace DA {
    int da[maxn], val[maxn], dpos;
    void init() { dpos = 0; }
    void dfs(int x)
    {
        p[x].size = 1;
        for(size_t i=0; i<edge[x].size(); i++) {
            const int &y = edge[x][i];
            if(p[x].fa==y) continue;
            p[y].fa = x;
            p[y].dep = p[x].dep+1;
            dfs(y);
            p[x].size += p[y].size;
            if(p[p[x].son].size < p[y].size)
                p[x].son = y;
        }
    }
    void getda(int x)
    {
        p[x].rank = dpos;
        da[dpos++] = x;
        if(p[x].son) {
            p[p[x].son].top = p[x].top;
            getda(p[x].son);
        }
        for(size_t i=0; i<edge[x].size(); i++) {
            const int &y = edge[x][i];
            if(p[x].fa == y || p[x].son == y) continue;
            p[y].top = y;
            getda(y);
        }
    }
    void build() { BIT::init(dpos); }
    void add(int l, int r, int val)
    {
        while(p[l].top != p[r].top) {
            if(p[p[l].top].dep > p[p[r].top].dep) {
                BIT::add(p[p[l].top].rank, p[l].rank+1, val);
                l = p[p[l].top].fa;
            } else {
                BIT::add(p[p[r].top].rank, p[r].rank+1, val);
                r = p[p[r].top].fa;
            }
        }
        BIT::add(min(p[l].rank, p[r].rank), max(p[l].rank, p[r].rank)+1, val);
    }
    int query(int index)
    {
        return BIT::getsum(p[index].rank);
    }
};

using namespace DA;

int main()
{
    int n,m,q;
    while(scanf("%d %d %d", &n, &m, &q)!=EOF) {
        memset(p, 0, (n+1)*sizeof(p[0]));
        epos = 0;
        for(int i=1; i<=n; i++) {
            scanf("%d", &p[i].val);
        }
        for(int i=0; i<m; i++) {
            int a, b;
            scanf("%d %d", &a, &b);
            // addedge(a, b);
            edge[a].push_back(b);
            edge[b].push_back(a);
        }
        init(); // reset length of DFS Array
        dfs(1); // calculate the size of each vertex; find weithted son
        getda(1); // get DFS Array
        build(); // build data structure on DFS Array
        char cmd[7];
        for(int i=0; i<q; i++) {
            scanf("%s", cmd);
            int a,b,c;
            switch(cmd[0]) {
                case 'I':
                    scanf("%d %d %d", &a, &b, &c);
                    DA::add(a, b, c);
                    break;
                case 'D':
                    scanf("%d %d %d", &a, &b, &c);
                    DA::add(a, b, -c);
                    break;
                case 'Q':
                    scanf("%d", &a);
                    printf("%d\n", DA::query(a)+p[a].val);
                    break;
                default:
                    puts("fuck");
                    break;
            }
        }
        for(int i=1; i<=n; i++) edge[i].clear();
    }
    return 0;
}

