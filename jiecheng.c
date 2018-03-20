#include<math.h>
#include<stdio.h>
int factorial(int n)
{
	long a[10000];
	int i,j,l,c,m=0,w;
	a[0]=1;
	for(i=1;i<=n;i++)
	{
		c=0;
		for(j=0;j<=m;j++)
		{
			a[j]=a[j]*i+c;
			c=a[j]/10000;
			a[j]=a[j]%10000;
		}
		if(c>0){
			m++;
			a[m]=c;
		}
	}
	w=m*4+log10(a[m])+1;
	printf("\n%ld",a[m]);
	for(i=m-1;i>=0;i--)
		printf("%4.4ld",a[i]);
	return w;
}
int main(){
	int n;
	int p;
	scanf("%d",&n);
	
	p=factorial(n);
	printf("%d",p);
}


