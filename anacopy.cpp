#include<cstdio>
#include<cstring>
#define MAX_LEN 100
#define STACK_SIZE 200
int state[2*MAX_LEN];
int len;
int flag=0;
int print(int depth)
{
	int i=0;
	while(i<depth-1)
	{
		if(state[i]==1)
			putchar('i');
		if(state[i]==-1)
			putchar('o');
		if(i<depth-1)
			putchar(' ');
		i++
	}
	putchar('\n');
	return 0;

}
int dfs(int depth,int npush,int npop)
{
	if(npush==len&&npop==len)
	{
		flag=1;
		print(depth);
		return 0;
	}
	if(npush<len)
	{
		state[depth-1]=1;
		stack[stack[0]]=source[npush];
		stack[0]++;
		dfs(depth+1,npush+1,npop);
		stack[0]--;
	}
	if(stack[0]>0&&target[npop]==stack[stack[0]-1])
	{
		state[depth-1]=-1;
		int tmp=stack[stack[0]-1];
		stack[0]--;
		dfs(depth+1,npush,npop+1);
		stack[stack[0]]=tmp;
		stack[0]++;
	}
	return 0;
}
int main()
{
	stack[0]=1;
	while(scanf("%s%s",source,target)!=EOF)
	{
		puts("[");
		flag=0;
		len=strlen(source);
		if(len==strlen(target))
			dfs(1,0,0);
		puts("]");
	}
	return 0;
}	

