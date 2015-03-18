#pragma once
#include "UpdateMessage.h"
#include "ShadowPrey.h"
#include "gameApp.h"

class PreyUpdateMessage :
	public UpdateMessage
{
public:
	PreyUpdateMessage();

	PreyUpdateMessage(ShadowPrey sP);//this ocnstructor is needed for sending the message

	PreyUpdateMessage(gameApp* gA);//this constructor is needed for parsing the message
	
	~PreyUpdateMessage();
	void sendMessage(SOCKET sockfd);
	void parseMessage(void);
private:
	ShadowPrey sP;
	gameApp *gA;
};

