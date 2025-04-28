#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h> 
#include <string.h>
#include <map>
#include "functions.h"

#define TIME_PORT	27015


void main()
{
	// Initialize Winsock (Windows Sockets).
	WSAData wsaData;
	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Time Client: Error at WSAStartup()\n";
		return;
	}

	// Client side:
	// Create a socket and connect to an internet address.

	SOCKET connSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == connSocket)
	{
		cout << "Time Client: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// For a client to communicate on a network, it must connect to a server.

	// Need to assemble the required data for connection in sockaddr structure.

	// Create a sockaddr_in object called server. 
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(TIME_PORT);

	// Send and receive data.

	int bytesSent = 0;
	int bytesRecv = 0;
	char sendBuff[255];
	char recvBuff[255];
	char userChoice[255];
	int userChoiceNum;
	bool end = false;

	// Asks the server what's the currnet time.
	// The send function sends data on a connected socket.
	// The buffer to be sent and its size are needed.
	// The fourth argument is an idicator specifying the way in which the call is made (0 for default).
	// The two last arguments hold the details of the server to communicate with. 
	// NOTE: the last argument should always be the actual size of the client's data-structure (i.e. sizeof(sockaddr)).
	while (!end)
	{
		showMenu();
		cin >> userChoice;
		userChoiceNum = atoi(userChoice);

		switch (userChoiceNum)
		{
		case 1:
		{
			strcpy(sendBuff, "1");
			break;
		}
		case 2:
		{
			strcpy(sendBuff, "2");
			break;
		}
		case 3:
		{
			strcpy(sendBuff, "3");
			break;
		}
		case 4:
		{
			GetClientToServerDelayEstimation(connSocket, server);
			break;
		}
		case 5:
		{
			measureRTT(connSocket, server);
			break;
		}
		case 6:
		{
			strcpy(sendBuff, "6");
			break;
		}
		case 7:
		{
			strcpy(sendBuff, "7");
			break;
		}
		case 8:
		{
			strcpy(sendBuff, "8");
			break;
		}
		case 9:
		{
			strcpy(sendBuff, "9");
			break;
		}
		case 10:
		{
			strcpy(sendBuff, "10");
			break;
		}
		case 11:
		{
			strcpy(sendBuff, "11");
			break;
		}
		case 12:
		{
			getTimeWithoutDateInCity(sendBuff);
			break;
		}
		case 13:
		{
			strcpy(sendBuff, "13");
			break;
		}
		case 14:
		{
			end = true;
			break;
		}
		default:
		{
			strcpy(sendBuff, userChoice);
			break;
		}
		}
		if (userChoiceNum != 5 && userChoiceNum != 4)
		{
			bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
			if (SOCKET_ERROR == bytesSent)
			{
				cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
				closesocket(connSocket);
				WSACleanup();
				return;
			}
			cout << "Time Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";
		}
		if (!end)
		{
			if (userChoiceNum != 5 && userChoiceNum != 4)
			{
				// Gets the server's answer using simple recieve (no need to hold the server's address).
				bytesRecv = recv(connSocket, recvBuff, 255, 0);
				if (SOCKET_ERROR == bytesRecv)
				{
					cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
					closesocket(connSocket);
					WSACleanup();
					return;
				}

				recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
				cout << "Time Client: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";
			}
		}
		else
		{
			// Closing connections and Winsock.
			cout << "Time Client: Closing Connection.\n";
			closesocket(connSocket);
		}
		cout << "-------------------------------------------------------\n";
	}
	system("pause");
}