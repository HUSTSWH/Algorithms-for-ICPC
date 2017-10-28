#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
typedef long long LL;

namespace num_theory {
    const int maxn = 1e5+9;
    int prime[maxn], miu[maxn], phi[maxn];
    bool npri[maxn];

    int sieve()
    {
        int pos = 0;
        phi[1] = 1;// NOTE: phi1 equals 1
        miu[1] = 1;// NOTE: miu1 equals 1
        for(int i=2; i<maxn; i++) {
            if(!npri[i]) {
                prime[pos++] = i;
                phi[i] = i-1;
                miu[i] = -1;
            }
            for(int j=0; i*prime[j]<maxn; j++) {
                const int &x=prime[j];
                npri[i*x] = true;
                if(i%x) {
                    phi[i*x] = phi[i]*phi[x];
                    miu[i*x] = -miu[i];
                } else {
                    phi[i*x] = phi[i]*x;
                    miu[i*x] = 0;
                    break;
                }
            }
        }
        return pos;
    }
};
using namespace num_theory;

int main()
{
    ios::sync_with_stdio(false); cin.tie(0);
    int pnum = sieve();
#ifdef SONG
    printf("pnum: %d\n", pnum);
    printf("prime:");
    for(int i=0; i<10; i++)
        printf(" %d", prime[i]);
    puts("");
    printf("phi:");
    for(int i=0; i<10; i++)
        printf(" %d", phi[i]);
    puts("");
    printf("miu:");
    for(int i=0; i<10; i++)
        printf(" %d", miu[i]);
    puts("");
#endif
    return 0;
}

