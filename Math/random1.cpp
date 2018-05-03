#include<iostream>
#include<ctime>
#include"MyOutput.hpp"
using namespace std;

static unsigned int a = 17;
static unsigned int b = 139;
static const unsigned long BASE_VALUE = ULONG_MAX;/*unsigned long max value*/
static unsigned int r = static_cast<unsigned int>(time(0)/*time_t is long*/);

inline void setRandSeed(unsigned int v){
    r = v;
}

inline const double random(){
    r = r*a + b;
    // println(r);
    r = r%BASE_VALUE;
    // println(r);
    double ret  = static_cast<double>(r);
    // println(ret);
    ret = ret / BASE_VALUE;
    // println(ret);
    
    return ret;
}

/**
 * For Test Random
 * P
 * 
 * 
 */ 
template<typename T>
const double varValiance(T arr[],int length){
    double X = 0;
    for(int i=0;i<length;i++){
        X += arr[i];
    }
    X /= length;
    double Sigma2 = 0;
    for(int i=0;i<length;i++){
        Sigma2 += (arr[i] - X)*(arr[i] - X);
    }
    Sigma2 /= length-1;
    return Sigma2;
}

void testP(int numOfArr,int testCount){

    double *count = new double[numOfArr]{0};
 
    // int count[100] = {0};
    
    for(int i=0;i<testCount;i++){
        count[int(numOfArr*random())] += 1; 
    }
    // println("输出每个元素的赋值次数:");
    // for(int i=0;i<numOfArr;i++){
    //     print(count[i],' ');
    // }
    println();


    double first = testCount;
    double second = numOfArr;

    double idealVal = first/second;
    
    println("理想情况下,",numOfArr,"个元素,","赋值总次数",testCount,",平均每个元素的赋值次数: ",idealVal);
    double varVal = varValiance(count,numOfArr);
    println("方差为: ",varVal);
    double abs = fabs(idealVal-varVal);
    println("相差: ",abs);

    delete []count;

}

static int main1(){

    println(sizeof time_t);
    const type_info & type = typeid(time_t);
    println("type name: ",type.name());
    println("type raw name: ",type.raw_name());

    for(int i=0;i<100;i++){
        print(random(),' ');
    }
    println();

    double d = random();
    println(d);

    println("======================");
    /** 
     * testP(随机数个数,所有元素赋值总次数)
     * 简单验证下随机生成函数的效果.
    */
    testP(10,100);
    
    println("=======================");
    testP(100,1000);
    println("=======================");
    testP(10,1021);
    println("=======================");
    testP(100,19999);
    println("=======================");
    testP(100,888);
    println("=======================");
    testP(1000,888);
    println("=======================");
    testP(10000,77777);
    println("========================");
    testP(1000,10000000);


    return 0;
  
}


