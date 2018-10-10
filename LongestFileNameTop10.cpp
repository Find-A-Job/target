// LongestFileNameTop10.cpp : �������̨Ӧ�ó������ڵ㡣
//


//��һ�����򣬲��������D�����ļ������10���ļ�����ӡ������ 

//������windows�������ļ������228�ֽڣ��ļ���+��+��׺����-�����ԣ����ĺ�Ӣ�ĳ��ȶ�һ����228���ȣ�δ������TCHAR���ͻ���char����
//		�ļ�����������216
//		��ʵ�����������ܽ���о����ԣ��ļ���������·���йأ�·������Խ�����ļ���Խ��
//		��fopen_s����Ϊ���Ի�׼��vs2015, win7 service pack 1 x64,
//		����·����
//		��·����1��б��ʱ����d://abc.txt��·���ܳ��Ȳ��ܳ���259�������ַ���������־NUL)
//		��·����2��б��ʱ����d://a//abc.txt��·���ܳ��Ȳ��ܳ���260�������ַ���������־NUL)
//		��·����3��б��ʱ����d://a//b//abc.txt��·���ܳ��Ȳ��ܳ���262�������ַ���������־NUL)
//		��·����4��б��ʱ����d://a//b//c//abc.txt��·���ܳ��Ȳ��ܳ���262�������ַ���������־NUL)
//		��·����5��������б��ʱ����d://a//b//c//d//abc.txt��·���ܳ��Ȳ��ܳ���262�������ַ���������־NUL)
//		���·����
//		vs2015��ʹ�ú��Զ���ȫ·�����ļ�������Ϊ203�������ַ���������־NUL)
//		�Ѿ�������幦�ܣ�����������ļ���ϵͳ�ļ�
#include "stdafx.h"
#include "Macor.h"

