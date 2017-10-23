/*
An online algorithm to solve Least Common Ancestor(LCA) of a tree
Time complexity: O(n log n) for pre-processing, O(1) for each query
Memory cost: maxn * (6 + 2 * maxh) * sizeof(int) //maxn=50000: cost = 8.0MB
2017/4/25
*/
#include <algorithm>
#include <vector>

using namespace std;
const int maxn = 50000+10, maxh = 17;//2^(maxh-1) >= maxn
int rf[maxn], depth[maxn];
int seq[maxn<<1], rmq[maxn<<1][maxh];
int pos;
vector<int > vec[maxn];

bool cmp(int a, int b)
{
    return depth[seq[a]] < depth[seq[b]];
}

void seqadd(int node)
{
    rf[node]=pos;
    seq[pos]=node;
    pos++;
}

void DFS(int node)
{
    static int dep=0;
    dep++;
    seqadd(node);
    depth[node] = dep;
    for(unsigned i=0; i<vec[node].size(); i++){
        int &pi = vec[node][i];
        if(depth[pi])continue;
        DFS(pi);
        seqadd(node);
    }
    dep--;
}

void rmqinit()
{
    for(int i=0; i<pos; i++)
        rmq[i][0]=i;
    for(int i=1; i<maxh; i++)
    for(int j=0; j+(1<<i) <= pos; j++)//"<=" is required intead of "<"
        if(cmp(rmq[j][i-1], rmq[j+(1<<(i-1))][i-1] ) )
            rmq[j][i] = rmq[j][i-1];
        else
            rmq[j][i] = rmq[j+(1<<(i-1))][i-1];
}

int LCA(int a, int b)
{
    b++;
    int bit = 8 * sizeof(int ) - __builtin_clz(b-a);
    bit--;
    int ans;
    if(cmp(rmq[a][bit], rmq[b-(1<<bit)][bit] ) )
        ans = rmq[a][bit];
    else
        ans = rmq[b-(1<<bit)][bit];
    return seq[ans];
}
