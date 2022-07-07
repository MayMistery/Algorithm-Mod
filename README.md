
# 计蒜客实训进阶挑战部分题目题解

#### 0x01-0x04 题目难度一般且较经典，故此处略

#### 0x05-0x0a 题目均在[nanti.jisuanke.com]()

ps:有未解答的疑惑可以提issue

#### <u>***请勿抄袭***</u>

## 0x05 字符串

####  难度：提高T4 -> 省选

#### 思路简述：链表 + Tier维护border， dfs求border，

##### 详解：

$$
引：对于字符串s
$$

$$
pre(s,x) 表示字符串 s 长度为 x 的前缀
$$

$$
suf(s,x) 表示字符串 s 长度为 x 的后缀
$$

$$
若 0≤r<|s|, pre(s,r)=suf(s,r)，则称 r 为 s 的一个 border
$$

##### 	注意到题目所定义的独特前缀与border的定义类似，而border也是KMP中所定义的next指针（或fail）。题目可转化为求border并维护一个tier树上的border查询。对于求border，若用KMP求每个链的border时间复杂度接近n方，考虑树形dp。




```c++
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
//if you define DEBUG it will be a c++ program
//#define DEBUG
//File redirect
//#define FILEE
#ifdef DEBUG
    #include <iostream>
    #include <vector>
    using namespace std;
#endif
#define T 26
#define MAXN 210010
#define MAXL 18
#define Max(a, b) ((a) > (b) ? (a) : (b))

struct Query
{
    int idx, l, r;
};
struct LIST
{
    struct Query data;
    struct LIST * next;
};
struct TrieNode
{
    int d, bor, nx[T], pre[MAXL], Qlen;
#ifdef DEBUG
    vector<struct Query> Q;
#endif

#ifndef DEBUG
    struct LIST *Q;
    struct LIST *H;
#endif
} m[MAXN];

int cnt = 1, vis[MAXN], ans[MAXN], lim[MAXN][T];

#ifndef DEBUG
void push(struct LIST **Qtail, struct LIST **Qhead, struct Query d)
{
    struct LIST *p = NULL;
    if ((*Qtail) == NULL)
    {
        (*Qtail) = (struct LIST *)malloc(sizeof(struct LIST));
        (*Qtail)->next = NULL;
        (*Qtail)->data = d;
        (*Qhead) = (*Qtail);
    }
    else
    {
        p = (struct LIST *)malloc(sizeof(struct LIST));
        p->data = d;
        p->next = NULL;
        (*Qtail)->next = p;
        (*Qtail) = p;
    }
}
#endif

void init()
{
    int r = 1;
    m[r].d = 0;
    m[r].bor = r;
    for (int i = 0; i < MAXL; i++)
        m[r].pre[i] = r;
    for (int i = 0; i < T; i++)
        lim[r][i] = r;
}

int add(int x, char ch)
{
    if (m[x].nx[ch - 'a'])
        return m[x].nx[ch - 'a'];
    m[x].nx[ch - 'a'] = ++cnt;
    m[cnt].d = m[x].d + 1;
    return cnt;
}

int find(int x, int op)
{
    int tmp = 0;
    if (m[x].d < op)
        return tmp;
    op = Max(1, op);
    int cur = x;
    for (int l = MAXL - 1; l >= 0; l--)
    {
        if (m[m[cur].pre[l]].d >= op)
        {
            tmp += (1 << l);
            cur = m[cur].pre[l];
        }
    }
    return tmp + 1;
}

void dfs(int x)
{
    memcpy(lim[x], lim[m[x].bor], sizeof(lim[x]));
    m[x].pre[0] = m[x].bor;
    for (int i = 1; i < MAXL; i++)
        m[x].pre[i] = m[m[x].pre[i - 1]].pre[i - 1];

#ifdef DEBUG
    for (auto q : m[x].Q)
#endif

#ifndef DEBUG
        for (struct LIST *i = m[x].H; i != NULL; i = i->next)
#endif
        {
#ifndef DEBUG
            struct Query q = i->data;
#endif
            int l = find(x, q.l), r = find(x, q.r + 1);
            ans[q.idx] = l - r;
        }
    for (int i = 0; i < T; i++)
    {
        int y = m[x].nx[i];
        if (!y)
            continue;
        m[y].bor = lim[m[x].bor][i];
        int old = lim[x][i];
        lim[x][i] = y;
        dfs(y);
        lim[x][i] = old;
    }
}

int main()
{
#ifdef FILEE
    freopen("string.in", "r", stdin);
    freopen("string.out", "w", stdout);
#endif
    init();

    int n;
    scanf("%d", &n);

    int cur = 1;
    vis[0] = cur;
    for (int i = 1; i <= n; i++)
    {
        int op, x, l, r;
        char ch;
        scanf("%d", &op);
        if (op == 1)
        {
            scanf(" %c %d %d", &ch, &l, &r);
            cur = add(cur, ch);
            vis[i] = cur;
#ifdef DEBUG
            m[cur].Q.push_back((struct Query){i, l, r});
#endif
#ifndef DEBUG
            struct Query q;
            q.idx = i, q.l = l, q.r = r;
            push(&m[cur].Q, &m[cur].H, q);
#endif
        }
        else
        {
            scanf("%d %d %d", &x, &l, &r);
            cur = vis[x];
            vis[i] = cur;
#ifdef DEBUG
            m[cur].Q.push_back((struct Query){i, l, r});
#endif
#ifndef DEBUG
            struct Query q;
            q.idx = i, q.l = l, q.r = r;
            push(&m[cur].Q, &m[cur].H, q);
#endif
        }
    }

    dfs(1);

    for (int i = 1; i <= n; i++)
        printf("%d\n", ans[i]);
    return 0;
}
```



