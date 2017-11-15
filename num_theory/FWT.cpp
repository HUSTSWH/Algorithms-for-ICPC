/******
 * FWT Algorithm
 * NOTE: unlike NTT or FFT, FWT does not need twice the array length to calculate transfer
 * prototype problem: HDU5909
 ******/
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;
typedef long long LL;
typedef unsigned long long ULL;
#define zeros(a,n) memset(a,0,(n)*sizeof(a[0]))
const LL modn = 1e9+7;
const int maxn = 1024+9;
const LL inv = (modn+1)>>1;

vector<int> edge[maxn];
LL X[maxn], Y[maxn];
int val[maxn];
int fa[maxn];
int dp[maxn][maxn];
int n, m;
inline int nor(int x) { return x<0?x+modn:x<modn?x:x-modn; }
void fwt(LL a[], int len)
{
    for(int k=1; k<len; k<<=1) {
        for(int i=0; i<len; i+=2*k) {
            for(int j=0; j<k; j++) {
                LL x = a[i+j];
                LL y = a[i+j+k];
                // xor
                a[i+j] = nor(x+y);
                a[i+j+k] = nor(x-y);
                // and
                // a[i+j] = nor(x+y);
                // or
                // a[i+j+k] = nor(x-y);
            }
        }
    }
}
void ifwt(LL a[], int len)
{
    for(int k=1; k<len; k<<=1) {
        for(int i=0; i<len; i+=2*k) {
            for(int j=0; j<k; j++) {
                LL x = a[i+j];
                LL y = a[i+j+k];
                // xor
                a[i+j] = inv*(x+y)%modn;
                a[i+j+k] = inv*(x-y+modn)%modn;
                // and
                // a[i+j] = nor(x-y);
                // or
                // a[i+j+k] = nor(y-x);
            }
        }
    }
}

void solve(int x)
{
    memset(dp[x], 0, sizeof(dp[0]));
    int l = edge[x].size();
    dp[x][val[x]] = 1;
    for(int i=0; i<l; i++) {
        const int &y = edge[x][i];
        if(fa[x] == y) continue;
        fa[y] = x;
        solve(y);
        int xlen=0, ylen=0;
        for(int i=0; i<m; i++) {
            if(dp[x][i])
                xlen = i;
            if(dp[y][i])
                ylen = i;
        }
        int len = 1<<(8*sizeof(int) - __builtin_clz(max(xlen,ylen)));
        copy(dp[x], dp[x]+len, X);
        copy(dp[y], dp[y]+len, Y);
        fwt(X, len);
        fwt(Y, len);
        for(int i=0; i<len; i++)
            X[i] = X[i]*Y[i]%modn;
        ifwt(X, len);
        copy(X, X+len, dp[x]);
    }
    dp[x][0] = nor(dp[x][0]+1);
}

int main()
{
    int T;
    scanf("%d", &T);
    for(int cas=1; cas<=T; cas++) {
        while(scanf("%d %d", &n, &m)!=EOF) {
            for(int i=1; i<=n; i++)
                scanf("%d", val+i);
            for(int i=1; i<n; i++) {
                int a, b;
                scanf("%d %d", &a, &b);
                edge[a].push_back(b);
                edge[b].push_back(a);
            }
            fa[1] = 0;
            solve(1);
            zeros(X, m);
            for(int i=1; i<=n; i++)
                for(int j=0; j<m; j++)
                    X[j] += dp[i][j];
            X[0] += modn - n;
            for(int i=0; i<m; i++)
                printf("%lld%c", X[i]%modn, " \n"[i+1==m]);
            for(int i=1; i<=n; i++)
                edge[i].clear();
        }
    }
    return 0;
}
