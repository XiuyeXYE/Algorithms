#include"MyOutput.hpp"
template<typename T>
void selectionSort(T t[],int len){

    for(int i=0;i<len;i++){
        int minIdx = i;
        for(int j=i+1;j<len;j++){
            if(t[minIdx] > t[j]){
                minIdx = j;
            }    
        }
        swap(t+i,t+minIdx);
    }

}




int main(){

    int a[10000] = {0};
    createRandomArray(a,10000);
    // println(a,10000);
    TimeSpec::begin();
    selectionSort(a,10000);
    TimeSpec::print_cost_time();
    // println(a,10000);

    //no effect
    // std::vector<int> v(10);
    // createRandomArray(a,10);
    // for(int i=0;i<10;i++){
    //     print(v[i],' ');
    // }
    // println();
    return 0;
}