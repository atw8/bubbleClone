#ifndef _SADDLE_STRUCT_H_
#define _SADDLE_STRUCT_H_

#include "cocos2d.h"
USING_NS_CC;


#define MOVE_BUBBLE_ROTATE_WHEEL 21312
#define MOVE_BUBBLE_ROTATE_WHEEL_TIME 0.5f

class SaddleViewStruct {
public:
	Vec2 leftCoord;
	Vec2 rightCoord;
	Vec2 midCoord;

	float minRadius;
	float maxRadius;
	Vec2 gravity;

	int numOfSaddleBalls;

	bool hasMinSaddleRotation;
	float minSaddleRotation;

	bool hasMaxSaddleRotation;
	float maxSaddleRotation;

	float innerRadius;


	Vec2 reserveBubblesCenter;
	float minRotateCircleRadius;
};

#endif //_SADDLE_STRUCT_H_