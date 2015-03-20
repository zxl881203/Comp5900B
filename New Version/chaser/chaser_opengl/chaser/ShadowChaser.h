#pragma once
#include "chaser.h"
class ShadowChaser :
	public Car
{

private: Chaser* currentChaser;
		 Chaser* oldChaser;
		 double lastTime;
		 double currentTime;

public:
	ShadowChaser();
	ShadowChaser(Chaser *chaser);
	int updateState(int time);
	~ShadowChaser();
};

