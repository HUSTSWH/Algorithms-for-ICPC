/*********
 * 高斯消元解线性方程组（浮点数）
 * 时间复杂度：n*n*m
 * 日期：2017-10-22
 * 用法：A * X = Y,A矩阵为方程矩阵，B矩阵为一族已知数向量
 * 注意：1.高斯消元通过构造变元能一次解决规模更大的问题
         2.通过扩展Y矩阵的维度，高斯消元能同时对多组已知数向量求解
 *********/

#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;
const double eps = 1e-6;
const int maxn = 107;

double A[maxn][maxn], X[maxn][2], B[maxn];

inline int sgn(double x)
{
    if(x<-eps) return -1;
    else if(x<eps) return 0;
    else return 1;
}
inline void elimi(double *a, double *b, int l, int r, double val_a, double val_b)
{
    for(int i=l; i<r; i++)
        a[i] -= b[i]*val_a/val_b;
}
bool gauss(int n, int m)
{
    int xi=0;
    for(int yi=0; yi<m; yi++) {
        if(sgn(A[xi][yi])==0) {
            for(int xj=xi; xj<n; xj++) {
                if(sgn(A[xj][yi]) ) {
                    for(int i=yi; i<m; i++)
                        swap(A[xi][i], A[xj][i]);
                    swap(B[xi], B[xj]);
                    break;
                }
            }
        }
        if(sgn(A[xi][yi])==0) continue;
        for(int xj=xi+1; xj<n; xj++) {
            if(sgn(A[xj][yi])==0) continue;
            B[xj] -= B[xi]*A[xj][yi]/A[xi][yi];
            elimi(A[xj], A[xi], yi, m, A[xj][yi], A[xi][yi]);
        }
        xi++; if(xi==n) break;
    }
    bool invalid = false;
    for(int xj=xi; xj<n; xj++)
        invalid |= sgn(B[xj]);
    for(;xi;) {
        --xi;
        int yi;
        for(yi=xi; yi<m && sgn(A[xi][yi])==0; yi++);
        for(int xj=0; xj<xi; xj++) {
            if(sgn(A[xj][yi])==0) continue;
            B[xj] -= B[xi]*A[xj][yi]/A[xi][yi];
            elimi(A[xj], A[xi], xj, yi+1, A[xj][yi], A[xi][yi]);
        }
        X[yi][0] = A[xi][yi];
        X[yi][1] = B[yi];
    }
    return invalid;
}

int main()
{
    int n,m,d;
    bool fst = false;
    while(scanf("%d %d %d", &m, &n, &d) &&(n+m+d) ) {
        if(fst) puts("");
        else fst = true;
        memset(A, 0, n*m*sizeof(A[0]));
        memset(B, 0, n*m*sizeof(B[0]));
        for(int i=0; i<n; i++)
        for(int j=0; j<m; j++)
        for(int ki=0; ki<n; ki++)
        for(int kj=0; kj<m; kj++) {
            if(abs(ki-i)+abs(kj-j)>d) continue;
            A[i*m+j][ki*m+kj] = 1.0;
            B[i*m+j] += 1.0;
        }
        for(int i=0; i<n*m; i++) {
            double temp;
            scanf("%lf", &temp);
            B[i] *= temp;
        }
#ifdef SONG
        for(int i=0; i<n*m; i++) {
            printf("%d:", i);
            for(int j=0; j<n*m; j++)
                printf(" %.3f", A[i][j]);
            printf(" :%.3f", B[i]);
            puts("");
        }
#endif
        gauss(n*m, n*m);
        for(int i=0; i<n; i++) {
            for(int j=0; j<m; j++) {
                printf("%8.2f", X[i*m+j][1]/X[i*m+j][0]);
            }
            puts("");
        }
    }
    return 0;
}

