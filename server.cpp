#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include<iostream>
#include<time.h>
/* Linux
#include<sys/socket.h>
#include<arpa.inet.h>
#include<sys/stat.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
*/
void LottoShuffle();
#define BUF_SIZE 1024
using namespace std;
void ErrorHandling(char *message);

SOCKET hServSock, hClntSock;
int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	WSADATA wsaData;
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
		int len,Enter;
		int temp,Enter_temp;
		int boolOne, booltemp;
		int boolTwo=1;
	//Logic
		while (1)
		{
			len = recv(hClntSock, (char *)&temp, sizeof(temp), 0);
			if (len == SOCKET_ERROR)
			{
				printf("����()����\n");
				break;
			}
			//�ڵ���ġ
			if (temp == 1)
			{
				printf("Ŭ���̾�Ʈ���� %d�� �Է��߽��ϴ�.\n", temp);
				printf("�ڵ���ġ�� �����մϴ�.\n");
				Sleep(1000);
				printf("�ڵ���ġ�� �Ϸ��߽��ϴ�.\n");
				printf("������÷�� �����մϴ�.\n");
				//enter
				for (int i = 0; i < 7; i++)
				{
					boolOne = recv(hClntSock, (char *)&booltemp, sizeof(booltemp), 0);
					if (booltemp == 1)
					{
						Enter = recv(hClntSock, (char *)&Enter_temp, sizeof(Enter_temp), 0);
						printf("%d...Ŭ���̾�Ʈ�κ��� ���͸� �Է¹޾ҽ��ϴ�.%c\n", i+1,Enter);
						printf("��÷�� ��ȣ�� Ŭ���̾�Ʈ�� ���½��ϴ�.\n");
						LottoShuffle();
						
					}
					else
						continue;
				}
				printf("��ȣ ��÷�� ���Ȯ���� �մϴ�.\n");
				send(hClntSock, (char*)&boolTwo, sizeof(boolTwo), 0);
			}
			//������ġ
			else if (temp == 2)
			{
				printf("Ŭ���̾�Ʈ���� %d�� �Է��߽��ϴ�.\n", temp);
			}
			
			else
			{
				printf("�޴��� ���� ���ڸ� �Է��߽��ϴ�..\n");
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

void LottoShuffle()
{
	int num1, num2, temp;
	int Number_Lotto[45] = {};
	for (int i = 0; i < 45; i++)
	{
		Number_Lotto[i] = i + 1;
	}

	//���� 
	for (int i = 0; i < 500; i++)
	{
		num1 = rand() % 45;
		num2 = rand() % 45;

		temp = Number_Lotto[num1];
		Number_Lotto[num1] = Number_Lotto[num2];
		Number_Lotto[num2] = temp;
	}

	//����
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (Number_Lotto[i] < Number_Lotto[j])
			{
				temp = Number_Lotto[i];
				Number_Lotto[i] = Number_Lotto[j];
				Number_Lotto[j] = temp;
			}
		}
	}
	for (int i = 0; i < 7; i++)
	{
		Sleep(20);
		send(hClntSock, (char*)&Number_Lotto[i], sizeof(Number_Lotto[i]), 0);
	}
	printf("\n");
}