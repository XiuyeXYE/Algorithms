/*#include"anagrams.h"

#define MAX_N 100


char s[MAX_N];
char d[MAX_N];

int main()
{
	
	cin>>s>>d;
	
	
	solve(s,d);
	
	system("pause");
}*/
#include<iostream>


using namespace std;



void GetNumber( char a[],int c[],int len)
{
	for(int i=0;i<len;i++)
		c[i]=a[i]-'0';
}

int mult( int a[],int len)
{
	int *c=new int[2*len];

	memset(c,0,2*len*sizeof(int));

	for(int i=0;i<len;i++)
		for(int j=0;j<len;j++)
		{
			c[i+j]+=a[i]*a[j];

		}
	for(int i=0;i<2*len-1;i++)
	{
		c[i+1]+=c[i]/10;
		c[i]%=10;
	}

	for(int i=0;i<2*len;i++)
	{
		a[i]=c[i];
	}
	delete [] c;
	int i=2*len;
	
	while(!a[i--]);
	
	return i+2;
}


// 实现2*2=4  4*4=16  16*16=256 可以满足任意位数

void  pow(char a[],int n)
{
	
	int *c=new int[3*n];
	int len=strlen(a);

	memset(c,0,3*n*sizeof(int));
	
	GetNumber(a,c,len);
	
	/*for(int j=2*len;j>=0;j--)
	{
		cout<<c[j];
	}

	cout<<endl;*/

	for(int i=0;i<n;i++)
	{


		len=mult(c,len);
	

		
	}
	while(!c[len--]);
	for(int i=len+1;i>=0;i--)
	{
		cout<<c[i];
	}
	delete [] c;


}

int main()
{
	int len=0;
	/*int a[100];
	cout<<sizeof(a)<<endl;*/
	cout<<"Please input number length:";
	cin>>len;
	char *c=new char[len];
	/*cout<<sizeof(c)<<endl;*/
	int *p=new int[100];
	//cout<<sizeof(p)<<*(p-1)<<" "<<*(p-4)<<" "<<p-4<<" "<<p<<endl;//在首地址前16个字节即4个类型单元前存储数组长度
	memset(c,0,len*sizeof(char));
	int n=0;
	cout<<"Please input N times:";
	cin>>n;
	cout<<endl;
	cout<<"Please Enter Numbers:";
	cin>>c;
	cout<<endl;
	pow(c,n);
	delete [] c;
	return 0;
}



//#include<iostream>
//using namespace std;
//
//void malloc(int a[])
//{
//	for(int i=0;i<100;i++)
//		cout<<a[i]<<" ";
//	cout<<endl;
//}
//
//int main()
//{
//	int *b=new int[100];
//	memset(b,0,100*sizeof(int));
//	malloc(b);
//	for(int i=0;i<100;i++)
//	{
//		cout<<b[i]<<" ";
//
//	}
//	cout<<endl;
//	return 0;
//}













