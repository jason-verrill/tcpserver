#pragma once

#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include "tcphead.h"

#pragma comment(lib, "ws2_32.lib")


class TCPCLIENT : TCPHEAD {
public:
	TCPCLIENT(std::string serverIP, int serverPort);
	void start();
private:
	RESULT_CODE connectToServer();
	RESULT_CODE sendAndReceiveMessages();
	void run();
};
