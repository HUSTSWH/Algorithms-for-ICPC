/***
 * SA-IS: a linear algorithm to generate Suffix-Array
 * Thanks for RatingAccelerator2 http://uoj.ac/submission/61066
 * Time complexity: O(n)
 ***/


namespace SA {
    int sa[N], rk[N], ht[N], s[N<<1], t[N<<1], p[N], cnt[N], cur[N];
#define pushS(x) sa[cur[s[x]]--] = x
#define pushL(x) sa[cur[s[x]]++] = x
#define inducedSort(v) fill_n(sa, n, -1); fill_n(cnt, m, 0);                  \
    for (int i = 0; i < n; i++) cnt[s[i]]++;                                  \
    for (int i = 1; i < m; i++) cnt[i] += cnt[i-1];                           \
    for (int i = 0; i < m; i++) cur[i] = cnt[i]-1;                            \
    for (int i = nn-1; ~i; i--) pushS(v[i]);                                  \
    for (int i = 1; i < m; i++) cur[i] = cnt[i-1];                            \
    for (int i = 0; i < n; i++) if (sa[i] > 0 &&  t[sa[i]-1]) pushL(sa[i]-1); \
    for (int i = 0; i < m; i++) cur[i] = cnt[i]-1;                            \
    for (int i = n-1;  ~i; i--) if (sa[i] > 0 && !t[sa[i]-1]) pushS(sa[i]-1)

    void sais(int n, int m, int *s, int *t, int *p) {
        int nn = t[n-1] = 0, ch = rk[0] = -1, *ss = s+n;
        for (int i = n-2; ~i; i--) t[i] = s[i] == s[i+1] ? t[i+1] : s[i] > s[i+1];
        for (int i = 1; i < n; i++) rk[i] = t[i-1] && !t[i] ? (p[nn] = i, nn++) : -1;
        inducedSort(p);
        for (int i = 0, x, y; i < n; i++) if (~(x = rk[sa[i]])) {
            if (ch < 1 || p[x+1] - p[x] != p[y+1] - p[y]) ch++;
            else for (int j = p[x], k = p[y]; j <= p[x+1]; j++, k++)
                if ((s[j]<<1|t[j]) != (s[k]<<1|t[k])) {ch++; break;}
            y = x;
            ss[y] = ch;
        }
        if (ch+1 < nn) sais(nn, ch+1, ss, t+n, p+nn);
        else for (int i = 0; i < nn; i++) sa[ss[i]] = i;
        for (int i = 0; i < nn; i++) ss[i] = p[sa[i]];
        inducedSort(ss);
    }
    
    int mapCharToInt(int n, const char str[]) {
        int m = *max_element(str, str+n);
        fill_n(rk, m+1, 0);
        for (int i = 0; i < n; i++) rk[str[i]-'$'] = 1;
        for (int i = 0; i < m; i++) rk[i+1] += rk[i];
        for (int i = 0; i < n; i++) s[i] = rk[str[i]-'$'] - 1;
        return rk[m];
    }
    
    // Ensure that str[n] is the unique lexicographically smallest character in str.
    void suffixArray(int n, const char str[]) {
        int m = mapCharToInt(++n, str);
        sais(n, m, s, t, p);
        for (int i = 0; i < n; i++) rk[sa[i]] = i;
        for (int i = 0, h = ht[0] = 0; i < n-1; i++) {
            int j = sa[rk[i]-1];
            while (i+h < n && j+h < n && s[i+h] == s[j+h]) h++;
            ht[rk[i]] = h;
            if (h) h--;
        }
    }
};