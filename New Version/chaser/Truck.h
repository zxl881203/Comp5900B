#pragma once
#include "Prey.h"
class Truck :
	public Car
{
private: Prey* currentPrey;
		 Prey* oldPrey;
		 double lastTime;
		 double currentTime;

public:
	Truck();
	Truck(Prey *prey);
	int updateState(int time);



	~Truck();
};

