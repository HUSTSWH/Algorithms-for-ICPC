/********************
 * Simplified Gauss elimination
 * An algorithm to solve linear system of equation or xor system of equation
 * Note: This algorithm assumes system of linear equation has one definite solution
 * Time complexity: O(nm^2)
 * Author: songwenhao
 * 2017-08-22
 *******************/
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;

#define mod(a) (((a)%modn+modn)%modn)
const int modn = 1e9+7;

typedef double Type;

/*
void reduce(Type a[], const int &m, Type& b)
{
    Type temp = b;
    for(int i=0; i<m; i++)
        temp = gcd(temp, a[i]);
    if(abs(temp)<2) return;
    for(int i=0; i<m; i++)
        a[i] = a[i]/temp;
    b /= temp;
}
*/
const double eps = 1e-7;
void solve(Type *a[], const int &m, Type ans[], Type b[])
{
    for(int i=0; i<m; i++){
        for(int j=i+1; j<m; j++)
            if(abs(a[j][i]) > eps){
                // a[i].swap(a[j]);
                swap(a[i], a[j]);
                swap(b[i], b[j]);
                break;
            }
        for(int j=0; j<m; j++){
            if(i == j) continue;
            b[j] -= b[i]*a[j][i]/a[i][i];
            // b[j] = b[j]*a[i][i] - b[i]*a[j][i];
            // b[j] = mod(b[j]);
            // b[j] = b[j]^a[j];
            eliminate(a[j], a[i], m, a[j][i], a[i][i]);
			Type val_a=a[j][i], val_b=a[i][i];
			for(int k=0; k<m; k++){
                a[j][k] -= a[i][k]*val_a/val_b;
                // a[j][k] = a[j][k]*val_b - a[i][k]*val_a;
                // a[j][k] = mod(a[j][k]);
                // a[j][k] = a[j][k]^a[i][k];
            }
            // reduce(a[j], b[j]);
        }
    }
    for(int i=0; i<m; i++){
        ans[i] = b[i]/a[i][i];
        // ans[i] = b[i]*quickpow(a[i][i], p-2, p)%p;
        // ans[i] = b[i];
    }
}

const int maxn = 100;
double *a[maxn], b[maxn], ans[maxn];

int main()
{
    int n,m,k;
    bool flag=0;
    for(int i=0; i<maxn; i++)
        a[i] = new double[maxn];
    for(;;){
        scanf("%d %d %d",&m,&n,&k);
        if(n==0 && m==0 && k==0)break;
        if(flag==0)
            flag=1;
        else
            printf("\n");
        for(int i=0; i<n*m; i++){
            double temp;
            scanf("%lf",&temp);
            b[i] = temp;
        }
        for(int xi=0; xi<n; xi++)
        for(int yi=0; yi<m; yi++){
            int cnt = 0;
            for(int xj=0; xj<n; xj++)
            for(int yj=0; yj<m; yj++){
                if(abs(xj-xi)+abs(yj-yi) <= k){
                    a[xi*m+yi][xj*m+yj] = 1.0;
                    cnt ++;
                } else {
                    a[xi*m+yi][xj*m+yj] = 0.0;
                }
            }
            b[xi*m+yi] = b[xi*m+yi]*cnt;
        }
        solve(a, n*m, ans, b);
        for(int xi=0; xi<n; xi++){
            for(int yi=0; yi<m; yi++)
                printf("%8.2f", ans[xi*m+yi]);
            //puts("");
            printf("\n");
        }
    }
    for(int i=0; i<maxn; i++)
        delete[] a[i];
    return 0;
}
