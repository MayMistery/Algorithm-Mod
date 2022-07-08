#include <stdio.h>
#include <stdbool.h>
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAXN 102

int f[MAXN][MAXN], n, m, k, x, y, z;
bool use[MAXN][MAXN];

int main()
{
    scanf("%d%d", &n, &m);

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            f[i][j] = 0x3f3f3f;

    for (int i = 1; i <= m; i++)
    {
        scanf("%d %d %d", &x, &y, &z);
        f[x][y] = f[y][x] = z;
    }

    scanf("%d", &k);

    for (int i = 1; i <= k; i++)
    {
        scanf("%d %d", &x, &y);
        use[x][y] = use[y][x] = true;
    }

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (use[i][j] == false && f[i][j] != 0x3f3f3f)
                f[i][j] = 0;

    for (int k = 1; k <= n; ++k)
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                f[j][i] = f[i][j] = MIN(f[i][j], f[i][k] + f[k][j]);
    scanf("%d %d", &x, &y);
    printf("%d", f[x][y]);
    return 0;
}