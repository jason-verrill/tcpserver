#include "tcpclient.h"


TCPCLIENT::TCPCLIENT(std::string serverIP, int serverPort) {
	this->serverIP = serverIP;
	this->serverPort = serverPort;
}

// Do TCPHEAD initializiation
void TCPCLIENT::start() {
	TCPHEAD::start();
}

void TCPCLIENT::run() {
	// Connect to a server
	if (connectToServer() == FAIL) {
		std::cerr << "Failed to connect to the server. Error: " << WSAGetLastError() << std::endl;
		shutdown();
		return;
	}

	// Send messages to the server
	if (sendAndReceiveMessages() == FAIL) {
		std::cerr << "Failed to send/receive messages. Error: " << WSAGetLastError() << std::endl;
		shutdown();
		return;
	}
}

TCPCLIENT::RESULT_CODE TCPCLIENT::connectToServer() {
	int connectionResult = connect(serverSocket, (sockaddr*)&hint, sizeof(hint));
	if (connectionResult == SOCKET_ERROR) {
		return FAIL;
	}

	return SUCCESS;
}

TCPCLIENT::RESULT_CODE TCPCLIENT::sendAndReceiveMessages() {
	// Send text
	const int bufSize = 4096;
	char buf[bufSize];
	std::string userInput;

	do {
		std::cout << "> ";
		getline(std::cin, userInput);

		if (userInput.size() > 0) {
			int sendResult = send(serverSocket, userInput.c_str(), userInput.size() + 1, 0);

			if (sendResult == SOCKET_ERROR) {
				return FAIL;
			}

			ZeroMemory(buf, bufSize);
			int bytesReceived = recv(serverSocket, buf, bufSize, 0);

			if (bytesReceived > 0) {
				std::cout << "SERVER> " << std::string(buf, 0, bytesReceived) << std::endl;
			}
		}

	} while (userInput.size() > 0);

	return SUCCESS;
}