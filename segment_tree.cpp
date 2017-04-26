#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;
#define root 1,0,n
#define lson node<<1,s,m
#define rson node<<1|1,m,t

const int maxn = 50000;
int arr[maxn];
int seg[maxn*4], lazy[maxn*4];

void build(int node, int s, int t)
{
    if(t-s<=1){
        seg[node]=arr[s];
        return;
    }
    int m = (s+t)>>1;
    build(lson);
    build(rson);
    seg[node]=min(seg[node<<1], seg[node<<1|1]);
}

void updataDown(int node, int s, int t)
{
    if(lazy[node]){
        if(t-s>1){
            lazy[node<<1]+=lazy[node];
            lazy[node<<1|1]+=lazy[node];
        }
        seg[node]+=lazy[node];
        lazy[node]=0;
    }
}

void update(int node, int s, int t, int l ,int r, int v)
{
    updateDown(node, s, t);
    if(t<=l || r<=s) return;
    if(l<=s && t<=r){
        lazy[node]+=v;
        updateDown(node);
        return;
    }
    int m = (s+t)>>1;
    update(lson, l, r, v);
    update(rson, l, r, v);
    seg[node]=min(seg[node<<1], seg[node<<1|1]);
}

int query(int node, int s, int t, int l ,int r)
{
    updateDown(node, s, t);
    if(t<=l || r<=s) return inf;
    if(l<=s && t<=r) return seg[node];
    int m = (s+t) >> 1;
    int a = query(lson, l, r);
    int b = query(rson, l, r);
    seg[node]=min(seg[node<<1], seg[node<<1|1]);
    return min(a,b);
}
