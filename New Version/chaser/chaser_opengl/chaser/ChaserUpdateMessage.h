#pragma once
#include "UpdateMessage.h"
#include "ShadowChaser.h"
#include "gameApp.h"

class ChaserUpdateMessage :
	public UpdateMessage
{
public:
	ChaserUpdateMessage();

	ChaserUpdateMessage(ShadowChaser sC);////this constructor is needed for sending the message

	ChaserUpdateMessage(gameApp* gA);//this constructor is needed for parsing the message

	~ChaserUpdateMessage();
	void send();
	void parse();
private: 
	ShadowChaser sC;
	gameApp *gA;
};

