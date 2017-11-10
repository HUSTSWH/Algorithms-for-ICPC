#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
using namespace std;
typedef long long LL;
typedef unsigned long long ULL;
#define zeros(a,n) memset(a,0,(n)*sizeof(a[0]))
const LL modn = 1e9+7;
const int maxn = 5.2e5+9, sigma = 26; // maxn equals double of the length of string

struct state
{
    int son[sigma], len, link;
}st[maxn];
int last, spos;
void init()
{
    zeros(st, spos);
    st[0].link = -1;
    spos = 1;
    last = 0;
}
inline int alloc() { return spos++; }
void extend(int c)
{
    int p=last, np=alloc();
    st[np].len = st[p].len + 1;
    for(; ~p && !st[p].son[c]; p=st[p].link)
        st[p].son[c] = np;
    if(p==-1) st[np].link = 0;
    else {
        int q = st[p].son[c];
        if(st[q].len==st[p].len+1)
            st[np].link = q;
        else {
            int nq = alloc();
            st[nq] = st[q];
            st[nq].len = st[p].len + 1;
            st[q].link = st[np].link = nq;
            for(; ~p && st[p].son[c]==q; p=st[p].link)
                st[p].son[c] = nq;
        }
    }
    last = np;
}
int rnum[maxn], ra[maxn];
void radixsort()
{
    int m=st[last].len+1;
    zeros(rnum, m);
    for(int i=0; i<spos; i++)
        rnum[st[i].len]++;
    for(int i=1; i<m; i++)
        rnum[i] += rnum[i-1];
    for(int i=spos-1; ~i; i--)
        ra[--rnum[st[i].len]] = i;
}
char s1[maxn], s2[maxn];
int main()
{
    while(scanf("%s", s1)!=EOF) {
        scanf("%s", s2);
        int l1 = strlen(s1);
        int l2 = strlen(s2);
        clear();
        for(int i=0; i<l1; i++)
            extend(s1[i]-'a');
        int ans = 0;
        int x = 0;
        int l = 0;
        for(int i=0; i<l2; i++) {
            int v=s2[i]-'a';
            while(x && !st[x].son[v]) {
                x = st[x].link;
                l = st[x].len;
            }
            if(st[x].son[v]) {
                x = st[x].son[v];
                l++;
            }
            ans = max(ans, l);
        }
        printf("%d\n", ans);
    }
    return 0;
}