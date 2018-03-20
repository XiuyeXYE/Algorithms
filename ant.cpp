int L,n;
int x[MAX_N];
void solve(){
	//calculate shortest time
	int minT=0;
	for(int i=0;i<n;i++)
	{
		minT=max(minT,min(x[i],L-x[i]));
	}
	//calculate longest time
	for(int i=0;i<n;i++)
	{
		maxT=max(maxT,max(x[i],L-x[i]));
	}
	cout<< minT << maxT <<endl;
}
