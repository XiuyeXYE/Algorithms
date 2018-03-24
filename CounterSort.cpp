#include"MyOutput.hpp"

template<typename T>
void CounterSort(T a[],int len){
    int max = 0;
    for(int i=0;i<len;i++){
        if(a[i] > max){
            max = a[i];
        }
    }

    if(max <= 0){
        println("Error:Someone of Input Array Element must be >0 at least,and don't contains negative-num (负数)");
        return;
    }
    T *tmp = new T[max+1]{0};
    
    //相当于桶排序的中(pre step)统计数字个数.
    for(int i=0;i<len;i++){
        tmp[a[i]]++;
    }
    //统计小于<=a[i]的数字个数
    //如<=maxVal的数字必定是len个(包含本身).
    //
    for(int i=1;i<=max;i++){
        tmp[i] += tmp[i-1];
    }
    // int totalNums = 0;
    // for(int i=0;i<=max;i++){
    //     print(i,'=',tmp[i],' ');
    //     totalNums += tmp[i];
    // }
    // println();
    // println("len= ",len," ; tatalNums= ",totalNums);

    //排序
    int *ret = new T[len]();
    for(int i=0;i<len;i++){
        //算法核心仔细体会过程,画图观察规律.
        //<=maxVal的数字必定是len个(包含本身).
        //那其他的数字也一样
        ret[tmp[a[i]] - 1] = a[i];
        tmp[a[i]]--;
    }
    //赋值给原来的数组.
    for(int i=0;i<len;i++){
        a[i] = ret[i];
    }
    delete []tmp;
}


int main(){
    //速度很快额!
    constexpr int len=10000;
    int a[len] = {0};
    createRandomArrayWithBase(a,len,len);
    // printlnArr(a,len);
    TimeSpec::begin();
    CounterSort(a,len);
    TimeSpec::print_cost_time();
    // printlnArr(a,len);
    println("========================================");
    constexpr int len2=10;
    int a2[len2] = {0};
    createRandomArrayWithBase(a2,len2,len2);
    printlnArr(a2,len2);
    TimeSpec::begin();
    CounterSort(a2,len2);
    TimeSpec::print_cost_time();
    printlnArr(a2,len2);

    return 0;
}