## 0x06 Good Karma

#### 难度：提高T4 -> 省选

#### 思路简述：推导公式，预处理后动态规划（含乘法逆元

```c++
#include <stdio.h>
#define MAXN 500000
#define MOD 998244353
#define ll long long
#define DEBUG

ll f[MAXN], g[MAXN], mim[MAXN], n, m, x, y, u, v;

ll fpm(ll z, ll power, ll mod)
{
    z %= mod;
    ll ans = 1;
    for (; power; power >>= 1, z = (z * z) % mod)
        if (power & 1)
            ans = (ans * z) % mod;
    return ans;
}

ll pre_fpm(ll x)
{
    return fpm(x, MOD - 2, MOD);
}

int main()
{
#ifdef DEBUG
    freopen("karma.in", "r", stdin);
    freopen("karma.out", "w", stdout);
#endif

    scanf("%d %d", &n, &m);

    ll U = (1 << n) - 1;

    for (int S = 0; S <= U; S++)
        g[S] = 1;

    for (int i = 0; i < m; i++)
    {
        scanf("%d %d %d %d", &u, &v, &x, &y);
        u--,v--;
        for (int S = 0; S <= U; S++)
            if ((S & (1 << u)) && (S & (1 << v)))
                g[S] = (((long long)g[S] * x) % MOD * pre_fpm(x + y)) % MOD;
    }

    for (int S = 0; S <= U; S++)
        mim[S] = pre_fpm(g[S]);
    
    int ans = 0;

    for (int S = 0; S <= U; S++)
    {
        if (!(S & 1))
            continue;
        f[S] = 1;
        int S0 = (S >> 1), T;
        for (int T0 = (S0 - 1) & S0; T0; T0 = (T0 - 1) & S0)
        {
            T = ((T0 << 1) | 1);
            f[S] = (f[S] - ((((long long)f[T] * g[S]) % MOD * mim[T]) % MOD * mim[S ^ T]) % MOD + MOD) % MOD;
        }
        T = 1;
        if (S != T)
            f[S] = (f[S] - ((((long long)f[T] * g[S]) % MOD * mim[T]) % MOD * mim[S ^ T]) % MOD + MOD) % MOD;
        
        int tt = 0, SS = S; 
        while (SS)
	    {
          SS &= (SS - 1);
          tt++;
        }

        ans = (ans + ((((((long long)tt * tt) % MOD * f[S]) % MOD * g[U]) % MOD * mim[S]) % MOD * mim[U ^ S]) % MOD) % MOD;
    }

    printf("%d\n", ans);
    return 0;
}

```





