#include <cstdio>
#include <algorithm>
using namespace std;

const int maxn = 1e5+9;
namespace SEG
{
#define self node,s,t
#define lson node<<1,s,m
#define rson node<<1|1,m,t
    int seg[maxn<<2], lazy[maxn<<2];
    int pos;
    void build(int arr[], int node=1, int s=0, int t=pos) {
        lazy[node] = 0; // NOTE: this line should not be missed!
        if(t-s==1) {
            seg[node] = arr[s];
            return;
        }
        int m=(s+t)>>1;
        build(arr, lson);
        build(arr, rson);
        seg[node] = min(seg[node<<1], seg[node<<1|1]);
    }
    void init(int arr[], int n)
    {
        pos = n;
        build(arr);
    }
    inline void pushdown(int node, int s, int t)
    {
        if(t-s>1) {
            lazy[node<<1] += lazy[node];
            lazy[node<<1|1] += lazy[node];
        }
        seg[node] += lazy[node];
        lazy[node] = 0;
    }
    int query(int l, int r, int node=1, int s=0, int t=pos)
    {
        pushdown(self);
        if(l<=s && t<=r) return seg[node];
        if(t<=l || r<=s) return 0x3f3f3f3f;
        int m=(s+t)>>1;
        return min(query(l, r, lson), query(l, r, rson));
    }
    void add(int l, int r, int val, int node=1, int s=0, int t=pos)
    {
        if(l<=s && t<=r) {
            lazy[node] += val;
            pushdown(self);
            return;
        }
        pushdown(self);
        if(r<=s || t<=l) return;
        int m=(s+t)>>1;
        add(l, r, val, lson);
        add(l, r, val, rson);
        seg[node] = min(seg[node<<1], seg[node<<1|1]);
    }
};

int main()
{
    int n;
    scanf("%d", &n);
    for(int i=0; i<n; i++)
        scanf("%d", arr+i);
    SEG::resize(n);
    SEG::build(arr);
    for(int i=0; i<n; i++) {
        SEG::add(0, i+1, i);
        printf("%d\n", SEG::query(i, n));
    }
    return 0;
}

