# tcpserver
Basic tcp server with a modular tcp header

To use TCPHEADER, write a new class that inherits TCPHEADER, hook the TCPHEAD start() into the new class, and define a run() function. <br>
Use the run() function to determine what you want to do with your TCP connection. <br>
<br>
<br>
To use the example extended TCPSERVER, simply: <br>
<br>
int serverPort = 9999; <br>
TCPSERVER tcpserver(serverPort); <br>
tcpserver.start(); <br>
<br>
<br>
To use the example extended TCPCLIENT, simply: <br>
<br>
int serverIP = "127.0.0.1"; <br>
int serverPort = 9999; <br>
TCPCLIENT client(serverIP, serverPort); <br>
client.start(); <br>
