#include<iostream>
using namespace std;
#define MAX_N 10
int n,a[MAX_N]={
	2,34,32,12,56,34,67,9,6,7
};
void solve()
{
	int ans=0;
	// 让i<k<j，这样棍子就不会被重复选中了
	for(int i=0;i<n;i++)
	{
		for(int j=i+1;j<n;j++)
		{
			for(int k=j+1;k<n;j++)
			{
				int len=a[i]+a[j]+a[k];
				int ma=max(a[i],max(a[j],a[k]));
				int rest=len-ma;
				if(ma<rest)
				{
					ans=max(ans,len);
				}
			}
		}
	}
	cout<<ans<<endl;
}
int max(int a,int b)
{
	if(a>=b)
	{
		return a;
	}
	else
	{
		return b;
	}
}


int main()
{
	n=10;
	solve();
}

