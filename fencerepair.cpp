typedef long long ll;
int N, L[MAX_N];
void solve()
{
	ll ans = 0;
	
	//直到计算机到木板为1块时为止
	while(N>1)
	{
		int mii1=0,mii2=1;
		if(L[mii1]>L[mii2])swap(mii1,mii2);
		for(int i=2;i<N;i++)
		{
			if(L[i]<L[mii1]){
				mii2=mii1;
				mii1=i;
			}
			else if(L[i]<L[mii2])
			{
				mii2=i;
			}
		}
		//将两块板拼合
		int t=L[mii1]+L[mii2];
		ans+=t;
		if(mii1==N-1)
			swap(mii1,mii2);
		L[mii1]=t;
		L[mii2]=L[N-1];
		N--;
	}
	printf("%lld\n",ans);
}
