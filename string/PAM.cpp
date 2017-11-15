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
#define zeros(a,n) memset(a,0,(n)*sizeof(a[0]))
const LL modn = 1e9+7;
const int maxn = 3e5+7, sigma = 26;
char s[maxn];
struct PAM
{
    int son[sigma], link, len;
    int cnt;
}st[maxn];
int spos, last;
inline int alloc() { return spos++; }
void init()
{
    zeros(st, spos);
    st[0].link = st[1].link = 1;
    st[1].len = -1;
    last = 1;
    spos = 2;
}
void extend(int pos)
{
    int c = s[pos]-'a';
    int p = last;
    while(st[p].len == pos || s[pos-st[p].len-1] != s[pos]) p = st[p].link;
    if(!st[p].son[c]) {
        int np = alloc();
        int q = st[p].link;
        st[np].len = st[p].len + 2;
        while(s[pos-st[q].len-1] != s[pos]) q = st[q].link;
        st[np].link = st[q].son[c];
        st[p].son[c] = np;
    }
    last = st[p].son[c];
    st[last].cnt++;
}
int main()
{
    while(scanf("%s", s)!=EOF) {
        int l = strlen(s);
        init();
        for(int i=0; i<l; i++)
            extend(i);
        for(int i=spos-1; i; i--)
            st[st[i].link].cnt += st[i].cnt;
#ifdef SONG
        printf("cnt:");
        for(int i=0; i<spos; i++)
            printf(" %d", st[i].cnt);
        puts("");
#endif
        LL ans = 0;
        for(int i=0; i<spos; i++)
            ans = max(ans, (LL)st[i].len*st[i].cnt);
        printf("%lld\n", ans);
    }
    return 0;
}

