/**
 *Author: songwenhao
 *prototype question: bzoj1901
 *An offline algorithm to solve Range k-th smallest element Question with element modification.
 *(Online if without discretization)
 *
 *Time complexity: n log(n) + q log^2(n)
 *Maximum memory cost: (maxn*(4 + ceil(log2(maxn + maxq)) + 1) + maxq*2*(ceil(log2(maxn + maxq)) + 1)(ceil(log2(maxn)) + 1))*sizeof(pstnode)
 *get as much memory as possible when solving
 *maxn = 10000, maxq = 10000: cost = 60MB
 *2017/08/20
 */

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;
/*****constant value clarification*****/
const int maxn = 20000+10;
const int maxp = maxn * 100;
/*****class clarification*****/
class pstnode
{
    public:
        void init(const int &size);
        pstnode *update(const int &size, const int &index, const int &val);
        friend int query(vector<pstnode *> &pls, vector<pstnode *> &prs, const int &s, const int &t, const int &k);
    private:
        pstnode *lson, *rson;
        int sum;
};

template<typename T, int size>
class memorypool
{
    public:
        T *alloc() { return data+(pos++); }
        void clear() { pos = 0; }
    private:
        T data[size];
        int pos;
};
memorypool<pstnode, maxp> pool;
/*****class member function definitation*****/
void pstnode::init(const int &size)
{
    sum = 0;
    if(size == 1) {
        lson = NULL;
        rson = NULL;
        return;
    }
    lson = pool.alloc();
    rson = pool.alloc();
    lson->init(size/2);
    rson->init(size - size/2);
}

pstnode *pstnode::update(const int &size, const int &index, const int &val)
{
    pstnode *temp = pool.alloc();
    *temp = *this;
    temp->sum += val;
    if(size==1) return temp;
    if(index < size/2) temp->lson = lson->update(size/2, index, val);
    else temp->rson = rson->update(size - size/2, index - size/2, val);
    return temp;
}

int query(vector<pstnode *> &pls, vector<pstnode *> &prs, const int &s, const int &t, const int &k)
{
    if(t-s<=1) return s;
    int m = (s+t)>>1;
    int lsum = 0;
    for(size_t i=0; i<prs.size(); i++){
        lsum += prs[i]->lson->sum;
    }
    for(size_t i=0; i<pls.size(); i++){
        lsum -= pls[i]->lson->sum;
    }
    if(k < lsum){
        for(size_t i=0; i<pls.size(); i++)
            pls[i] = pls[i]->lson;
        for(size_t i=0; i<prs.size(); i++)
            prs[i] = prs[i]->lson;
        return query(pls, prs, s, m, k);
    } else {
        for(size_t i=0; i<pls.size(); i++)
            pls[i] = pls[i]->rson;
        for(size_t i=0; i<prs.size(); i++)
            prs[i] = prs[i]->rson;
        return query(pls, prs, m, t, k-lsum);
    }
}
/*****global variables*****/
pstnode *layer[maxn], *BIT[maxn];
int arr[maxn], tab[maxn];
int qa[maxn], qb[maxn], qc[maxn];
char qcmd[maxn];
/*****main body*****/
int main()
{
    int n, m;
    vector<pstnode* > va, vb;
    while(scanf("%d %d",&n,&m)!=EOF){
        for(int i=0; i<n; i++)
            scanf("%d",arr+i);
        memcpy(tab, arr, n*sizeof(arr[0]));
        int u = n;
        for(int i=0; i<m; i++){
            scanf("%s",qcmd+i);
            switch(qcmd[i]){
                case 'Q':
                    scanf("%d %d %d",qa+i, qb+i, qc+i);
                    break;
                case 'C':
                    scanf("%d %d",qa+i, qb+i);
                    tab[u++] = qb[i];
                    break;
                default:
                    puts("error: illegal command");
                    break;
            }
        }
        sort(tab, tab+u);
        u = unique(tab, tab+u) - tab;
        pool.clear();
        layer[0] = pool.alloc();
        layer[0]->init(u);
        for(int i=0; i<=n; i++)
            BIT[i] = layer[0];
        for(int i=0; i<n; i++){
            int index = lower_bound(tab, tab+u, arr[i]) - tab;
            layer[i+1] = layer[i]->update(u, index, 1);
        }
        for(int i=0; i<m; i++){
            int a,b,c,vo,vn;
            switch(qcmd[i]){
                case 'C':
                    a = qa[i], b = qb[i];
                    vo = lower_bound(tab, tab+u, arr[a-1]) - tab;
                    vn = lower_bound(tab, tab+u, b) - tab;
                    for(int j = a; j<=n; j+=(j&-j)){
                        BIT[j] = BIT[j]->update(u, vn, 1)->update(u, vo, -1);
                    }
                    arr[a-1] = b;
                    break;
                case 'Q':
                    a = qa[i], b = qb[i], c = qc[i];
                    va.clear(); vb.clear();
                    va.push_back(layer[a-1]); vb.push_back(layer[b]);
                    for(int j = a-1; j; j-=(j&-j))
                        va.push_back(BIT[j]);
                    for(int j = b; j; j-=(j&-j))
                        vb.push_back(BIT[j]);
                    printf("%d\n", tab[ query(va, vb, 0, u, c-1) ]);
                    break;
                default:
                    puts("error: illegal command");
                    break;
            }
        }
    }
    return 0;
}
