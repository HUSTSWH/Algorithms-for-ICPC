#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
const int maxn = 3e4+10;

class pstnode
{
    public:
        void init(const int &totsize);
        pstnode *update(const int &s, const int &t, const int &index, const int &val);
        friend int query(const pstnode *ma, const pstnode *pr, const int &s, const int &t, const int &l, const int &r);
    private:
        pstnode *lson, *rson;
        int sum;
};

template<class T, int size>
class memorypool
{
    public:
        T *alloc(){ return data+(pos++); }
        void clear(){ pos = 0; }
    private:
        T data[size];
        int pos;
};
memorypool<pstnode, maxn*(4+16*2)> pool;

void pstnode::init(const int &size)
{
    sum = 0;
    if(unlikely(size == 1)){
        lson = NULL;
        rson = NULL;
        return;
    }
    lson = pool.alloc();
    rson = pool.alloc();
    lson -> init(size/2);
    rson -> init(size - size/2);
}

pstnode *pstnode::update(const int &s, const int &t, const int &index, const int &val)
{
    pstnode *temp = pool.alloc();
    *temp = *this;
    temp->sum += val;
    if(unlikely(t-s == 1)) return temp;
    int m = (s+t)>>1;
    if(index < m)temp->lson = this->lson->update(s, m, index, val);
    else temp->rson = this->rson->update(m, t, index, val);
    return temp;
}

int query(const pstnode *pl, const pstnode *pr, const int &s, const int &t, const int &l, const int &r)
{
    if(t<=l || r<=s) return 0;
    if(l<=s && t<=r) return pr->sum - pl->sum;
    int m = (s+t)>>1;
    return query(pl->lson, pr->lson, s, m, l, r) + query(pl->rson, pr->rson, m, t, l, r);
}

pstnode *layer[maxn];
int arr[maxn], pre[maxn], tab[maxn];

int main()
{
    int n;
    while(scanf("%d",&n)!=EOF){
        for(int i=0; i<n; i++)
            scanf("%d",arr+i);
        memcpy(tab, arr, n*sizeof(tab[0]));
        sort(tab, tab+n);
        int u = unique(tab, tab+n) - tab;
        pool.clear();
        layer[0] = pool.alloc();
        layer[0]->init(n);
        memset(pre, -1, n*sizeof(pre[0]));
        for(int i=0; i<n; i++){
            int index = distance(tab, lower_bound(tab, tab+u, arr[i]));
            pstnode *temp = layer[i]->update(0, n, i, 1);
            if(pre[index]!=-1)temp = temp->update(0, n, pre[index], -1);
            pre[index] = i;
            layer[i+1] = temp;
        }
        int q;
        scanf("%d",&q);
        for(int i=0; i<q; i++){
            int a,b;
            scanf("%d %d", &a, &b);
            printf("%d\n", query(layer[a-1], layer[b], 0, n, a-1, b) );
        }
    }
    return 0;
}

