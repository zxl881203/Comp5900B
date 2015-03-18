#include "ChaserUpdateMessage.h"
#include <ctime>

#define TIME_SCALE 1000//the value is arbitrary, I need to modify it later

#define SHORT_TIME 3
#define MEDIUM_TIME 6
#define LONG_TIME 9

ChaserUpdateMessage::ChaserUpdateMessage(ShadowChaser sC)//this ocnstructor is needed for sending the message
{
	this->sC = sC;
}

ChaserUpdateMessage::ChaserUpdateMessage(gameApp* gA)//this constructor is needed for parsing the message
{
	this->gA = gA;
}


ChaserUpdateMessage::ChaserUpdateMessage()
{

}


ChaserUpdateMessage::~ChaserUpdateMessage()
{
}


void ChaserUpdateMessage::send(){
	//send information about the shadowChaser: sC
	this->sC.mPosition;
	this->sC.lookAtVector;
	int sendTime = std::time(0) * 1000;
	this->sC.getSpeed();
}

enum TimeDifference{
	SHORT, MEDIUM, LONG, INFI
};

void ChaserUpdateMessage::parse(){
	//update the corresponding Chaser's position, lookAtVector, speed according to the "message delay interval"
	//read out the information
	Vector3f realPosition;//read from socket
	Vector3f realLookAtVector;//read from socket
	float realSpeed;
	double sentTime;//read from socket
	double currentTime = std::time(0) * 1000;
	enum TimeDifference timeDifference = (TimeDifference)((currentTime - sentTime) / TIME_SCALE);
	int timeOfCatchUp = -1;//define the time of catch up, the initialization is in the following switch: the larger time difference is, the longer catch up will be, 
	//however if it is over a certain range, we set the location and lookAtVector directly to the current one
	switch (timeDifference){
	case SHORT:
		timeOfCatchUp = SHORT_TIME;
		break;
	case MEDIUM:
		timeOfCatchUp = MEDIUM_TIME;

		break;
	case LONG:
		timeOfCatchUp = LONG_TIME;

		break;

	}

	if (timeOfCatchUp != -1){//if time of catch up is not too long to be manipulated

		//I only need to know the chaser's current position which is the origin,
		//the future point which is calculated by using the transmitted mPosition
		//then I need to update the current this->gA->shadowchaserYellowChaser's lookAtVector and speed
		Chaser *currentChaser = this->gA->chaserYellowKia;

		//TIME_OF_CATCH_UP is set according to time of receiving message difference
		Vector3f futurePosition = realPosition + timeOfCatchUp*realLookAtVector;//I need to let my shadowChaser move to that position within 0.5 second
		currentChaser->lookAtVector = futurePosition - currentChaser->mPosition;
		currentChaser->lookAtVector.normalize();
		float dPF = (futurePosition - realPosition).length();//the distance between the chaser and the future position
		float dSF = (futurePosition - currentChaser->mPosition).length();//the distance between the shadowChaser and the future position
		float chaserSpeed = realSpeed;
		float increasedSpeed = chaserSpeed*dSF / dPF;
		currentChaser->setSpeed(increasedSpeed);
		currentChaser->moveForward(increasedSpeed);

		//recover the lookAtVector and speed
		currentChaser->lookAtVector = realLookAtVector;
		currentChaser->setSpeed(realSpeed);



	}
	else{//if tje update information is much too long, we need to brutely set the mLocaiton and lookAtVector of the gameApp's chaser
		Chaser *currentChaser = this->gA->chaserYellowKia;
		currentChaser->mPosition = realPosition;
		currentChaser->lookAtVector = realLookAtVector;
		currentChaser->setSpeed(realSpeed);
	}


}