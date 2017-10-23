#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
typedef long long LL;
typedef unsigned long long ULL;
const LL modn = 1e9+7;
const int maxn = 1e4+9;

namespace PST
{
#define NOC -1
    const int maxp = 5e6+9;
    struct pstnode
    {
        int val, lson, rson;
    }p[maxp];
    int ppos;
    // NOTE: this line will clear the pool of PST without reset its size
    void clear()
    {
        memset(p, 0, ppos*sizeof(p[0]));
        // NOTE: node 0 reserved for NULL segment tree
        ppos = 1;
    }

    inline int alloc() { 
#ifndef SONG
        // these lines are for test: ensure RTE verdict when array bounds
        if(ppos >= maxp) {
            printf("%d\n", 1/0);
            exit(1);
        }
#endif
        return ppos++; 
    }
    inline int getson(int &son) { return son?son:son=alloc(); }

    int spos=modn;
    int add(int node, int index, int val)
    {
        int s=0, t=spos;
        int nnode = alloc();
        int ans = nnode;
        while(t-s>1) {
            p[nnode] = p[node];
            p[nnode].val += val;
            int m=(s+t)>>1;
            if(index<m) {
                node = p[node].lson;
                p[nnode].lson = alloc();
                nnode = p[nnode].lson;
                t = m;
            } else {
                node = p[node].rson;
                p[nnode].rson = alloc();
                nnode = p[nnode].rson;
                s = m;
            }
        }
        p[nnode] = p[node];
        p[nnode].val += val;
        return ans;
    }
    int find(vector<int> &pls, vector<int> &prs, int k=1, int kp=1)
    {
        int sum = 0;
        for(size_t i=0; i<pls.size(); i++)
            sum -= p[pls[i]].val;
        for(size_t i=0; i<prs.size(); i++)
            sum += p[prs[i]].val;
        if(k>=kp+sum) return NOC; //check before iteration
        int s=0, t=spos;
        while(t-s>1) {
            sum = 0;
            for(size_t i=0; i<pls.size(); i++)
                sum -= p[p[pls[i]].lson].val; // NOTE: lsons are expected, please avoid typo to rson
            for(size_t i=0; i<prs.size(); i++)
                sum += p[p[prs[i]].lson].val;
            int m=(s+t)>>1;
            if(k<kp+sum) {
                // pl = p[pl].lson;
                // pr = p[pr].lson;
                for(size_t i=0; i<pls.size(); i++)
                    pls[i] = p[pls[i]].lson; // NOTE: lsons are expected, please avoid typo to rson
                for(size_t i=0; i<prs.size(); i++)
                    prs[i] = p[prs[i]].lson;
                t = m;
            } else {
                // pl = p[pl].rson;
                // pr = p[pr].rson;
                for(size_t i=0; i<pls.size(); i++)
                    pls[i] = p[pls[i]].rson; // NOTE: rsons are expected, please avoid typo to lson
                for(size_t i=0; i<prs.size(); i++)
                    prs[i] = p[prs[i]].rson;
                s = m;
                kp += sum;
            }
        }
        return s;
    }
};


namespace BIT {
    const int maxb = maxn;
    int bit[maxb], bpos;
    int base[maxb]; // This array includes the initial prefix_sum of segtrees
    void resize(int n)
    {
        PST::clear(); // NOTE: this line will clear the pool of PST without reset its size
        memset(bit, 0, bpos*sizeof(bit[0]));
        memset(base, 0, bpos*sizeof(base[0]));
        bpos = n;
    }
    void addnum(int index, int val, int op)
    {
        for(int i=index+1; i<=bpos; i+=(i&-i)) {
            bit[i] = PST::add(bit[i], val, op);
        }
    }
    int find(int l, int r, int k)
    {
        static vector<int> pls, prs;
        pls.clear(); pls.push_back(base[l]); // get the origin sum of sequence
        prs.clear(); prs.push_back(base[r]); 
        for(int i=l; i; i-=(i&-i))
            pls.push_back(bit[i]); // get modified diff of sequence
        for(int i=r; i; i-=(i&-i))
            prs.push_back(bit[i]);
        return PST::find(pls, prs, k);
    }
};

int pre[maxn];
int tab[maxn<<1], tpos;
inline int getidx(int x)
{
    return lower_bound(tab, tab+tpos, x)-tab;
}
struct QUERY
{
    int type, a, b, c;
}qu[maxn];

int main()
{
    int n;
    int q;
    while(scanf("%d", &n)!=EOF) {
        scanf("%d", &q);
        BIT::resize(n); // Initialize BIT with PST in it (size of segment tree remain unset)
        for(int i=0; i<n; i++)
            scanf("%d", pre+i);
        copy(pre, pre+n, tab);
        tpos = n;
        for(int i=0; i<q; i++) {
            char cmd[7];
            scanf("%s", cmd);
            qu[i].type = (cmd[0]=='Q')+1;
            // scanf("%d", &qu[i].type);
            switch(qu[i].type) {
                case 1:
                    scanf("%d %d", &qu[i].a, &qu[i].b);
                    qu[i].a--;
                    tab[tpos++] = qu[i].b;
                    break;
                case 2:
                    scanf("%d %d %d", &qu[i].a, &qu[i].b, &qu[i].c);
                    qu[i].a--;
                    break;
            }
        }
        sort(tab, tab+tpos);
        tpos = unique(tab, tab+tpos) - tab;
        PST::spos = tpos; // set the size of segment tree
        for(int i=0; i<n; i++) {
            BIT::base[i+1] = PST::add(BIT::base[i], getidx(pre[i]), 1);
#ifdef SONG
            printf(">>>%d->%d\n", BIT::base[i], BIT::base[i+1]);
#endif
            // BIT::addnum(i, getidx(pre[i]), 1);
        }
        for(int i=0; i<q; i++) {
            switch(qu[i].type) {
                case 1:
                    BIT::addnum(qu[i].a, getidx(pre[qu[i].a]), -1);
                    pre[qu[i].a] = qu[i].b;
                    BIT::addnum(qu[i].a, getidx(pre[qu[i].a]), 1);
                    break;
                case 2:
                    printf("%d\n", tab[BIT::find(qu[i].a, qu[i].b, qu[i].c)]);
                    break;
                default:
                    puts("fuck");
                    break;
            }
        }
    }
    return 0;
}

