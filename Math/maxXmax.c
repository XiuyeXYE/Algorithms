#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define N 100
//将数组中的字符串转成数字存到int数组中
void getdigits(int *a,char *s)
{

	int i;
	char digit;
	int len=strlen(s);
	//对数组初始化
	for(i=0;i<N;i++)
		*(a+i)=0;
	for(i=0;i<len;i++)
	{
		digit=*(s+i);
		*(a+len-1-i)=digit-'0';
	}
}


//将数组a与数组b逐位相乘后存入c
void multiply(int *a,int *b,int *c)
{
	int i,j;
	//数组初始化
	for(i=0;i<2*N;++i)
		*(c+i)=0;
	///将数组a与数组b逐位相乘后存入c
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			*(c+i+j)+=*(a+i)**(b+j);


	for(i=0;i<2*N-1;i++)
	{
		*(c+i+1)+=*(c+i)/10;
		*(c+i)=*(c+i)%10;
	}
}

int main(){
	
	int a[N],b[N],c[2*N];
	char s1[N],s2[N];
	int j=2*N-1;
	int i;
	printf("input the first number:");
	scanf("%s",s1);
	printf("\ninput the second number:");
	scanf("%s",s2);
	getdigits(a,s1);
	getdigits(b,s2);
	multiply(a,b,c);
	while(c[j]==0)j--;
	for(i=j;i>=0;i--)
		printf("%d",c[i]);
	printf("\n");
	return 0;
}



		
