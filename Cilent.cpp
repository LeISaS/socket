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
void LottoGrade(int LottoNuber, int Bonus);
void manual();
int Lotto1[8] = {0,}, Lotto2[8] = {0,}, Lotto3[8] = {0,}, Lotto4[8] = {0,}, Lotto5[8] = {0,};
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
	int LottoCount1 = 0, LottoCount2 = 0, LottoCount3 = 0, LottoCount4 = 0, LottoCount5 = 0;
	int arrayCount = 0;
	printf("로또 시뮬레이션\n");
	printf("---------Menu----------\n");
	printf("1.자동배치\n");
	printf("2.수동배치\n");
	printf("-----------------------\n");

	printf("입력:");
	scanf("%d", &M_input);
	send(hSocket, (char*)&M_input, sizeof(M_input), 0);
	int Lotto_ins[7] = {};
 	while(1)
	{
		//자동 
		if (M_input == 1)		
		{
			system("cls");
			printf("자동으로 로또 숫자를 배치합니다.\n");

			AutoLotto();

			//enter
			if (EnterCheck)
			{
				for (int i = 0; i < 7; i++)
				{					
					LottoPrn();

					int boolOne = 1;
					
					send(hSocket, (char*)&boolOne, sizeof(boolOne), 0);
					getchar();
					send(hSocket, (char*)&Enter, sizeof(Enter), 0);
					
					/*최종 번호*/
					recv(hSocket, (char *)&Lotto_temp, sizeof(Lotto_temp), 0);
					Lotto_ins[i] = Lotto_temp;
					/*최종 번호*/
					if (i == 6)
					{
						arrayCount = Lotto_temp;
						system("cls");
						printf("\n보너스 추첨 번호 : %d\n", Lotto_temp);
						
						printf("-------Result------\n");
						printf("최종 번호\n");
						for (int j = 0; j < 7; j++)
							printf("%d\t", Lotto_ins[j]);
						printf("\n");
					}
					else
					{
						system("cls");
						printf("\n%d번째 추첨 번호 : %d\n", i + 1, Lotto_temp);
						
					}

					/*start count*/

					for (int j = 0; j < 7; j++)
					{
						if (Lotto_temp == Lotto1[j])
						{
							Lotto1[6]++;
							LottoCount1++;
							if (arrayCount == Lotto1[j])
							{
								Lotto1[6]--;
								Lotto1[7]++;
							}
						}
						if (Lotto_temp == Lotto2[j])
						{
							Lotto2[6]++;
							LottoCount2++;
							if (arrayCount == Lotto2[j])
							{
								Lotto2[6]--;
								Lotto2[7]++;
							}
						}
						if (Lotto_temp == Lotto3[j])
						{
							Lotto3[6]++;
							LottoCount3++;
							if (arrayCount == Lotto3[j])
							{
								Lotto3[6]--;
								Lotto3[7]++;
							}
						}
						if (Lotto_temp == Lotto4[j])
						{
							Lotto4[6]++;
							LottoCount4++;
							if (arrayCount == Lotto4[j])
							{
								Lotto4[6]--;
								Lotto4[7]++;
							}
						}
						if (Lotto_temp == Lotto5[j])
						{
							Lotto5[6]++;
							LottoCount5++;
							if (arrayCount == Lotto5[j])
							{
								Lotto5[6]--;
								Lotto5[7]++;
							}
						}
					}
					/*end count*/
				}
				LottoPrn();
				Sleep(1000);
				printf("===========================\n");
				printf("==========최종등수=========\n");
				printf("===========================\n");
				printf("A :");
				LottoGrade(LottoCount1, Lotto1[7]);
				printf("B :");
				LottoGrade(LottoCount2, Lotto2[7]);
				printf("C :");
				LottoGrade(LottoCount3, Lotto3[7]);
				printf("D :");
				LottoGrade(LottoCount4, Lotto4[7]);
				printf("E :");
				LottoGrade(LottoCount5, Lotto5[7]);

				EnterCheck = false;
			}
			break;
		}
		//수동
		else if (M_input == 2)
		{
			system("cls");
			printf("로또 숫자를 배치하세요 !!\n");
			
			//수동입력
			manual();

			printf("--------로또 번호 입력완료 \n------");
			Sleep(2000);
			if (EnterCheck)
			{
				for (int i = 0; i < 7; i++)
				{
					LottoPrn();

					int boolOne = 1;
					send(hSocket, (char *)&boolOne, sizeof(boolOne), 0);
					getchar();
					send(hSocket, (char *)&Enter, sizeof(Enter), 0);
					/*최종 번호*/
					recv(hSocket, (char *)&Lotto_temp, sizeof(Lotto_temp), 0);
					Lotto_ins[i] = Lotto_temp;
					/*최종 번호*/
					if (i == 6)
					{
						arrayCount = Lotto_temp;
						system("cls");
						printf("\n보너스 추첨 번호 : %d\n", Lotto_temp);

						printf("-------Result------\n");
						printf("최종 번호\n");
						for (int j = 0; j < 7; j++)
							printf("\t%d", Lotto_ins[j]);
						printf("\n");
					}
					else
					{
						system("cls");
						printf("\n%d번째 추첨 번호 : %d\n", i + 1, Lotto_temp);

					}
					/*start count*/

					for (int j = 0; j < 7; j++)
					{
						if (Lotto_temp == Lotto1[j])
						{
							Lotto1[6]++;
							LottoCount1++;
							if (arrayCount == Lotto1[j])
							{
								Lotto1[6]--;
								Lotto1[7]++;
							}
						}
						if (Lotto_temp == Lotto2[j])
						{
							Lotto2[6]++;
							LottoCount2++;
							if (arrayCount == Lotto2[j])
							{
								Lotto2[6]--;
								Lotto2[7]++;
							}
						}
						if (Lotto_temp == Lotto3[j])
						{
							Lotto3[6]++;
							LottoCount3++;
							if (arrayCount == Lotto3[j])
							{
								Lotto3[6]--;
								Lotto3[7]++;
							}
						}
						if (Lotto_temp == Lotto4[j])
						{
							Lotto4[6]++;
							LottoCount4++;
							if (arrayCount == Lotto4[j])
							{
								Lotto4[6]--;
								Lotto4[7]++;
							}
						}
						if (Lotto_temp == Lotto5[j])
						{
							Lotto5[6]++;
							LottoCount5++;
							if (arrayCount == Lotto5[j])
							{
								Lotto5[6]--;
								Lotto5[7]++;
							}
						}
					}
					/*end count*/
				}
				LottoPrn();
				Sleep(1000);
				printf("===========================\n");
				printf("==========최종등수=========\n");
				printf("===========================\n");
				printf("A :");
				LottoGrade(LottoCount1, Lotto1[7]);
				printf("B :");
				LottoGrade(LottoCount2, Lotto2[7]);
				printf("C :");
				LottoGrade(LottoCount3, Lotto3[7]);
				printf("D :");
				LottoGrade(LottoCount4, Lotto4[7]);
				printf("E :");
				LottoGrade(LottoCount5, Lotto5[7]);

				EnterCheck = false;
			}
			break;
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
void LottoGrade(int LottoNumber, int Bonus)
{
	if (LottoNumber == 6)
		printf("1등\n");
	else if (LottoNumber == 5 && Bonus == 1)
		printf("2등\n");
	else if (LottoNumber == 5)
		printf("3등\n");
	else if (LottoNumber == 4)
		printf("4등\n");
	else if (LottoNumber == 3)
		printf("5등\n");
	else
		printf("꽝\n");
		
}
void LottoShuffle(int Number_Lotto[45])
{
	int num1, num2, temp;
	//int Number_Lotto[45] = {};
	for (int i = 0; i < 45; i++)
	{
		Number_Lotto[i] = i + 1;
	}

	//셔플 
	for (int i = 0; i < 250; i++)
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
	for (int i = 0; i < 8; i++)
	{
		Number_Lotto[7] = 0;
		Number_Lotto[6] = 0;
		printf("%d\t", Number_Lotto[i]);
		Sleep(50);
		//send(hSocket, (char*)&Number_Lotto[i], sizeof(Number_Lotto[i]), 0);
	}
	printf("\n");
}
void LottoPrn()
{
	printf("\n");
	printf("A :\t");
	//Lotto Prn
	for (int i = 0; i < 8; i++)
		printf("%d\t", Lotto1[i]);
	printf("\n");
	printf("B :\t");
	for (int i = 0; i < 8; i++)
		printf("%d\t", Lotto2[i]);
	printf("\n");
	printf("C :\t");
	for (int i = 0; i < 8; i++)
		printf("%d\t", Lotto3[i]);
	printf("\n");
	printf("D :\t");
	for (int i = 0; i < 8; i++)
		printf("%d\t", Lotto4[i]);
	printf("\n");
	printf("E :\t");
	for (int i = 0; i < 8 ; i++)
		printf("%d\t", Lotto5[i]);
	printf("\n\n");

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

//수동
void LottoRange(int Number_Lotto[45])
{
	int temp;
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
}
void manual()
{
	printf("한 라인당 숫자 6개(0~45의 숫자)를 입력합니다.\n");
	printf("A : ");
	for (int i = 0; i < 6; i++)
	{
		Lotto1[6] = 0;
		Lotto1[7] = 0;
		scanf("%d", &Lotto1[i]);
		if (Lotto1[i] <= 0 || Lotto1[i] >= 46)
		{
			i--;
			printf("0~45사이의 숫자를 입력해주세요\n");
			continue;
		}
	}
	printf("B : ");
	for (int i = 0; i < 6; i++)
	{
		Lotto2[6] = 0;
		Lotto2[7] = 0;
		scanf("%d", &Lotto2[i]);
		if (Lotto2[i] <= 0 || Lotto2[i] >= 46)
		{
			i--;
			printf("0~45사이의 숫자를 입력해주세요\n");
			continue;
		}
	}
	printf("C : ");
	for (int i = 0; i < 6; i++)
	{
		Lotto3[6] = 0;
		Lotto3[7] = 0;
		scanf("%d", &Lotto3[i]);
		if (Lotto3[i] <= 0 || Lotto3[i] >= 46)
		{
			i--;
			printf("0~45사이의 숫자를 입력해주세요\n");
			continue;
		}
	}
	printf("D : ");
	for (int i = 0; i < 6; i++)
	{
		Lotto4[6] = 0;
		Lotto4[7] = 0;
		scanf("%d", &Lotto4[i]);
		if (Lotto4[i] <= 0 || Lotto4[i] >= 46)
		{
			i--;
			printf("0~45사이의 숫자를 입력해주세요\n");
			continue;
		}
	}
	printf("E : ");
	for (int i = 0; i < 6; i++)
	{
		Lotto5[6] = 0;
		Lotto5[7] = 0;
		scanf("%d", &Lotto5[i]);
		if (Lotto5[i] <= 0 || Lotto5[i] >= 46)
		{
			i--;
			printf("0~45사이의 숫자를 입력해주세요\n");
			continue;
		}
	}
	LottoRange(Lotto1);
	LottoRange(Lotto2);
	LottoRange(Lotto3);
	LottoRange(Lotto4);
	LottoRange(Lotto5);
	int a = 1;
	send(hSocket, (char *)&a, sizeof(a), 0);
}
