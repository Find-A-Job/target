// HugeNum30.cpp : 定义控制台应用程序的入口点。
//

//编写一个程序，计算1*2*3*。。。*100等于多少
//
//分析：大数乘法,模拟乘法叠加
//		以数组代替整型数值
//
#include "stdafx.h"
#include "Macor.h"

//------------------------------------------------
//全局变量

//------------------------------------------------
//主函数
int MainDealWithArgv(int, TCHAR **, TCHAR *, TCHAR *);
int MainMncfdj(TCHAR *, TCHAR *, TCHAR *);
int MainShowData(TCHAR *);

//------------------------------------------------
//
int HugeNumAdd(TCHAR *, int, TCHAR *, int*);
int InvertedOrder(TCHAR *, int);				//字符倒序排列

//------------------------------------------------
//
int _tmain(int argc, TCHAR **argv)
{
	//------------------------------------------------
	//
	TCHAR num1[HUGESIZE + 1] = { NULL };
	TCHAR num2[HUGESIZE + 1] = { NULL };
	TCHAR num5[HUGESIZE * 2 + 1] = { NULL };

	ZeroMemory(num1, _countof(num1) * 2);
	ZeroMemory(num2, _countof(num2) * 2);
	ZeroMemory(num5, _countof(num5) * 2);

	//------------------------------------------------
	//
	MainDealWithArgv(argc, argv, num1, num2);

	//------------------------------------------------
	//
	MainMncfdj(num1, num2, num5);

	//------------------------------------------------
	//
	MainShowData(num5);

	//------------------------------------------------
	//

	//system("pause");
    return 0;
}
//------------------------------------------------
//
int MainDealWithArgv(int argc, TCHAR **argv, TCHAR *out_num1, TCHAR *out_num2) {
	/*函数内部变量*/

	/*...*/
	setlocale(LC_ALL, "chs");
	switch (argc)
	{
	case 1: {
		_tprintf(_T("请输入两个数(每个数不超过1024位)\n"));
		exit(1);
		;
	}break;
	case 2: {
		_tprintf(_T("请输入两个数(每个数不超过1024位)\n"));
		exit(1);
		;
	}break;
	case 3: {
		for (int i = 0; i < wcslen(argv[1]); ++i) {
			if (argv[1][i] > 57 || argv[1][i] < 48) {
				_tprintf(_T("请输入数字\n"));
				exit(1);
			}
		}
		for (int i = 0; i < wcslen(argv[2]); ++i) {
			if (argv[2][i] > 57 || argv[2][i] < 48) {
				_tprintf(_T("请输入数字\n"));
				exit(1);
			}
		}
		CopyMemory(out_num1, argv[1], wcslen(argv[1]) * 2);
		CopyMemory(out_num2, argv[2], wcslen(argv[2]) * 2);
	}break;
	default:
		_tprintf(_T("未定义该操作\n"));
		exit(1);
		break;
	}

	return 0;
}
int MainMncfdj(TCHAR *in_num1, TCHAR *in_num2, TCHAR *out_num5) {
	/*函数内部变量*/
	TCHAR num3[HUGESIZE * 2 + 1] = { NULL };	//逐位乘法值
	TCHAR num4[HUGESIZE * 2 + 1] = { NULL };	//最终结果
	int last = 0;	//进位
	int res = 0;	//
	int LengA = (int)wcslen(in_num1);
	int LengB = (int)wcslen(in_num2);
	int LengNum3 = 0;
	int LengNum4 = 0;

	/*...*/
	ZeroMemory(num3, _countof(num3) * 2);
	ZeroMemory(num4, _countof(num4) * 2);
	for (int i = 0; i < LengA; ++i) {
		for (int j = 0; j < LengB; ++j) {
			if (('\0' == in_num1[LengA-i-1])||('\0' == in_num2[LengB-j-1])) {	//读取到空字符
				_tprintf(_T("空字符\n"));
				system("pause");
				exit(1);
			}
			if ((in_num1[LengA - i-1]<48) || (in_num1[LengA - i-1]>57) || (in_num2[LengB - j-1]<48) || (in_num2[LengB - j-1]>57)) {		//读取到非数字字符
				_tprintf(_T("非数字字符\n"));
				system("pause");
				exit(1);
			}
			if (48 == in_num1[LengA-i-1]) {								//因数是0，乘积直接为0,跳出循环
				break;
			}
			res = ((in_num1[LengA - i-1] - 48)*(in_num2[LengB - j-1] - 48) + last);
			num3[j + i] = res % 10 + 48;		//从左开始记录，当乘以10的倍数时需要向右移动位数
			last = res / 10;
		}
		
		if (48 == in_num1[LengA-i-1]) {								//因数是0，乘积直接为0，并进行下一位运算 '\48'表示字符0
			continue;
		}
		LengNum3 = LengB + i;
		if (last > 0) {							//进位
			num3[LengB + i] = last + 48;
			num3[LengB + i + 1] = '\0';
			LengNum3 = LengB + i + 1;
			last = 0;
		}
		for (int k = 0; k < i; ++k) {			//补0
			num3[k] = 48;
		}
		InvertedOrder(num3, LengNum3);		//换顺序
		HugeNumAdd(num3, LengNum3, num4, &LengNum4);		//值相加运算
		ZeroMemory(num3, _countof(num3) * 2);
	}

	CopyMemory(out_num5, num4, _countof(num4) * 2);

	return 0;
}
int MainShowData(TCHAR *in_num) {
	/*函数内部变量*/

	/*...*/
	for (int i = 0; i < wcslen(in_num); ++i) {
		_tprintf(_T("%c"), in_num[i]);
	}

	return 0;
}

