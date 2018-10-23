开源软件库编译心得：

1.在使用开源软件库之前需要将源文件编译成.h和.lib文件
2.阅读readme文件，这点非常重要，该文档会对安装进行说明
3.有些安装是在install文档中，输入命令行指令
4.安装可能需要下载其他软件进行辅助安装，比如使用perl来安装openssl
5.编译时需要vs的cl.exe才能工作
6.执行安装指令时应该指定编译后文件的存放目录，或在某目录下执行指令，这样就会把编译后的文件放在该目录下

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
框架
	meth = SSLv23_client_method();		//选择版本协议
	ctx = SSL_CTX_new(meth);			//配置ssl环境
	wsastartup();						//windows下套接字环境设置
	fd = socket();						//创建一个套接字
	connect();							//连接
	ssl = SSL_new(ctx);					//申请一个ssl
	SSL_set_fd(ssl, fd);				//把ssl绑定到一个套接字上
	SSL_connect(ssl);					//ssl连接
	SSL_write(ssl, "Hello world", strlen("Hello World!"));		//通过ssl发送消息
	SSL_read()							//通过ssl读取消息
	SSL_shutdown(ssl);					//关闭ssl
	closesocket(client_socket);			//关闭套接字
	SSL_free (ssl);						//关闭ssl
	SSL_CTX_free (ctx);					//释放ssl环境（中，所申请的内存）
	WSACleanup();						//释放套接字环境

例子：
编译openssl
1.解压文件至新建的文件夹（c:\\openssl）中
2.找到readme.txt文件，记事本打开，里面写着安装说明步骤在另一个文件中
3.找到说明文件（INSTALL.W64），记事本打开，里面有说明需要安装perl和vs工具
4.安装完成之后，进入../vc/bin/amd64/目录，运行vcvars64.bat
5.转到c:\\openssl目录下，执行INSTALL.W64中的命令
6.编译过程需要几分钟，完成后出现几个新文件夹和文件，其中inc32文件夹（头文件）和outdll文件夹（lib文件）及tem32文件夹（编译中间文件）
7.解决方案->属性->vc++目录->包含目录->选择inc32
						  ->库目录->选择outdll
				->链接器->输入->附加依赖项->选择outdll中的所有.lib文件，分号隔开

URL只需要提取协议，域名，端口部分即可。在制作请求头时将整个URL放入首行（GET "URL" HTTP/1.1\r\n）