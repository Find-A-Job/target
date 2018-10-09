#pragma once
#ifndef MACOR_H_ZMX
#define MACOR_H_ZMX

#define MAXFILENAMESIZE		512
#define MAXPATHSIZE			512

#define MSGSIZE				256

struct FNINFO
{
	size_t FileNameLength;
	TCHAR FileName[MAXFILENAMESIZE];
	struct FNINFO *next;
};

#endif // !MACOR_H_ZMX

