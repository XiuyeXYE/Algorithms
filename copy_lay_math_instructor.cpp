#include<map>
#include<stack>
#include<cctype>
#include<iostream>
#include<string>
using namespace std;
map<char,int> m;//���ַ�ӳ�䵽��ѧ
string s1,s2,r1,r2;
string transform(string s)//�ѱ��ʽת��Ϊ��׺���ʽ�������
{
	int i,j,len;
	char c[81];
	stack<char> exp;//����һ���ַ�ջ����������
	len=s.length();
	for(i=j=0;i<=len;i++){
		if(isalnum(s[i])) c[j++]=s[i];//����ǲ�����ֱ�ӵ�����
		else{
			switch(s[i]){
				case '(':
					exp.push(s[i]);
					break;
				case ')':
					while(exp.top()!='(')//����������
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

















