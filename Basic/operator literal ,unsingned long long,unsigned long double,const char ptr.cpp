#include<iostream>
#include<thread>
using namespace std;
long a = 0;
void increment(){
	for(int i=0;i<10000;i++){
		a ++;
	}
	
}

void operator""S(unsigned long long S){
	cout << S << endl;
}
void operator""S(unsigned long double S){
	cout << S << endl;
}

int main(){

	1s+2s;
	cout << typeid(1s).name() << endl;
	//cout << type_index(typeid(1s)) << endl;
	cout << "test" << endl;

	1S;
	5.5S;
	thread t1(increment);
	thread t2(increment);
	t2.join();
	t1.join();
	
	cout << "end : " << a << endl;
	
	//cin >> a;
	
	return 0;
}