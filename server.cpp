#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include<iostream>

#define BUF_SIZE 1024
using namespace std;
void ErrorHandling(char *message);
int recvn(SOCKET s, char *buf, int len, int flags);
int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	char message[BUF_SIZE],LottoM[BUF_SIZE];
	
	int strLen, i;
	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSize;
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAdr,sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	clntAdrSize = sizeof(clntAdr);

		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);

		if (hClntSock == -1)
			ErrorHandling("accept() error");
		else
			printf("Connected client \n");
	
		char M_input[BUF_SIZE];
		int len;
	//Logic
		
		while (1)
		{
			len = recvn(hClntSock, M_input, BUF_SIZE, 0);
			if (len == SOCKET_ERROR)
			{
				printf("수신()에러\n");
			}
			else
			{
				printf("수신성공");
				M_input[len] = '\0';
				printf("클라이언트에서 %d번 입력했습니다.", M_input);
			}
		}
		
		

	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;
	
	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}
	return (len - left);
}