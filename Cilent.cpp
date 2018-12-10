#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>
#include<iostream>
#include<time.h>

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
		puts("������ ����Ǿ����ϴ�.\n");

	int M_input, P_input;
	cout << "�ζ� �ùķ��̼�" << endl;
	cout << "---------Menu----------" << endl;
	cout << "1.�ڵ���ġ" << endl;
	cout << "2.������ġ" << endl;
	cout << "-----------------------" << endl;

	cout << "�Է�:";
	cin >> M_input;
	
	send(hSocket, (char*)M_input, sizeof(M_input), 0);

	
	while(1)
	{
		if (M_input == 1)		//�ڵ���ġ
		{
			system("cls");
			cout << "�ڵ����� �ζ� ���ڸ� ��ġ�մϴ�." << endl;

			for (int i = 0; i < 5; i++)
			{
				LottoShuffle();
			}
			cout << "�ζ� ��ȣ ���� �Ϸ�" << endl;
			cout << "����Ű�� �����ּ��� !" << endl;
			getchar();
		}
		else if (M_input == 2)	//������ġ
		{
			system("cls");
			cout << "�ζ� ���ڸ� ��ġ�ϼ��� !!" << endl;
		}
		else	//�׿��� ���ڵ�
		{
			cout << "���ڸ� �߸� �Է��ϼ̽��ϴ�." << endl;
			cout << "�Է�:";
			cin >> M_input;
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
		cout <<Number_Lotto[i]<<"\t";
		//send(hSocket, (char*)Number_Lotto[i], sizeof(Number_Lotto[i]), 0);
	}
	cout << endl;
}