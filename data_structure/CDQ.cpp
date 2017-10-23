/**********
 * CDQ Divide & Conquer
 * Time complexity: O(n log(n))
 * Space complexity: O(n)
 **********/
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <list>
#include <string>
#include <queue>
#include <stack>
#include <map>
#include <complex>
using namespace std;
typedef long long LL;
const int maxn = 2e5+9;

struct elem
{
    int rank[3];
    bool operator<(const elem &t) const {
        return rank[0] < t.rank[0];
    }
}q1[maxn], q2[maxn];

namespace BIT{
    LL val[maxn];
    int len;
    void addval(const int &x, const LL &v)
    {
        for(int i=x+1; i<=len; i+=(i&-i))
            val[i] += v;
    }
    LL getsum(const int &x)
    {
        LL ans=0;
        for(int i=x; i; i-=(i&-i))
            ans += val[i];
        return ans;
    }  
};
using namespace BIT;

LL cdq(const int &s, const int &t)
{
    if(t-s<=1) return 0LL;
    int m = (s+t)>>1;
    LL ans = cdq(s, m) + cdq(m, t);
    int i=s, pos=s;
    for(int j=m; j<t; j++) {
        while(i<m && q1[i].rank[1] < q1[j].rank[1]) {
            addval(q1[i].rank[2], 1);
            q2[pos++] = q1[i++];
        }
        ans += getsum(q1[j].rank[2]);
        q2[pos++] = q1[j];
    }
    for(int j=s; j<i; j++)
        addval(q1[j].rank[2], -1);
    copy(q1+i, q1+m, q2+pos);
    copy(q2+s, q2+t, q1+s);
    return ans;
}

int main()
{
    int n;
    while(scanf("%d", &n) != EOF) {
        BIT::len = n;
        for(int r=0; r<3; r++) {
            for(int i=0; i<n; i++) {
                int temp;
                scanf("%d", &temp);
                temp--;
                q1[temp].rank[r] = i;
            }
        }
        sort(q1, q1+n);
        printf("%lld\n", cdq(0, n));
    }
    return 0;
}
