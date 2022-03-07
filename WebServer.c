#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>

#define PORT 80
#define DATASIZE 1024

#pragma comment(lib,"ws2_32.lib")

void sendHtml(SOCKET client, char* filenName);

/**
*	温馨提示：建议访问本机 IPv4:127.0.0.1
*/

int main() {

	printf("图灵服务器开启成功\n");
	Sleep(2000);
	printf("请在浏览器访问IPv4地址：127.0.0.1\n");
	/**
	*	确定使用的 Socket 版本信息
	*	第一个参数：期望使用的版本
	*	第二个参数：获取版本信息
	*/
	WSADATA wsadata;
	int ISOK = WSAStartup(MAKEWORD(2, 2), &wsadata);
	
	if (ISOK == WSAEINVAL) {
		perror("SOCKET 请求失败……\n");
	}
	/**
	*	创建一个 Socket
	*	第一个参数：协议族。指定使用什么协议 AF_INET 使用 IPv4
	*	第二个参数：传输类型 Sock_STREAM 流传输
	*	第三个参数：遵从 TCP 协议
	*/
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (server == INVALID_SOCKET) {
		perror("SOCKET 创建失败……\n");
	}
	/* 绑定端口号和 IP 地址 */
	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(PORT);	// 注意：网络是大端存储，电脑是小端存储
	seraddr.sin_addr.s_addr = INADDR_ANY;
	ISOK = bind(server, (struct sockaddr*) & seraddr, sizeof(seraddr));
	if (ISOK == SOCKET_ERROR) {
		perror("BIND 绑定失败……\n");
	}
	/* 监听 listen 第二个参数：同时监听多少 */
	while (1)
	{
		printf("等待链接中……\n");
		listen(server, 5);
		SOCKET client = accept(server, NULL, NULL);
		if (client == INVALID_SOCKET) {
			perror("ACCEPT 监听失败……\n");
		}
		/* 接收信息 */
		char buffer[DATASIZE];
		recv(client, buffer, sizeof(buffer), 0);
		//printf("%s 共接收到%d条数据\n", buffer, strlen(buffer));
		//char sneddata[DATASIZE] = "<h1 style = \"color:red;\">Hello Turing</h1>";
		//send(client, sneddata, strlen(sneddata), 0);

		/* 网页文件传输 */
		sendHtml(client, "./index.html");

		/* 通信完毕，断开链接 */
		closesocket(client);
	}
	/* 关闭服务器 */
	closesocket(server);
	/* 关闭 Socket 套接字请求 */
	WSACleanup();
	
	int num = getchar();

	return 0;
}

void sendHtml(SOCKET client, char* filenName) {
	FILE* pfile = fopen(filenName, "r");
	if (pfile == NULL) {
		perror("HTML 文件打开失败……\n");
		return;
	}

	do {
		char senddata[DATASIZE];
		fgets(senddata, DATASIZE, pfile);
		send(client, senddata, strlen(senddata), 0);
	} while (!feof(pfile));

	/* 关闭 */
	fclose(pfile);
}