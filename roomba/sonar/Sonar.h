#ifndef SONAR_H
#define SONAR_H

#include <stdio.h>
#include <time.h>
#include "Gpio.h"

// time in usec before next measurement
#define SONAR_MEASURE_RATE 100000
// time in usec before switch to other sonar
#define SONAR_WAIT_TIME	10000

class Sonar
{
public:
	Sonar(unsigned int gpioPinNum);
	~Sonar();
	
	float Run();
	bool isEnding;
	
private:
	Gpio * _gpio;
	float _minDist;
	float _maxDist;
	struct timespec _risingTOI;
	struct timespec _fallingTOI;
	
	struct timespec TimeDiff();
	float DisplayMeasurement();
	void StartPulse();
	
};

#endif
