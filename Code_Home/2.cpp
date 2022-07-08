#include <stdio.h>
#include <stdbool.h>
#define MAX 30

int N,ans;
int f[MAX][MAX];
bool use1[MAX],use2[MAX],use3[MAX],use11[MAX],use22[MAX],use33[MAX],vis[MAX][MAX]; 

void DFS_w(int x)
{
    if (x==N) {ans++; return;}
    for (int y=0; y<N; y++)
    {
        if (f[x][y]==0||use1[y]||use2[y-x+N]||use3[y+x]||vis[x][y])
            continue;
        use1[y]=use2[y-x+N]=use3[y+x]=1,vis[x][y]=1;
        DFS_w(x+1);
        use1[y]=use2[y-x+N]=use3[y+x]=0,vis[x][y]=0;
    }
}

void DFS_b(int x)
{
    if (x==N) {DFS_w(0); return;}
    for (int y=0; y<N; y++)
    {
        if (f[x][y]==0||use11[y]||use22[y-x+N]||use33[y+x])
            continue;
        use11[y]=use22[y-x+N]=use33[y+x]=1,vis[x][y]=1;
        DFS_b(x+1);
        use11[y]=use22[y-x+N]=use33[y+x]=0,vis[x][y]=0;
    }
}

int main()
{
    scanf("%d", &N);
    for (int i=0; i<N; i++)
        for (int j=0; j<N; j++)
            scanf("%d", &f[i][j]);

    DFS_b(0);

    printf("%d",ans);
    return 0;
}

