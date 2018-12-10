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
//void LottoShuffle();
using namespace std;
void LottoShuffle(int Number_Lotto[45]);
void ErrorHandling(char *message);
WSADATA wsaData;
SOCKET hSocket;
void AutoLotto();
void LottoPrn();
int Lotto1[7], Lotto2[7], Lotto3[7], Lotto4[7], Lotto5[7];
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
		puts("������ ����Ǿ����ϴ�.\n");
	int M_input;
	const int Enter = 13;
	bool EnterCheck = true;
	int LottoNum,Lotto_temp;
	printf("�ζ� �ùķ��̼�\n");
	printf("---------Menu----------\n");
	printf("1.�ڵ���ġ\n");
	printf("2.������ġ\n");
	printf("-----------------------\n");

	printf("�Է�:");
	scanf("%d", &M_input);
	send(hSocket, (char*)&M_input, sizeof(M_input), 0);
	int Lotto_ins[7] = {};
 	while(1)
	{
		if (M_input == 1)		//�ڵ���ġ
		{
			system("cls");
			printf("�ڵ����� �ζ� ���ڸ� ��ġ�մϴ�.\n");

			AutoLotto();
			

			printf("�ζ� ��ȣ ���� �Ϸ�\n");
			printf("����Ű�� �����ּ��� !\n");
			//enter
			if (EnterCheck == true)
			{
				for (int i = 0; i < 7; i++)
				{
	
					LottoPrn();

					int boolOne = 1;
					
					send(hSocket, (char*)&boolOne, sizeof(boolOne), 0);
					getchar();
					send(hSocket, (char*)&Enter, sizeof(Enter), 0);
					
					LottoNum = recv(hSocket, (char *)&Lotto_temp, sizeof(Lotto_temp), 0);
					Lotto_ins[i] = Lotto_temp;
					if (i == 6)
					{
						system("cls");
						printf("\n���ʽ� ��÷ ��ȣ : %d\n", Lotto_temp);
						
						printf("-------Result------\n");
						printf("���� ��ȣ\n");
						for (int j = 0; j < 7; j++)
							printf("%d\t", Lotto_ins[j]);
						printf("\n");
					}
					else
					{
						system("cls");
						printf("\n%d��° ��÷ ��ȣ : %d\n", i + 1, Lotto_temp);
						
					}

					for (int j = 0; j < 6; j++)
					{
						if (Lotto_temp == Lotto1[j])
						{
							Lotto1[6]++;
						}
						if (Lotto_temp == Lotto2[j])
						{
							Lotto2[6]++;
						}
						if (Lotto_temp == Lotto3[j])
						{
							Lotto3[6]++;
						}
						if (Lotto_temp == Lotto4[j])
						{
							Lotto4[6]++;
						}
						if (Lotto_temp == Lotto5[j])
						{
							Lotto5[6]++;
						}
					}
				}
				LottoPrn();
				EnterCheck = false;
			}
			break;
		}
		
		else if (M_input == 2)	//������ġ
		{
			system("cls");
			printf("�ζ� ���ڸ� ��ġ�ϼ��� !!\n");
		}
		else	//�׿��� ���ڵ�
		{
			printf("���ڸ� �߸� �Է��ϼ̽��ϴ�.\n");
			printf("�Է�: \n");
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

void LottoShuffle(int Number_Lotto[45])
{
	int num1, num2, temp;
	//int Number_Lotto[45] = {};
	for (int i = 0; i < 45; i++)
	{
		Number_Lotto[i] = i + 1;
	}

	//���� 
	for (int i = 0; i < 100; i++)
	{
		num1 = rand() % 45;
		num2 = rand() % 45;

		temp = Number_Lotto[num1];
		Number_Lotto[num1] = Number_Lotto[num2];
		Number_Lotto[num2] = temp;
	}

	//����
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
		Number_Lotto[6] = 0;
		printf("%d\t", Number_Lotto[i]);
		Sleep(20);
		//send(hSocket, (char*)&Number_Lotto[i], sizeof(Number_Lotto[i]), 0);
	}
	printf("\n");
}
void LottoPrn()
{
	printf("\n");
	printf("A :\t");
	for (int i = 0; i < 6; i++)
		printf("%d\t", Lotto1[i]);
	printf("\n");
	printf("B :\t");
	for (int i = 0; i < 6; i++)
		printf("%d\t", Lotto2[i]);
	printf("\n");
	printf("C :\t");
	for (int i = 0; i < 6; i++)
		printf("%d\t", Lotto3[i]);
	printf("\n");
	printf("D :\t");
	for (int i = 0; i < 6; i++)
		printf("%d\t", Lotto4[i]);
	printf("\n");
	printf("E :\t");
	for (int i = 0; i < 6; i++)
		printf("%d\t", Lotto5[i]);
	printf("\n\n");
	printf("A. ��÷ Ƚ�� : %d\n", Lotto1[6]);
	printf("B. ��÷ Ƚ�� : %d\n", Lotto2[6]);
	printf("C. ��÷ Ƚ�� : %d\n", Lotto3[6]);
	printf("D. ��÷ Ƚ�� : %d\n", Lotto4[6]);
	printf("E. ��÷ Ƚ�� : %d\n", Lotto5[6]);
}
void AutoLotto()
{
	for (int i = 0; i<1; i++)
		LottoShuffle(&Lotto1[i]);
	for (int i = 0; i<1; i++)
		LottoShuffle(&Lotto2[i]);
	for (int i = 0; i<1; i++)
		LottoShuffle(&Lotto3[i]);
	for (int i = 0; i<1; i++)
		LottoShuffle(&Lotto4[i]);
	for (int i = 0; i<1; i++)
		LottoShuffle(&Lotto5[i]);
}