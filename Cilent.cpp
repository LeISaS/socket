#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>
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
#define BUF_SIZE 1024
void LottoShuffle();
using namespace std;
int Number_Lotto[45] = {};

void ErrorHandling(char *message);
WSADATA wsaData;
SOCKET hSocket;

int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	char message[BUF_SIZE];
	int strLen;
	SOCKADDR_IN servAdr;
	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	else
		puts("서버에 연결되었습니다.\n");
	int M_input;
	const int Enter = 13;
	bool EnterCheck = true;
	int LottoNum,Lotto_temp;
	printf("로또 시뮬레이션\n");
	printf("---------Menu----------\n");
	printf("1.자동배치\n");
	printf("2.수동배치\n");
	printf("-----------------------\n");

	printf("입력:");
	scanf("%d", &M_input);
	send(hSocket, (char*)&M_input, sizeof(M_input), 0);

	
 	while(1)
	{
		if (M_input == 1)		//자동배치
		{
			system("cls");
			printf("자동으로 로또 숫자를 배치합니다.\n");

			for (int i = 0; i < 5; i++)
			{
				LottoShuffle();
			}

			printf("로또 번호 생성 완료\n");
			printf("엔터키를 눌러주세요 !\n");
			//enter
			if (EnterCheck == true)
			{
				for (int i = 0; i < 7; i++)
				{
					int boolOne = 1;
					send(hSocket, (char*)&boolOne, sizeof(boolOne), 0);
					getchar();
					send(hSocket, (char*)&Enter, sizeof(Enter), 0);
					
					LottoNum = recv(hSocket, (char *)&Lotto_temp, sizeof(Lotto_temp), 0);
					printf("%d번째 추첨 번호 : %d", i + 1, Lotto_temp);
				}
				EnterCheck = false;
			}
			break;
		}
		
		else if (M_input == 2)	//수동배치
		{
			system("cls");
			printf("로또 숫자를 배치하세요 !!\n");
		}
		else	//그외의 숫자들
		{
			printf("숫자를 잘못 입력하셨습니다.\n");
			printf("입력: \n");
			scanf("%d", &M_input);
			send(hSocket, (char*)&M_input, sizeof(M_input), 0);
		}
	}

	closesocket(hSocket);
	
	WSACleanup();
	return 0;
}

void ErrorHandling(char * message)
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
	for (int i = 0; i < 100; i++)
	{
		num1 = rand() % 45;
		num2 = rand() % 45;

		temp = Number_Lotto[num1];
		Number_Lotto[num1] = Number_Lotto[num2];
		Number_Lotto[num2] = temp;
	}

	//정렬
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
	for (int i = 0; i < 6; i++)
	{
		printf("%d\t", Number_Lotto[i]);
		Sleep(20);
		//send(hSocket, (char*)&Number_Lotto[i], sizeof(Number_Lotto[i]), 0);
	}
	printf("\n");
}