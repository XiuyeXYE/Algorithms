#include"MyOutput.hpp"


/**
 * ���ֲ�������,���Ѿ��ź��������,�����ַ�
 * �Ƚϴ�С����,����ȷ��left == �������м��(���ȷ���ĵ�)����.
 * a[left] > x (Ҫ����ĵ�)
 * 
 * 
 */ 
template<typename T>
void BinaryInsertionSort(T a[],int len){
    int mid=-1;
    for(int i=0;i<len;i++){
        int x = a[i];
        int left = 0;
        int right = i-1;
        //�жϹ����е��Ƿ�������
        //�����ٽ�����!
        //1 2 3 4 5 6 7 8 ;9
        //5;6 7 8
        //7;left = 8
        //8;left = 9 => 9<9 == false
        //a[9] = 9;
        while(left <= right){
            mid = (left + right)/2;
            if(x > a[mid]){
                left = mid + 1;
            }
            else{
                right = mid - 1;
            }
           
        }
        for(int j=i;j>left;j--){
            a[j] = a[j-1];
        }
        a[left] = x;
    }


}


int main(){

    constexpr int len = 10000;
    int a[len] = {0};
    createRandomArrayWithBase(a,len,len);
    // printlnArr(a,len);
    TimeSpec::begin();
    BinaryInsertionSort(a,len);
    TimeSpec::print_cost_time();
    // printlnArr(a,len);

    println("==========================================");

    constexpr int len2 = 10;
    int a2[len2] = {0};
    createRandomArrayWithBase(a2,len2,len2);
    printlnArr(a2,len2);
    TimeSpec::begin();
    BinaryInsertionSort(a2,len2);
    TimeSpec::print_cost_time();
    printlnArr(a2,len2);

    return 0;
}