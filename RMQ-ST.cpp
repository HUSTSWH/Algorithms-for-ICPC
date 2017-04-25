#include <algorithm>
using namespace std;

const int maxn = 50000+10;
int rmq[maxn][19];
int value[maxn];

void rmqinit()
{
    for(int i=0; i<pos; i++)
        rmq[i][0]=i;
    for(int i=1; i<19; i++)
    for(int j=0; j+(1<<i) <= pos; j++)//It is important to use "<=" instead of "<"
        if(value[ rmq[j][i-1] ] < value[ rmq[j+(1<<(i-1))][i-1] ])
            rmq[j][i] = rmq[j][i-1];
        else
            rmq[j][i] = rmq[j+(1<<(i-1))][i-1];
}

int rmqsearch(int a, int b)
{
    unsigned bit = 8 * sizeof(int ) - __builtin_clz(b-a);//To get floor( log2(b-a) )
    bit--;
    if(value[ rmq[a][bit] ] < value[ rmq[b-(1<<bit)][bit] ])
        return rmq[a][bit];
    else
        return rmq[b-(1<<bit)][bit];
}
