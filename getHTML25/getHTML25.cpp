// getHTML25.cpp : 定义控制台应用程序的入口点。
//

//给你一个网址，你写一个程序，把网址对应的网页保存到你的电脑里
//
//
#include "stdafx.h"
#include "mm.h"

//----------------------------------------------
//引用openssl文件
#include "openssl\ssl.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")

//----------------------------------------------
//全局变量

//----------------------------------------------
//主函数
int MainCommandLine(ARGCVA *);						//命令行参数处理
int MainUrlAnly(TCHAR *, URLSTR *);					//URL分析
int MainGlobalSet(FILESET *);						//全局参数设置
int MainConnect(TCHAR *, URLSTR *, FILESET *);		//连接，存储

//----------------------------------------------
//
int MakeReqHead(TCHAR *, URLSTR *, TCHAR *);
int RecvData(TCHAR *, TCHAR *, FILESET *, SSL *);

int _tmain(int argc, TCHAR **argv)
{
	//----------------------------------------------
	//函数内部变量
	ARGCVA *ar = (ARGCVA *)malloc(sizeof(ARGCVA));
	ZeroMemory(ar, sizeof(ARGCVA));
	memset(ar->URL, '\0', _countof(ar->URL));
	ar->argc = argc;
	ar->argv = argv;  //浅拷贝

	URLSTR *us = (URLSTR *)malloc(sizeof(URLSTR));
	ZeroMemory(us, sizeof(URLSTR));

	FILESET *fs = (FILESET *)malloc(sizeof(FILESET));
	ZeroMemory(us, sizeof(URLSTR));
	//----------------------------------------------
	//处理命令行参数
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
	/*函数内部变量*/

	/*...*/
	setlocale(LC_ALL, "chs");
	switch (ar->argc)
	{
	case 1: {			//未指定参数
		_tprintf(_T("请输入一个URL作为参数\n"));
		exit(0);
	}break;
	case 2: {			//接收一个参数URL
		CopyMemory(ar->URL, (ar->argv)[1], wcslen((ar->argv[1])) * 2);
		_tprintf(_T("%s\n"), ar->URL);
	}break;
	default:
		_tprintf(_T("参数异常：%d\n"), ar->argc);
		exit(1);
		break;
	}

	return 0;
}
/*
*采用关键点标识方式解析URL
*
*/
int MainUrlAnly(TCHAR *URL, URLSTR *us) {
	/*函数内部变量*/
	int DoubleSlash = CANNOTFIND;		//'://'冒号双斜杠-第一个冒号位置
	int Colon = CANNOTFIND;				//冒号-端口号
	int SlashNum = 0;					//单斜杠数量-不包括连续两个单斜杠 表示数量而不是下标
	int SlashArr[SLASHSIZE] = { NULL };	//各个斜杠所在位置--文件夹，文件
	int QuestionMark = CANNOTFIND;		//问号-参数
	int Sharp = 0;						//井号-锚点

	int begin = 0;
	int end = 0;

	/*...*/
	for (int i = 0; i < wcslen(URL); ++i) {
		switch (URL[i])
		{
		case ':': {
			if (DoubleSlash != CANNOTFIND) {						//若协议部分已经找到
				if (Colon != CANNOTFIND) {							//若已出现过冒号
					_tprintf(_T("符号':'太多\n"));
					exit(1);
				}
				else {												//未出现过冒号
					if (URL[i + 1] <= 57 && URL[i + 1] >= 48) {		//
						Colon = i;
					}
					else {
						_tprintf(_T("冒号后并不是数字\n"));
						exit(1);
					}
				}
			}
			else {													//未找到协议部分
				if ('/' == URL[i + 1] && '/' == URL[i + 2]) {		//若接下来两个字符都是'/'
					DoubleSlash = i;
					i += 2;
				}
				else if (URL[i + 1] <= 57 && URL[i + 1] >= 48) {	//
					Colon = i;
				}
				else {
					_tprintf(_T("冒号后并不是数字\n"));
					exit(1);
				}
			}
		}break;
		case '/': {
			SlashArr[SlashNum] = i;
			SlashNum += 1;
		}break;
		case '?': {													//参数部分
			if (QuestionMark != CANNOTFIND) {
				_tprintf(_T("符号'?'太多\n"));
				exit(1);
			}
			else {
				QuestionMark = i;
			}
		}break;
		case '#': {													//锚点部分
			if (Sharp != CANNOTFIND) {
				_tprintf(_T("符号'#'太多\n"));
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

	//提取协议部分
	if (DoubleSlash != CANNOTFIND) {								
		CopyMemory(us->protocal, URL, (DoubleSlash) * 2);
	}
	else {															//默认采用https
		CopyMemory(us->protocal, _T("https"), 5 * 2);
	}

	//提取域名部分
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
			_tprintf(_T("未发现根目录\n"));
			exit(1);
		}
	}
	CopyMemory(us->domain, &(URL[begin]), (end - begin) * 2);
	begin = 0;
	end = 0;

	//提取端口部分
	if (Colon != CANNOTFIND) {
		begin = Colon;
		if (SlashNum > 0) {
			end = SlashArr[0];
		}
		else {
			_tprintf(_T("未发现根目录\n"));
			exit(1);
		}
		CopyMemory(us->port, &(URL[begin]), (end - begin) * 2);
	}
	else {
		CopyMemory(us->port, _T("443"), 3 * 2);
	}

	//提取目录部分 以'/'结尾的字符串
	//暂不需要

	//提取文件部分
	//暂不需要

	//提取参数部分
	//暂不需要

	//提取锚点部分
	//暂不需要

	return 0;
}
int MainGlobalSet(FILESET *fs) {
	/*函数内部变量*/
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
	/*函数内部变量*/
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
	int time_out_recv = 2000;	//超时时间（毫秒）
	/*...*/
	//ssl初始化
	SSL_library_init();

	//选择版本协议
	meth = SSLv23_client_method();		

	//配置ssl环境
	ctx = SSL_CTX_new(meth);				
	if (NULL == ctx) {
		_tprintf(_T("err in SSL_CTX_new\n"));
		system("pause");
		exit(1);
	}

	//申请一个ssl
	ssl = SSL_new(ctx);						
	if (NULL == ctx) {
		_tprintf(_T("err in SSL_new\n"));
		SSL_CTX_free(ctx);					//释放ssl环境（中，所申请的内存）
		system("pause");
		exit(1);
	}

	//windows中使用socket必需的初始化工作
	sockVer = MAKEWORD(2, 2);				
	if (WSAStartup(sockVer, &wsadata) != 0) {
		_tprintf(_T("WSAGetLastError:%d in WSAStartup\n"), WSAGetLastError());
		SSL_shutdown(ssl);					//关闭ssl
		SSL_free(ssl);						//释放ssl
		SSL_CTX_free(ctx);					//释放ssl环境（中，所申请的内存）
		system("pause");
		exit(1);
	}

	//创建套接字
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		//return和exit之前要关闭cSock
	if (INVALID_SOCKET == s) {
		_tprintf(_T("WSAGetLastError:%d in socket\n"), WSAGetLastError());
		SSL_shutdown(ssl);					//关闭ssl
		SSL_free(ssl);						//释放ssl
		SSL_CTX_free(ctx);					//释放ssl环境（中，所申请的内存）
		WSACleanup();
		system("pause");
		exit(1);
	}

	//把ssl绑定到一个套接字上
	iFuncStat = SSL_set_fd(ssl, (int)s);			
	if (0 == iFuncStat) {
		_tprintf(_T("err in SSL_set_fd\n"));
		shutdown(s, SD_SEND);
		closesocket(s);
		SSL_shutdown(ssl);					//关闭ssl
		SSL_free(ssl);						//释放ssl
		SSL_CTX_free(ctx);					//释放ssl环境（中，所申请的内存）
		WSACleanup();
		system("pause");
		exit(1);
	}

	//域名转ip
	ai.ai_socktype = SOCK_STREAM;
	ai.ai_family = AF_INET;
	iFuncStat = GetAddrInfo(us->domain, us->protocal, &ai, &pai);
	if (iFuncStat != 0) {
		_tprintf(_T("err:%d in GetAddrInfo\n"), iFuncStat);
		_tprintf(_T("%s, %s\n"), us->domain, us->protocal);
		//
		shutdown(s, SD_SEND);
		closesocket(s);
		SSL_shutdown(ssl);					//关闭ssl
		SSL_free(ssl);						//释放ssl
		SSL_CTX_free(ctx);					//释放ssl环境（中，所申请的内存）
		WSACleanup();
		//
		system("pause");
		exit(1);
	}
	((sockaddr_in *)&caddre)->sin_addr.S_un.S_addr = ((sockaddr_in *)(pai->ai_addr))->sin_addr.S_un.S_addr;
	((sockaddr_in *)&caddre)->sin_port = ((sockaddr_in *)(pai->ai_addr))->sin_port;
	caddre.sa_family = (pai->ai_addr)->sa_family;

	//建立套接字连接
	iFuncStat = connect(s, &caddre, sizeof(caddre));
	if (SOCKET_ERROR == iFuncStat) {		
		_tprintf(_T("WSAGetLastError:%d in connect\n"), WSAGetLastError());
		//
		shutdown(s, SD_SEND);
		closesocket(s);
		SSL_shutdown(ssl);					//关闭ssl
		SSL_free(ssl);						//释放ssl
		SSL_CTX_free(ctx);					//释放ssl环境（中，所申请的内存）
		WSACleanup();
		//
		system("pause");
		exit(1);
	}

	//超时设置
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&time_out_recv, sizeof(int));		//设置接收超时返回

	//ssl连接
	iFuncStat = SSL_connect(ssl);					
	if (1 != iFuncStat) {
		SSL_get_error(ssl, iFuncStat);
		_tprintf(_T("SSL_get_error:%d in SSL_connect\n"), iFuncStat);
		//
		shutdown(s, SD_SEND);
		closesocket(s);
		SSL_shutdown(ssl);					//关闭ssl
		SSL_free(ssl);						//释放ssl
		SSL_CTX_free(ctx);					//释放ssl环境（中，所申请的内存）
		WSACleanup();
		//
		system("pause");
		exit(1);
	}

	//发送请求头，接收存储响应体
	RecvData(URL, us->domain, fs, ssl);

	//关闭
	shutdown(s, SD_SEND);
	closesocket(s);
	SSL_shutdown(ssl);					//关闭ssl
	SSL_free(ssl);						//释放ssl
	SSL_CTX_free(ctx);					//释放ssl环境（中，所申请的内存）
	WSACleanup();
	//

	return 0;
}

//----------------------------------------------
//
int MakeReqHead(TCHAR *URL, URLSTR *us, TCHAR *rh) {
	/*函数内部变量*/
	size_t retVal = 0;

	/*...*/
	_stprintf_s(rh, REQHEADSIZE, _T("GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:60.0) Gecko/20100101 Firefox/60.0\r\nAccept: */*\r\n\
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2\r\nAccept-Encoding:identity\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\n\r\n"), URL, us->domain);

	return 0;
}

int RecvData(TCHAR *url, TCHAR *domain, FILESET *fs, SSL *ssl) {
	/*函数内部变量*/
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
	//制作请求头
	wcstombs_s(&RetVal, send_url, _countof(send_url), url, _countof(send_url));
	wcstombs_s(&RetVal, send_domain, _countof(send_domain), domain, _countof(send_domain));
	sprintf_s(ReqHead, _countof(ReqHead), "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:60.0) Gecko/20100101 Firefox/60.0\r\nAccept: */*\r\n\
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2\r\nAccept-Encoding:identity\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\n\r\n", send_url, send_domain);


	//打开文件
	_stprintf_s(fp_t, _countof(fp_t), _T("%s%s"), fs->path, fs->file);
	wcstombs_s(&RetVal, fp, _countof(fp), fp_t, _countof(fp));
	iFuncStat = _tfopen_s(&f, fp_t, _T("wb"));
	if (iFuncStat != 0) {
		_tprintf(_T("err in fopen_s\n"));
		printf(fp);
		system("pause");
		exit(1);
	}


	//发送请求头
	iFuncStat = SSL_write(ssl, ReqHead, (int)strlen(ReqHead));
	if (iFuncStat != strlen(ReqHead)) {
		_tprintf(_T("err in SSL_write\n"));
		fclose(f);
		system("pause");
		exit(1);
	}

	//接收响应
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
		//设置超时
	}

	//
	fclose(f);

	return 0;
}