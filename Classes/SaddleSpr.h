#ifndef _SADDLE_SPR_H_
#define _SADDLE_SPR_H_

#include "cocos2d.h"
USING_NS_CC;

#include "SaddleStruct.h"

class SaddleSpr : public Layer {
private:
	SaddleSpr(SaddleViewStruct _saddleViewStruct);
public:
	static SaddleSpr *create(SaddleViewStruct _saddleViewStruct);

	bool init() override;

	Vec2 getSprDirection(Vec2 location);
	Vec2 getSprDirection();


	float getSprStretch();
	Vec2 getSprPosition();

	void setSprPosition(Vec2 position);

	bool canSetSprPosition(Vec2 position);
	
	Vec2 getSprRestPosition();
private:

	const SaddleViewStruct saddleStruct;
private:
	std::vector<DrawNode *> saddleLeftBalls;
	std::vector<DrawNode *> saddleRightBalls;

	Sprite *spr;

	float getSaddleRotationForPosition(Vec2 location);
	float getSaddleRotationForPosition();

	void updateSaddleLeftRightBallsPosition();
};


#endif //_SADDLE_SPR_H_