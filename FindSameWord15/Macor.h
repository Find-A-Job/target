#pragma once
#ifndef MACOR_H_ZMX
#define MACOR_H_ZMX
#include "stdafx.h"

//--------------------------------------------------
//����
#define MSGSIZE							256
#define WORDSIZE						64
#define FILEPATHSIZE					512

//--------------------------------------------------
//�ṹ��
struct STRUCTWORDTIMES
{
	char word[WORDSIZE];
	size_t times;
	STRUCTWORDTIMES *next;
};

#endif // !MACOR_H_ZMX
