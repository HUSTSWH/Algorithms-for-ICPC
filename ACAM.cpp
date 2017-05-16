/* 
An implement of Aho-Corasick automaton(AC-AM).
Time complexity: O( L(T) + max(L(Pi)) + m ) in which:
P: template string
T: target string
m: number of successive matches
*/

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;

const int maxl = 1000000+10,//capacity of Trie(maximum length of P * maximun number of P)
		  sigma = 26;//number of char elements
int trie[maxn][sigma], drop[maxn], top;
int q[maxn];
char s[maxn];

//some array to save info of successive match in each node
int wordcnt[maxn], wordtag[maxn];
bool visited[10000+10];

void insert(const char s[], const int tag)
{
    int i,pos=0;
    for(i=0; s[i]; i++)
        pos = trie[pos][s[i]-'a']?trie[pos][s[i]-'a']:trie[pos][s[i]-'a']=top++;
	// operation to sign info or index of model string
    wordcnt[pos]++;
    wordtag[pos]=tag;
}

void ACinit()
{
    int qf=0, qe=0;
    for(int i=0; i<sigma; i++)
		if(trie[0][i])
			q[qe++]=trie[0][i];
    while(qf<qe){
        int x=q[qf++];
        for(int i=0; i<sigma; i++){
            int xf = drop[x];
            if(!trie[x][i]){
                trie[x][i] = trie[xf][i];
                continue;
            }
            drop[trie[x][i]] = trie[xf][i];
            q[qe++]=trie[x][i];
        }
    }
    for(int i=1; i<top; i++)
        drop[i]=wordcnt[drop[i]]?drop[i]:drop[drop[i]];
}

int AC(const char s[])
{
    int pos=0, ans=0;
    for(int i=0; s[i]; i++){
        pos=trie[pos][s[i]-'a'];
        for(int pp=pos; pp; pp=drop[pp]){
			// operation when match happens
			if(visited[wordtag[pp]])continue;
            ans+=wordcnt[pp];
            visited[wordtag[pp]]=1;
			// end operation
        }
    }
    return ans;
}

void clear()
{
    for(int i=0; i<top; i++)
        memset(trie[i], 0, sizeof(trie[0]));
    memset(drop, 0, top*sizeof(int));
    memset(wordcnt, 0, top*sizeof(int));
    memset(wordtag, 0, top*sizeof(int));
    memset(visited, 0, sizeof(visited));
}

int main ()
{
    int T;
    scanf("%d",&T);
    while(T--){
        int n;
        scanf("%d",&n);
        top=1;
        for(int i=0; i<n; i++){
            scanf("%s",s);
            insert(s, i+1);
        }
        ACinit();
        scanf("%s",s);
        printf("%d\n", AC(s));
        clear();
    }
    return 0;
}