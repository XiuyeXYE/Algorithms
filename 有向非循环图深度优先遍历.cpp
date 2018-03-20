int edge[v][v],pre[v],post[v],tag;
void dfs(int cur,int n)//pre/post开始和结束
{
	pre[cur]=++tag;
	for(int i=0;i<n;i++)
	{
		if(edge[cur][i])
		{
			if(pre[i]==0)
			{
				printf("tree edge\n");
				dfs(i,n);
			}
			else
			{
				if(post[i]==0)
					printf("back edge\n");
				else if(pre[i]>pre[cur])
					printf("down edge\n");
				else printf("cross edge\n");
			}
		}
		post[cur]=++tag;
	}
}