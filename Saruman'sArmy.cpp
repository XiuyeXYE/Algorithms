int N,R;
int X[MAX_N];
void solve(){
	sort(X,X+N);
	int i=0;ans=0;
	while(i<N)
	{
		//sû�б����ǵ�����ĵ��λ��
		int s=X[i++];
		//һֱ����ǰ��ֱ����s�ľ������R�ĵ�
		while(i<N&&X[i]<=s+R)i++;
		//p���¼��ϱ�ǵĵ��λ��
		int p=X[i-1];
		//һֱ����ǰ��ֱ����p�ľ������R�ĵ�
		while(i<N&&X[i]<=p+R)i++;
		ans++;
	}
	printf("%d\n",ans);
}