## 0x07 知识探讨会

#### 难度：提高T4 -> 省选（个人感觉最难

#### 思路简述：线段树或并查集后求类似区间最大子段和



> 朋友思路：
>
> 题目要求从1到n扫，扫到一个就执行一次区间覆盖。我们对s之前的先一个个处理（用线段树实现区间覆盖和单点查询），处理完后我们s在最前面。然后我们先不管s,考虑每一个后面的元素去修改它那个区间时候的01状态，因为是在环上一个个扫，扫到它的时候还没有扫到后面的，所以后面的不会对其造成影响；而前面的，因为是覆盖，只能是离他最近的且覆盖区间包含它的那一个最终覆盖了它，即：当前元素第一次扫到时的01状态刚好为它前面离他最近且覆盖区间包含它的那个元素第一次被扫到时的01状态与1异或的结果。那么我们顺序获取这个每个元素第一次被扫到时的01状态值即可。其中找到当前元素前面的离他最近的且区间包含它的还是用线段树维护（一个一个元素做，做到一个元素将它覆盖的区间区间覆盖为它的编号，这样单点查询即可获取答案）。然后考虑题目中要求的最后的01状态，我们如上已经知道每个元素刚被扫到时的01状态，相当于知道了每个元素去覆盖它那个区间时的01状态，那么我们只要知道每个元素最终被谁覆盖即可知道它最终的01状态。方法用线段树同上。
> 例如：6最终被9覆盖，9之前最近覆盖它的是7，7之前最近覆盖它的是4，4之前没有被任何区间覆盖，s=2,那么我们发现，6最终的值与4的值有直接关联，更具体的，若上面说的覆盖路径长度是偶数，6最终的值刚好为4的值，若奇数，6的值为4的值与1异或，而任何其它元素的值对6的值可以认为没有影响。因此，我们只需要对每个值都知道它被谁直接影响即可，这按前面所说的步骤在图上往前找即可。
> 最终我们知道每个元素被谁直接影响后，我们反过来统计每个元素会直接影响多少个其它元素，我们可以知道若该元素为0，最终可以导致多少元素为1，若该元素为1，最终可以导致多少元素为1
> 这样我们环上进行类似求区间和最大值就找到了答案

##### 详解：我在他的思路基础上使用并查集代替线段树（简单且可避免卡常）维护每个点最后被哪个点覆盖（分别计算轮到该点前和轮到该点后）。维护的具体方法是维护当前已经区间覆盖的区间的右端点， 从n到1枚举，覆盖未覆盖的点，同时合并两端的区间，遇到覆盖到的点时，转而维护该点所在区间右端点。



