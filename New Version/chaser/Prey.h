#pragma once



//=============================================================================
// Prey.h
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. 
// b.  
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












#include "car.h"
#include "TargetHouse.h"



class Prey : public Car
{
public:
	//set a camera at the prey, and I should add some functions to manipulate the perspective of the camera
	//this functionalities are to be implemented in the prey's constructor
	camera* fpCam;
	camera* tpCam;

	

	Prey(void);
	~Prey(void);

	virtual int updateState(int time); 	// update the object state
	virtual int processKeys(unsigned char key); // responds to a key stroke
	virtual int processKeys(unsigned char *key, int numKeys); // responds to  seq. of key strokes
	


	void changePerspective();
	camera* getFirstPersonPers();
	camera* getThirdPersonPers();

	void collisionReaction(Car* c2);


	//speed's getter and setter
	void setSpeed(float newSpeed);
	float getSpeed();
	

private:
	void setFirstPersonPers();
	void setThirdPersonPers();
	void outPutCollideWithTragetReaction();
	
	float speed;

	static int moveForwardCount;
	static double lastMoveForwardTime;
};

