#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
typedef long long LL;
typedef unsigned long long ULL;
const LL modn = 1e9+7;

const int maxbit = 10000, maxl = 1000;
struct bigint
{
    int digit[maxl];
    int n;
    bigint(string s) {
        int base=1, l=s.size();
        n = (l-1)>>2|1;
        reverse(s.begin(),s.end());
        memset(digit, 0, sizeof(digit));
        for(int i=0; i<l; i++) {
            digit[i>>2] += (s[i]-'0')*base;
            base *= 10;
            if(base>=maxbit) base = 1;
        }
        while(n && digit[n]==0) n--;
        n++;
    }
    bigint(int v=0) {
        memset(digit, 0, sizeof(digit));
        n=1;
        digit[0] = v%maxbit;
        digit[1] = v/maxbit;
        while(n && digit[n]==0) n--;
        n++;
    }
    string tostring() const
    {
        string s;
        s.reserve(n<<2);
        for(int i=0; i<n; i++)
            for(int b=1; b<maxbit; b*=10)
                s.push_back(digit[i]/b%10+'0');
        int l = s.size()-1;
        while(l && s[l]=='0') l--;
        s.resize(l+1);
        reverse(s.begin(), s.end());
        return s;
    }
    bigint operator+(const bigint &t) const
    {
        bigint c; c.n=max(n, t.n);
        for(int i=0; i<c.n; i++) {
            c.digit[i] += digit[i]+t.digit[i];
            if(c.digit[i]>=maxbit) {
                c.digit[i] -= maxbit;
                c.digit[i+1]++;
            }
        }
        while(c.n && c.digit[c.n]==0)
            c.n--;
        c.n++;
        return c;
    }
    bigint operator-(const bigint &t) const
    {
        bigint c; c.n=max(n,t.n);
        for(int i=0; i<c.n; i++) {
            c.digit[i] += digit[i] - t.digit[i];
            if(c.digit[i]<0) {
                c.digit[i] += maxbit;
                c.digit[i+1]--;
            }
        }
        while(c.n && c.digit[c.n]==0)
            c.n--;
        c.n++;
        return c;
    }
    bigint operator *(const bigint &t) const
    {
        bigint c; c.n = n+t.n;
        for(int i=0; i<n; i++) {
            int temp=0;
            for(int j=0; j<t.n; j++) {
                temp += digit[i]*t.digit[j]+c.digit[i+j];
                c.digit[i+j] = temp%maxbit;
                temp /= maxbit;
            }
            c.digit[i+t.n] = temp;
        }
        while(c.n && c.digit[c.n]==0)
            c.n--;
        c.n++;
        return c;
    }
    bool operator<(const bigint &t) const
    {
        if(n!=t.n) return n<t.n;
        for(int i=n-1; ~i; i--)
            if(digit[i] != t.digit[i])
                return digit[i] < t.digit[i];
        return false;
    }
}mid[15],d;

bool smaller(const bigint &a, const bigint &b, int delta)
{
    if(a.n+delta != b.n) return a.n+delta<b.n;
    for(int i=a.n-1; ~i; i--)
        if(a.digit[i]!=b.digit[i+delta])
            return a.digit[i]<b.digit[i+delta];
    return true;
}

void Minus(bigint &a, const bigint &b, int delta)
{
    int temp = 0;
    for(int i=0; i<a.n-delta; i++) {
        temp += maxbit+a.digit[i+delta]-b.digit[i];
        a.digit[i+delta] = temp%maxbit;
        temp = temp/maxbit-1;
    }
    while(a.n && a.digit[a.n]==0)
        a.n--;
    a.n++;
}

bigint operator/(const bigint &a, const bigint &b)
{
    bigint c;
    d = a;
    int temp;
    mid[0] = b;
    for(int i=1; i<=13; i++)
        mid[i] = mid[i-1]*2;
    for(int i=a.n-b.n; ~i; i--) {
        temp = 8192;
        for(int j=13; ~j; j--) {
            if(smaller(mid[j],d,i)) {
                Minus(d,mid[j],i);
                c.digit[i] += temp;
            }
            temp >>= 1;
        }
    }
    c.n = max(a.n-b.n, 0);
    while(c.n && c.digit[c.n]==0)
        c.n--;
    c.n++;
    return c;
}

int main ()
{
#ifdef SONG
    puts("BigInteger test");
    bigint a("123456789"), b("8000008");
    bigint add=a+b;
    bigint minus=a-b;
    bigint mul=a*b;
    bigint mul2=a*8;
    bigint div=a/b;
    cout<<"add: "<<add.tostring()<<endl;
    cout<<"minus: "<<minus.tostring()<<endl;
    cout<<"mul: "<<mul.tostring()<<endl;
    cout<<"mul2: "<<mul2.tostring()<<endl;
    cout<<"div: "<<div.tostring()<<" rem: "<<d.tostring()<<endl;
#endif
    int T;
    // scanf("%d", &T);
    cin>>T;
    for(int cas=1; cas<=T; cas++) {
        if(cas>1) puts("");
        string astr, bstr;
        cin>>astr>>bstr;
        bigint a(astr), b(bstr);
        bigint c = a + b;
        cout<<"Case "<<cas<<':'<<endl;
        cout<<astr<<" + "<<bstr<<" = "<<c.tostring()<<endl;
    }
    return 0;
}