```c++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FOR(a,b) for(int i = (a); i <= (b); i++)
#define ROF(a,b) for(int i = (a); i > (b); i--)
#define N 2000006

int rd()
{
    register int ret;char ch;
    for (; (ch = getchar() - '0') < 0 || ch > 9;);
    for (ret = ch; (ch = getchar() - '0') >= 0 && ch <= 9; ret = (ret << 3) + (ret << 1) + ch);
    return ret;
}

int n, S, sum, state[N], l[N], r[N], bef[N], aft[N], f[N], rnk[N], ans = -2, Lans, Rans, ori;

//Union-Find set
int Find(int x)
{
    return f[x] ^ x ? f[x] = Find(f[x]) : x;
}

void Union(int x, int y)
{
    rnk[x] = rnk[rnk[y] > rnk[x] ? y : x]; 
    f[y] = x;
    /*    
    if(i==j) return ;
    if(rnk[i]>rnk[j]) f[j]=i;
    else
    {
        if(rnk[i]==rnk[j]) rnk[j]++;  
        f[i]=j;
    }
    */
}

//Merge region to creat connected block 
void Merge(int ll, int rr, int id, int *st)
{
    FOR(ll,rr)
    {
        if (st[i])
            i = rnk[Find(i)];
        else
        {
            st[i] = id;
            if (st[i - 1])
                Union(Find(i - 1), Find(i));
            if (st[i + 1])
                Union(Find(i), Find(i + 1));
        }
    }
}

//simulation
void preSimul(int ll, int rr)
{
    //slove bef
    FOR(1,n)
        f[i] = rnk[i] = i, bef[i] = 0;
    
    ROF(rr,ll-1)
        l[i] > r[i] ? Merge(l[i], n, i, bef), Merge(1, r[i], i, bef) : Merge(l[i], r[i], i, bef);
    
    //slove aft
    FOR(1,n)
        f[i] = rnk[i] = i, aft[i] = 0;
    
    ROF(rr,ll-1)
    {
        if (l[i] > r[i])
        {
            if (i < n)
                Merge((l[i] < i + 1) ? (i + 1) : l[i], n, i, aft);
            
            if (r[i] > i)
                Merge(i + 1, r[i], i, aft);
        }
        else if (r[i] > i)
            Merge((l[i] < i + 1) ? (i + 1) : l[i], r[i], i, aft);
    }
}

int main()
{
    freopen("Meeting.in", "r", stdin);
    freopen("Meeting.out", "w", stdout);

    n = rd(), S = rd();
    FOR(1,n)
        state[i] = rd();
    
    FOR(1,S-1)
        l[i] = rd(), r[i] = rd();
    
    //init 1 -> S-1
    preSimul(0, S - 1);

    //Simul 1 - S-1
    FOR(1,n)
        aft[i] ? state[i] = state[aft[i]] ^ 1 : 0;
    
    //init for next preSimul
    FOR(1,n)
    {
        f[i] = bef[i] ? (state[bef[i]] ^ 1) : state[i];
        bef[i] = aft[i] = 0;
    }
    
    FOR(1,n)
        state[i] = f[i];
    
    FOR(S+1,n)  
        l[i] = rd(), r[i] = rd();
    
    //preSimul s+1 -> n
    preSimul(S + 1, n);

    //cnt influence opposite
    FOR(1,n)
        l[i] = r[i] = 0;
    
    FOR(1,n)
        bef[i] ? (l[bef[i]]++) : (r[i]++);
    
    ROF(n,S)
        aft[i] ? (l[aft[i]] += r[i], r[aft[i]] += l[i]) : 0;
    
    //cal Maximum subsequence sum in circle
    FOR(1,n)
    {
        if (!state[S])
        {
            int tmp = l[i];
            l[i] = r[i];
            r[i] = tmp;
        }
        if (aft[i])
            l[i] = r[i] = 0;
        (state[S] ^ state[i]) ? (ori += l[i], l[i] = r[i] = 0) : (ori += r[i]);
    }

    f[0] = n, bef[0] = 0;

    FOR(1,n)
    {
        sum += l[i] - r[i];
        (sum < bef[i - 1]) ? (bef[i] = bef[i - 1], f[i] = f[i - 2]) : (bef[i] = sum, f[i] = i);
    }

    sum = S = 0;
    ROF(n,0)
    {
        sum += l[i] - r[i];
        (ans < sum + bef[i - 1]) ? (ans = sum + bef[i - 1], Lans = i, Rans = f[i - 1]) : 0;
    }

    f[0] = 0;
    FOR(1,n)
    {
        S += l[i] - r[i];
        (S < bef[i - 1]) ? (bef[i] = S, f[i] = i) : (bef[i] = bef[i - 1], f[i] = f[i - 1]);
    }

    S = 0;
    ROF(n,1)
    {
        S += l[i] - r[i];
        (ans < sum - S - bef[i - 2]) ? (ans = sum - S - bef[i - 2], Lans = f[i - 2] + 1, Rans = i - 1) : 0;
    }


    printf("%d\n%d %d", ans + ori, Lans, Rans);
    return 0;
}
```





