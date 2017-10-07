/**
 * NTT Algorithm
 * Time Complexity: O(nlog n)
 * available data range rely on the value of g and modn
 * default range: 
 * * 0 <= ai < 1004535809 (a number slightly bigger than 1e9)
 * * n<= 2^21
 * prototype problem: HDU1402
 **/

#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
const long long g = 3;
const int modn = (479<<21)+1;
const int maxn = 1<<17;

#define mod(a) ((((a)%modn)+modn)%modn)
typedef long long LL;

LL a[maxn], b[maxn];
char s[maxn];

LL quickpow(LL base, LL n, const LL &P)
{
    LL ans = 1;
    while(n) {
        if(n&1) ans = ans*base%P;
        base = base*base%P;
        n >>= 1;
    }
    return ans;
}

inline long long multi(long long x,long long y,long long P)
{
    long long tmp=(x*y-(long long)((long double)x/P*y+1.0e-8)*P);
    return tmp<0 ? tmp+P : tmp;
}

/// len: must be 2^k and not smaller than length of x
/// note that len of NTT must be equal to len of INTT
void NTT(LL x[], const int &len, const int &on)
{
    
    for(int i=1, j=len/2; i<len-1; i++) {
        if(i<j) swap(x[i], x[j]);
        int k = len/2;
        while(j>=k) {
            j -= k;
            k >>=1;
        }
        if(j<k) j += k;
    }
    int pos=0;
    for(int k=1; k<len; k<<=1) {
        // complex<double> wm(cos(PI/k), sin(PI/k)*on);
        for(int i=0; i<len; i+=2*k) {
            // complex<double> w(1.0,0.0);
            long long w = 1;
            for(int j=0; j<k; j++) {
                long long u = x[i+j];
                long long t = w*x[i+j+k]%modn;
                x[i+j] = (u+t)%modn;
                x[i+j+k] = (u-t+modn)%modn;
                w = w*wm[pos]%modn;
            }
        }
        pos++;
    }
    if(on == -1) {
        reverse(x+1, x+len);
        long long inv = quickpow(len, modn-2, modn);
        for(int i=0; i<len; i++)
            x[i] = inv*x[i]%modn;
    }
}

int main()
{
    // DO NOT MISS THIS LINE!!!
    for(int i=0; (1<<i)<maxn; i++)
        wm[i] = quickpow(g, modn>>(i+1), modn);
    
    while(scanf("%s", s)!= EOF) {
        int l1 = strlen(s);
        reverse(s, s+l1);
        for(int i=0; i<l1; i++)
            a[i] = s[i] - '0';
        scanf("%s", s);
        int l2 = strlen(s);
        reverse(s, s+l2);
        for(int i=0; i<l2; i++)
            b[i] = s[i] - '0';
        
        // get the minimum of 2^k which is greater than length of a or b
        int n = 1<<(8*sizeof(int) - __builtin_clz(max(l1, l2) ) ); 
        n<<=1;  //DO NOT MISS IT!!!
        
        fill(a+l1, a+n, 0);
        fill(b+l2, b+n, 0);
        NTT(a, n, 1);
        NTT(b, n, 1);
        for(int i=0; i<n; i++)
            a[i] = a[i]*b[i]%modn; // NOTE: a[i]*b[i] may exceed INT_MAX!!
        NTT(a, n, -1);
        int l = 0;
        for(int i=0; i<n; i++) {
            a[i+1] += a[i]/10;
            a[i] %= 10;
            if(a[i]) l = i;
        }
        for(int i=0; i<=l; i++)
            s[i] = a[i] + '0';
        s[l+1] = 0;
        reverse(s, s+l+1);
        puts(s);
    }
    return 0;
}

/**
prime number:
  g   r   k             modn
  3   1  16  1<<16|1   = 65537
  3   7  26  7<<26|1   = 469762049
  3 119  23  119<<23|1 = 998244353
  3 479  21  479<<21|1 = 1004535809
 31  15  27  15<<27|1  = 2013265921
  3  17  27  17<<27|1  = 2281701377
  5  27  56  27<<56|1  = 1945555039024054273
  **/