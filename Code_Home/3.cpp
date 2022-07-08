#include <stdio.h>
#include <stdbool.h>
//#include <algorithm>
#define ll long long
#define MAX(a,b) ((a) > (b) ? (a) : (b))

const ll mp[10][10]={
    {0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,2,2,2,3,3,3},
    {0,1,1,1,2,2,2,3,3,3},
    {0,1,1,1,2,2,2,3,3,3},
    {0,4,4,4,5,5,5,6,6,6},
    {0,4,4,4,5,5,5,6,6,6},
    {0,4,4,4,5,5,5,6,6,6},
    {0,7,7,7,8,8,8,9,9,9},
    {0,7,7,7,8,8,8,9,9,9},
    {0,7,7,7,8,8,8,9,9,9},
};

const ll score[10][10]={
    {0,0,0,0,0,0,0,0,0,0},
    {0,6,6,6,6,6,6,6,6,6},
    {0,6,7,7,7,7,7,7,7,6},
    {0,6,7,8,8,8,8,8,7,6},
    {0,6,7,8,9,9,9,8,7,6},
    {0,6,7,8,9,10,9,8,7,6},
    {0,6,7,8,9,9,9,8,7,6},
    {0,6,7,8,8,8,8,8,7,6},
    {0,6,7,7,7,7,7,7,7,6},
    {0,6,6,6,6,6,6,6,6,6},
};

ll a[10][10],Max,Next[10],hang[10][10],lie[10][10],gong[10][10];
struct node{
	ll place,value;
}zero[10];

ll Sum(){
    ll sum=0;
    for(ll i=1; i<=9; i++){
        for(ll j=1; j<=9; j++){
            sum+=a[i][j]*score[i][j];
        }
    }
    return sum;
}

bool check(ll x,ll y){
    if(hang[x][a[x][y]]) return false;
    if(lie[y][a[x][y]]) return false;
    if(gong[mp[x][y]][a[x][y]]) return false;
    return true;
}

ll cmp(node a,node b){
	if(a.value==b.value) return a.place<b.place;
	return a.value<b.value;
}

void qsort(int l, int r)
{
    if (l < r)
    {
        int i = l, j = r;
        node x = zero[l];
        while (i < j)
        {
            while(i < j && !cmp(zero[j] , x)) 
                j--;  
            if(i < j) 
                zero[i++] = zero[j];
            
            while(i < j && cmp(zero[i] , x)) 
                i++;  
            if(i < j) 
                zero[j--] = zero[i];
        }
        zero[i] = x;
        qsort(l, i - 1);  
        qsort(i + 1, r);
    }
}
void dfs(ll x,ll y){
//	printf("%lld %lld\n",x,y);
    if(x==10){
        Max=MAX(Max,Sum());
        return;
    }
    if(y==10){
        dfs(Next[x],1);
        return;
    }
    if(a[x][y]){
        dfs(x,y+1);
        return;
    }
    
    for(ll i=1; i<=9; i++){
        a[x][y]=i;
        if(check(x,y)){
            hang[x][a[x][y]]++;
            lie[y][a[x][y]]++;
            gong[mp[x][y]][a[x][y]]++;
            dfs(x,y+1);
            hang[x][a[x][y]]--;
            lie[y][a[x][y]]--;
            gong[mp[x][y]][a[x][y]]--;
        }
        a[x][y]=0;
    }
}


int main(){
    for(ll i=1; i<=9; i++){
    	zero[i].place=i;
        for(ll j=1; j<=9; j++){
            scanf("%lld",&a[i][j]);
            if(a[i][j]){
	            hang[i][a[i][j]]++;
	            lie[j][a[i][j]]++;
	            gong[mp[i][j]][a[i][j]]++;
			}
			else zero[i].value++;
        }
    }
    qsort(1,9);
    //std::sort(zero+1,zero+10,cmp);
    for(ll i=1; i<=8; i++) Next[zero[i].place]=zero[i+1].place;
    Next[zero[9].place]=10;
	
    dfs(zero[1].place,1);
    if(Max==0) printf("-1");
    else printf("%lld",Max);
}