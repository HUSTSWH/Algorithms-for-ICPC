/*******
 *An implement of Aho-Corasick automaton(AC-AM).
 *Time complexity: O( L(T) + sum(L(Pi)) + m ) in which:
 *P: template string
 *T: target string
 *m: number of successive matches
 *prototype problem: HDU2222
 *******/

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;

const int maxn = 1000000+10,//capacity of Trie(maximum length of P * maximun number of P)
          sigma = 26;//number of char elements
int trie[maxn][sigma], drop[maxn], tpos;
int q[maxn];
char s[maxn];

//some array to save info of successive match in each node
int wordcnt[maxn], wordtag[maxn];
bool visited[10000+10];

inline int alloc() { return tpos++; }
inline int getson(int &son)
{
    return son?son:son=alloc();
}

void insert(const char s[], const int tag)
{
    int pos = 0;
    for(int i=0; s[i]; i++)
        pos = getson(trie[pos][s[i]-'a']);
    // operation to sign info or index of model string
    wordcnt[pos]++;
    wordtag[pos] = tag;
}

void ACinit()
{
    int qf=0, qe=0;
    for(int i=0; i<sigma; i++)
        if(trie[0][i])
            q[qe++] = trie[0][i];
    while(qf<qe){
        int x = q[qf++];
        int xf = drop[x];
        for(int i=0; i<sigma; i++){
            if(!trie[x][i]){
                trie[x][i] = trie[xf][i];
                continue;
            }
            drop[trie[x][i]] = trie[xf][i];
            q[qe++] = trie[x][i];
        }
        drop[x] = wordcnt[xf]?xf:drop[xf];
        // compress fail pointer to contain only wordtail
        // Very important! This operation reduces time complexity.
    } 
}

int AC(const char s[])
{
    int pos = 0, ans = 0;
    for(int i=0; s[i]; i++){
        pos=trie[pos][s[i]-'a'];
        for(int pp=wordcnt[pos]?pos:drop[pos]; pp; pp=drop[pp]){
            // operation when match happens
            
            // Note: break preferred for higher speed
            if(visited[wordtag[pp]]) break; 
            
            ans += wordcnt[pp];
            visited[wordtag[pp]] = 1;
            // end operation
        }
    }
    return ans;
}

void clear()
{
    memset(trie, 0, tpos*sizeof(trie[0]));
    memset(drop, 0, tpos*sizeof(int));
    memset(wordcnt, 0, tpos*sizeof(int));
    memset(wordtag, 0, tpos*sizeof(int));
    memset(visited, 0, sizeof(visited));
    tpos = 1;
}

int main ()
{
    int T;
    scanf("%d",&T);
    while(T--){
        int n;
        scanf("%d",&n);
        clear(); // DON'T MISS THIS LINE!!!
        for(int i=0; i<n; i++){
            scanf("%s",s);
            insert(s, i+1);
        }
        ACinit();  // DON'T MISS THIS LINE!!!
        scanf("%s",s);
        printf("%d\n", AC(s));
    }
    return 0;
}
