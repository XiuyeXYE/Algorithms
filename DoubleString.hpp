

#ifndef DOUBLECONVERSION_H_H_H
#define DOUBLECONVERSION_H_H_H

#include <Windows.h>



//说明:  将double类型浮点数转换为字符串
//参数:
//       lpszBuffer: 转换结果缓冲区
//       value: 要转换的浮点数
//       FractionalDigitCount: 保留的小数位数
//返回值: 转换结果（字符串）
LPTSTR Double2String(LPTSTR lpszBuffer, double value, UINT32 FractionalDigitCount);



//转换状态枚举
typedef enum
{
	SLD_OK,
	SLD_NODIGITS,
	SLD_UNDERFLOW,
	SLD_OVERFLOW
} SLD_STATUS;



//说明:  将字符串转换为double类型浮点数
//参数:
//       lpszString: 要转换的字符串
//       result: 转换结果（double类型浮点数）
//返回值: 转换状态枚举
SLD_STATUS String2Double(LPCTSTR lpszString, double& result);


#endif