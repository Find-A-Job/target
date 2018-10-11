// WordAppearTimes15.cpp : �������̨Ӧ�ó������ڵ㡣
//

//��֪һ���ļ��������кܶ൥�ʣ���һ�����򣬼���һ���ĵ���ÿһ�����ʳ��ֵĴ�����
//
//��������������洢���г��ֹ��ĵ���
//		���������������ӵ�һ��Ӣ����ĸ����һ����Ӣ����ĸ���ڼ��������ĸ���һ������
//

#include "stdafx.h"
#include "Macor.h"

//--------------------------------------------------
//ȫ�ֱ���


//--------------------------------------------------
//������
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
	//�����ڲ�����
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
//������
int MainDealWithArgv(int argc, TCHAR **argv, TCHAR *out_filepath) {
	/*�����ڲ�����*/

	/*...*/
	setlocale(LC_ALL, "chs");
	switch (argc)
	{
	case 1: {
		_tprintf(_T("��ѡ��һ���ļ���Ϊ����\n"));
		system("pause");
		exit(1);
	}break;
	case 2: {
		_tprintf(_T("%s\n"), argv[1]);
		CopyMemory(out_filepath, argv[1], wcslen(argv[1]) * 2);
	}break;
	default:
		_tprintf(_T("δ�������Ϊ\n"));
		system("pause");
		exit(1);
		break;
	}

	return 0;
}

int MainReadWork(TCHAR *in_filepath, STRUCTWORDTIMES *in_htop) {
	/*�����ڲ�����*/
	FILE *FileRead = NULL;
	TCHAR msg_t[MSGSIZE] = { NULL };
	int iFuncStat = 0;
	char OneWord = 0;
	char msg[MSGSIZE] = { NULL };
	int WordIndex = 0;

	/*...*/
	iFuncStat = _tfopen_s(&FileRead, in_filepath, _T("rt"));
	if (0 != iFuncStat) {
		_tprintf(_T("�ļ���ʧ��, �������;%d\n"), errno);
		system("pause");
		exit(1);
	}
	while (1) {
		fread(&OneWord, sizeof(char), 1, FileRead);
		if (ferror(FileRead) != 0) {		//����0��ʾδ��������
			_tprintf(_T("�ļ���ȡ����, �������;%d\n"), errno);
			system("pause");
			exit(1);
		}
		if (feof(FileRead) != 0) {				//����0��ʾδ�����ļ�β
			if (WordIndex > 0) {
				msg[WordIndex] = '\0';
				DealWithWord(msg, in_htop);		//���е��ʱȽ�
				ZeroMemory(msg, _countof(msg));
				WordIndex = 0;
			}
			break;								//��ȡ���ļ�β���˳�
		}
		if ((65 <= OneWord && 90 >= OneWord) || (97 <= OneWord && 122 >= OneWord)) {	//�ҵ���ĸ
			msg[WordIndex] = OneWord;
			WordIndex += 1;
			if (WordIndex < MSGSIZE) {
				;
			}
			else {
				_tprintf(_T("���ʹ���\n"));
				system("pause");
				exit(1);
			}
		}
		else if ((65 > OneWord) || (90 < OneWord && 97 > OneWord) || (122 > OneWord)) {		//���ַ���ĸ
			if (WordIndex > 0) {
				msg[WordIndex] = '\0';
				DealWithWord(msg, in_htop);				//���е��ʱȽ�
				ZeroMemory(msg, _countof(msg));
				WordIndex = 0;
			}
		}
		else {
			_tprintf(_T("δ������Ϊ\n"));
			system("pause");
			exit(1);
		}
	}
	fclose(FileRead);

	return 0;
}
int MainShowData(STRUCTWORDTIMES *in_htop) {
	/*�����ڲ�����*/
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
	/*�����ڲ�����*/
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
	/*�����ڲ�����*/
	STRUCTWORDTIMES *curr = NULL;

	/*...*/
	if (NULL == in_htop->next) {		//��ͷָ��,ֱ�Ӹ�ֵ
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