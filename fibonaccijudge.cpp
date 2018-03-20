#include<iostream>
using namespace std;


int Fibonacci(int n)
{
	if(n==1)return 1;
	if(n==2)return 1;
	return Fibonacci(n-1)+Fibonacci(n-2);
}
int main()
{
	int n;
	int m;
	cin>>n;
	while(n)
	{
	        m=Fibonacci(n);	
		if(m%2) cout<<"ODD"<<endl;
    		else cout<<"EVEN"<<endl;
                cin>>n;	

	}
	return 0;
}





