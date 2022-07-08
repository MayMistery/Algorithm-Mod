#include <iostream>
#include <cstdio>
#include <algorithm>
#include <set>

using namespace std;

#define MAXN 110000
#define INF 0x3f3f3f3f
#define Max(a, b) ((a) > (b) ? (a) : (b))
#define Min(a, b) ((a) <= (b) ? (a) : (b))
#define ll long long
#define DEBUG

set<int> S;
ll n, ans, q = -1, cnt, tmp, a[MAXN], fac[MAXN], po[MAXN];

void fac_init(ll x)
{
    fac[x] = -1;
    ll s = Max(a[x], a[x - 1]), t = Min(a[x], a[x - 1]);
    if(s == t)
        return;
    if (s % t == 0)
    {
        ll div = s / t, ori = 1, pfac = -1;
        bool flag = 1;
        for (int i = 2; i <= 1000; i++)
        {
            int t = 0;
            while (div % i == 0)
            {
                div /= i;
                t++;
            }
            // i^tmp
            if (t)
            {
                if (pfac == -1)
                    pfac = t;
                else if (pfac != t)
                {
                    flag = 0;
                    break;
                }
                ori *= i;
            }
        }
        if (div == 1 && flag)
        {
            fac[x] = ori; // ori^pfac = x/y
            po[x] = pfac; // power of ori
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