/** problem name: anagrams by stack
 *  coder:mike
 *  date:2011-01-26
 *  note:��û��ÿһ�ε��ö�copyһ��ջ����Ϊ����Ϊ�⿪��̫�󣬲�����ҲΪ�Ҵ����ĺܶ��鷳
 *      �һ���������ԣ�����ʶ���ڻ��ݵĹ��ָ̻���ʼ״̬����Ҫ�ԡ�
 *      �����������ʽ�ϻ���һЩ��Ҫע��ĵط�����ĩ�޿ո�
 *  result: 0s,106kb,AC
 */

#include<stdio.h>
#include<string.h>
#define MAX_LEN 100
#define STACK_SIZE 200

char source[MAX_LEN],       /**< Դ�ַ��� */
    target[MAX_LEN],        /**< Ŀ���ַ��� */
    stack[STACK_SIZE];      /**<  ջ*/
int state[2*MAX_LEN];       /**< ÿһ�׶ε�״̬��Ҳ����·��OR�� */
int len;                    /**< �ַ������� */
int flag=0;                 /**< ����Ƿ��н� */

int print(int depth)        /**< ������ */
{
	int i=0;
	while(i<depth-1)        /**< ������i<depth-1����i<depth��������ո���WA */
	{
		if(state[i]==1)     /**< ����1��ʾѹջ��-1��ʾ��ջ */
			putchar('i');
		if(state[i]==-1)
			putchar('o');
		if(i<depth-1)       /**< ATTENTION!����ÿһ�����涼�пո����һ���ַ���û�У� */
			putchar(' ');
        i++;
	}
	putchar('\n');
	return 0;
}

int dfs(int depth,int npush,int npop)       /**< depth�����ѵ���ȣ�npush��ѹջ����npop����ջ�� */
{
    if(npush==len&&npop==len)               /**< ������ */
    {
        flag=1;                             /**< �޸ı�־���н� */
        print(depth);                       /**< ����� */
        return 0;
    }
    if(npush<len)                           /**< ����ѹջ */
    {
        state[depth-1]=1;                   /**< ��¼״̬ */
        stack[stack[0]]=source[npush];      /**< ��ջ */
        stack[0]++;                         /**< �ı�ջ��ָ�� */
        dfs(depth+1,npush+1,npop);          /**< GO ON */
        stack[0]--;     /**< ����Ҫ�ָ�ջ��״̬����Ϊ����ÿһ�ε��ö�����һ��ջ������������ */
    }
    if(stack[0]>0&&target[npop]==stack[stack[0]-1]) /**< �ж��Ƿ���Ե�ջ�����ܵ�ջ�ͻ��� */
    {
        state[depth-1]=-1;              /**< ��¼״̬ */
        int  tmp=stack[stack[0]-1];      /**< ��ΪҪ��ջ������Ҫ����ջԭ����״̬ */
        stack[0]--;                     /**< �ı�ջ��ָ�� */

        dfs(depth+1,npush,npop+1);      /**< ����DFS */
        stack[stack[0]]=tmp;            /**< �ָ�ջ��״̬ */
        stack[0]++;                     /**< �ָ�ջ��ָ�� */
    }
    return 0;
}

int main(void)
{
    stack[0]=1;                         /**< stack[0]ָ��ջ������һλ����ʼ��*/
	while(scanf("%s%s",source,target)!=EOF)
	{
	    puts("[");
		flag=0;                         /**< ��ʼ����־ */
		len=strlen(source);
		if(len==strlen(target))         /**< ���Ȳ�ͬ=������ */
            dfs(1,0,0);
        puts("]");
	}
	return 0;
}

