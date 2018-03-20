int N,R;
int X[MAX_N];
void solve(){
	sort(X,X+N);
	int i=0;ans=0;
	while(i<N)
	{
		//s没有被覆盖的最左的点的位置
		int s=X[i++];
		//一直向右前进直到距s的距离大于R的点
		while(i<N&&X[i]<=s+R)i++;
		//p是新加上标记的点的位置
		int p=X[i-1];
		//一直向右前进直到距p的距离大于R的点
		while(i<N&&X[i]<=p+R)i++;
		ans++;
	}
	printf("%d\n",ans);
}
