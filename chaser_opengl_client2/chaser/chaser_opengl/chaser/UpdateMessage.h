#pragma once
#include "nuss_vector.h"

#include <winsock.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Convert.h"
#include <vector>
#include <sstream>
using namespace std;
/*
connection related operation
*/
struct temp{
	Vector3f myposition;
	Vector3f lookatVector;
	double sendtime;
	float speed;

};


class UpdateMessage
{
public:
	UpdateMessage();
	~UpdateMessage();

	virtual void messageSend(SOCKET sockfd);
	virtual void messageParse(SOCKET sockfd, char*);

	//Util
	char *PackData(SOCKET sockfd, Vector3f position, Vector3f lookAtVector, int sendTime, float speed);
	temp UnpackData(char str[]);
	string vectortostring(Vector3f position);


};

