//优秀代码: 
#include <stdio.h>
int a,i,j,l;
void prime(int pri[])
{
    for(i=2;i<=100;i++)
        if(!pri[i])
            for(j=i+i;j<=100;j+=i)
                pri[j]=1;
}
void bingo(int ans[][45],int pri[])
{
    
    for(a=3;a<=100;a++)  //打表
    {
        ans[a][0]=1;
        for(i=2;i<=a;i++)
        {
            if(!pri[i])
            {
                j=i;
                while(j<=a)
                {
                    j*=i;
                    
		         int temp=0;

                    for(l=0;l<50;l++)
                    {
                        int p=ans[a][l]*i+temp;
                        ans[a][l]=p%10;
                        temp=p/10;
                    }
                }
            }
        }
    }
}
int main()
{
    int pri[100]={1,1,0},ans[101][45]={0};
    prime(pri);
    bingo(ans,pri);
    while(scanf("%d",&a)!=EOF)
    {
        if(a==1||a==2)
            printf("%d\n",a);
        else
        {
            for(i=44;i>=0;i--)
                if(ans[a][i]) break;
            for(j=i;j>=0;j--)
                printf("%d",ans[a][j]);
            printf("\n");
        }
    }
    return 0;
}
