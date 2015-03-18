#include "NetClient.h"
#include "gameApp.h"


NetClient::NetClient()
{
	
}


NetClient::NetClient(gameApp *gA)
{
	this->gA = gA;
}

NetClient::~NetClient()
{
}

int NetClient::commuicate(void){
	int rc = 0;
	WSAData wsaData;
	SOCKET sockfd = INVALID_SOCKET;

	DWORD second;


	char* ipaddress;
	char* port;
	char buffer[256];
	string parastring[6];

	fstream out;
	int i = 0;
	out.open("Text.txt", ios::in);
	while (!out.eof()){
		out.getline(buffer, 256, '\n');
		if (buffer[0] != ':'){
			parastring[i] = buffer;
			i++;
		}

	}
	out.close();
	ipaddress = const_cast<char*>(parastring[0].c_str());
	port = const_cast<char*>(parastring[1].c_str());
	second = (DWORD)const_cast<char*>(parastring[2].c_str());


	rc = initCommunication(ipaddress, port, &wsaData);
	if (rc != 0) {
		cout << "Could not establish connectrion to the server" << endl;
		cout << "Check the server, ip address and port number and try again" << endl;
		goto err;
	}




err:

	// Shut Winsock back down and take off.
	if (sockfd != INVALID_SOCKET) {
		closesocket(sockfd);
	}

	WSACleanup();

	return (rc);

}

int NetClient::initCommunication(char *serverIPAdd, char *port, WSAData *wsaData){
	int rc = 0;
	unsigned short serverPort = 0;
	//SOCKET clientSock = INVALID_SOCKET;
	struct sockaddr_in serverAdd;
	int clientAddLen = 0;
	//fd_set write_fds;
	timeval timeout;		// used to determine a wait time for the completion of the select action
	int count = 0;			// counter for the number of attmepts to connected to the server
	int connected = 0;		// a flag to determine whether the connectivity was successful

	// init the time out
	this->sockfd = INVALID_SOCKET;
	timeout.tv_sec = 0;
	timeout.tv_usec = 500000;	// 0.5 seconds

	// Start Winsock up
	if ((rc = WSAStartup(MAKEWORD(2, 2), wsaData)) != 0) {
		cout << "WSAStartup error.  rc = " << rc << "." << endl;
		return(1);
	}

	cout << " max number of sockets = " << wsaData->iMaxSockets << " fd_setsocket = " << FD_SETSIZE << endl;

	// open the socket 
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);	// service request will determine the protocol could be IPPRORTO_TCP
	if (this->sockfd == INVALID_SOCKET) {
		// error
		rc = -1;
		goto err;
	}


	sscanf(port, "%hu", &serverPort);
	serverAdd.sin_family = AF_INET;
	serverAdd.sin_addr.S_un.S_addr = inet_addr(serverIPAdd);
	serverAdd.sin_port = htons(serverPort);

	while (!connected && count < NUM_CONNECTION_ATTEMPTS) {
		count++;
		rc = connect(this->sockfd, (struct sockaddr *) &serverAdd, sizeof(serverAdd));
		if (rc == SOCKET_ERROR) {
			// error
			rc = WSAGetLastError();
			switch (rc) {
			case WSAECONNREFUSED:
				// server may not be up yet
				Sleep(1000);
				break;
			case WSAEWOULDBLOCK:
				// connection was not established yet.  
				// do not call connect again on the same socket.

				Sleep(1000);
				break;
			default:
				rc = -1;
				goto err;
				break;
			}
		}
		else {
			connected = 1;

			// If iMode!=0, non-blocking mode is enabled.
			u_long iMode = 1;
			ioctlsocket(this->sockfd, FIONBIO, &iMode);
		}
	}

	if (!connected) rc = -1;
err:
	return(rc);

}


void NetClient::send( UpdateMessage msg){
	msg.messageSend(this->sockfd);
}

void NetClient::parse( char*){

}





