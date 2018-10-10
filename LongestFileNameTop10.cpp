// LongestFileNameTop10.cpp : 定义控制台应用程序的入口点。
//


//编一个程序，查找你电脑D盘中文件名最长的10个文件，打印出来。 

//分析：windows电脑中文件名最长是228字节（文件名+点+后缀名）-经测试，中文和英文长度都一样是228长度，未测试是TCHAR类型还是char类型
//		文件夹名长度是216
//		经实践发现以上总结具有局限性，文件名长度与路径有关，路径本身越长，文件名越短
//		以fopen_s函数为测试基准，vs2015, win7 service pack 1 x64,
//		绝对路径：
//		当路径带1个斜杠时（如d://abc.txt）路径总长度不能超过259（包括字符串结束标志NUL)
//		当路径带2个斜杠时（如d://a//abc.txt）路径总长度不能超过260（包括字符串结束标志NUL)
//		当路径带3个斜杠时（如d://a//b//abc.txt）路径总长度不能超过262（包括字符串结束标志NUL)
//		当路径带4个斜杠时（如d://a//b//c//abc.txt）路径总长度不能超过262（包括字符串结束标志NUL)
//		当路径带5个及以上斜杠时（如d://a//b//c//d//abc.txt）路径总长度不能超过262（包括字符串结束标志NUL)
//		相对路径：
//		vs2015会使用宏自动补全路径，文件名长度为203（包括字符串结束标志NUL)
//		已经完成主体功能，不检查隐藏文件和系统文件
#include "stdafx.h"
#include "Macor.h"

//----------------------------
//
unsigned int DirNum = 0;		//遍历文件夹数量
unsigned int FileNum = 0;		//遍历文件数量
unsigned int SpecialNum = 0;	//特殊文件数量
struct FILETYPE ft = { NULL };
struct FNINFO *hTop = NULL;		//文件数组10，从长到短排序

//----------------------------
//
int MainDealWithArgv(int, TCHAR **, TCHAR *);
int MainSearchFile(TCHAR *);
int MainShowArr(void);
int MainCleanUp(struct FNINFO*);

//----------------------------
//
int RecursionDirAndFile(TCHAR *);
int ArrOfTop10(TCHAR *);
int GetStructLength(struct FNINFO*);
int KeepTop10(struct FNINFO*);

int _tmain(int argc, TCHAR **argv)
{
	/*函数内部变量*/
	TCHAR msg_t[MSGSIZE] = { NULL };
	TCHAR FilePath[MAXPATHSIZE] = { NULL };

	//----------------------------
	//处理命令行参数及初始化配置
	MainDealWithArgv(argc, argv, FilePath);

	//----------------------------
	//遍历文件
	MainSearchFile(FilePath);

	//----------------------------
	//打印数组
	MainShowArr();

	//----------------------------
	//清理内存
	MainCleanUp(hTop);

	system("pause");
    return 0;
}
//----------------------------
//
int MainDealWithArgv(int argc, TCHAR **argv, TCHAR *out_filepath) {
	/*函数内部变量*/
	

	/*...*/
	setlocale(LC_ALL, "chs");		//配置地域信息，使程序本地化	//使_tprintf()能输出中文
	switch (argc)
	{
	case 1: {
		_tprintf(_T("默认路径，\"D:\""));
		CopyMemory(out_filepath, _T("D:"), 4);
	}break;
	case 2: {
		CopyMemory(out_filepath, argv[1], 2 * wcslen(argv[1]));	//CopyMemory使用字节流BYTE, TCHAR是BYTE的两倍
	}break;
	default:
		_tprintf(_T("未定义该行为，暂时使用默认路径，\"D:\""));
		CopyMemory(out_filepath, _T("D:"), 4);
		break;
	}
	hTop = (struct FNINFO *)malloc(sizeof(struct FNINFO));
	ZeroMemory(hTop, sizeof(struct FNINFO));
	ZeroMemory(&ft, sizeof(struct FILETYPE));
	hTop->next = NULL;

	return 0;
}
int MainSearchFile(TCHAR *in_filepath) {
	/*函数内部变量*/
	struct _tfinddata_t FileInfo = { NULL };			//文件信息结构体
	TCHAR msg_t[MSGSIZE] = { NULL };
	char msg[MSGSIZE] = { NULL };
	FILE *file_t = NULL;
	int iFuncStat = 0;
	__int64 fileHandle = 0;								//文件句柄

	/*...*/
	_stprintf_s(msg_t, _T("%s"), in_filepath);
	RecursionDirAndFile(msg_t); 

	printf("文件夹:%u, 文件:%u, 特殊文件:%u\n", DirNum, FileNum, SpecialNum);
	
	return 0;
}
int MainShowArr(void) {
	/*函数内部变量*/
	struct FNINFO *curr = NULL;		//当前节点

	/*...*/
	curr = hTop->next;
	while (curr != NULL) {
		_tprintf(_T("name:%s, length:%llu\n"), curr->FileName, curr->FileNameLength);
		curr = curr->next;
	}

	return 0;
}
int MainCleanUp(struct FNINFO* hTop) {
	/*函数内部变量*/
	struct FNINFO *curr = NULL;		//当前节点
	struct FNINFO *next = NULL;		//上一级节点

	/*...*/
	curr = hTop;
	next = curr->next;
	free(curr);
	hTop = NULL;
	while (next != NULL) {
		curr = next;
		next = curr->next;
		free(curr);
	}
	
	return 0;
}

