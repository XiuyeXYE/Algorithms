int dp[MAX_N+1][MAX_W+1];//���仯����
int rec(int i,int j)
{
	if(dp[i][j]>=0)
	{
		//�Ѿ�������Ļ�ֱ��ʹ��֮ǰ�Ľ��
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
	//�������¼��¼��������
	return dp[i][j]=res;
}

void solve(){
	//��-1��ʾ��δ���������ʼ���������
	memset(dp,-1,sizeof(dp));
	printf("%d\n",rec(0,w));
}