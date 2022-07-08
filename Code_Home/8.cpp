#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Max(a,b) ((a) > (b) ? (a) : (b))
#define MAXN 5100
#define INF 0x3f3f3f
#define DEBUG

int d, B, n, x, y, f[MAXN][MAXN][2], cnt0[MAXN];
char s[MAXN], ch;

void INPUT()
{
    scanf("%d %d", &d, &B);
    getchar();

    while (~scanf("%c", &ch))
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