//----------------------------
//
unsigned int DirNum = 0;		//�����ļ�������
unsigned int FileNum = 0;		//�����ļ�����
unsigned int SpecialNum = 0;	//�����ļ�����
struct FILETYPE ft = { NULL };
struct FNINFO *hTop = NULL;		//�ļ�����10���ӳ���������

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
	/*�����ڲ�����*/
	TCHAR msg_t[MSGSIZE] = { NULL };
	TCHAR FilePath[MAXPATHSIZE] = { NULL };

	//----------------------------
	//���������в�������ʼ������
	MainDealWithArgv(argc, argv, FilePath);

	//----------------------------
	//�����ļ�
	MainSearchFile(FilePath);

	//----------------------------
	//��ӡ����
	MainShowArr();

	//----------------------------
	//�����ڴ�
	MainCleanUp(hTop);

	system("pause");
    return 0;
}
//----------------------------
//
int MainDealWithArgv(int argc, TCHAR **argv, TCHAR *out_filepath) {
	/*�����ڲ�����*/
	

	/*...*/
	setlocale(LC_ALL, "chs");		//���õ�����Ϣ��ʹ���򱾵ػ�	//ʹ_tprintf()���������
	switch (argc)
	{
	case 1: {
		_tprintf(_T("Ĭ��·����\"D:\""));
		CopyMemory(out_filepath, _T("D:"), 4);
	}break;
	case 2: {
		CopyMemory(out_filepath, argv[1], 2 * wcslen(argv[1]));	//CopyMemoryʹ���ֽ���BYTE, TCHAR��BYTE������
	}break;
	default:
		_tprintf(_T("δ�������Ϊ����ʱʹ��Ĭ��·����\"D:\""));
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
	/*�����ڲ�����*/
	struct _tfinddata_t FileInfo = { NULL };			//�ļ���Ϣ�ṹ��
	TCHAR msg_t[MSGSIZE] = { NULL };
	char msg[MSGSIZE] = { NULL };
	FILE *file_t = NULL;
	int iFuncStat = 0;
	__int64 fileHandle = 0;								//�ļ����

	/*...*/
	_stprintf_s(msg_t, _T("%s"), in_filepath);
	RecursionDirAndFile(msg_t); 

	printf("�ļ���:%u, �ļ�:%u, �����ļ�:%u\n", DirNum, FileNum, SpecialNum);
	
	return 0;
}
int MainShowArr(void) {
	/*�����ڲ�����*/
	struct FNINFO *curr = NULL;		//��ǰ�ڵ�

	/*...*/
	curr = hTop->next;
	while (curr != NULL) {
		_tprintf(_T("name:%s, length:%llu\n"), curr->FileName, curr->FileNameLength);
		curr = curr->next;
	}

	return 0;
}
int MainCleanUp(struct FNINFO* hTop) {
	/*�����ڲ�����*/
	struct FNINFO *curr = NULL;		//��ǰ�ڵ�
	struct FNINFO *next = NULL;		//��һ���ڵ�

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
	/*�����ڲ�����*/
	struct _tfinddata_t FileInfo = { NULL };			//�ļ���Ϣ�ṹ��
	__int64 fileHandle = 0;								//�ļ����
	TCHAR msg_t[MSGSIZE] = { NULL };
	int iFuncStat = 0;

	/*...*/
	_stprintf_s(msg_t, _T("%s\\*"), in_filename);
	fileHandle = _tfindfirst(msg_t, &FileInfo);		//�ļ���������
	if (-1L == fileHandle) {
		_tprintf(_T("errno:%d, in_filename:%s, msg_t:%s, _tfindfirst, MainSearchFile\n"), errno, in_filename, msg_t);
		return -1; 
	}
	if ((_A_HIDDEN == (_A_HIDDEN&FileInfo.attrib)) || (_A_SYSTEM == (_A_SYSTEM&FileInfo.attrib))) {		//��ϵͳ�ļ��������ļ�
		SpecialNum += 1;
	}
	else if (0 == wcscmp(_T(".\0"), FileInfo.name) || 0 == wcscmp(_T("..\0"), FileInfo.name)) {			//��'.','..'���������ļ���
		;
	}
	else if(_A_SUBDIR == (_A_SUBDIR & FileInfo.attrib)) {												//���ļ���
		DirNum += 1;
		//_tprintf(_T("name:%s, attrib:%02x\n"), FileInfo.name, FileInfo.attrib);
		_stprintf_s(msg_t, _T("%s\\%s"), in_filename, FileInfo.name);
		iFuncStat = RecursionDirAndFile(msg_t);		//�ݹ��ļ���
		if (-1 == iFuncStat) {
			return -1;
		}
	}
	else {																								//���ļ�
		FileNum += 1;
		//_tprintf(_T("name:%s, attrib:%02x\n"), FileInfo.name, FileInfo.attrib);
		ArrOfTop10(FileInfo.name);
	}
	for (; -1L != _tfindnext(fileHandle, &FileInfo); iFuncStat = 0) {			//�ļ���������				//�ݹ��ļ���
		if ((_A_HIDDEN == (_A_HIDDEN&FileInfo.attrib)) || (_A_SYSTEM == (_A_SYSTEM&FileInfo.attrib))) {		//��ϵͳ�ļ��������ļ�
			SpecialNum += 1;
		}
		else if (0 == wcscmp(_T(".\0"), FileInfo.name) || 0 == wcscmp(_T("..\0"), FileInfo.name)) {			//��'.','..'���������ļ���
			;
		}
		else if (_A_SUBDIR == (_A_SUBDIR & FileInfo.attrib)) {												//���ļ���
			DirNum += 1;
			//_tprintf(_T("name:%s, attrib:%02x\n"), FileInfo.name, FileInfo.attrib);
			_stprintf_s(msg_t, _T("%s\\%s"), in_filename, FileInfo.name);
			iFuncStat = RecursionDirAndFile(msg_t);		//�ݹ��ļ���
			if (-1 == iFuncStat) {
				return -1;
			}
		}
		else {																								//���ļ�
			FileNum += 1;
			//_tprintf(_T("name:%s, attrib:%02x\n"), FileInfo.name, FileInfo.attrib);
			ArrOfTop10(FileInfo.name);			//��������
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
	/*�����ڲ�����*/
	struct FNINFO *node = NULL;		//�����ڵ�
	struct FNINFO *curr = NULL;		//��ǰ�ڵ�
	struct FNINFO *last = NULL;		//��һ���ڵ�

	/*...*/
	node = (struct FNINFO*)malloc(sizeof(struct FNINFO));
	ZeroMemory(node, sizeof(struct FNINFO));
	node->FileNameLength = wcslen(in_filename);
	CopyMemory(node->FileName, in_filename, node->FileNameLength*2);	//�澯���ڴ濽�����ֽ������У���TCHARռ�����ֽڣ�ע��
	node->next = NULL;

	curr = hTop->next;
	last = hTop;
	if (NULL == curr) {			//��de����ͷ��
		hTop->next = node;
		return 0;
	}
	else {						//�ǿ�����ͷ�������в���
		while (curr != NULL) {
			if (curr->FileNameLength < node->FileNameLength) {	//���ָ������ļ���
				last->next = node;
				node->next = curr;
				if (GetStructLength(hTop) > 10) {				//���֮����������10�����нض�
					KeepTop10(hTop);
				}
				return 0;
			}
			else {												//δ���ָ������ļ���
				last = curr;
				curr = curr->next;
				continue;
			}
		}
		if (GetStructLength(hTop) < 10) {						//δ���ָ������ļ�������10��ָ��δ�ﵽ
			last->next = node;
			return 0;
		}
		else {													//δ���ָ������ļ�������10��ָ���Ѿ��ﵽ
			free(node);
			return 0;
		}
	}

	return 0;
}
int GetStructLength(struct FNINFO *in_hTop) {
	/*�����ڲ�����*/
	int length = 0;
	struct FNINFO *curr = NULL;		//��ǰ�ڵ�

	/*...*/
	curr = hTop->next;
	while (curr != NULL) {
		length += 1;
		curr = curr->next;
	}

	return length;
}
int KeepTop10(struct FNINFO *in_hTop) {
	/*�����ڲ�����*/
	struct FNINFO *curr = NULL;		//��ǰ�ڵ�
	struct FNINFO *last = NULL;		//��һ���ڵ�

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