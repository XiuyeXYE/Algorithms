#include<stdio.h>
#include<stdlib.h>
#define maxN 128
int h,w,ans;
char g[maxN][maxN];
int vis[maxN][maxN];
int dx[4]={0,0,-1,1};
int dy[4]={1,-1,0,0};
int ddx[4]={-1,1,1,-1};
int ddy[4]={1,-1,1,-1};
int dfs(int i,int j)
{
	vis[i][j]=1;
	int s=1;
	for(int k=0;k<4;++k){
		int nx=i+dx[k];
		int ny=j+dy[k];
		if(nx<0||nx>=h||ny<0||ny>=w)
			return -1;
		if(g[nx][ny]=='.'&&vis[nx][ny]==-1){
			int t=dfs(nx,ny);
			if(t==-1)
				return -1;
			s+=t;
		}	
	}
	for(int k=0;k<4;++k){
		int nx=i+ddx[k];
		int ny=j+ddy[k];
		if(nx<0||nx>=h||ny<0||ny>=w)
			return -1;
		int minx=min(nx,i);
		int miny=min(ny,j);
		int maxx=max(nx,i);
		int maxy=max(ny,j);
		if((minx==nx&&miny==ny)||(minx==i&&miny==j))
			if(g[minx][miny+1]=='/')
				continue;
		if((minx==nx&&maxy==ny)||(minx==i&&maxy==j))
			if(g[minx][miny]=='\\')
				continue;
		if(g[nx][ny]=='.'&&vis[nx][ny]==-1){
			int t=dfs(nx,ny);
			if(t==-1)
				return -1;
			s+=t;
		}	
	}		
	return s;
}

