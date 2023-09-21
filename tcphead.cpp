#include "tcphead.h"


TCPHEAD::TCPHEAD() {
	winsockInUse = false;
	socketInUse = false;
	serverPort = 0;
	serverSocket = 0;
	ZeroMemory(&hint, sizeof(hint));
}

TCPHEAD::~TCPHEAD() {
	shutdown();
}

// Virtual function because start must be called
// High level sequence of operations with error checking
void TCPHEAD::start() {
	// Setup winsock
	if (setupWinsock() == FAIL) {
		std::cerr << "Failed to start winsock" << std::endl;
		shutdown();
		return;
	}
	winsockInUse = true;

	// Bind a socket
	if (createSocket() == FAIL) {
		std::cerr << "Failed to create socket. Error: " << WSAGetLastError() << std::endl;
		shutdown();
		return;
	}
	socketInUse = true;

	// Virtual function because the user needs to define how they want to use the TCP connection in the run() function
	run();

	// Clean up
	shutdown();
}

TCPHEAD::RESULT_CODE TCPHEAD::setupWinsock() {
	WSAData data;
	WORD version = MAKEWORD(2, 2);
	int wsResult = WSAStartup(version, &data);

	if (wsResult != 0) {
		return FAIL;
	}

	return SUCCESS;
}

TCPHEAD::RESULT_CODE TCPHEAD::createSocket() {
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket == INVALID_SOCKET) {
		return FAIL;
	}

	hint.sin_family = AF_INET;
	hint.sin_port = htons(serverPort);
	inet_pton(AF_INET, serverIP.c_str(), &hint.sin_addr);

	return SUCCESS;
}

void TCPHEAD::shutdown() {
	if (socketInUse) {
		closesocket(serverSocket);
		socketInUse = false;
	}

	if (winsockInUse) {
		WSACleanup();
		winsockInUse = false;
	}
}
