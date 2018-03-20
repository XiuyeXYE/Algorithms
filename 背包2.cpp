int dp[MAX_N+1][MAX_W+1];//dpÊý×é
void solve(){
	for(int i=n-1;i>=0;i--)
		for(int j=0;j<=w;j++)
	{
		if(j<w[i])
		{
			dp[i][j]=dp[i+1][j];
		}
		else
		{
			dp[i][j]=max(dp[i+1][j],dp[i+1][j-w[i]]+v[i]);
		}
	}
	printf("%d\n",dp[0][w]);
}
