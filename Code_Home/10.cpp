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