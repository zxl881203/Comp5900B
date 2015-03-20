

//=============================================================================
// Chaser.cpp 
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstrates the creation of a surface and of vertex and index buffers
// b. demonstartes how to rpesent a surface 
//
//
// Description:
//--------------
//
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes and for assignments in the gaming courses at 
// the School of Compuer Science at Carleton University.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================
#define ANGLE_OF_VISION 180
#define LENGTH_OF_VISION 1000




#include "Chaser.h"
#include <cmath>


/**************************************************************************/

Chaser::Chaser(void)
{
}

Chaser::Chaser(Prey *p){
	this->prey = p;
	left = false;
	right = false;
}

/**************************************************************************/
Chaser::~Chaser(void)
{
}
/**************************************************************************/
  
// this is the object behaviour.  Here the logic should be
int Chaser::updateState(int time) 	// update the object state

{
	chase(time);
	
	return 0;
}

/*************************************************************************************/

/**************************************************************************/
// responds to a key stroke
int Chaser::processKeys(unsigned char key)
{
	
	return 0;
}

/**************************************************************************/
// responds to  seq. of key strokes
int Chaser::processKeys(unsigned char *key, int numKeys)

{

	return 0;
}


//my methods
//1. this is the all inclusive movement of the chaser
void Chaser::chase(int time){
	if (seePrey()){
			//move to the prey's current position
			moveDirectlyToPosition(getPreyPosition());
			lastSeenPosition = getPreyPosition();//if the chaser saw the prey, it will remember it
	}
	else if (lastSeenPosition.length() < pow(10, -6)){
		//here I assume that if the lastSeenPosition is not (0,0,0), then the chaser must seen the prey previously
		//(0,0,0) is just a symbol
			moveDirectlyToPosition(lastSeenPosition);
			lastSeenPosition = Vector3f(0, 0, 0);//clear the lastSeenPosition field

			//check collision with surface boundary
	}
	else{
		wander(time);
	}

	//std::cout << lastSeenPosition.x <<" "<< lastSeenPosition.y<<" "<< lastSeenPosition.z << std::endl;
}
	



//2. the movement of the chaser seeing the prey
void Chaser::moveDirectlyToPosition(Vector3f pos){
	
	//get the angle A between the chaser's lookAtVector and z-axis
	//apply the following functions to map the prey's location to the chaser's local coordinate
	//x=x*cosA+y*sinA
	//y=-x*sinA+y*cosA
	Vector3f p1 = pos - this->mPosition;
	Vector3f p2 = this->lookAtVector + this->mPosition;
	float A = Vector3f::cross_value_value(p1,p2);

	//float x = pos.x*cos(A) + pos.z*sin(A);

	if (A <0){
		left = true;
	}
	if (A > 0){
		right = true;
	}
	
	//movement
	if (left){
		yaw(2);
	}
	if (right){
		yaw(-2);
		
	}
	moveForward(CAR_SPEED*0.01);
	
	
	
	left = false;
	right = false;
	return;
}

//3. the pattern of the chaser has not seen the prey
void Chaser::wander(int time){
	//moveForward(0.001);

	static int dir = 1;

	// temporarily add motion to objects.  should be moved to the "behaviour" of objects
	moveForward((float) 0.01*dir);
	if (time  % 300 < 100) {
		dir *= -1;		// change direction
	}

	if (time % 200 == 0) {
		yaw(1.5);  // rotate around the up vector of object by 10 degrees
		this->updateSpeed(1);
	}

	if (time  % 1 == 0) {
		yaw(0.1);  // rotate around the up vector of object by 10 degrees
	}
	return;

}

//helper method
bool Chaser::seePrey(){
	//we should satisfy two requirements
	bool ableToSee = false;

	//1.the prey is within the angle of the vision
	bool withinAngle = false;
	float cosPreyChaser = (Vector3f::dot(lookAtVector, getPreyPosition() - mPosition))
		/ (lookAtVector.length()*(getPreyPosition() - mPosition).length());
	withinAngle = cosPreyChaser<cos(0) && cosPreyChaser>cos(ANGLE_OF_VISION / 2);

	//2.the prey is within the range(distance) of sight
	bool withInLength = (mPosition - getPreyPosition()).length() < LENGTH_OF_VISION;

	//3. there is no house blocking the vision of the chaser, which is not implemented yet
	if (true == withinAngle)
		if (true == withInLength)
			ableToSee =  true ;
	//std::cout << ableToSee << std::endl;
	return ableToSee;
}

//helper methtod
Vector3f Chaser::getPreyPosition(){
	return this->prey->mPosition;
}

void Chaser::collisionReaction(){
	this->moveForward(-1);
}