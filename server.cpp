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
		int Count1, Count2, Count3, Count4, Count5;
		int Count_temp1, Count_temp2, Count_temp3, Count_temp4, Count_temp5;
		int N_Complete;
		
	//Logic
		while (1)
		{
			len = recv(hClntSock, (char *)&temp, sizeof(temp), 0);
			if (len == SOCKET_ERROR)
			{
				printf("프로그램을 종료합니다.\n");
				break;
			}
			//자동배치
			if (temp == 1)
			{
				printf("클라이언트에서 %d번 입력했습니다.\n", temp);
				printf("--------------자동배치-----------\n");
				printf("자동배치를 시작합니다.\n");
				Sleep(3000);
				printf("자동배치를 완료했습니다.\n");
				printf("숫자추첨을 시작합니다.\n");
				//enter
				for (int i = 0; i < 7; i++)
				{
					recv(hClntSock, (char *)&booltemp, sizeof(booltemp), 0);
					if (booltemp == 1)
					{
						recv(hClntSock, (char *)&Enter_temp, sizeof(Enter_temp), 0);
						printf("%d...클라이언트로부터 엔터를 입력받았습니다.%c\n", i+1,Enter_temp);
						printf("추첨된 번호를 클라이언트로 보냈습니다.\n");
						LottoShuffle();
						
					}
					else
						continue;
				}
				printf("번호 추첨끝 클라이언트에서 등수확인을 합니다.\n");
				Sleep(1000);
			}
			//수동배치
			else if (temp == 2)
			{
				printf("클라이언트에서 %d번 입력했습니다.\n", temp);
				printf("------------수동배치-------------\n");
				recv(hClntSock, (char *)&N_Complete, sizeof(N_Complete), 0);
				if (N_Complete==1);
				{
					printf("숫자 배치 완료 ");
				}
				
				for (int i = 0; i < 7; i++)
				{
					recv(hClntSock, (char *)&booltemp, sizeof(booltemp), 0);
					if (booltemp == 1)
					{
						recv(hClntSock, (char *)&Enter_temp, sizeof(Enter_temp), 0);
						printf("%d...클라이언트로부터 엔터를 입력받았습니다.%c\n", i + 1, Enter_temp);
						printf("추첨된 번호를 클라이언트로 보냈습니다.\n");
						LottoShuffle();
					}
					else
						continue;
				}
				printf("번호 추첨끝 클라이언트에서 등수확인을 합니다\n");
				Sleep(1000);
			}
			else
			{
				printf("메뉴에 없는 숫자를 입력했습니다..\n");
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

	//셔플 
	for (int i = 0; i < 500; i++)
	{
		num1 = rand() % 45;
		num2 = rand() % 45;

		temp = Number_Lotto[num1];
		Number_Lotto[num1] = Number_Lotto[num2];
		Number_Lotto[num2] = temp;
	}
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
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
