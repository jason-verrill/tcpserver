#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include "tcphead.h"

#pragma comment (lib, "ws2_32.lib")


class TCPSERVER : TCPHEAD {
public:
	TCPSERVER(int serverPort);
	void start();
private:
	RESULT_CODE createListeningSocket();
	RESULT_CODE listenForClientConnection();
	RESULT_CODE listenForClientMessages();
	void run();
	void shutdown();

	sockaddr_in client;
	int clientSize;
	SOCKET listenSocket;
	SOCKET clientSocket;
	bool listenSocketInUse;
	bool clientSocketInUse;
};
