#include<iostream>


using namespace std;



void GetNumber( char a[],int c[],int len)
{
	for(int i=0;i<len;i++)
		c[i]=a[i]-'0';
}

int mult( int a[],int len,int b[],int size)
{
	int *c=new int[2*len];

	memset(c,0,2*len*sizeof(int));

	for(int i=0;i<len;i++)
		for(int j=0;j<size;j++)
		{
			c[i+j]+=a[i]*b[j];

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
	/*cout<<len<<endl;
	
	int *b=new int[len];
	
	int size=*(b-4);
	
	for(int i=0;i<len;i++)
		b[i]=c[i];
   
    
	cout<<size<<endl;
	*/
	int b[200]={0};
	int size=len;

	for(int i=0;i<len;i++)
		b[i]=c[i];
   
	for(int i=1;i<n;i++)
	{


		len=mult(c,len,b,size);
	

		
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
	cout<<"Please input number length:";
	cin>>len;
	char *c=new char[len];
	memset(c,0,len*sizeof(char));
	int n=0;
	cout<<endl;
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
