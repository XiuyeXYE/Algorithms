#include"MyOutput.hpp"
// #include <algorithm>
template<typename T>
void dubble_sort_optimized(T *t,int len){//effect not good!

    bool swapped = true;
    // do{
    //     swapped = false;
    //     for(int i=0;i<len-1;i++){
    //         if(t[i] > t[i+1]){
    //             swap(t+i,t+i+1);
    //             swapped = true;
    //         }
    //     }
    // }while(swapped);
    for(;swapped;){
        swapped = false;
        for(int i=0;i<len-1;i++){
            if(t[i] > t[i+1]){
                swap(t+i,t+i+1);
                swapped = true;
            }
        }
    }
}
template<typename T>
void dubble_sort(T *t,int len){
    for(int j=0;j<len-1;j++){
        for(int i=0;i<len-j-1;i++){
            if(t[i] > t[i+1]){
                swap(t+i,t+i+1);
            }
        }
    }
}





int main(){

    int arr1[10000] = {0};
    createRandomArray(arr1,10000);

    int arr2[10000] = {0};

    copySameLengthArray(arr1,arr2,10000);

    
    TimeSpec::begin();
    dubble_sort(arr1,10000);
    TimeSpec::print_cost_time();

    //sort ordered nums
    TimeSpec::begin();
    dubble_sort(arr1,10000);
    TimeSpec::print_cost_time();
    TimeSpec::begin();
    dubble_sort_optimized(arr1,10000);
    TimeSpec::print_cost_time();

    println("=======================TEST OPTIMIZED DUBBLE SORT======================");
    
    TimeSpec::begin();
    //not good!
    dubble_sort_optimized(arr2,10000);
    // std::sort(arr2,arr2+10000);//approximately 0.003/4 s
    TimeSpec::print_cost_time();
    // println(arr2,10000);

    return 0;
}