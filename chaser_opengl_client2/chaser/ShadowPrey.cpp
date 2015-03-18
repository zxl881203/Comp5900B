#include "ShadowPrey.h"
#include <ctime>
#define	DISTANCE_THRESHOLD 1
#define ANGLE_THRESHOLD 60
#define TIME_OF_CATCH_UP 5

ShadowPrey::ShadowPrey()
{
}

ShadowPrey::ShadowPrey(Prey* prey){
	this->currentTime = std::time(0)*1000;
	this->lastTime = this->currentTime;

	//initiate currentPrey
	this->currentPrey = prey;//currentPrey reference the prey, however the oldPrey is a deep copy of the prey

	//initiate oldPrey
	this->oldPrey = new Prey();
	Vector3f *oldPreyLookAtVector = new Vector3f(prey->lookAtVector.x, prey->lookAtVector.y, prey->lookAtVector.z);
	Vector3f *oldPreyMPosition = new Vector3f(prey->mPosition.x, prey->mPosition.y, prey->mPosition.z);
	this->oldPrey->lookAtVector = *oldPreyLookAtVector;
	this->oldPrey->mPosition = *oldPreyMPosition;

	setPositionOrientation(currentPrey->mPosition, currentPrey->lookAtVector, this->currentPrey->upVector);
}



ShadowPrey::~ShadowPrey()
{
}

int ShadowPrey::updateState(int time){
	/*here I need to update the shadowPrey's 
		1.position
		2.lookAtVector(this represents the direction and velocity information)

	  there are two situations
		1.if the distance between the shadowPrey and its wrapping prey is with in the distance it moves according to the prey's moving mechanism
		2.if the distance is over a certain range, then the shadowPrey should update its position and its lookAtVector
	*/

	//the next line are function code
	
	Vector3f *v = new Vector3f(this->mPosition.x - this->currentPrey->mPosition.x, this->mPosition.y - this->currentPrey->mPosition.y, this->mPosition.z - this->currentPrey->mPosition.z);
	float distance = v->length();
	float directionDifference = Vector3f::dot(this->currentPrey->lookAtVector, this->lookAtVector);

	if(distance <= DISTANCE_THRESHOLD&&directionDifference<ANGLE_THRESHOLD){// 1.if the distance between the shadowPrey and its wrapping prey is with in the distance
		//1.update the lookAtVector and position according to Dead Reckoning mechanism
		Vector3f *oldPreyLookAtVector = new Vector3f(this->oldPrey->lookAtVector.x, this->oldPrey->lookAtVector.y, this->oldPrey->lookAtVector.z);
		Vector3f *oldPreyMPosition = new Vector3f(this->oldPrey->mPosition.x, this->oldPrey->mPosition.y, this->oldPrey->mPosition.z);

		//get the old "direction"
		this->lookAtVector = *oldPreyLookAtVector;

		//get the time difference
		this->currentTime =std::time(0) * 1000;
		double timeDifference = this->currentTime - this->lastTime;//this is the real time, it should be further divided by how often is moveforward() called, e.g. the time interval
		
		switch ((int)timeDifference){
			//we can modify the factor of future meeting point here, if time is within threshold, we assign number, if time is out of threshold, we set the position
		}
		
		
		//int moveForwardCount = timeDifference / 1000;


		float oldPreySpeed = this->oldPrey->getSpeed();
		
		this->moveForward(oldPreySpeed);

		this->mPosition;
	}
	else{//if the distance is over a certain range or the direction is also over a certain range
		//update the oldPrey information to the new informations


		
		this->oldPrey = new Prey();
		Vector3f *oldPreyLookAtVector = new Vector3f(currentPrey->lookAtVector.x, currentPrey->lookAtVector.y, currentPrey->lookAtVector.z);
		Vector3f *oldPreyMPosition = new Vector3f(currentPrey->mPosition.x, currentPrey->mPosition.y, currentPrey->mPosition.z);
		this->oldPrey->lookAtVector = *oldPreyLookAtVector;
		this->oldPrey->mPosition = *oldPreyMPosition;

		this->lastTime = this->currentPrey->moveForwardTime;
		//March 10/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//the above are the first version of my reaction which is merely assign the shadowPrey to the current position
		//the below is the second version of my reaction, which is letting shadowPrey moving towards a future position wihtin 0.5 second

		//TIME_OF_CATCH_UP is set according to time of receiving message difference
		Vector3f futurePosition = this->currentPrey->mPosition + TIME_OF_CATCH_UP*this->currentPrey->lookAtVector;//I need to let my shadowPrey move to that position within 0.5 second
		this->lookAtVector = futurePosition - this->mPosition;
		this->lookAtVector.normalize();
		float dPF = (futurePosition - this->currentPrey->mPosition).length();//the distance between the prey and the future position
		float dSF = (futurePosition-this->mPosition).length();//the distance between the shadowPrey and the future position
		float preySpeed = this->currentPrey->getSpeed();
		float increasedSpeed = preySpeed*dSF / dPF;
		this->setSpeed(increasedSpeed);
		this->moveForward(increasedSpeed);
		this->lookAtVector = this->currentPrey->lookAtVector;

		this->lastTime = this->currentPrey->moveForwardTime;


	}
	
	
	
	return 0;
}