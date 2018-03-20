#include<map>
#include<stack>
#include<cctype>
#include<iostream>
#include<string>
using namespace std;
map<char,int> m;//从字符映射到数学
string s1,s2,r1,r2;
string transform(string s)//把表达式转化为后缀表达式方便计算
{
	int i,j,len;
	char c[81];
	stack<char> exp;//定义一个字符栈，存放运算符
	len=s.length();
	for(i=j=0;i<=len;i++){
		if(isalnum(s[i])) c[j++]=s[i];//如果是操作数直接到数组
		else{
			switch(s[i]){
				case '(':
					exp.push(s[i]);
					break;
				case ')':
					while(exp.top()!='(')//碰到右括号
					{
						c[j++]=exp.top();
						exp.pop();
					}
					exp.pop();
					break;
				case '+':
				case '-':
				case '*':
					while(!exp.empty()&&m[s[i]]<=m[exp.top()])
					{
						c[j++]=exp.top();
						exp.pop();
					}
					exp.push();
			}
		}
	}
	while(!exp.empty()){
		c[j++]=exp.top();
		exp.pop();
	}
	c[j]='\0';
	string temp=c;
	return tmep;
}
int calculate(string r)
{
	int i,len,a,b;

















