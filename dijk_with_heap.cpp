#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <ext/pb_ds/priority_queue.hpp>

using namespace std;
typedef __gnu_pbds::priority_queue<pair<long long, pair<int, int> >, greater<pair<long long, pair<int, int> > >, __gnu_pbds::thin_heap_tag > heap;
typedef heap::point_iterator myit;


const int maxn = 100000+10;
char stat[30+7];
vector<pair<int, long long> > edge[maxn];
long long dist[maxn][2];
myit pit[maxn][2];
int pre[maxn][2];
bool color[maxn];

int main ()
{
    int T; 
    scanf("%d",&T);
    heap p;
    for(int cas=1; cas<=T; cas++){
        int n,m;
        scanf("%d %d",&n,&m);
        for(int i=1; i<=n; i++){
            edge[i].clear();
            pit[i][0] = pit[i][1] = NULL;
        }
        memset(color, 0, (n+1)*sizeof(color[0]) );
        memset(pre, 0, (n+1)*sizeof(pre[0]));
        for(int i=0; i<m; i++){
            int a,b,c;
            scanf("%d %d %d",&a,&b,&c);
            edge[a].push_back(make_pair((long long)b,c));
        }
        int t;
        scanf("%d",&t);
        for(int i=0; i<t; i++){
            int temp;
            scanf("%d",&temp);
            color[temp] = true;
        }
        memset(dist, 0x3f, (n+1)*sizeof(dist[0]) );
        for(int i=1; i<=n; i++) {
            if(color[i]==0) continue;
            pit[i][0] = p.push(make_pair(0, make_pair(i, i) ) );
            dist[i][0] = 0;
            pre[i][0] = i;
        }
        long long ans;
        while(!p.empty()) {
            int x = p.top().second.first;
            int xpre = p.top().second.second;
            p.pop();
            int tag;
            if(xpre == pre[x][0]){
                pit[x][0] = NULL;
                tag = 0;
            } else {
                assert(xpre == pre[x][1]);
                pit[x][1] = NULL;
                tag = 1;
            }
            if(color[x] && xpre != x) {
                ans = dist[x][tag];
                break;
            }
            int l = edge[x].size();
            for(int i=0; i<l; i++){
                int y = edge[x][i].first;
                long long d = edge[x][i].second;
                if(pre[y][1] != xpre && (pre[y][0] == 0 || dist[y][0] > dist[x][tag] + d)) {
                    dist[y][0] = dist[x][tag] + d;
                    pre[y][0] = xpre;
                    if(pit[y][0] != NULL)
                        p.modify(pit[y][0], make_pair(dist[y][0], make_pair(y, xpre) ) );
                    else
                        pit[y][0] = p.push(make_pair(dist[y][0], make_pair(y, xpre) ) );
                } else if(pre[y][0] != xpre && (pre[y][1] == 0 || dist[y][1] > dist[x][tag] + d) ) {
                    dist[y][1] = dist[x][tag] + d;
                    pre[y][1] = xpre;
                    if(pit[y][1] != NULL)
                        p.modify(pit[y][1], make_pair(dist[y][1], make_pair(y, xpre) ) );
                    else
                        pit[y][1] = p.push(make_pair(dist[y][1], make_pair(y, xpre) ) );
                }
            }
        }
        while(!p.empty())p.pop();
        printf("Case #%d: %lld\n", cas, ans);
    }
    return 0;
}


