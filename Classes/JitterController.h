#ifndef _JITTER_CONTROLLER_H_
#define _JITTER_CONTROLLER_H_

#include "cocos2d.h"
USING_NS_CC;

class JitterController {
public:
	JitterController(float _startTime, Vec2 _startDirection, float _magnitude, float _period, float _decayConstant, float _timeOffset);

	Vec2 getPosition(float time);
	Vec2 getVelocity(float time);

	bool isFinished(float time);
private:

	
	const float startTime;

	const Vec2 startDirection;
	const float magnitude;

	const float decayConstant;
	const float pediod;

	const float timeOffset;
public:
	static float getTimeOffset(float diff, float magnitude, float period);
};

#endif //_JITTER_CONTROLLER_H_