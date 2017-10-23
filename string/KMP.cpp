#include <cstdio>
#include <cstring>

const int maxn = 1e6+10, maxm = 1e3+10;
#define next HKJournalist
int next[maxn];
int s[maxn];

void getnext(const char p[], int next[])
{
    next[0] = -1;
    next[1] = 0;
    int l = strlen(p);
    for(int i=1; i<l; i++){
        next[i+1] = 0;
        for(int j=next[i]; j>=0; j=next[j])
            if(p[i] == p[j]){
                next[i+1] = j+1;
                break;
            }
    }
}

// NOTE: t[l] should be different from any character in t
int match(const char p[], const int next[], const char t[], int s[])
{
    int l = strlen(t);
    int ans = 0;
    int j = 0;
    for(int i=0; i<l; i++){
        int k=j;
        for(j=0; k>=0; k=next[k])
            if(t[i] == p[k]){
                j = k+1;
                break;
            }
        if(p[j] == '\0')
            s[ans++] = i+1-j;
    }
    return ans;
}

int main ()
{
    const char pattern[] = "ababcabab", target[] = "abcababcababcababcabab";
    getnext(pattern, next);
    for(int i=0; pattern[i]; i++)
        printf("%c ", pattern[i]);
    puts("");
    for(int i=0; pattern[i]; i++)
        printf("%d ",next[i+1]);
    puts("");
    int num = match(pattern, next, target, s);
    printf("The number of successful match is: %d\n", num);
    for(int i=0; i<num; i++)
        printf("%s\n",target + s[i]);
    return 0;
}

