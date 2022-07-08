#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#define DEBUG
#define FILEE
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
/*
typedef struct DYNAMICARRAY
{
    Query* pAddr;
    int size;
    int cap;
}VECTOR;
*/
struct LIST
{
    Query data;
    struct LIST *next = NULL;
};
struct TrieNode
{
    int d, bor, nx[T], pre[MAXL], Qlen;
    // struct Query Q[500];
#ifdef DEBUG
    vector<struct Query> Q;
#endif
    // VECTOR Q;
#ifndef DEBUG
    LIST *Q = NULL;
    LIST *H = NULL;
#endif
} m[MAXN];

int cnt = 1, vis[MAXN], ans[MAXN], lim[MAXN][T];
#ifndef DEBUG
void push(struct LIST **Qtail, struct LIST **Qhead, struct Query d)
{
    struct LIST *p = NULL;
    if ((*Qtail) == NULL)
    {
        (*Qtail) = (struct LIST *)malloc(sizeof(LIST));
        (*Qtail)->next = NULL;
        (*Qtail)->data = d;
        (*Qhead) = (*Qtail);
    }
    else
    {
        p = (struct LIST *)malloc(sizeof(LIST));
        p->data = d;
        p->next = NULL;
        (*Qtail)->next = p;
        (*Qtail) = p;
    }
}
#endif
/*
void swapData(Query* oldAddr, int size, Query* newAddr)
{
    for (int i = 0; i < size; ++i)
        newAddr[i] = oldAddr[i];
}

void push(VECTOR* dyArr, Query data)
{
    Query* pTemp = (Query*)malloc(sizeof(Query)* dyArr->cap+1);
    swapData(dyArr->pAddr, dyArr->cap, pTemp);

    free(dyArr->pAddr);
    dyArr->pAddr = pTemp;
    pTemp = NULL;

    dyArr->cap = dyArr->cap + 1;

    dyArr->pAddr[dyArr->size] = data;
    dyArr->size++;
}
*/
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

        // for (int i = 0, l, r; i < m[x].Qlen; i++)
#ifdef DEBUG
    for (auto q : m[x].Q)
#endif
    // for(int i = 0; i < m[x].Q.size; i++)
#ifndef DEBUG
        // while(m[x].Q != NULL)
        for (struct LIST *i = m[x].H; i != NULL; i = i->next)
#endif
        {
            // struct Query q = m[x].Q[i];
            // struct Query q = m[x].Q.pAddr[i];
#ifndef DEBUG
            // struct Query q = m[x].Q->data;
            // m[x].Q = m[x].Q->next;
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
            // m[cur].Q[m[cur].Qlen++] = ((struct Query){i, l, r});
#ifdef DEBUG
            m[cur].Q.push_back((struct Query){i, l, r});
#endif
#ifndef DEBUG
            // struct Query q = (struct Query){i, l, r};
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
            // m[cur].Q[m[cur].Qlen++] = ((struct Query){i, l, r});
#ifdef DEBUG
            m[cur].Q.push_back((struct Query){i, l, r});
#endif
#ifndef DEBUG
            // struct Query q = (struct Query){i, l, r};
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