#include "functions.h"

void showMenu() {
	cout << "\n--- Menu ---\n";
	cout << "1. Get Time\n";
	cout << "2. Get Time Without Date\n";
	cout << "3. Get Time Since Epoch\n";
	cout << "4. Get Client-to-Server Delay Estimation\n";
	cout << "5. Measure RTT\n";
	cout << "6. Get Time Without Date or Seconds\n";
	cout << "7. Get Year\n";
	cout << "8. Get Month and Day\n";
	cout << "9. Get Seconds Since Beginning of Month\n";
	cout << "10. Get Week of Year\n";
	cout << "11. Get Daylight Savings\n";
	cout << "12. Get Time Without Svings\n";
	cout << "13. Measure Time Lap\n";
	cout << "14. Exit\n";
	cout << "Enter your choice: ";
}

int sendData(SOCKET socket, const sockaddr_in& serverAddress, const char* dataBuffer) {
	int bytesSent = sendto(socket, dataBuffer, strlen(dataBuffer), 0, (const sockaddr*)&serverAddress, sizeof(serverAddress));
	if (SOCKET_ERROR == bytesSent) {
		cerr << "Error at sendto(): " << WSAGetLastError() << endl;
		return SOCKET_ERROR;
	}
	return bytesSent;
}

int receiveData(SOCKET socket, sockaddr_in& serverAddress, char* recvBuffer, int bufferSize) {
	int serverAddressLength = sizeof(serverAddress);
	int bytesReceived = recvfrom(socket, recvBuffer, bufferSize, 0, (sockaddr*)&serverAddress, &serverAddressLength);
	if (SOCKET_ERROR == bytesReceived) {
		cerr << "Error at recvfrom(): " << WSAGetLastError() << endl;
		return SOCKET_ERROR;
	}
	recvBuffer[bytesReceived] = '\0';
	return bytesReceived;
}


void measureRTT(SOCKET clientSocket, sockaddr_in& serverAddress) {
	char sendBuffer[255];
	char recvBuffer[255];
	int successfulMessagesCount = 0;
	DWORD sendTime, totalRTT = 0;
	DWORD recvTime = 0;

	for (int i = 0; i < NUM_MESSAGES; i++) {
		sprintf(sendBuffer, "%d", 5);

		sendTime = GetTickCount();
		if (sendData(clientSocket, serverAddress, sendBuffer) == SOCKET_ERROR) {
			continue;
		}

		if (receiveData(clientSocket, serverAddress, recvBuffer, sizeof(recvBuffer)) == SOCKET_ERROR) {
			continue;
		}

		recvTime = GetTickCount();

		DWORD rtt = recvTime - sendTime;
		totalRTT += rtt;
		successfulMessagesCount++;

		cout << "Client: Sent " << strlen(sendBuffer) << " bytes: \"" << sendBuffer << "\"\n";
	}

	if (successfulMessagesCount > 0) {
		double averageRTT = static_cast<double>(totalRTT) / successfulMessagesCount;
		cout << "\nAverage RTT over " << successfulMessagesCount << " messages: " << averageRTT << " ms" << endl;
	}
	else {
		cout << "No successful RTT measurements." << endl;
	}
}

void GetClientToServerDelayEstimation(SOCKET clientSocket, sockaddr_in& serverAddress) {
	char sendBuffer[255];
	char recvBuffer[255];
	DWORD clientTimes[NUM_MESSAGES] = { 0 };
	DWORD serverTimes[NUM_MESSAGES] = { 0 };
	DWORD totalDelay = 0;
	int successfulMessagesCount = 0;

	for (int i = 0; i < NUM_MESSAGES; i++) {
		clientTimes[i] = GetTickCount();
		sprintf(sendBuffer, "%d", 4);

		if (sendData(clientSocket, serverAddress, sendBuffer) == SOCKET_ERROR) {
			clientTimes[i] = 0;
		}
	}

	for (int i = 0; i < NUM_MESSAGES; i++) {
		if (receiveData(clientSocket, serverAddress, recvBuffer, sizeof(recvBuffer)) == SOCKET_ERROR) {
			continue;
		}

		if (sscanf(recvBuffer, "%lu", &serverTimes[i]) != 1) {
			cerr << "Error converting server response to DWORD." << endl;
			continue;
		}

		if (clientTimes[i] != 0) {
			DWORD delay = serverTimes[i] - clientTimes[i];
			totalDelay += delay;
			successfulMessagesCount++;
			cout << "Delay for message " << i + 1 << ": " << delay << " ms" << endl;
		}
	}

	if (successfulMessagesCount > 0) {
		double averageDelay = static_cast<double>(totalDelay) / successfulMessagesCount;
		cout << "\nAverage Client-to-Server Delay over " << successfulMessagesCount << " messages: " << averageDelay << " ms" << endl;
	}
	else {
		cout << "No successful measurements." << endl;
	}
}

int getCityChoice() {
	cout << "Available cities:\n";
	cout << "1. Doha (Qatar)\n";
	cout << "2. Prague (Czech Republic)\n";
	cout << "3. New York (USA)\n";
	cout << "4. Berlin (Germany)\n";
	cout << "Choose a city: ";

	char inputBuffer[255];
	int cityChoice;

	cin >> inputBuffer;
	while ((sscanf(inputBuffer, "%d", &cityChoice) != 1) || strlen(inputBuffer) == 0) {
		cout << "Invalid input. Please enter a number: ";
		cin >> inputBuffer;
	}

	return cityChoice;
}

string mapCityNumberToName(int cityChoice) {
	map<int, string> cities = {
		{1, "Doha"},
		{2, "Prague"},
		{3, "New York"},
		{4, "Berlin"}
	};

	if (cities.find(cityChoice) != cities.end())
		return cities[cityChoice];
	else
		return "other";
}

void getTimeWithoutDateInCity(char sendBuff[])
{
	int cityChoice = getCityChoice();
	string cityName = mapCityNumberToName(cityChoice);

	strcpy(sendBuff, "12 "); // Start with "12 "
	strcat(sendBuff, cityName.c_str()); // Concatenate the city
}