#pragma once

#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")


class TCPHEAD {
public:
	TCPHEAD();
	~TCPHEAD();
	virtual void start();

protected:
	enum RESULT_CODE {
		FAIL,
		SUCCESS,
		DISCONNECTED
	};

	RESULT_CODE setupWinsock();
	RESULT_CODE createSocket();
	virtual void run() = 0;
	void shutdown();

	std::string serverIP;
	int serverPort;
	SOCKET serverSocket;
	sockaddr_in hint;
	bool winsockInUse;
	bool socketInUse;
};
