int dp[MAX_N+1][MAX_W+1];//记忆化数组
int rec(int i,int j)
{
	if(dp[i][j]>=0)
	{
		//已经计算过的话直接使用之前的结果
		return dp[i][j];
	}
	int res;
	if(i==n){
		res=0;
	}
	else if(j<w[i])
	{
		res=rec(i+1,j);
	}
	else
	{
		res=max(rec(i+1,j),rec(i+1,i-w[i])+v[i]);
	}
	//将结果记录记录在数组中
	return dp[i][j]=res;
}

void solve(){
	//用-1表示尚未计算过，初始化这个数组
	memset(dp,-1,sizeof(dp));
	printf("%d\n",rec(0,w));
}