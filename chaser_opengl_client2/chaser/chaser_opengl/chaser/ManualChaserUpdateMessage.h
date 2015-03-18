#pragma once
#include "UpdateMessage.h"
#include "ShadowChaser.h"
#include "gameApp.h"

class ManualChaserUpdateMessage :
	public UpdateMessage
{
public:
	ManualChaserUpdateMessage();

	ManualChaserUpdateMessage(ShadowChaser sC);////this ocnstructor is needed for sending the message

	ManualChaserUpdateMessage(gameApp* gA);//this constructor is needed for parsing the message

	~ManualChaserUpdateMessage();
	void send();
	void parse();
private:
	ShadowChaser sC;
	gameApp *gA;
};

