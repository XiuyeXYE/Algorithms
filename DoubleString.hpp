

#ifndef DOUBLECONVERSION_H_H_H
#define DOUBLECONVERSION_H_H_H

#include <Windows.h>



//˵��:  ��double���͸�����ת��Ϊ�ַ���
//����:
//       lpszBuffer: ת�����������
//       value: Ҫת���ĸ�����
//       FractionalDigitCount: ������С��λ��
//����ֵ: ת��������ַ�����
LPTSTR Double2String(LPTSTR lpszBuffer, double value, UINT32 FractionalDigitCount);



//ת��״̬ö��
typedef enum
{
	SLD_OK,
	SLD_NODIGITS,
	SLD_UNDERFLOW,
	SLD_OVERFLOW
} SLD_STATUS;



//˵��:  ���ַ���ת��Ϊdouble���͸�����
//����:
//       lpszString: Ҫת�����ַ���
//       result: ת�������double���͸�������
//����ֵ: ת��״̬ö��
SLD_STATUS String2Double(LPCTSTR lpszString, double& result);


#endif