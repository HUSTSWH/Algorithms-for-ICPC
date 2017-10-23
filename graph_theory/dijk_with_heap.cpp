/***********************
 * Dijistra with heap
 * Time complexity: O(nlog(n) + m)
 * Author: songwenhao
 * Date: 2017/08/23
 *********************/
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <ext/pb_ds/priority_queue.hpp>

using namespace std;
typedef __gnu_pbds::priority_queue<pair<long long, pair<int, int> >, greater<pair<long long, pair<int, int> > >, __gnu_pbds::thin_heap_tag > heap;
typedef heap::point_iterator myit;


const int maxn = 100000+10;
vector<pair<int, int> > myvec[maxn];
int dist[maxn];
myit pit[maxn];

int main ()
{
    int n;
    memset(dist, 0x3f, sizeof(dist));
    while(scanf("%d",&n)!=EOF && n!=-1){
		// create graph
		...
		// create graph
        heap p;
        //for(int i=1; i<tag; i++)pit[i]=NULL;
        pit[1]=p.push(pair<int, int>(0, 1));
        dist[1]=0;
        while(!p.empty()){
            int x = p.top().second;
            p.pop();
            pit[x]=NULL;
            int l=myvec[x].size();
            for(int i=0; i<l; i++){
                int y=myvec[x][i].first;
                int d=myvec[x][i].second;
                if(dist[y]<=dist[x]+d)continue;
                dist[y] = dist[x]+d;
                if(pit[y]!=NULL)
                    p.modify(pit[y], pair<int, int>(dist[y], y));
                else
                    pit[y] = p.push(pair<int, int>(dist[y], y));
            }
        }
        for(int i=1; i<tag; i++)myvec[i].clear();
        // print answer
		...
		// print answer
    }
    return 0;
}

