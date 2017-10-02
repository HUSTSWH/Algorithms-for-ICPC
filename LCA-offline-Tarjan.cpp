/*
 *An off-line method for Least Common Ancestor(LCA)
 *Time complexity: O(n + q) for total.
 */

#include <algorithm>
#include <vector>
using namespace std;

const int maxn = 1000;//total number of nodes in the tree
int fa[maxn];
int ans[maxn];
vector<int > vec[maxn];//edges of the graph
vector<int > query[maxn];//edges of the query

int find_set(int n)
{
    int ans = n;
    while(ans != tag[ans]) ans = tag[ans];
    int m = n;
    while(n != ans) {
        m = fa[n];
        fa[n] = ans;
        n = m;
    }
    return ans;
}

// recursive implement
//begin with root
void DFS(const int &node)
{
    fa[node]=node;
    for(unsigned i=0; i<query[node].size(); i++)if(fa[query[node][i]])
        //works after a query been calculated
        ans[find_set(query[node][i])]++;
    for(unsigned i=0; i<vec[node].size(); i++){
        DFS(vec[node][i]);
        fa[vec[node][i]]=node;
    }
}

// iterative implement
void solve()
{
    stack<pair<int, int> > s;
    s.push(make_pair(1, 1));
    fa[1] = 0;
    while(!s.empty()) {
        int x=s.top().first;
        int on = s.top().second;
        s.pop();
        if(on == 1) {
            s.push(make_pair(x, -1));
            fa[x] = x;
            int l = query[x].size();
            for(int i=0; i<l; i++) if(tag[query[x][i].first]) {
                q[query[x][i].second].lca = find_set(query[x][i].first);
            }
            l = edge[x].size();
            for(int i=0; i<l; i++) {
                fa[edge[x][i]] = x;
                s.push(make_pair(edge[x][i], 1));
            }
        } else {
            assert(on == -1);
            tag[x] = fa[x];
        }
    }
}
