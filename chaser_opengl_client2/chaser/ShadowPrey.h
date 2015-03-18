#pragma once
#include "Prey.h"
class ShadowPrey :
	public Car
{
private: Prey* currentPrey;
		 Prey* oldPrey;
		 double lastTime;
		 double currentTime;

public:
	ShadowPrey();
	ShadowPrey(Prey *prey);
	int updateState(int time);



	~ShadowPrey();
};