//------------------------------------------------
//
int HugeNumAdd(TCHAR *in_num3, int in_len3, TCHAR *out_num4, int *out_len4) {
	/*函数内部变量*/
	int iMax = 0;
	int last = 0;
	int res = 0;
	int LengNum = 0;
	int LengN3 = in_len3;
	int LengN4 = *out_len4;
	TCHAR num[HUGESIZE * 2 + 1] = { NULL };

	/*...*/
	if (48 == in_num3[0]) {		//加数为0，直接返回函数
		return 0;
	}
	if (48 == out_num4[0]) {		//基加数为零，返回另一加数
		CopyMemory(out_num4, in_num3, in_len3 * 2);
		return 0;
	}
	if (LengN3 > LengN4) {   //找出较长的那个字符串
		iMax = LengN3;
	}else {
		iMax = LengN4;
	}
	for (int i = 0; i < iMax; ++i) {
		if ((LengN4 - i - 1) < 0) {
			res = in_num3[LengN3 - i - 1] - 48 + last;
			num[i] = res % 10 + 48;
			last = res / 10;
			continue;
		}
		if ((LengN3 - i - 1) < 0) {
			res = out_num4[LengN4 - i - 1] - 48 + last;
			num[i] = res % 10 + 48;
			last = res / 10;
			continue;
		}
		res = in_num3[LengN3 - i - 1] - 48 + out_num4[LengN4- i - 1] - 48 +last;
		num[i] = res % 10 + 48;
		last = res / 10;
	}
	LengNum = iMax;
	if (last > 0) {
		num[iMax] = last + 48;
		num[iMax + 1] = '\0';
		LengNum = iMax + 1;
	}
	InvertedOrder(num, LengNum);
	CopyMemory(out_num4, num, _countof(num) * 2);
	*out_len4 = LengNum;

	return 0;
}
int InvertedOrder(TCHAR *out_num, int in_size) {
	/*函数内部变量*/
	TCHAR num[HUGESIZE * 2 + 1] = { NULL };

	/*...*/
	ZeroMemory(num, _countof(num) * 2);
	for (int i = 0; i < in_size; ++i) {
		//CopyMemory(&(num[in_size - i - 1]), &(out_num[i]), 1 * 2);
		num[i] = out_num[in_size - i - 1];
	}
	CopyMemory(out_num, num, in_size * 2);

	return 0;
}

//------------------------------------------------
//