## 0x08 最高段位

#### 难度：提高T3

#### 思路简述：dp

```c++
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Max(a,b) ((a) > (b) ? (a) : (b))
#define MAXN 5100
#define INF 0x3f3f3f
//#define DEBUG

int d, B, n, x, y, f[MAXN][MAXN][2], cnt0[MAXN];
char s[MAXN], ch;

void INPUT()
{
    scanf("%d %d", &d, &B);
    //getchar();

    while (~scanf(" %c", &ch))
    {
        s[++n] = ch;
        cnt0[n] += cnt0[n - 1] + (int)(ch == '0');
    }

    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= B; j++)
            f[i][j][0] = f[i][j][1] = -INF;
}

int main()
{
#ifdef DEBUG
    freopen("8.in", "r", stdin);
    freopen("8.out", "w", stdout);
#endif

    INPUT();

    f[0][0][0] = 0;

    for (int i = 1; i <= n; i++)
        for (int j = 0; j <= i - cnt0[i] && j <= B; j++)
        {
            if (s[i] != '1')
                f[i][j][0] = Max(f[i - 1][j][0], f[i - 1][j][1]);
            if (s[i] != '0' && j != 0)
            {
                f[i][j][1] = Max(f[i - 1][j - 1][0], f[i - 1][j - 1][1]);
                if (i >= d && j >= d && cnt0[i] - cnt0[i - d] == 0)
                    f[i][j][1] = Max(f[i][j][1], Max(f[i - d][j - d][0], f[i - d][j - d][1]) + 1);
            }
        }

    x = (2 * B - n + f[n][B][1]) / 5;
    y = (2 * B - n + f[n][B][0] + 1) / 5;

    printf("%d\n", Max(1, Max(x, y) + 1));
    return 0;
}
```





## 0x09 朋友圈

#### 难度：提高T3

#### 思路简述：质因数分解，求序列最小公比

```c++
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <set>

using namespace std;

#define MAXN 110000
#define INF 0x3f3f3f3f
#define Max(a, b) ((a) > (b) ? (a) : (b))
#define Min(a, b) ((a) < (b) ? (a) : (b))
#define ll long long
#define DEBUG

set<int> S;
ll n, ans, q = -1, cnt, tmp, a[MAXN], fac[MAXN], po[MAXN];

void fac_init(ll x)
{
    fac[x] = -1;
    ll s = Max(a[x], a[x - 1]), t = Min(a[x], a[x - 1]);
    if (s % t == 0)
    {
        ll div = s / t, ori = 1, pfac = -1;
        bool flag = 1;
        
        for (int i = 2; i <= 1000; i++)
        {
            tmp = 0;
            while (div % i == 0)
            {
                div /= i;
                tmp++;
            }
            // i^tmp
            if (tmp)
            {
                if (pfac == -1)
                    pfac = tmp;
                else if (pfac != tmp)
                {
                    flag = 0;
                    return;
                }
                ori *= i;
            }
        }
        if (div == 1)
        {
            fac[x] = ori; // ori^pfac = x/y
            po[x] = pfac * (a[x] > a[x-1] ? (1):(-1)); // power of ori
        }
    }
}
int main()
{
#ifdef DEBUG
    freopen("Moments.in", "r", stdin);
    freopen("Moments.out", "w", stdout);
#endif

    scanf("%lld", &n);

    for (int i = 0; i < n; i++)
    {
        scanf("%lld", &a[i]);
        if (i > 0)
            fac_init(i);
        if (i == 0 || a[i] == a[i - 1])
            cnt++;
        else
            ans = max(ans, cnt), cnt = 1;
    }
    ans = max(ans, cnt);

    for (int i = 0; i < n; i++)
    {
        S.clear();
        q = -1, tmp = 0;

        for (int j = 1; i + j - 1 < n; j++)
        {
            if (S.empty())
                S.insert(0);
            else
            {
                if (fac[i + j - 1] == -1)
                    break;
                if (q == -1)
                    q = fac[i + j - 1];
                else if(fac[i + j - 1] != q)
                    break;

                tmp += po[i + j - 1];
                if (!S.count(tmp))
                {
                    S.insert(tmp);
                    ans = Max(ans, j);
                }
                else
                    break;
            }
        }
    }

    printf("%lld\n", ans);
    return 0;
}
```





