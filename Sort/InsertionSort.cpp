#include"MyOutput.hpp"

template<typename T>
void insertionSort(T t[],int len){
    // println(typeid(t).name());

    for(int i=1;i<len;i++){
        T tmp = t[i];
        // The following code is wrong!
        //  Why? j--后少了一次赋值操作!
        // for(int j=i-1;j>=0;j--){
        //     if(tmp < t[j]){
        //         t[j+1] = t[j];
        //     }
        //     else{
        //         t[j+1] = tmp;
        //     }
        // }
        int j = i-1;
        while(j>-1 && tmp < t[j]){
            t[j+1] = t[j];
            j--;
        }

        t[j+1] = tmp;

    }

}


int main(){

    int a[10000] = {0};
    createRandomArray(a,10000);
    // println(a,10000);
    TimeSpec::begin();
    insertionSort(a,10000);
    TimeSpec::print_cost_time();
    // println(a,10000);


    return 0;
}