//----------------------------
//
int RecursionDirAndFile(TCHAR *in_filename) {
	/*函数内部变量*/
	struct _tfinddata_t FileInfo = { NULL };			//文件信息结构体
	__int64 fileHandle = 0;								//文件句柄
	TCHAR msg_t[MSGSIZE] = { NULL };
	int iFuncStat = 0;

	/*...*/
	_stprintf_s(msg_t, _T("%s\\*"), in_filename);
	fileHandle = _tfindfirst(msg_t, &FileInfo);		//文件遍历函数
	if (-1L == fileHandle) {
		_tprintf(_T("errno:%d, in_filename:%s, msg_t:%s, _tfindfirst, MainSearchFile\n"), errno, in_filename, msg_t);
		return -1; 
	}
	if ((_A_HIDDEN == (_A_HIDDEN&FileInfo.attrib)) || (_A_SYSTEM == (_A_SYSTEM&FileInfo.attrib))) {		//是系统文件或隐藏文件
		SpecialNum += 1;
	}
	else if (0 == wcscmp(_T(".\0"), FileInfo.name) || 0 == wcscmp(_T("..\0"), FileInfo.name)) {			//是'.','..'两个特殊文件夹
		;
	}
	else if(_A_SUBDIR == (_A_SUBDIR & FileInfo.attrib)) {												//是文件夹
		DirNum += 1;
		//_tprintf(_T("name:%s, attrib:%02x\n"), FileInfo.name, FileInfo.attrib);
		_stprintf_s(msg_t, _T("%s\\%s"), in_filename, FileInfo.name);
		iFuncStat = RecursionDirAndFile(msg_t);		//递归文件夹
		if (-1 == iFuncStat) {
			return -1;
		}
	}
	else {																								//是文件
		FileNum += 1;
		//_tprintf(_T("name:%s, attrib:%02x\n"), FileInfo.name, FileInfo.attrib);
		ArrOfTop10(FileInfo.name);
	}
	for (; -1L != _tfindnext(fileHandle, &FileInfo); iFuncStat = 0) {			//文件遍历函数				//递归文件夹
		if ((_A_HIDDEN == (_A_HIDDEN&FileInfo.attrib)) || (_A_SYSTEM == (_A_SYSTEM&FileInfo.attrib))) {		//是系统文件或隐藏文件
			SpecialNum += 1;
		}
		else if (0 == wcscmp(_T(".\0"), FileInfo.name) || 0 == wcscmp(_T("..\0"), FileInfo.name)) {			//是'.','..'两个特殊文件夹
			;
		}
		else if (_A_SUBDIR == (_A_SUBDIR & FileInfo.attrib)) {												//是文件夹
			DirNum += 1;
			//_tprintf(_T("name:%s, attrib:%02x\n"), FileInfo.name, FileInfo.attrib);
			_stprintf_s(msg_t, _T("%s\\%s"), in_filename, FileInfo.name);
			iFuncStat = RecursionDirAndFile(msg_t);		//递归文件夹
			if (-1 == iFuncStat) {
				return -1;
			}
		}
		else {																								//是文件
			FileNum += 1;
			//_tprintf(_T("name:%s, attrib:%02x\n"), FileInfo.name, FileInfo.attrib);
			ArrOfTop10(FileInfo.name);			//数组排序
		}
	}

	_findclose(fileHandle);
	return 0;
}
/*
*
*
*/
int ArrOfTop10(TCHAR *in_filename) {
	/*函数内部变量*/
	struct FNINFO *node = NULL;		//新增节点
	struct FNINFO *curr = NULL;		//当前节点
	struct FNINFO *last = NULL;		//上一级节点

	/*...*/
	node = (struct FNINFO*)malloc(sizeof(struct FNINFO));
	ZeroMemory(node, sizeof(struct FNINFO));
	node->FileNameLength = wcslen(in_filename);
	CopyMemory(node->FileName, in_filename, node->FileNameLength*2);	//告警，内存拷贝以字节流进行，而TCHAR占两个字节，注意
	node->next = NULL;

	curr = hTop->next;
	last = hTop;
	if (NULL == curr) {			//空de链表头部
		hTop->next = node;
		return 0;
	}
	else {						//非空链表头部，进行插入
		while (curr != NULL) {
			if (curr->FileNameLength < node->FileNameLength) {	//发现更长的文件名
				last->next = node;
				node->next = curr;
				if (GetStructLength(hTop) > 10) {				//添加之后数量超过10，进行截断
					KeepTop10(hTop);
				}
				return 0;
			}
			else {												//未发现更长的文件名
				last = curr;
				curr = curr->next;
				continue;
			}
		}
		if (GetStructLength(hTop) < 10) {						//未发现更长的文件名，且10个指标未达到
			last->next = node;
			return 0;
		}
		else {													//未发现更长的文件名，且10个指标已经达到
			free(node);
			return 0;
		}
	}

	return 0;
}
int GetStructLength(struct FNINFO *in_hTop) {
	/*函数内部变量*/
	int length = 0;
	struct FNINFO *curr = NULL;		//当前节点

	/*...*/
	curr = hTop->next;
	while (curr != NULL) {
		length += 1;
		curr = curr->next;
	}

	return length;
}
int KeepTop10(struct FNINFO *in_hTop) {
	/*函数内部变量*/
	struct FNINFO *curr = NULL;		//当前节点
	struct FNINFO *last = NULL;		//上一级节点

	/*...*/
	if (GetStructLength(in_hTop) <= 10) {
		return 0;
	}
	curr = hTop;
	last = curr;
	while (curr->next != NULL) {
		last = curr;
		curr = curr->next;
	}
	free(curr);
	last->next = NULL;

	return 0;
}