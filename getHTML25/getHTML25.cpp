// getHTML25.cpp : �������̨Ӧ�ó������ڵ㡣
//

//����һ����ַ����дһ�����򣬰���ַ��Ӧ����ҳ���浽��ĵ�����
//
//
#include "stdafx.h"
#include "mm.h"

//----------------------------------------------
//����openssl�ļ�
#include "openssl\ssl.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")

//----------------------------------------------
//ȫ�ֱ���

//----------------------------------------------
//������
int MainCommandLine(ARGCVA *);						//�����в�������
int MainUrlAnly(TCHAR *, URLSTR *);					//URL����
int MainGlobalSet(FILESET *);						//ȫ�ֲ�������
int MainConnect(TCHAR *, URLSTR *, FILESET *);		//���ӣ��洢

//----------------------------------------------
//
int MakeReqHead(TCHAR *, URLSTR *, TCHAR *);
int RecvData(TCHAR *, TCHAR *, FILESET *, SSL *);

int _tmain(int argc, TCHAR **argv)
{
	//----------------------------------------------
	//�����ڲ�����
	ARGCVA *ar = (ARGCVA *)malloc(sizeof(ARGCVA));
	ZeroMemory(ar, sizeof(ARGCVA));
	memset(ar->URL, '\0', _countof(ar->URL));
	ar->argc = argc;
	ar->argv = argv;  //ǳ����

	URLSTR *us = (URLSTR *)malloc(sizeof(URLSTR));
	ZeroMemory(us, sizeof(URLSTR));

	FILESET *fs = (FILESET *)malloc(sizeof(FILESET));
	ZeroMemory(us, sizeof(URLSTR));
	//----------------------------------------------
	//���������в���
	MainCommandLine(ar);

	//----------------------------------------------
	//
	MainUrlAnly(ar->URL, us);

	//----------------------------------------------
	//
	MainGlobalSet(fs);

	//----------------------------------------------
	//
	MainConnect(ar->URL, us, fs);

	//----------------------------------------------
	//

	//----------------------------------------------
	//

	system("pause");
    return 0;
}
//----------------------------------------------
//
int MainCommandLine(ARGCVA *ar) {
	/*�����ڲ�����*/

	/*...*/
	setlocale(LC_ALL, "chs");
	switch (ar->argc)
	{
	case 1: {			//δָ������
		_tprintf(_T("������һ��URL��Ϊ����\n"));
		exit(0);
	}break;
	case 2: {			//����һ������URL
		CopyMemory(ar->URL, (ar->argv)[1], wcslen((ar->argv[1])) * 2);
		_tprintf(_T("%s\n"), ar->URL);
	}break;
	default:
		_tprintf(_T("�����쳣��%d\n"), ar->argc);
		exit(1);
		break;
	}

	return 0;
}
/*
*���ùؼ����ʶ��ʽ����URL
*
*/
int MainUrlAnly(TCHAR *URL, URLSTR *us) {
	/*�����ڲ�����*/
	int DoubleSlash = CANNOTFIND;		//'://'ð��˫б��-��һ��ð��λ��
	int Colon = CANNOTFIND;				//ð��-�˿ں�
	int SlashNum = 0;					//��б������-����������������б�� ��ʾ�����������±�
	int SlashArr[SLASHSIZE] = { NULL };	//����б������λ��--�ļ��У��ļ�
	int QuestionMark = CANNOTFIND;		//�ʺ�-����
	int Sharp = 0;						//����-ê��

	int begin = 0;
	int end = 0;

	/*...*/
	for (int i = 0; i < wcslen(URL); ++i) {
		switch (URL[i])
		{
		case ':': {
			if (DoubleSlash != CANNOTFIND) {						//��Э�鲿���Ѿ��ҵ�
				if (Colon != CANNOTFIND) {							//���ѳ��ֹ�ð��
					_tprintf(_T("����':'̫��\n"));
					exit(1);
				}
				else {												//δ���ֹ�ð��
					if (URL[i + 1] <= 57 && URL[i + 1] >= 48) {		//
						Colon = i;
					}
					else {
						_tprintf(_T("ð�ź󲢲�������\n"));
						exit(1);
					}
				}
			}
			else {													//δ�ҵ�Э�鲿��
				if ('/' == URL[i + 1] && '/' == URL[i + 2]) {		//�������������ַ�����'/'
					DoubleSlash = i;
					i += 2;
				}
				else if (URL[i + 1] <= 57 && URL[i + 1] >= 48) {	//
					Colon = i;
				}
				else {
					_tprintf(_T("ð�ź󲢲�������\n"));
					exit(1);
				}
			}
		}break;
		case '/': {
			SlashArr[SlashNum] = i;
			SlashNum += 1;
		}break;
		case '?': {													//��������
			if (QuestionMark != CANNOTFIND) {
				_tprintf(_T("����'?'̫��\n"));
				exit(1);
			}
			else {
				QuestionMark = i;
			}
		}break;
		case '#': {													//ê�㲿��
			if (Sharp != CANNOTFIND) {
				_tprintf(_T("����'#'̫��\n"));
				exit(1);
			}
			else {
				Sharp = i;
			}
		}break;
		default:
			break;
		}
	}

	//��ȡЭ�鲿��
	if (DoubleSlash != CANNOTFIND) {								
		CopyMemory(us->protocal, URL, (DoubleSlash) * 2);
	}
	else {															//Ĭ�ϲ���https
		CopyMemory(us->protocal, _T("https"), 5 * 2);
	}

	//��ȡ��������
	if (DoubleSlash != CANNOTFIND) {
		begin = DoubleSlash += 3;
	}
	else {
		begin = 0;
	}
	if (Colon != CANNOTFIND) {
		end = Colon;
	}
	else {
		if (SlashNum > 0) {
			end = SlashArr[0];
		}
		else {
			_tprintf(_T("δ���ָ�Ŀ¼\n"));
			exit(1);
		}
	}
	CopyMemory(us->domain, &(URL[begin]), (end - begin) * 2);
	begin = 0;
	end = 0;

	//��ȡ�˿ڲ���
	if (Colon != CANNOTFIND) {
		begin = Colon;
		if (SlashNum > 0) {
			end = SlashArr[0];
		}
		else {
			_tprintf(_T("δ���ָ�Ŀ¼\n"));
			exit(1);
		}
		CopyMemory(us->port, &(URL[begin]), (end - begin) * 2);
	}
	else {
		CopyMemory(us->port, _T("443"), 3 * 2);
	}

	//��ȡĿ¼���� ��'/'��β���ַ���
	//�ݲ���Ҫ

	//��ȡ�ļ�����
	//�ݲ���Ҫ

	//��ȡ��������
	//�ݲ���Ҫ

	//��ȡê�㲿��
	//�ݲ���Ҫ

	return 0;
}
int MainGlobalSet(FILESET *fs) {
	/*�����ڲ�����*/
	time_t TimeNow = 0;
	TCHAR msg_t[MSGSIZE] = { NULL };

	/*...*/
	memset(fs->path, '\0', _countof(fs->path));
	memset(fs->file, '\0', _countof(fs->file));
	CreateDirectory(_T("DATA"), NULL);
	CopyMemory(fs->path, _T("DATA/"), 5 * 2);
	time(&TimeNow);
	ZeroMemory(msg_t, _countof(msg_t) * 2);
	_stprintf_s(msg_t, _countof(msg_t), _T("%lld.html"), TimeNow);
	//_stprintf_s(fs->file, _countof(fs->file), _T("%lld.html"), TimeNow);
	CopyMemory(fs->file, msg_t, wcslen(msg_t) * 2);

	//_tprintf(_T("%s, %s, %s, %llu\n"), fs->path, fs->file, msg_t, wcslen(msg_t));

	return 0;
}
int MainConnect(TCHAR *URL, URLSTR *us, FILESET *fs) {
	/*�����ڲ�����*/
	const SSL_METHOD *meth = NULL;
	SSL_CTX *ctx = NULL;
	SSL *ssl = NULL;

	WORD sockVer = 0;
	WSADATA wsadata = { NULL };
	SOCKET s = 0;
	ADDRINFOW ai;
	ZeroMemory(&ai, sizeof(ai));
	PADDRINFOW pai = NULL;
	sockaddr caddre;
	ZeroMemory(&caddre, sizeof(caddre));

	int iFuncStat = 0;
	int time_out_recv = 2000;	//��ʱʱ�䣨���룩
	/*...*/
	//ssl��ʼ��
	SSL_library_init();

	//ѡ��汾Э��
	meth = SSLv23_client_method();		

	//����ssl����
	ctx = SSL_CTX_new(meth);				
	if (NULL == ctx) {
		_tprintf(_T("err in SSL_CTX_new\n"));
		system("pause");
		exit(1);
	}

	//����һ��ssl
	ssl = SSL_new(ctx);						
	if (NULL == ctx) {
		_tprintf(_T("err in SSL_new\n"));
		SSL_CTX_free(ctx);					//�ͷ�ssl�������У���������ڴ棩
		system("pause");
		exit(1);
	}

	//windows��ʹ��socket����ĳ�ʼ������
	sockVer = MAKEWORD(2, 2);				
	if (WSAStartup(sockVer, &wsadata) != 0) {
		_tprintf(_T("WSAGetLastError:%d in WSAStartup\n"), WSAGetLastError());
		SSL_shutdown(ssl);					//�ر�ssl
		SSL_free(ssl);						//�ͷ�ssl
		SSL_CTX_free(ctx);					//�ͷ�ssl�������У���������ڴ棩
		system("pause");
		exit(1);
	}

	//�����׽���
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		//return��exit֮ǰҪ�ر�cSock
	if (INVALID_SOCKET == s) {
		_tprintf(_T("WSAGetLastError:%d in socket\n"), WSAGetLastError());
		SSL_shutdown(ssl);					//�ر�ssl
		SSL_free(ssl);						//�ͷ�ssl
		SSL_CTX_free(ctx);					//�ͷ�ssl�������У���������ڴ棩
		WSACleanup();
		system("pause");
		exit(1);
	}

	//��ssl�󶨵�һ���׽�����
	iFuncStat = SSL_set_fd(ssl, (int)s);			
	if (0 == iFuncStat) {
		_tprintf(_T("err in SSL_set_fd\n"));
		shutdown(s, SD_SEND);
		closesocket(s);
		SSL_shutdown(ssl);					//�ر�ssl
		SSL_free(ssl);						//�ͷ�ssl
		SSL_CTX_free(ctx);					//�ͷ�ssl�������У���������ڴ棩
		WSACleanup();
		system("pause");
		exit(1);
	}

	//����תip
	ai.ai_socktype = SOCK_STREAM;
	ai.ai_family = AF_INET;
	iFuncStat = GetAddrInfo(us->domain, us->protocal, &ai, &pai);
	if (iFuncStat != 0) {
		_tprintf(_T("err:%d in GetAddrInfo\n"), iFuncStat);
		_tprintf(_T("%s, %s\n"), us->domain, us->protocal);
		//
		shutdown(s, SD_SEND);
		closesocket(s);
		SSL_shutdown(ssl);					//�ر�ssl
		SSL_free(ssl);						//�ͷ�ssl
		SSL_CTX_free(ctx);					//�ͷ�ssl�������У���������ڴ棩
		WSACleanup();
		//
		system("pause");
		exit(1);
	}
	((sockaddr_in *)&caddre)->sin_addr.S_un.S_addr = ((sockaddr_in *)(pai->ai_addr))->sin_addr.S_un.S_addr;
	((sockaddr_in *)&caddre)->sin_port = ((sockaddr_in *)(pai->ai_addr))->sin_port;
	caddre.sa_family = (pai->ai_addr)->sa_family;

	//�����׽�������
	iFuncStat = connect(s, &caddre, sizeof(caddre));
	if (SOCKET_ERROR == iFuncStat) {		
		_tprintf(_T("WSAGetLastError:%d in connect\n"), WSAGetLastError());
		//
		shutdown(s, SD_SEND);
		closesocket(s);
		SSL_shutdown(ssl);					//�ر�ssl
		SSL_free(ssl);						//�ͷ�ssl
		SSL_CTX_free(ctx);					//�ͷ�ssl�������У���������ڴ棩
		WSACleanup();
		//
		system("pause");
		exit(1);
	}

	//��ʱ����
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&time_out_recv, sizeof(int));		//���ý��ճ�ʱ����

	//ssl����
	iFuncStat = SSL_connect(ssl);					
	if (1 != iFuncStat) {
		SSL_get_error(ssl, iFuncStat);
		_tprintf(_T("SSL_get_error:%d in SSL_connect\n"), iFuncStat);
		//
		shutdown(s, SD_SEND);
		closesocket(s);
		SSL_shutdown(ssl);					//�ر�ssl
		SSL_free(ssl);						//�ͷ�ssl
		SSL_CTX_free(ctx);					//�ͷ�ssl�������У���������ڴ棩
		WSACleanup();
		//
		system("pause");
		exit(1);
	}

	//��������ͷ�����մ洢��Ӧ��
	RecvData(URL, us->domain, fs, ssl);

	//�ر�
	shutdown(s, SD_SEND);
	closesocket(s);
	SSL_shutdown(ssl);					//�ر�ssl
	SSL_free(ssl);						//�ͷ�ssl
	SSL_CTX_free(ctx);					//�ͷ�ssl�������У���������ڴ棩
	WSACleanup();
	//

	return 0;
}

