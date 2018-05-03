/** problem name: anagrams by stack
 *  coder:mike
 *  date:2011-01-26
 *  note:我没有每一次调用都copy一份栈，因为我认为这开销太大，不过这也为我带来的很多麻烦
 *      我花了两天调试，才认识到在回溯的过程恢复初始状态的重要性。
 *      此题在输出格式上还有一些需要注意的地方，行末无空格！
 *  result: 0s,106kb,AC
 */

#include<stdio.h>
#include<string.h>
#define MAX_LEN 100
#define STACK_SIZE 200

char source[MAX_LEN],       /**< 源字符串 */
    target[MAX_LEN],        /**< 目标字符串 */
    stack[STACK_SIZE];      /**<  栈*/
int state[2*MAX_LEN];       /**< 每一阶段的状态，也就是路径OR解 */
int len;                    /**< 字符串长度 */
int flag=0;                 /**< 标记是否有解 */

int print(int depth)        /**< 输出结果 */
{
	int i=0;
	while(i<depth-1)        /**< 这里是i<depth-1不是i<depth否则会多出空格导致WA */
	{
		if(state[i]==1)     /**< 我用1表示压栈；-1表示弹栈 */
			putchar('i');
		if(state[i]==-1)
			putchar('o');
		if(i<depth-1)       /**< ATTENTION!不是每一个后面都有空格，最后一个字符后没有！ */
			putchar(' ');
        i++;
	}
	putchar('\n');
	return 0;
}

int dfs(int depth,int npush,int npop)       /**< depth：深搜的深度；npush：压栈数；npop：弹栈数 */
{
    if(npush==len&&npop==len)               /**< 求解完成 */
    {
        flag=1;                             /**< 修改标志：有解 */
        print(depth);                       /**< 输出解 */
        return 0;
    }
    if(npush<len)                           /**< 可以压栈 */
    {
        state[depth-1]=1;                   /**< 记录状态 */
        stack[stack[0]]=source[npush];      /**< 入栈 */
        stack[0]++;                         /**< 改变栈顶指针 */
        dfs(depth+1,npush+1,npop);          /**< GO ON */
        stack[0]--;     /**< 回溯要恢复栈的状态，因为不是每一次调用都复制一次栈。那样开销大 */
    }
    if(stack[0]>0&&target[npop]==stack[stack[0]-1]) /**< 判断是否可以弹栈，不能弹栈就回溯 */
    {
        state[depth-1]=-1;              /**< 记录状态 */
        int  tmp=stack[stack[0]-1];      /**< 因为要弹栈，所以要保存栈原来的状态 */
        stack[0]--;                     /**< 改变栈顶指针 */

        dfs(depth+1,npush,npop+1);      /**< 继续DFS */
        stack[stack[0]]=tmp;            /**< 恢复栈的状态 */
        stack[0]++;                     /**< 恢复栈顶指针 */
    }
    return 0;
}

int main(void)
{
    stack[0]=1;                         /**< stack[0]指向栈顶的上一位；初始化*/
	while(scanf("%s%s",source,target)!=EOF)
	{
	    puts("[");
		flag=0;                         /**< 初始化标志 */
		len=strlen(source);
		if(len==strlen(target))         /**< 长度不同=》跳过 */
            dfs(1,0,0);
        puts("]");
	}
	return 0;
}

