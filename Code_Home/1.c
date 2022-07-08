#include <stdio.h>
#define MAXN 1000005
int n, m, a[MAXN], b[2005], cnt, ans, l, r, ll, rr;
int main()
{
    scanf("%d %d", &n, &m);

    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);

    l = 1, r = 1, cnt = 1, b[a[1]] = 1;
    ans = MAXN;

    while (l <= r && r <= n)
    {
        if (cnt == m)
        {
            if (ans > r - l + 1)
            {
                ans = r - l + 1;
                ll = l;
                rr = r;
            }
            b[a[l]]--;
            if (b[a[l]] == 0)
                cnt--;
            l++;
        }
        else
            if (++b[a[++r]] == 1)
                cnt++;
    }

    printf("%d %d", ll, rr);
    return 0;
}