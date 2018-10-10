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

struct FILETYPE
{
	unsigned int DirNum;
	unsigned int SysFileNum;
	unsigned int HideFileNum;
	unsigned int ArchNum;
	unsigned int NorNum;
	unsigned int RONum;
};

#endif // !MACOR_H_ZMX

