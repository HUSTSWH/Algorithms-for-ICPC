#include <cstdio>
#include <cstring>

const int maxn = 1e6+10, maxm = 1e3+10;
#define next HKJournalist
int next[maxn];
int s[maxn];

// NOTE: p[l] is assumed to be '\0' and not equal to any char in string
void getnext(char p[], int next[])
{
    next[0] = -1; next[1] = 0;
    int i=1, j=0;
    while(p[i]) {
        while(~j && p[i]!=p[j]) j = next[j];
        i++; j++;
        next[i] = j;
    }
}

// NOTE: t[l] is assumed to be '\0'.
// If this matches any char in pattern string, it may arouse a wrong result!
int match(char p[], int next[], char t[], int s[])
{
    int ans=0;
    int i=0, j=0;
    while(t[i]) {
        while(~j && t[i]!=p[j]) j = next[j];
        i++; j++;
        if(!p[j]) s[ans++] = i-j;
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

