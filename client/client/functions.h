#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define NUM_MESSAGES 100
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h> 
#include <string.h>
#include <map>

void showMenu();
int sendData(SOCKET socket, const sockaddr_in& serverAddress, const char* dataBuffer);
int receiveData(SOCKET socket, sockaddr_in& serverAddress, char* recvBuffer, int bufferSize);
void measureRTT(SOCKET clientSocket, sockaddr_in& serverAddress);
void GetClientToServerDelayEstimation(SOCKET clientSocket, sockaddr_in& serverAddress);
int getCityChoice();
string mapCityNumberToName(int cityChoice);
void getTimeWithoutDateInCity(char sendBuff[]);