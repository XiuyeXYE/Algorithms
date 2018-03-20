#include<stdio.h>
#include<string.h>
void mult(char c[],char t[],int m)
{
	int i,l,j,k,flag,add=0;
	char s[100];
	l=strlen(c);
	for(i=0;i<l;i++)
		s[l-i-1]=c[i]-'0';
	for(i=0;i<l;i++)
	{
		k=s[i]*m+add;
		if(k>=10){
			s[i]=k%10;
			add=k/10;
			flag=1;
		}
		else
		{
			s[i]=k;
			flag=0;
			add=0;
		}
	}
		if(flag)
		{
			l=i+1;
			s[i]=add;
		}
		else l=i;
		for(i=0;i<l;i++)
		{
			t[l-1-i]=s[i]+'0';
		}
		t[l]='\0';
	
}
int main(){
	int i=0;
	char c[]={'3','4','6','8','9'};
	char t[9];
	mult(c,t,19);
	for(i=0;i<9;i++)
		printf("%d",t[i]);
}