## 0x0a 宝箱

#### 难度：提高T2

#### 思路简述：树形dp(求反

```c++
#include <stdio.h>
#define ll long long
#define N 1100000
#define MOD 998244353
#define DEBUG

struct edge
{
    ll to, next;
} e[N << 1];

ll head[N], tot, n, ud[N], vd[N], d[N], dp[N][2], g[N][2], ans[N];

void swap(ll *a, ll *b)
{
    ll tmp = *a;
    *a = *b;
    *b = tmp;
}

void add(ll u, ll v)
{
    e[++tot].to = v;
    e[tot].next = head[u];
    head[u] = tot;
}

ll fpm(ll x, ll power, ll mod) {
    x %= mod;
    ll ans = 1;
    for (; power; power >>= 1, (x *= x) %= mod)
    	if(power & 1) (ans *= x) %= mod;
    return ans;
}

ll pre_fpm(ll a)
{
    return fpm(a, MOD - 2, MOD);
}

void dfsf(ll u, ll fa)
{
    dp[u][0] = dp[u][1] = 1;
    for (int i = head[u]; i; i = e[i].next)
    {
        ll v = e[i].to;
        if (v == fa)
            continue;
        d[v] = d[u] + 1;
        dfsf(v, u);
        dp[u][0] = (dp[u][0] * (dp[v][0] + dp[v][1]) % MOD) % MOD;
        dp[u][1] = (dp[u][1] * dp[v][0]) % MOD;
    }
}

void dfsg(ll u, ll fa)
{
    if (u == 1)
        g[u][0] = g[u][1] = 1;

    for (int i = head[u]; i; i = e[i].next)
    {
        ll v = e[i].to;
        if (v == fa)
            continue;
        //dfsg(v, u);
        ll mim = pre_fpm(dp[v][0] + dp[v][1]);//mul inv mod
        g[v][1] = g[u][0] * dp[u][0] % MOD * mim % MOD;
        g[v][0] = (g[u][0] * dp[u][0] % MOD * mim % MOD + g[u][1] * dp[u][1] % MOD * pre_fpm(dp[v][0]) % MOD) % MOD;
        dfsg(v, u); 
    }
}
int main()
{
#ifdef DEBUG
    freopen("chests.in", "r", stdin);
    freopen("chests.out", "w", stdout);
#endif

    scanf("%lld", &n);
    for (int i = 1; i < n; i++)
    {
        scanf("%lld %lld", &ud[i], &vd[i]);
        add(ud[i], vd[i]), add(vd[i], ud[i]);
    }

    dfsf(1, 0);
    dfsg(1, 0);

    for (int i = 1; i < n; i++)
    {
        if (d[ud[i]] > d[vd[i]])
            swap(&ud[i], &vd[i]);

        ll u = ud[i], v = vd[i];
        ll mim1 = dp[u][1] * pre_fpm(dp[v][0]) % MOD, mim2 = dp[u][0] * pre_fpm(dp[v][0] + dp[v][1]) % MOD;
        ans[i] = ((dp[v][1] + dp[v][0]) % MOD * ((g[u][1] * mim1) % MOD) % MOD + ((g[u][0] * mim2) % MOD * dp[v][1] % MOD)) % MOD;
        printf("%lld\n", ans[i]);
    }
    return 0;
}
```
