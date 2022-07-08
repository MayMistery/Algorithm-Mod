#include <stdio.h>
#define MAXN 5000
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
