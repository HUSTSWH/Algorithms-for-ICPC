#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;
typedef long long LL;
typedef unsigned long long ULL;

const int maxn = 5e4+19, maxd = 5;

LL bound[maxn];
int id[maxn];
int n, dim, idx, kpos;
struct point
{
    LL coo[maxd];
    bool operator<(const point &t) const
    {
        return coo[idx]<t.coo[idx];
    }
}pt[maxn];

LL dist(const point &lhs, const point &rhs)
{
    LL ans = 0;
    for(int i=0; i<dim; i++) {
        LL temp = rhs.coo[i] - lhs.coo[i];
        ans += temp*temp;
    }
    return ans;
}

int alloc() { return kpos++; }
#define zeros(a, n) memset(a, 0, (n)*sizeof(a[0]))
void init() { kpos = 1; }

void build(int s=0, int t=n)
{
    static LL d[maxd];
    if(t-s<=1) return;
    int m=(s+t)>>1;
    for(int i=0; i<dim; i++) {
        idx = i;
        d[i] = max_element(pt+s, pt+t)->coo[i] - min_element(pt+s, pt+t)->coo[i];
    }
    idx = id[m] = max_element(d, d+dim) - d;
    nth_element(pt+s, pt+m, pt+t);
    bound[m] = pt[m].coo[idx];
    build(s, m);
    build(m, t);
}

priority_queue<pair<LL, point> > pq;
point rt;
void query(size_t k, int s=0, int t=n)
{
    if(t-s<=1) {
        pq.push(make_pair(dist(rt, pt[s]), pt[s]));
        if(pq.size() > k) pq.pop();
        return;
    }
    int m=(s+t)>>1;
    LL d = rt.coo[id[m]] - bound[m];
    if(d<0) {
        query(k,s,m);
        if(pq.size() < k || pq.top().first > d*d)
            query(k,m,t);
    } else {
        query(k,m,t);
        if(pq.size() < k || pq.top().first > d*d)
            query(k,s,m);
    }
}

point ans[maxn];

int main()
{
    while(scanf("%d %d", &n, &dim)!=EOF) {
        for(int i=0; i<n; i++)
            for(int j=0; j<dim; j++)
                scanf("%lld", &pt[i].coo[j]);
        init();
        build();
        int q;
        scanf("%d", &q);
        for(int i=0; i<q; i++) {
            for(int j=0; j<dim; j++)
                scanf("%lld", &rt.coo[j]);
            int k;
            scanf("%d", &k);
            query(k);
            int apos = 0;
            while(!pq.empty()) {
                ans[apos++] = pq.top().second;
                pq.pop();
            }
            reverse(ans, ans+apos);
            printf("the closest %u points are:\n", k);
            for(int i=0; i<apos; i++)
                for(int j=0; j<dim; j++)
                    printf("%lld%c", ans[i].coo[j], " \n"[j+1==dim]);
        }
    }
    return 0;
}

