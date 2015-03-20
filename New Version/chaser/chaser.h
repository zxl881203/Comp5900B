#pragma once




//=============================================================================
// Chaser.h 
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
#include "Prey.h"



class Chaser : public Car
{
public:
	Chaser(void);
	Chaser(Prey *p);

	~Chaser(void);

	virtual int updateState(int time); 	// update the object state
	virtual int processKeys(unsigned char key); // responds to a key stroke
	virtual int processKeys(unsigned char *key, int numKeys); // responds to  seq. of key strokes
	void collisionReaction();
	//1. this is the all inclusive movement of the chaser
	void chase(int time);

private:
//method	
	//2. the movement of the chaser seeing the prey
	void moveDirectlyToPosition(Vector3f pos);

	//3. the pattern of the chaser has not seen the prey
	void wander(int time);
	
	//helper methtod
	Vector3f getPreyPosition();

	//helper method
	bool seePrey();

//field
	//directions
	bool left;
	bool right;

	Prey *prey;
	Vector3f lastSeenPosition; 


};

