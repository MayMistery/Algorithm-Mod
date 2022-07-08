#include <stdio.h>
#define FOR(a,b) for(int i = (a); i <= (b); i++)
#define ROF(a,b) for(int i = (a); i > (b); i--)
#define N 2000006
#define DEBUG


inline int rd()
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
#ifdef DEBUG
    freopen("Meeting.in", "r", stdin);
    freopen("Meeting.out", "w", stdout);
#endif

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