#include "ShadowChaser.h"
#include <ctime>
#define	DISTANCE_THRESHOLD 1
#define ANGLE_THRESHOLD 60
#define TIME_OF_CATCH_UP 5

ShadowChaser::ShadowChaser()
{
}


ShadowChaser::ShadowChaser(Chaser* chaser)
{
	this->currentTime = std::time(0) * 1000;
	this->lastTime = this->currentTime;

	//initiate currentPrey
	this->currentChaser = chaser;//currentPrey reference the prey, however the oldPrey is a deep copy of the prey

	//initiate oldPrey
	this->oldChaser = new Chaser();
	Vector3f *oldChaserLookAtVector = new Vector3f(chaser->lookAtVector.x, chaser->lookAtVector.y, chaser->lookAtVector.z);
	Vector3f *oldChaserMPosition = new Vector3f(chaser->mPosition.x, chaser->mPosition.y, chaser->mPosition.z);
	this->oldChaser->lookAtVector = *oldChaserLookAtVector;
	this->oldChaser->mPosition = *oldChaserMPosition;

	setPositionOrientation(currentChaser->mPosition, currentChaser->lookAtVector, this->currentChaser->upVector);
}



ShadowChaser::~ShadowChaser()
{
}

int ShadowChaser::updateState(int time){
	/*here I need to update the shadowPrey's
	1.position
	2.lookAtVector(this represents the direction and velocity information)

	there are two situations
	1.if the distance between the shadowPrey and its wrapping prey is with in the distance it moves according to the prey's moving mechanism
	2.if the distance is over a certain range, then the shadowPrey should update its position and its lookAtVector
	*/

	//the next line are function code

	Vector3f *v = new Vector3f(this->mPosition.x - this->currentChaser->mPosition.x, this->mPosition.y - this->currentChaser->mPosition.y, this->mPosition.z - this->currentChaser->mPosition.z);
	float distance = v->length();
	float directionDifference = Vector3f::dot(this->currentChaser->lookAtVector, this->lookAtVector);

	if (distance <= DISTANCE_THRESHOLD&&directionDifference<ANGLE_THRESHOLD){// 1.if the distance between the shadowPrey and its wrapping prey is with in the distance
		//1.update the lookAtVector and position according to Dead Reckoning mechanism
		Vector3f *oldChaserLookAtVector = new Vector3f(this->oldChaser->lookAtVector.x, this->oldChaser->lookAtVector.y, this->oldChaser->lookAtVector.z);
		Vector3f *oldChaserMPosition = new Vector3f(this->oldChaser->mPosition.x, this->oldChaser->mPosition.y, this->oldChaser->mPosition.z);

		//get the old "direction"
		this->lookAtVector = *oldChaserLookAtVector;

		//get the time difference
		this->currentTime = std::time(0) * 1000;
		double timeDifference = this->currentTime - this->lastTime;//this is the real time, it should be further divided by how often is moveforward() called, e.g. the time interval

		switch ((int)timeDifference){
			//we can modify the factor of future meeting point here, if time is within threshold, we assign number, if time is out of threshold, we set the position
		}


		//int moveForwardCount = timeDifference / 1000;


		float oldChaserSpeed = this->oldChaser->getSpeed();

		this->moveForward(oldChaserSpeed);

		this->mPosition;
	}
	else{//if the distance is over a certain range or the direction is also over a certain range
		//update the oldPrey information to the new informations
		this->oldChaser = new Chaser();
		Vector3f *oldChaserLookAtVector = new Vector3f(currentChaser->lookAtVector.x, currentChaser->lookAtVector.y, currentChaser->lookAtVector.z);
		Vector3f *oldChaserMPosition = new Vector3f(currentChaser->mPosition.x, currentChaser->mPosition.y, currentChaser->mPosition.z);
		this->oldChaser->lookAtVector = *oldChaserLookAtVector;
		this->oldChaser->mPosition = *oldChaserMPosition;

		this->lastTime = this->currentChaser->moveForwardTime;
		//March 10/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//the above are the first version of my reaction which is merely assign the shadowPrey to the current position
		//the below is the second version of my reaction, which is letting shadowPrey moving towards a future position wihtin 0.5 second

		//TIME_OF_CATCH_UP is set according to time of receiving message difference
		Vector3f futurePosition = this->currentChaser->mPosition + TIME_OF_CATCH_UP*this->currentChaser->lookAtVector;//I need to let my shadowPrey move to that position within 0.5 second
		this->lookAtVector = futurePosition - this->mPosition;
		this->lookAtVector.normalize();
		float dPF = (futurePosition - this->currentChaser->mPosition).length();//the distance between the prey and the future position
		float dSF = (futurePosition - this->mPosition).length();//the distance between the shadowPrey and the future position
		float chaserSpeed = this->currentChaser->getSpeed();
		float increasedSpeed = chaserSpeed*dSF / dPF;
		this->setSpeed(increasedSpeed);
		this->moveForward(increasedSpeed);
		
		this->lookAtVector = this->currentChaser->lookAtVector;
		this->setSpeed = chaserSpeed;
		this->lastTime = this->currentChaser->moveForwardTime;


	}



	return 0;
}


