#include "tcpserver.h"


TCPSERVER::TCPSERVER(int serverPort) {
	this->serverPort = serverPort;
	listenSocket = 0;
	listenSocketInUse = false;
	clientSize = sizeof(client);
	ZeroMemory(&client, clientSize);
}

void TCPSERVER::start() {
	TCPHEAD::start();
}

TCPSERVER::RESULT_CODE TCPSERVER::createListeningSocket() {
	listenSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (listenSocket == INVALID_SOCKET) {
		return FAIL;
	}
	listenSocketInUse = true;

	bind(listenSocket, (sockaddr*)&hint, sizeof(hint));
	listen(listenSocket, SOMAXCONN);

	return SUCCESS;
}

TCPSERVER::RESULT_CODE TCPSERVER::listenForClientConnection() {
	char host[NI_MAXHOST];
	char service[NI_MAXHOST];
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	// Listen for client connection
	clientSocket = accept(listenSocket, (sockaddr*)&client, &clientSize);

	if (clientSocket == INVALID_SOCKET) {
		return FAIL;
	}
	clientSocketInUse = true;

	// Log client connection info
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		std::cout << host << " connected on port " << service << std::endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
	}

	// Close the listen socket
	closesocket(listenSocket);
	listenSocketInUse = false;

	return SUCCESS;
}

TCPSERVER::RESULT_CODE TCPSERVER::listenForClientMessages() {
	char buf[4096];

	// Send and receive
	while (true) {
		ZeroMemory(buf, 4096);

		int bytesReceived = recv(clientSocket, buf, 4096, 0);

		if (bytesReceived == SOCKET_ERROR) {
			return FAIL;
		}
		if (bytesReceived == 0) {
			return DISCONNECTED;
		}

		send(clientSocket, buf, bytesReceived + 1, 0);
	}

	return SUCCESS;
}

void TCPSERVER::run() {
	// Create a new listening socket
	if (createListeningSocket() == FAIL) {
		std::cerr << "Failed to create a listening socket" << std::endl;
		return;
	}
	listenSocketInUse = true;

	// Wait for a new client connection
	if (listenForClientConnection() == FAIL) {
		std::cerr << "Failed to create client socket" << std::endl;
		shutdown();
		return;
	}
	clientSocketInUse = true;

	// Send and receive messages from client
	switch (listenForClientMessages()) {
		case DISCONNECTED:
			std::cout << "Client disconnected" << std::endl;
			break;
		case FAIL:
			std::cerr << "Error receiving socket" << std::endl;
			break;
		}

	shutdown();
}

void TCPSERVER::shutdown() {
	TCPHEAD::shutdown();

	if (listenSocketInUse) {
		closesocket(listenSocket);
		listenSocketInUse = false;
	}

	if (clientSocketInUse) {
		closesocket(clientSocket);
		clientSocketInUse = false;
	}
}