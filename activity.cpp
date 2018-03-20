#include<cstdio>
int max(int a,int b)
{
	return a>b?a:b;
}
int activityManage(int s[],int f[],int w[],int n ,int m);
void sort(int s[],int f[],int w[],int n)
{
	int a,b,c;
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(f[i]>f[j]){
				a=f[i];
				f[i]=f[j];
				f[j]=a;
				b=s[i];
				s[i]=s[j];
				s[j]=b;
				c=w[i];
				w[i]=w[j];
				w[j]=c;
			}
		}
	}
}
int activitymanage(int s[],int f[],int w[],bool a[],int n)
{
	a[0]=1;
	int i;
	int j=0;
	int p=w[0];
	for(i=1;i<n;i++)
	{
		if(s[i]>=f[j])
		{
			a[i]=1;
			j=i;
			p+=w[j];
		}
		else a[i]=0;
	}
	
	int tmp=0;
	int m=0;
	for(i=0;i<n;i++)
		if(tmp<w[i])
	   {
			tmp=w[i];
			
			m=i;
			

	   }
	
	p=max(p,activityManage(s,f,w,n,m));
	
	return p;
}
int activityManage(int s[],int f[],int w[],int n ,int m)
{
	int p=w[m];
	int k=m;
    for(int i=m+1;i<n;i++)
	{
		if(s[i]>f[k])
		{
			k=i;
			p+=w[i];
		}
		
	}
	k=m;
	for(int i=m-1;i>=0;i--)
	{
		if(f[i]<s[k])
		{
			k=i;
			p+=w[i];
		}
	}
	return p;
}

int main()
{
	int i,n;
	int p;
	bool a[100];
	int s[100],f[100],w[100];
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%d %d %d",&s[i],&f[i],&w[i]);
	}
	sort(s,f,w,n);
	p=activitymanage(s,f,w,a,n);
	printf("%d",p);
	return 0;
}
