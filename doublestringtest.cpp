#include"DoubleString.hpp"
#include<stdio.h>
#include<iostream>
using namespace std;

int main(){

    // LPTSTR buffer = {0};
    // LPTSTR s = Double2String(buffer,11.345678,6);
    HANDLE std = GetStdHandle(STD_OUTPUT_HANDLE);
    // HDC hdc = GetDC(GetConsoleWindow());
    // TextOut(hdc,400,300,TEXT("!wwwwww,eeeee"),14);
    printf("Test\n");
    fprintf(stdout,"TEST\n");
    cout << typeid(stdout).name() << endl;
    cout << typeid(std).name() << endl;
    cout << typeid(HWND).name() << endl;
    cout << typeid(HINSTANCE).name() << endl;
    return 0;
}