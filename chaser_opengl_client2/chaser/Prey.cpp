#include "Prey.h"
#include "TargetHouse.h"
#include <winuser.h>
#include <ctime>

/**************************************************************************/


Prey::Prey(void)
{
	fpCam = new camera();
	tpCam = new camera();
	Prey::moveForwardCount=1;
	setSpeed(0.01*CAR_SPEED);
}

int Prey::moveForwardCount;
double Prey::lastMoveForwardTime;


/**************************************************************************/


Prey::~Prey(void)
{

}




/**************************************************************************/

// this is the object behaviour.  Here the logic should be  

int Prey::updateState(int time) 	// update the object state

{
	//movement section

	moveForward(this->speed);

	//this->moveForwardTime = std::time(0) * 1000;

	setFirstPersonPers();
	setThirdPersonPers();




	return 0;
}


camera* Prey::getFirstPersonPers(){
	return fpCam;
}

camera* Prey::getThirdPersonPers(){
	return tpCam;
}

void Prey::setFirstPersonPers(){
	//set camera section
	//I need to update the onCarCam in this update function
	Vector3f onCarCamPos;
	Vector3f onCarCamLV;

	onCarCamLV = mPosition + lookAtVector * 8;
	onCarCamPos = mPosition + lookAtVector + Vector3f(0, 1, 0);//first person perspective

	fpCam->setCamera(onCarCamPos, onCarCamLV, Vector3f(0, 1, 0));
	fpCam->setPerspectiveView(60, 1, (float) 0.2, 1000);
}

void Prey::setThirdPersonPers(){
	//set camera section
	//I need to update the onCarCam in this update function
	Vector3f onCarCamPos;
	Vector3f onCarCamLV;

	onCarCamLV = mPosition + lookAtVector * 8;
	onCarCamPos = Vector3f(mPosition.x-lookAtVector.x*3, 2, mPosition.z-lookAtVector.z*3);//third person perspective

	tpCam->setCamera(onCarCamPos, onCarCamLV, Vector3f(0, 1, 0));
	tpCam->setPerspectiveView(60, 1, (float) 0.2, 1000);
}




/**************************************************************************/
// responds to a key stroke
int Prey::processKeys(unsigned char key)
{
	
	return 0;
}

/**************************************************************************/
// responds to  seq. of key strokes
int Prey::processKeys(unsigned char* key, int numKeys)

{

	return 0;
}

void Prey::changePerspective(){
	
}

void Prey::collisionReaction(Car* c2){
	//if c2 is target then print a line: You Win
	TargetHouse* th = dynamic_cast<TargetHouse*>(c2);
	if (th != 0){
		//Print out:"You Win"
		outPutCollideWithTragetReaction();
		this->setPositionOrientation(Vector3f(5, 0, 5), Vector3f(1, 0, 1), Vector3f(0, 1, 0));
		std::cout << "collide with target" << std::endl;
	}
	else{
		this->moveForward(-1);
	}
		
}

void Prey::outPutCollideWithTragetReaction(){
	MessageBox(NULL, "You Win", "Congratulation", MB_OK | MB_ICONEXCLAMATION);


}


void Prey::setSpeed(float newSpeed){
	this->speed = newSpeed;
}

float Prey::getSpeed(){
	return this->speed;
}