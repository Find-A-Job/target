#pragma once
#ifndef MM_H_ZMX
#define MM_H_ZMX

#include "stdafx.h"

#define CANNOTFIND			-1
#define SLASHSIZE			32

#define URLSIZE				512
#define REQHEADSIZE			1024
#define ACCEPTSIZE			1024
#define FILESIZE			256
#define MSGSIZE				256
#define PATHSIZE			256

#define PROSIZE				8
#define DOMAINSIZE			64
#define PORTSIZE			8
#define DIRSIZE				128
#define FILESIZE			256
#define ARGVSIZE			128
#define ANCHORSIZE			64

struct ARGCVA
{
	int argc;
	TCHAR **argv;
	TCHAR URL[URLSIZE];
};

struct URLSTR
{
	TCHAR protocal[PROSIZE];
	TCHAR domain[DOMAINSIZE];
	TCHAR port[PORTSIZE];
	TCHAR dir[DIRSIZE];
	TCHAR file[FILESIZE];
	TCHAR argv[ARGVSIZE];
	TCHAR anchor[ANCHORSIZE];
};

struct FILESET
{
	TCHAR path[PATHSIZE];
	TCHAR file[FILESIZE];
};

#endif // !MM_H_ZMX
