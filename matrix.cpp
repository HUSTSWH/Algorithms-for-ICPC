/********************
 * Matrix & Gauss elimination
 * An algorithm to solve linear system of equation or xor system of equation
 * Time complexity: O(nm^2)
 * Author: songwenhao
 * Date: 2017-08-22
 *******************/
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

#define mod(a) (((a)%modn+modn)%modn)
const int modn = 1e9+7;

template<typename T>
T gcd(T a, T b)
{
    return b==0?a:gcd(b, a%b);
}

template<typename T>
T quickpow(const T &v, const unsigned long long &n)
{
	if(n==1) return v;
	T temp = quickpow(v*v, n>>1);
	if(n&1) return temp * v;
	else return temp;
}

template<typename T>
class matrix
{
public:
    matrix(const int nn=1, const int mm=1):n(nn), m(mm)
    {
        data.resize(n);
        for(int i=0; i<n; i++)
            data[i].resize(m);
    }
    void resize(const int nn=1, const int mm=1)
    {
        n=nn;
        m=mm;
        data.resize(n);
        for(int i=0; i<n; i++)
            data[i].resize(m);
        for(int i=0; i<n; i++)
            for(int j=0; j<m; j++)
                data[i][j] = 0;
    }
    T& at(const int nn, const int mm=0)
    {
        return data[nn][mm];
    }
    vector<T>& operator[](const int nn)
    {
        return data[nn];
    }
    matrix& operator+=(const matrix &t)
    {
        for(int i=0; i<n; i++)
            for(int j=0; j<m; j++)
                data[i][j] += t.data[i][j];
        return *this;
    }
    matrix operator+(const matrix &t)const
    {
        matrix<T> ans(*this);
        ans += t;
        return ans;
    }
    matrix operator*(const matrix &t)const
    {
        matrix<T> ans(n,m);
        for(int i=0; i<n; i++)
            for(int j=0; j<m; j++)
                ans.data[i][j] =0;
        for(int i=0; i<n; i++)
            for(int j=0; j<m; j++)
                for(int k=0; k<t.m; k++){
                    ans.data[i][k] += data[i][j]*t.data[j][k];
					ans.data[i][k] = mod(ans.data[i][k]);
				}
        return ans;
    }
private:
    // val_a, val_b no reference
    inline void eliminate(vector<T> &a, const vector<T> &b, const int &l, const int &r, const T val_a, const T val_b)
    {
        for(int i=l; i<r; i++){
            a[i] = a[i]*val_b - b[i]*val_a;
            // a[i] = mod(a[i]);
            // a[i] = a[i]^b[i];
        }
    }
    inline void reduce(vector<T> &a, const int &l, const int &r, vector<T> &b)
    {
        T temp = 0;
        for(int i=l; i<r; i++)
            temp = gcd(temp, a[i]);
        for(size_t i=0; i<b.size(); i++)
            temp = gcd(temp, b[i]);
        if(abs(temp) < 2) return;
        for(int i=l; i<r; i++)
            a[i] = a[i]/temp;
        for(size_t i=0; i<b.size(); i++)
            b[i] = b[i]/temp;
    }
public:
    vector<bool> solve(matrix& ans, matrix& b)
    {
		ans.resize(m, b.m+1);
        vector<bool> invalid(b.m);
        int xi = 0;
        for(int yi=0; yi<m; yi++){
            for(int xj=xi+1; xj<n; xj++)
                if(data[xj][yi]!=0 && (data[xi][yi] == 0 || abs(data[xj][yi])<abs(data[xi][yi]) ) ){
                    data[xi].swap(data[xj]);
                    b[xi].swap(b[xj]);
                }
		    if(data[xi][yi] == 0) continue;
            for(int xj=xi+1; xj<n; xj++){
				if(data[xj][yi] == 0) continue;
                eliminate(b[xj], b[xi], 0, b.m, data[xj][yi], data[xi][yi]);
                eliminate(data[xj], data[xi], yi, m, data[xj][yi], data[xi][yi]);
                // reduce(data[xj], yi, m, b[xj]);
            }
            xi++;
            if(xi == n)break;
        }
        for(int xj=xi; xj<n; xj++)
            for(int yi=0; yi<b.m; yi++)
                invalid[yi] = invalid[yi]|b[xj][yi];
        for(;xi;){
            --xi;
            int yi;
            for(yi=0; yi<m && data[xi][yi]==0; yi++);
            for(int xj=0; xj<xi; xj++){
				if(data[xj][yi] == 0) continue;
                eliminate(b[xj], b[xi], 0, b.m, data[xj][yi], data[xi][yi]);
                eliminate(data[xj], data[xi], xj, yi+1, data[xj][yi], data[xi][yi]);
                // reduce(data[xj], xj, yi, b[xj]);
            }
            ans.data[yi][0] = data[xi][yi];
            for(int i=0; i<b.m; i++)
                ans.data[yi][i+1] = b[yi][i];
        }
        return invalid;
    }
private:
    vector<vector<T > > data;
    int n, m;
};


int main()
{
    int n,m;
	matrix<int> A(n,m),B(n,1);//1 means the number of system is 1
	// set the value of A & B
	...
	// finish
	matrix<int> ans;
	vector<bool> invalid = A.solve(ans, B);
    return 0;
}
