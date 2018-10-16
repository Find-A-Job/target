// FindSameWord35.cpp : �������̨Ӧ�ó������ڵ㡣
//


//
//�����ַ�ʱ���ǵð������������ڴ�����copymemory()
//
//
#include "stdafx.h"
#include "Macor.h"

//--------------------------------------------------
//ȫ�ֱ���


//--------------------------------------------------
//������
int MainDealWithArgv(int, TCHAR **, TCHAR *, TCHAR *);
int MainReadWork(TCHAR *, STRUCTWORDTIMES *);
int MainCompareSring(STRUCTWORDTIMES *, STRUCTWORDTIMES *, STRUCTWORDTIMES *);
int MainShowData(STRUCTWORDTIMES *);
int MainCleanUp(STRUCTWORDTIMES *);

//--------------------------------------------------
//
int DealWithWord(char *, STRUCTWORDTIMES *);
int SortWord(STRUCTWORDTIMES *, char *);

int _tmain(int argc, TCHAR **argv)
{
	//--------------------------------------------------
	//�����ڲ�����
	TCHAR FilePath[FILEPATHSIZE] = { NULL };
	STRUCTWORDTIMES *hTop = (STRUCTWORDTIMES *)malloc(sizeof(STRUCTWORDTIMES));
	TCHAR FilePath2[FILEPATHSIZE] = { NULL };
	STRUCTWORDTIMES *hTop2 = (STRUCTWORDTIMES *)malloc(sizeof(STRUCTWORDTIMES));
	STRUCTWORDTIMES *hTop3 = (STRUCTWORDTIMES *)malloc(sizeof(STRUCTWORDTIMES));

	ZeroMemory(hTop, sizeof(STRUCTWORDTIMES));
	hTop->next = NULL;
	ZeroMemory(hTop2, sizeof(STRUCTWORDTIMES));
	hTop2->next = NULL;
	ZeroMemory(hTop3, sizeof(STRUCTWORDTIMES));
	hTop3->next = NULL;

	//--------------------------------------------------
	//
	MainDealWithArgv(argc, argv, FilePath, FilePath2);

	//--------------------------------------------------
	//
	MainReadWork(FilePath, hTop);
	MainReadWork(FilePath2, hTop2);

	//--------------------------------------------------
	//
	MainCompareSring(hTop, hTop2, hTop3);

	//--------------------------------------------------
	//
	MainShowData(hTop3);

	//--------------------------------------------------
	//
	MainCleanUp(hTop);
	free(hTop);
	MainCleanUp(hTop2);
	free(hTop2);
	MainCleanUp(hTop3);
	free(hTop3);

	return 0;
}

//--------------------------------------------------
//������
int MainDealWithArgv(int argc, TCHAR **argv, TCHAR *out_filepath, TCHAR *out_filepath2) {
	/*�����ڲ�����*/

	/*...*/
	setlocale(LC_ALL, "chs");
	switch (argc)
	{
	case 1: {
		_tprintf(_T("��ѡ���������ļ���Ϊ����\n"));
		system("pause");
		exit(1);
	}break;
	case 2: {
		_tprintf(_T("��ѡ���������ļ���Ϊ����\n"));
		system("pause");
		exit(1);
	}break;
	case 3: {
		_tprintf(_T("%s\n"), argv[1]);
		CopyMemory(out_filepath, argv[1], wcslen(argv[1]) * 2);
		_tprintf(_T("%s\n"), argv[2]);
		CopyMemory(out_filepath2, argv[2], wcslen(argv[2]) * 2);
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
int MainCompareSring(STRUCTWORDTIMES *in_htop1, STRUCTWORDTIMES *in_htop2, STRUCTWORDTIMES *out_htop3) {
	/*�����ڲ�����*/
	STRUCTWORDTIMES *ht1 = NULL;
	STRUCTWORDTIMES *ht2 = NULL;
	STRUCTWORDTIMES *last2 = NULL;

	/*...*/
	ht1 = in_htop1->next;
	ht2 = in_htop2->next;
	last2 = in_htop2;
	if (NULL == in_htop1 || NULL == in_htop2) {
		_tprintf(_T("��ָ��\n"));
		system("pause");
		exit(1);
	}
	while (ht1 != NULL) {
		while (ht2 != NULL) {
			if (0 == _stricmp(ht1->word, ht2->word)) { //�ҵ��ظ�������ڲ�ѭ��
				SortWord(out_htop3, ht1->word);		//����ĸ����

				last2->next = ht2->next;		//���ظ���ȥ��������������
				free(ht2);
				ht2 = NULL;

				break;
			}
			else {
				last2 = ht2;
				ht2 = ht2->next;
				continue;
			}
		}
		if (NULL == in_htop2->next) {		//���ڶ�������ɾ��ʱ����ѭ��
			break;
		}
		ht1 = ht1->next;
		last2 = in_htop2;
		ht2 = last2->next;
	}

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
int SortWord(STRUCTWORDTIMES *out_htp, char *in_st) {
	/*�����ڲ�����*/
	STRUCTWORDTIMES *ht = NULL;
	STRUCTWORDTIMES *node = NULL;
	STRUCTWORDTIMES *last = NULL;
	int iFuncStat = 0;

	/*...*/
	if (NULL == out_htp||NULL==in_st) {
		_tprintf(_T("����Ϊ��\n"));
		system("pause");
		exit(1);
	}
	node = (STRUCTWORDTIMES *)malloc(sizeof(STRUCTWORDTIMES));
	ZeroMemory(node->word, sizeof(STRUCTWORDTIMES));
	CopyMemory(node->word, in_st, strlen(in_st));
	node->times = 1;
	node->next = NULL;
	if (NULL == out_htp->next) {
		out_htp->next = node;
		return 0;
	}
	last = out_htp;
	ht = out_htp->next;
	while (ht != NULL) {
		iFuncStat = _stricmp(ht->word, in_st);
		if (0 <= iFuncStat) {
			last->next = node;
			node->next = ht;
			return 0;
		}
		else {
			last = ht;
			ht = ht->next;
			continue;
		}
	}
	last->next = node;		//����ĸ�����ŵ������

	return 0;
}
