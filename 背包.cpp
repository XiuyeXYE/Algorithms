//����
int n,w;
int w[MAX_N],v[MAX_N];

//�ӵ�i����Ʒ��ʼ��ѡ����С��j�Ĳ���

int rec(int i,int j)
{
	int res;
	if(i==n)
	{
		//�Ѿ�û��ʣ����Ʒ
		res=0;
	}
	else if(j<w[j])
	{
		//�޷���ѡ�����Ʒ
		res=res(i+1,j);
	}
	else
	{
		//��ս�Ͳ���ս���������������һ��
		res=max(rec(i+1,j),rec(i+1,j-w[i])+v[i]);
	}
	return res;
}
void solve()
{
	printf("%d\n",rec(0,w));
}