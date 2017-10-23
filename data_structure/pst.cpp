#include <cstdio>
#include <cstring>
#include <cmath>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <deque>
#include <list>
#include <string>
#include <queue>
#include <stack>
#include <map>

using namespace std;

const int maxn = 2e5+17;
int arr[maxn];
int layer[maxn];
int pre[maxn];

class pstnode
{
    public:
        void init(const int &size);
        int update(const int &s, const int &t, const int &index, const int &val);
        int query(const int &s, const int &t, const int &l, const int &r);
        friend int kth_element(int pl, int pr, int s, int t, const int &l, const int &r, int k);
    private:
        int lson, rson, sum;
};

template<class T, int size>
class memorypool
{
    public:
        int alloc(){ return pos++; }
        void init(){ memset(data, 0, sizeof(data)); pos = 0; }
        void clear(){ memset(data, 0, pos*sizeof(T)); pos = 0; }
        T& operator[](const size_t &t){ return data[t]; }
    private:
        T data[size];
        int pos;
};
memorypool<pstnode, maxn*(4+(18+1)*2)> pool;

/******************
 * member function: init
 * build a new segment tree
 * member parameter: 
 *     size: number of this segment tree
 * Time complexity: O(n)
 ******************/
void pstnode::init(const int &size)
{
    sum = 0;
    if(size == 1) return;
    lson = pool.alloc();
    rson = pool.alloc();
    pool[lson].init(size/2);
    pool[rson].init(size - size/2);
}

/******************
 * member function: update
 * for a specific segment tree, return id of a new segment tree with element index added by val
 * member parameter: 
 *    [s, t): range of what this node describes
 *    index: index of the adding element
 * Time complexity: O(log n)
 ******************/
int pstnode::update(const int &s, const int &t, const int &index, const int &val)
{
    int temp = pool.alloc();
    pool[temp] = *this;
    pool[temp].sum += val;
    if(t-s == 1) return temp;
    int m = (s+t)>>1;
    if(index < m) pool[temp].lson = pool[this->lson].update(s, m, index, val);
    else pool[temp].rson = pool[this->rson].update(m, t, index, val);
    return temp;
}

/******************
 * member function: query
 * for a specific segment tree, query the sum with value in range [l, r)
 * member parameter: 
 *    [s, t): range of what this node describes
 *    [l, r): range of the query, i.e. this function is to calculate the sum of elements from l to r
 * Time complexity: O(log n)
 ******************/
int pstnode::query(const int &s, const int &t, const int &l, const int &r)
{
    if(t<=l || r<=s) return 0;
    if(l<=s && t<=r) return this->sum;
    int m = (s+t)>>1;
    return pool[this->lson].query(s, m, l, r) + pool[this->rson].query(m, t, l, r);
}

/******************
 * function: kth_element
 * find the kth element in the order of a value range (or in the order of all value range)
 * member parameter: 
 *    pl: index of left segment
 *    pr: index of right segment
 *    [l, r): range of the query, i.e. this function is search element from l to r
 * Time complexity: O(log^2 n) ( O(log n) if find element in all value range )
 ******************/
int kth_element(int pl, int pr, int s, int t, const int &l, const int &r, int k)
{
    while(t-s > 1) {
        int m = (s+t)>>1;

        // Case 1: find kth-element in all value
        // int lsize = pool[pool[pr].lson].size - pool[pool[pl].lson].size;

        // Case 2: find kth-element in value range [l,r)
        int lsize = pool[pool[pr].lson].query(s, m, l, r) - pool[pool[pl].lson].query(s, m, l, r);

        if(lsize >= k) {
            pl = pool[pl].lson;
            pr = pool[pr].lson;
            t = m;
        } else {
            pl = pool[pl].rson;
            pr = pool[pr].rson;
            s = m;
            k -= lsize;
        }
    }
    return s;
}

int main ()
{
    int T;
    scanf("%d", &T);
    pool.init();
    for(int cas=1; cas<=T; cas++) {
        printf("Case #%d:", cas);
        int n,q;
        scanf("%d %d", &n, &q);
        for(int i=0; i<n; i++)
            scanf("%d", arr+i);
        reverse(arr, arr+n);
        layer[0] = pool.alloc();
        pool[layer[0]].init(n);
        memset(pre, -1, sizeof(pre));
        for(int i=0; i<n; i++) {
            int temp = pool[layer[i]].update(0, n, i, 1);
            if(pre[arr[i]] != -1)
                temp = pool[temp].update(0, n, pre[arr[i]], -1);
            pre[arr[i]] = i;
            layer[i+1] = temp;
        }
        int ans = 0;
        for(int i=0; i<q; i++) {
            int l, r;
            scanf("%d %d", &l, &r);
            l = (l+ans)%n + 1;
            r = (r+ans)%n + 1;
            if(l<r) swap(l, r);
            l = n - l;
            r = n - r + 1;
            int sum = pool[layer[r]].query(0, n, l, r);
            int k = sum>>1;
            k++;
            ans = n - kth_element(layer[0], layer[r], 0, n, l, r, k);
            printf(" %d", ans);
        }
        puts("");
        pool.clear();
    }
    return 0;
}

