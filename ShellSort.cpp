#include"MyOutput.hpp"

template<typename T>
void ShellSort(T t[],int len){
    int insertNum;
    int gap = len/2;
    while(gap > 0){
        /**
         * 从后往前i = gap,i++,i<len;t[j-gap] ?= t[j];
         * 从前往后 i=0 ,i++,i<gap/2;t[i+j*gap] ?= t[i+(j+1)*gap];
         * 
         */ 
        for(int i=gap;i<len;i++){
            insertNum = t[i];
            int j=i;
            while(j>=gap && insertNum < t[j-gap]){
                t[j] = t[j-gap];
                j -= gap;
            }
            t[j] = insertNum;

        }
        gap /= 2;
    }
}


int main(){

    int arr[10000] = {0};
    createRandomArray(arr,10000);
    // println(arr,10000);
    TimeSpec::begin();
    ShellSort(arr,10000);
    TimeSpec::print_cost_time();
    // println(arr,10000);
    return 0;
}
