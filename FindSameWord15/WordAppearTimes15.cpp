// WordAppearTimes15.cpp : 定义控制台应用程序的入口点。
//

//已知一个文件，里面有很多单词，编一个程序，计算一个文档中每一个单词出现的次数。
//
//分析：建立链表存储所有出现过的单词
//		单词区分条件：从第一个英文字母到另一个非英文字母，期间的所有字母组成一个单词
//

#include "stdafx.h"
#include "Macor.h"

//--------------------------------------------------
//全局变量


//--------------------------------------------------
//主函数
int MainDealWithArgv(int, TCHAR **, TCHAR *);
int MainReadWork(TCHAR *, STRUCTWORDTIMES *);
int MainShowData(STRUCTWORDTIMES *);
int MainCleanUp(STRUCTWORDTIMES *);

//--------------------------------------------------
//
int DealWithWord(char *, STRUCTWORDTIMES *);

int _tmain(int argc, TCHAR **argv)
{
	//--------------------------------------------------
	//函数内部变量
	TCHAR FilePath[FILEPATHSIZE] = { NULL };
	STRUCTWORDTIMES *hTop = (STRUCTWORDTIMES *)malloc(sizeof(STRUCTWORDTIMES));

	ZeroMemory(hTop, sizeof(STRUCTWORDTIMES));
	hTop->next = NULL;

	//--------------------------------------------------
	//
	MainDealWithArgv(argc, argv, FilePath);

	//--------------------------------------------------
	//
	MainReadWork(FilePath, hTop);

	//--------------------------------------------------
	//
	MainShowData(hTop);

	//--------------------------------------------------
	//
	MainCleanUp(hTop);
	free(hTop);

    return 0;
}

//--------------------------------------------------
//主函数
int MainDealWithArgv(int argc, TCHAR **argv, TCHAR *out_filepath) {
	/*函数内部变量*/

	/*...*/
	setlocale(LC_ALL, "chs");
	switch (argc)
	{
	case 1: {
		_tprintf(_T("请选择一个文件作为参数\n"));
		system("pause");
		exit(1);
	}break;
	case 2: {
		_tprintf(_T("%s\n"), argv[1]);
		CopyMemory(out_filepath, argv[1], wcslen(argv[1]) * 2);
	}break;
	default:
		_tprintf(_T("未定义该行为\n"));
		system("pause");
		exit(1);
		break;
	}

	return 0;
}

int MainReadWork(TCHAR *in_filepath, STRUCTWORDTIMES *in_htop) {
	/*函数内部变量*/
	FILE *FileRead = NULL;
	TCHAR msg_t[MSGSIZE] = { NULL };
	int iFuncStat = 0;
	char OneWord = 0;
	char msg[MSGSIZE] = { NULL };
	int WordIndex = 0;

	/*...*/
	iFuncStat = _tfopen_s(&FileRead, in_filepath, _T("rt"));
	if (0 != iFuncStat) {
		_tprintf(_T("文件打开失败, 错误代码;%d\n"), errno);
		system("pause");
		exit(1);
	}
	while (1) {
		fread(&OneWord, sizeof(char), 1, FileRead);
		if (ferror(FileRead) != 0) {		//返回0表示未发生错误
			_tprintf(_T("文件读取错误, 错误代码;%d\n"), errno);
			system("pause");
			exit(1);
		}
		if (feof(FileRead) != 0) {				//返回0表示未读到文件尾
			if (WordIndex > 0) {
				msg[WordIndex] = '\0';
				DealWithWord(msg, in_htop);		//进行单词比较
				ZeroMemory(msg, _countof(msg));
				WordIndex = 0;
			}
			break;								//读取到文件尾，退出
		}
		if ((65 <= OneWord && 90 >= OneWord) || (97 <= OneWord && 122 >= OneWord)) {	//找到字母
			msg[WordIndex] = OneWord;
			WordIndex += 1;
			if (WordIndex < MSGSIZE) {
				;
			}
			else {
				_tprintf(_T("单词过长\n"));
				system("pause");
				exit(1);
			}
		}
		else if ((65 > OneWord) || (90 < OneWord && 97 > OneWord) || (122 > OneWord)) {		//发现非字母
			if (WordIndex > 0) {
				msg[WordIndex] = '\0';
				DealWithWord(msg, in_htop);				//进行单词比较
				ZeroMemory(msg, _countof(msg));
				WordIndex = 0;
			}
		}
		else {
			_tprintf(_T("未定义行为\n"));
			system("pause");
			exit(1);
		}
	}
	fclose(FileRead);

	return 0;
}
int MainShowData(STRUCTWORDTIMES *in_htop) {
	/*函数内部变量*/
	STRUCTWORDTIMES *curr = NULL;

	/*...*/
	curr = in_htop->next;
	while (curr != NULL) {
		//_tprintf(_T("word:%s, times:%llu\n"), curr->word, curr->times);
		printf("word:%s, times:%llu\n", curr->word, curr->times);
		curr = curr->next;
	}

	return 0;
}
int MainCleanUp(STRUCTWORDTIMES *in_htop) {
	/*函数内部变量*/
	STRUCTWORDTIMES *curr = NULL;
	STRUCTWORDTIMES *next = NULL;

	/*...*/
	curr = in_htop->next;
	next = curr;
	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}


	return 0;
}

//--------------------------------------------------
//
int DealWithWord(char *in_word, STRUCTWORDTIMES *in_htop) {
	/*函数内部变量*/
	STRUCTWORDTIMES *curr = NULL;

	/*...*/
	if (NULL == in_htop->next) {		//空头指针,直接赋值
		STRUCTWORDTIMES *node = (STRUCTWORDTIMES *)malloc(sizeof(STRUCTWORDTIMES));
		ZeroMemory(node, sizeof(STRUCTWORDTIMES));
		CopyMemory(node->word, in_word, strlen(in_word));
		node->times += 1;
		node->next = NULL;
		in_htop->next = node;
		return 0;
	}
	curr = in_htop; 
	while (curr->next != NULL) {
		curr = curr->next;
		if (0 == _stricmp(curr->word, in_word)) {
			curr->times += 1;
			return 0;
		}
	}

	STRUCTWORDTIMES *node = (STRUCTWORDTIMES *)malloc(sizeof(STRUCTWORDTIMES));
	ZeroMemory(node, sizeof(STRUCTWORDTIMES));
	CopyMemory(node->word, in_word, strlen(in_word));
	node->times += 1;
	node->next = NULL;
	curr->next = node;

	return 0;
}