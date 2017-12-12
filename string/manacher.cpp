#include <bits/stdc++.h>
using namespace std;
const int maxn = 1.2e5+9;
int ma[maxn<<1|1];
char s[maxn];

void manacher(char s[])
{
    int n = strlen(s);
    int id=0, ub=0;
    for(int i=0; i<2*n-1; i++) {
        int p=i/2, q=(i+1)/2;
        int l = q<ub?min(ub-q, ma[id-i]):0;
        while(p-l>=0 && q+l<n && s[p-l]==s[q+l]) l++;
        if(ub < q+l) {
            ub = q+l;
            id = i*2;
        }
        ma[i] = l;
    }
    for(int i=0; i<2*n-1; i++)
        ma[i] = ma[i]*2-(!(i&1));
}

int main ()
{
    while(scanf("%s", s)!=EOF) {
        int l = strlen(s);
        if(!l) continue;
        manacher(s);
        int ans = 0;
        for(int i=0; i<2*l-1; i++)
            ans = max(ans, ma[i]);
        printf("%d\n", ans);
    }
    return 0;
}
