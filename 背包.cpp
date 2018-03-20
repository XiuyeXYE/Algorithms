//输入
int n,w;
int w[MAX_N],v[MAX_N];

//从第i个物品开始挑选总重小于j的部分

int rec(int i,int j)
{
	int res;
	if(i==n)
	{
		//已经没有剩余物品
		res=0;
	}
	else if(j<w[j])
	{
		//无法挑选这个物品
		res=res(i+1,j);
	}
	else
	{
		//挑战和不挑战的两种情况都尝试一下
		res=max(rec(i+1,j),rec(i+1,j-w[i])+v[i]);
	}
	return res;
}
void solve()
{
	printf("%d\n",rec(0,w));
}