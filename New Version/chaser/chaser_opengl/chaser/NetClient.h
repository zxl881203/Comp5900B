
#include "chaser.h"
#include "gameApp.h"
#include <winsock.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Convert.h"
#include <vector>
#include <sstream>
#include "UpdateMessage.h"
#pragma once
using namespace std;


// DEFINE  
#define MSG_LENGTH 256
#define NUM_CONNECTION_ATTEMPTS 10
#define CONNECTION_CLOSED -10


/*
connection related operation
*/
struct temp{
	Vector3f myposition;
	Vector3f lookatVector;
	double sendtime;
	float speed;

};


class NetClient
{
public:
	SOCKET sockfd;
	gameApp  *gA;
	NetClient();
	NetClient(gameApp*);
	~NetClient();
	int	initCommunication(char *serverIPAdd, char *port, WSAData *wsaData);
	int clientAction(SOCKET sockfd, char* msg);
	int commuicate(void);
	string vectortostring(Vector3f position);

	char *PackData(Vector3f position, Vector3f lookAtVector, double sendTime, float speed);
	temp UnpackData(char str[]);

	void send(UpdateMessage msg);
	void parse( char*);
};

