/******
 * 2-D CDQ
 * Time complexity: O(n log^2(n))
 * Memory complexity: n d: dimension
 ******/

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

const int maxn = 6e4+9;

struct QUERY
{
    int type, x, y, z, val;
    QUERY(){}
    QUERY(const int &_type, const int &_x, const int &_y, const int &_z, const int &_val):type(_type), x(_x), y(_y), z(_z), val(_val){}
}q[maxn<<3|7], qx[maxn<<3|7], qy[maxn<<3|7];
int tab[maxn<<1|1], tpos;
int ans[maxn], apos;
int qpos;

// for each query get the sum in range[0, x), so query is in the front of modify with same index
bool cmpx(const QUERY &a, const QUERY &b)
{
    return a.x<b.x || (a.x == b.x && a.type > b.type);
}
bool cmpy(const QUERY &a, const QUERY &b)
{
    return a.y<b.y || (a.y == b.y && a.type > b.type);
}

namespace BIT{
    int val[maxn<<1|1]; // NOTE: this size should be equal to the length of discrete z
    int len;
    int getsum(const int &x)
    {
        int ans = 0;
        for(int i=x; i; i-=(i&-i))
            ans += val[i];
        return ans;
    }
    void setval(const int &x, const int &v)
    {
        for(int i=x+1; i<=len; i+=(i&-i))
            val[i] += v;
    }
    void clear(const int &x)
    {
        for(int i=x+1; i<=len && val[i]; i+=(i&-i))
            val[i] = 0;
    }
};

using namespace BIT;
void solve_z(const int &s, const int &t)
{
    if(t-s<=1) return;
    for(int i=s; i<t; i++) {
        switch(qy[i].type) {
            case 1:
                setval(qy[i].z, 1);
                break;
            case 2:
                ans[qy[i].val] -= getsum(qy[i].z);
                break;
            case 3:
                ans[qy[i].val] += getsum(qy[i].z);
                break;
            default:
                break;
        }
    }
    for(int i=s; i<t; i++)
        if(qy[i].type == 1)
            clear(qy[i].z);
}

void cdq_y(const int &s, const int &t)
{
    if(t-s<=1) return;
    int m=(s+t)>>1;
    cdq_y(s, m);
    cdq_y(m, t);
    int i=s, pos=s;
    for(int j=m; j<t; j++) {
        while(i<m && cmpy(qx[i], qx[j])) {
            if(qx[i].type == 1)
                qy[pos++] = qx[i];
            i++;
        }
        if(qx[j].type != 1)
            qy[pos++] = qx[j];
    }
    solve_z(s, pos);
    i=s, pos=s;
    for(int j=m; j<t; j++) {
        while(i<m && cmpy(qx[i], qx[j]))
            qy[pos++] = qx[i++];
        qy[pos++] = qx[j];
    }
    copy(qx+i, qx+m, qy+pos);
    copy(qy+s, qy+t, qx+s);
}

void cdq_x(const int &s, const int &t)
{
    if(t-s<=1) return;
    int m=(s+t)>>1;
    cdq_x(s, m);
    cdq_x(m, t);
    int i=s, pos=s;
    for(int j=m; j<t; j++) {
        while(i<m && cmpx(q[i], q[j])) {
            if(q[i].type == 1)
                qx[pos++] = q[i];
            i++;
        }
        if(q[j].type != 1)
            qx[pos++] = q[j];
    }
    cdq_y(s, pos);
    i=s, pos=s;
    for(int j=m; j<t; j++) {
        while(i<m && cmpx(q[i], q[j]))
            qx[pos++] = q[i++];
        qx[pos++] = q[j];
    }
    copy(q+i, q+m, qx+pos);
    copy(qx+s, qx+t, q+s);
}

int main()
{
    // ios::sync_with_stdio(false); cin.tie(0);
    int T;
    scanf("%d", &T);
    while(T--) {
        int n;
        scanf("%d", &n);
        qpos = 0; tpos = 0; apos = 0;
        for(int i=0; i<n; i++) {
            int type, xl, xr, yl, yr, zl, zr; 
            scanf("%d", &type);
            switch(type) {
                case 1:
                    scanf("%d %d %d", &xl, &yl, &zl);
                    // change to 0-indexed
                    xl--; yl--; zl--;
                    tab[tpos++] = zl;
                    tab[tpos++] = zl+1;
                    q[qpos++] = QUERY(1, xl, yl, zl, 1);
                    break;
                case 2:
                    scanf("%d %d %d %d %d %d", &xl, &yl, &zl, &xr, &yr, &zr);
                    // change to 0-indexed
                    xl--; yl--; zl--;

                    // for each [L, R] calculate sum_of[0, R+1) - sum_of[0, L)
                    q[qpos++] = QUERY(2, xl, yl, zl, apos);
                    q[qpos++] = QUERY(3, xl, yl, zr, apos);
                    q[qpos++] = QUERY(3, xl, yr, zl, apos);
                    q[qpos++] = QUERY(2, xl, yr, zr, apos);
                    q[qpos++] = QUERY(3, xr, yl, zl, apos);
                    q[qpos++] = QUERY(2, xr, yl, zr, apos);
                    q[qpos++] = QUERY(2, xr, yr, zl, apos);
                    q[qpos++] = QUERY(3, xr, yr, zr, apos);
                    apos++;
                    break;
                default:
                    break;
            }
        }
        // get discrete z
        sort(tab, tab+tpos);
        tpos = unique(tab, tab+tpos) - tab;
        BIT::len = tpos;
        fill(BIT::val, BIT::val+BIT::len, 0);
        for(int i=0; i<qpos; i++)
            q[i].z = lower_bound(tab, tab+tpos, q[i].z) - tab;

        cdq_x(0, qpos);
        for(int i=0; i<apos; i++)
            printf("%d\n", ans[i]);
        fill(ans, ans+apos, 0);
    }
    return 0;
}

