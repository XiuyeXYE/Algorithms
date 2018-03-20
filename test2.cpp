#include<iostream>
using namespace std;
int b[3][2];
bool check( char a[][3])
{
	int g=-1,k=-1;
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
		{
			
			if(a[i][j]=='o'||a[i][j]=='#')
			{
				b[0][0]=i;
				b[0][1]=j;
				for(int m=-1;m<=1;m++)
					for(int n=-1;n<=1;n++)
					{
						if(m+i>=0 && m+i<=2 && n+j>=0 && n+j<=2)
						{
							if(a[i][j]==a[m+i][n+j])
							{
								b[1][0]=m+i;
								b[1][1]=n+j;

								
								if( m+m+i<0) g=2;
								if( m+m+i>2) g=0;
								if( n+n+j<0) k=2;
								if( n+n+j>2) k=0;
								if( g>0&&k>0)
								{
									if(a[i][j]==a[g][k])
									{
										b[2][0]=g;
										b[2][1]=k;
										g=-1;
										k=-1;
										return true;
									}


								}
								else
								{
 									if( g>0)
									{
										if(a[i][j]==a[g][n+n+j])
										{
											b[2][0]=g;
											b[2][1]=n+n+j;
											g=-1;
											return true;
										}
									}
									if(k>0)
									{
										if(a[i][j]==a[m+m+i][k])
										{
											b[2][0]=m+m+i;
											b[2][1]=k;
											k=-1;
											return true;
										}
									}
								}


							}
						}
					}
			}
		}
	return false;
}

int main()
{
	cout<<"Input eg:"<<endl;
	char a[3][3]={
		' ','#','o',
		' ','o','#',
		'o',' ','#'
	};
	

	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
			cout<<a[i][j];
		cout<<endl;
	}

	bool b0=check(a);
	if(b0)
	{
		cout<<"succeeded!"<<endl;
	}
	else
	{
		cout<<"failed!"<<endl;

	}

	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
			a[i][j]=' ';
	}

	cout<<endl;
	cout<<"Input:"<<endl;
	for(int i=0;i<3;i++)
	{
		cin.getline(a[i],4);
	}
	cout<<endl;
	cout<<"Output:"<<endl;

	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
			cout<<a[i][j];
		cout<<endl;
	}

	b0=check(a);
	if(b0)
	{
		cout<<"succeeded!"<<endl;
		cout<<"зјБъ:"<<endl;
	    for(int i=0;i<3;i++)
		cout<<"("<<b[i][0]<<","<<b[i][1]<<")"<<endl;
	}
	else
	{
		
		cout<<"failed!"<<endl;
		cout<<"зјБъ:"<<endl;
	    for(int i=0;i<3;i++)
		cout<<"(-,-)"<<endl;

	}
	cout<<endl;
	
	system("pause");
	return 0;
}
