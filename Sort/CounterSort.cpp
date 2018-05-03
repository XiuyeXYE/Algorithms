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
        println("Error:Someone of Input Array Element must be >0 at least,and don't contains negative-num (����)");
        return;
    }
    T *tmp = new T[max+1]{0};
    
    //�൱��Ͱ�������(pre step)ͳ�����ָ���.
    for(int i=0;i<len;i++){
        tmp[a[i]]++;
    }
    //ͳ��С��<=a[i]�����ָ���
    //��<=maxVal�����ֱض���len��(��������).
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

    //����
    int *ret = new T[len]();
    for(int i=0;i<len;i++){
        //�㷨������ϸ������,��ͼ�۲����.
        //<=maxVal�����ֱض���len��(��������).
        //������������Ҳһ��
        ret[tmp[a[i]] - 1] = a[i];
        tmp[a[i]]--;
    }
    //��ֵ��ԭ��������.
    for(int i=0;i<len;i++){
        a[i] = ret[i];
    }
    delete []tmp;
}


int main(){
    //�ٶȺܿ��!
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