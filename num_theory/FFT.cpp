#include <cstdio>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <complex>
using namespace std;
const double PI = acos(-1.0);
const int maxn = 1<<20;

complex<double> a[maxn], b[maxn];

void FFT(complex<double> x[], const int len, const int &on)
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
    for(int k=1; k<len; k<<=1) {
        complex<double> wm(cos(PI/k), sin(PI/k)*on);
        for(int i=0; i<len; i+=2*k) {
            complex<double> w(1.0,0.0);
            for(int j=0; j<k; j++) {
                complex<double> u = x[i+j];
                complex<double> t = w*x[i+j+k];
                x[i+j] = u+t;
                x[i+j+k] = u-t;
                w *= wm;
            }
        }
    }
    if(on == -1)
        for(int i=0; i<len; i++)
            x[i] /= (double)len;
}

int main()
{
    for(int i=0; i<4; i++) {
        a[i] = i;
        b[i] = 8-i;
    }
    FFT(a, 8, 1);
    FFT(b, 8, 1);
    for(int i=0; i<8; i++)
        a[i] *= b[i];
    FFT(a, 8, -1);
    for(int i=0; i<8; i++)
        cout<<a[i]<<endl;
    return 0;
}

