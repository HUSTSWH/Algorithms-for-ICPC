#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;

const int maxn = 1000;//total number of nodes in the tree
int fa[maxn];
int ans[maxn];
vector<int > vec[maxn];//edges of the graph
vector<int > query[maxn];//edges of the query

int bcj(const int &n)
{
    return n==fa[n]?n:fa[n]=bcj(fa[n]);
}

//begin with root
void DFS(const int &node)
{
    for(unsigned i=0; i<query[node].size(); i++)if(fa[query[node][i]])
        //works after a query been calculated
        ans[bcj(query[node][i])]++;
    fa[node]=node;
    for(unsigned i=0; i<vec[node].size(); i++){
        DFS(vec[node][i]);
        fa[vec[node][i]]=node;
    }
}
