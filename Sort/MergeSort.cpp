#include"MyOutput.hpp"


template<typename T>
void MergeSortedArray(T a[],int alen,T b[],int blen){

    T *t = new T[alen+blen];

    int i = 0;
    int j = 0;
    int k = 0;
    while(i < alen && j < blen){
        if(a[i] < b[j]){
            t[k++] = a[i++];
        }
        else{
            t[k++] = b[j++];
        }
    }
    while(i<alen){
        t[k++] = a[i++];
    }
    while(j<blen){
        t[k++] = b[j++];
    }

    for(int n = 0;n<k;n++){
        a[n] = t[n];
    }


    delete []t;
}

template<typename T>
void MergeSort(T arr[],int len){
    if(len > 1){
        T *leftArr = arr;
        int leftArrLen = len/2;
        T *rightArr = arr + len/2;
        int rightArrLen = len - leftArrLen;
        MergeSort(leftArr,leftArrLen);
        MergeSort(rightArr,rightArrLen);
        MergeSortedArray(leftArr,leftArrLen,rightArr,rightArrLen);
    }
}





int main(){

    constexpr int len = 10000;
    int arr[len] = {0};
    createRandomArrayWithBase(arr,len,len);
    // printlnArr(arr,len);
    TimeSpec::begin();
    MergeSort(arr,len);
    TimeSpec::print_cost_time();
    // printlnArr(arr,len);
    println("======================================");
    constexpr int len2 = 100;
    int arr2[len2] = {0};
    createRandomArrayWithBase(arr2,len2,len2);
    printlnArr(arr2,len2);
    TimeSpec::begin();
    MergeSort(arr2,len2);
    TimeSpec::print_cost_time();
    printlnArr(arr2,len2);



    return 0;
}