//----------------------------------------------
//
int MakeReqHead(TCHAR *URL, URLSTR *us, TCHAR *rh) {
	/*�����ڲ�����*/
	size_t retVal = 0;

	/*...*/
	_stprintf_s(rh, REQHEADSIZE, _T("GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:60.0) Gecko/20100101 Firefox/60.0\r\nAccept: */*\r\n\
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2\r\nAccept-Encoding:identity\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\n\r\n"), URL, us->domain);

	return 0;
}

int RecvData(TCHAR *url, TCHAR *domain, FILESET *fs, SSL *ssl) {
	/*�����ڲ�����*/
	FILE *f = NULL;
	size_t RetVal = 0;
	int iFuncStat = 0;
	char fp[FILESIZE] = { NULL };
	TCHAR fp_t[FILESIZE] = { NULL };

	char send_url[URLSIZE] = { NULL };
	ZeroMemory(send_url, _countof(send_url));
	char send_domain[DOMAINSIZE] = { NULL };
	ZeroMemory(send_domain, _countof(send_domain));
	char ReqHead[REQHEADSIZE] = { NULL };
	ZeroMemory(ReqHead, _countof(ReqHead));
	char AcceptBody[ACCEPTSIZE] = { NULL };
	ZeroMemory(AcceptBody, _countof(AcceptBody));
	int AcceptNum = 0;

	/*...*/
	//��������ͷ
	wcstombs_s(&RetVal, send_url, _countof(send_url), url, _countof(send_url));
	wcstombs_s(&RetVal, send_domain, _countof(send_domain), domain, _countof(send_domain));
	sprintf_s(ReqHead, _countof(ReqHead), "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:60.0) Gecko/20100101 Firefox/60.0\r\nAccept: */*\r\n\
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2\r\nAccept-Encoding:identity\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\n\r\n", send_url, send_domain);


	//���ļ�
	_stprintf_s(fp_t, _countof(fp_t), _T("%s%s"), fs->path, fs->file);
	wcstombs_s(&RetVal, fp, _countof(fp), fp_t, _countof(fp));
	iFuncStat = _tfopen_s(&f, fp_t, _T("wb"));
	if (iFuncStat != 0) {
		_tprintf(_T("err in fopen_s\n"));
		printf(fp);
		system("pause");
		exit(1);
	}


	//��������ͷ
	iFuncStat = SSL_write(ssl, ReqHead, (int)strlen(ReqHead));
	if (iFuncStat != strlen(ReqHead)) {
		_tprintf(_T("err in SSL_write\n"));
		fclose(f);
		system("pause");
		exit(1);
	}

	//������Ӧ
	iFuncStat = SSL_read(ssl, AcceptBody, _countof(AcceptBody));
	if (iFuncStat <= 0) {
		_tprintf(_T("err in SSL_read\n"));
		fclose(f);
		system("pause");
		exit(1);
	}
	while (iFuncStat > 0) {
		fwrite(AcceptBody, sizeof(char), iFuncStat, f);
		iFuncStat = SSL_read(ssl, AcceptBody, _countof(AcceptBody));
		//���ó�ʱ
	}

	//
	fclose(f);

	return 0;
}