
#ifndef __My_Print_And_Println
#define __My_Print_And_Println
#include<iostream>
/**
 *  First implementation (success!) 
 * 
 * 
 */
/*
template<class ...Args>
void println(Args ...args);

template<> 
void println(){ 
    std::cout << std::endl;
}

template < class T,class... Args> 
void println(T t,Args... args){
    std::cout << t ;
    // std::cout << args... << std::endl;
    // std::cout << "sizeof = " << sizeof...(args) << std::endl;
    println(args...);
    // println(args...);

}
*/

/**
 *  Second implementation (success,too!) 
 * 
 * 
 */
inline void println(){
    std::cout << std::endl;
}

template < class T,class... Args> 
inline void println(T t,Args... args){
    std::cout << t ;
    println(args...);
}

/**
 * 以下声明可要可不要!
 */ 
// template<typename...Args>
// void print(Args...args);

template<typename T>
inline void print(T t){
    std::cout << t;
}

template<typename T,typename... Args>
inline void print(T t,Args ...args){

    std::cout << t;
    print(args...);

}

template<typename T>
inline void println(T *t,int len){
    print('[');
    for(int i=0;i<len-1;i++){
        print(t[i],',');
    }
    print(t[len-1]);
    println(']');
}

#endif


#ifndef __TIMESPEC_H_
#define __TIMESPEC_H_
#include<ctime>


class TimeSpec{
    private:
        static clock_t start;
        static clock_t end;
    private:
        static double cost_time();
    public:
        static void begin();
        static double to_end();
        static void print_cost_time();
};


clock_t TimeSpec::start = 0;
clock_t TimeSpec::end = 0;
void TimeSpec::begin(){
    start = clock();
}
double TimeSpec::to_end(){
    end = clock();
    return cost_time();
}
double TimeSpec::cost_time(){
    return double(end - start)/CLOCKS_PER_SEC;
}

void TimeSpec::print_cost_time(){
    println("From beginning to end totally costs: ",to_end()," s");
}

#endif

#ifndef __MY_MATH_H
#define __MY_MATH_H

/**
 * a b t args 参数必须相同的类型,否则报错!
 * 
 * 
 * 
 */ 

template<class T>
T xy_max(T a,T b){
    return a>b?a:b;
}

template<class T,class...Args>
T xy_max(T t,Args ...args){
    return xy_max(t,xy_max(args...));
}

template<class T>
T xy_min(T a,T b){
    
    return a>b?b:a;
}

template<class T,class...Args>
T xy_min(T t,Args...args){

    return xy_min(t,xy_min(args...));
}


#endif


#ifndef __A0000001_H_
#define __A0000001_H_


template<typename T>
void swap(T *t1,T *t2){
    T t = *t1;
    *t1 = *t2;
    *t2 = t;
}


template<typename T>
void createRandomArray(T *t,int len){
     srand(time(nullptr));
    for(int i=0;i<len;i++){
        int rnd = rand()%10000;
        t[i] = rnd;
        // print(rnd,' ');
    }
    // println();
}
template<typename T>
void copySameLengthArray(T *src,T *dest,int len){
    for(int i=0;i<len;i++){
        dest[i] = src[i];
    }
}


#endif
