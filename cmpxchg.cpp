#include<iostream>
#include<thread>
using namespace std;


/** 
* CMPXCHG r1/m,r2
* if eax == r1/m {
*	r1/m = r2;
*	zf = 1;	
* }
* else{
*	eax = r1/m;	
*	zf = 0;
* }
*	这个要修改内存,简单的方式,就是 首操作数 r1/m 就是内存,r1 和 r2 eax都是寄存器
*	
*
*	注:多核心 lock 指令?
*		赋值保证原子性?才能这样吧,否则不刷新变量内存的值,能执行但结果也不对吧?
*/
//刷新后变量获取准确的值,期望值,要修改的内存
int compare_and_swap(int old,int expect,int *dest){
	volatile int result = 0;
	
	__asm{
		mov eax,old;//如果old == *dest,则返回old的值(函数返回值调用约定:eax 为返回值)
		mov edx,dest;
		mov ecx,expect;
		//cmpxchg dword ptr[edx],ecx;
		cmpxchg [edx],ecx;//same
		//mov result,eax;
	}
	return;
}
//修改那个地址内存的值,一直到修改成功为止.
/*
	内存 addr指向的地址.
	old 读取原内存addr指向内存的值;
	old+1 期望的值,希望得到的值.
	其实就是单独的i++ 或者 ++i
	
	不停地循环读取,然后调用compare_and_swap,
	有可能现在addr指向内存的值已经改变,在执行compare_and_swap是不会成功的(why? cmpxchg sure that),
	然后又循环读取,然后调用compare_and_swap,直到addr指向内存的值改变了(就是和old旧值不相等了)
	
	这个有个好处 一般不会阻塞线程了
*/
void increment(int *addr){
	volatile int old = 0;
	do{
		old = *addr;//这里是不是要保证可见性
	}while(old != compare_and_swap(old,old+1,addr));
}

//每次加78下
void f(int *a){
	for(int i=0;i<78;i++){
		increment(a);
	}
	cout << "this thread id : " << this_thread::get_id() << endl;
	//this_thread::sleep_for(100s);//这个s怎么可以加在10后面呢? 暂停下可以在资源管理器看到线程数
	
}

//猜测 s achievement
int operator ""sencods(const char* timespace){
	cout << "input seconds: " << timespace << "s" << endl;
	
	return 1000*atoi(timespace);
}

void checkSuffix(){
	//cout << 10s << endl;//编译报错,可能不是哈
	cout << "output millisecond: " << 10sencods  << "ms"<< endl;
}


int main(){
	
	checkSuffix();
	
	cout << "===========================" << endl;

	int a = 0;
	cout << "init value a = " << a << endl;
	for(int i=0;i<20;i++){
		thread t(f,&a);
		t.join();//阻塞主线程运行,让子线程运行完
		//t.detach();//后台线程;这时候可以看到21线程如果快的话,main_thread + threads
	}
	//78 * 20 = 1560
	cout << "game over,last value a = " << a << endl;
	cin >> a;//暂停下让后台线程运行完,然后随意输入回车结束

	
	/*	
	cout << compare_and_swap(a-1,a+1,&a) << endl;
	cout << a << endl;
	cout << compare_and_swap(a,a+1,&a) << endl;
	cout << a << endl;
*/
	return 0;
}
