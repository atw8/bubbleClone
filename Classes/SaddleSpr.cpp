#include "SaddleSpr.h"
#include "Global.h"


SaddleSpr::SaddleSpr(SaddleViewStruct _saddleViewStruct)
:saddleStruct(_saddleViewStruct){

}

SaddleSpr *SaddleSpr::create(SaddleViewStruct _saddleViewStruct){
	SaddleSpr *saddleSpr = new SaddleSpr(_saddleViewStruct);

	if (saddleSpr && saddleSpr->init()){
		saddleSpr->autorelease();
	}
	else {
		delete saddleSpr;
		saddleSpr = nullptr;
	}

	return saddleSpr;
}

bool SaddleSpr::init(){
	if (!Layer::init()){
		return false;
	}

	spr = Sprite::create("balls/saddle.png");
	spr->setScale((saddleStruct.innerRadius / (getContWidth(spr) / 2.0f)) *1.1f);
	this->addChild(spr);


	const float minSaddleBallRadius = 3.0f;
	const float maxSaddleBallRadius = 10.0f;

	for (int i = 0; i < saddleStruct.numOfSaddleBalls; i++){
		float saddleBallRadius = minSaddleBallRadius + (maxSaddleBallRadius - minSaddleBallRadius)*(i / (saddleStruct.numOfSaddleBalls - 1.0f));

		DrawNode *saddleLeftBall = DrawNode::create();
		saddleLeftBall->drawCircle(Vec2(0.0f, 0.0f), saddleBallRadius, 0, 20, false, Color4F::WHITE);
		this->addChild(saddleLeftBall, 1);

		DrawNode *saddleRightBall = DrawNode::create();
		saddleRightBall->drawCircle(Vec2(0.0f, 0.0f), saddleBallRadius, 0, 20, false, Color4F::WHITE);
		this->addChild(saddleRightBall, 1);


		saddleLeftBalls.push_back(saddleLeftBall);
		saddleRightBalls.push_back(saddleRightBall);
	}


	setSprPosition(getSprRestPosition());
	return true;
}

float SaddleSpr::getSprStretch(){
	float stretch = (saddleStruct.midCoord - spr->getPosition()).length();
	stretch -= saddleStruct.minRadius;

	return stretch / (saddleStruct.maxRadius - saddleStruct.minRadius);
}

Vec2 SaddleSpr::getSprDirection(){
	return getSprDirection(spr->getPosition());
}

Vec2 SaddleSpr::getSprDirection(Vec2 location){
	return (saddleStruct.midCoord - location).getNormalized();
}


void SaddleSpr::setSprPosition(Vec2 position){
	//if (!canSetSprPosition(position)){
	//	return;
	//}

	//Normalize the position
	Vec2 sprDirection = getSprDirection(position);
	if (position.distance(saddleStruct.midCoord) > saddleStruct.maxRadius){
		position = saddleStruct.midCoord + (-sprDirection)*saddleStruct.maxRadius;
	}
	else if (position.distance(saddleStruct.midCoord) < saddleStruct.minRadius){
		position = saddleStruct.midCoord + (-sprDirection)*saddleStruct.minRadius;
	}



	float rotation = CC_RADIANS_TO_DEGREES(getSaddleRotationForPosition(position));
	if (saddleStruct.hasMinSaddleRotation && rotation < saddleStruct.minSaddleRotation){
		float rotationRadians = CC_DEGREES_TO_RADIANS(saddleStruct.minSaddleRotation);
		float gravityAngle = saddleStruct.gravity.getAngle();
		position = saddleStruct.midCoord - (position.distance(saddleStruct.midCoord))*Vec2(std::cos(rotationRadians - gravityAngle), std::sin(rotationRadians - gravityAngle));
	}
	else if (saddleStruct.hasMaxSaddleRotation && rotation > saddleStruct.maxSaddleRotation){
		float rotationRadians = CC_DEGREES_TO_RADIANS(saddleStruct.maxSaddleRotation);
		float gravityAngle = saddleStruct.gravity.getAngle();
		position = saddleStruct.midCoord - (position.distance(saddleStruct.midCoord))*Vec2(std::cos(rotationRadians - gravityAngle), std::sin(rotationRadians - gravityAngle));
	}


	spr->setPosition(position);
	spr->setRotation(-CC_RADIANS_TO_DEGREES(getSaddleRotationForPosition() - saddleStruct.gravity.getAngle()) + 90.0f);

	updateSaddleLeftRightBallsPosition();
}


bool SaddleSpr::canSetSprPosition(Vec2 position){
	bool ret = true;

	Vec2 sprDirection = getSprDirection(position);
	//if (position.distance(saddleStruct.midCoord) > saddleStruct.maxRadius){
	//	ret = false;
	//}
	if (position.distance(saddleStruct.midCoord) < saddleStruct.minRadius){
		ret = false;
	}



	float rotation = CC_RADIANS_TO_DEGREES(getSaddleRotationForPosition(position));
	if (saddleStruct.hasMinSaddleRotation && rotation < saddleStruct.minSaddleRotation){
		ret = false;
	}
	else if (saddleStruct.hasMaxSaddleRotation && rotation > saddleStruct.maxSaddleRotation){
		ret = false;
	}


	return ret;
}

Vec2 SaddleSpr::getSprPosition(){
	return spr->getPosition();
}
/*
Vec2 SaddleSpr::getSprPosition(){
	return spr->getPosition();
}
*/


float SaddleSpr::getSaddleRotationForPosition(Vec2 location){
	float angle = (location - saddleStruct.midCoord).getAngle() -saddleStruct.gravity.getAngle();

	while (angle > M_PI){
		angle -= 2.0f*M_PI;
	}

	while (angle < -M_PI){
		angle += 2.0f*M_PI;
	}	

	return angle;

}

float SaddleSpr::getSaddleRotationForPosition(){
	return getSaddleRotationForPosition(spr->getPosition());
}



Vec2 SaddleSpr::getSprRestPosition(){
	return saddleStruct.midCoord + saddleStruct.gravity * saddleStruct.minRadius;
}

void SaddleSpr::updateSaddleLeftRightBallsPosition(){
	Vec2 leftStart = saddleStruct.leftCoord;
	Vec2 leftFinish = PointApplyTransform(Vec2(0.0f, getContHeight(spr)), spr->getNodeToParentTransform());

	Vec2 rightStart = saddleStruct.rightCoord;
	Vec2 rightFinish = PointApplyTransform(Vec2(getContWidth(spr), getContHeight(spr)), spr->getNodeToParentTransform());

	for (int i = 0; i < saddleStruct.numOfSaddleBalls; i++){
		Vec2 left = leftStart + (leftFinish - leftStart)*(i / (saddleStruct.numOfSaddleBalls - 1.0f));
		Vec2 right = rightStart + (rightFinish - rightStart)*(i / (saddleStruct.numOfSaddleBalls - 1.0f));

		saddleLeftBalls[i]->setPosition(left);
		saddleRightBalls[i]->setPosition(right);
	}
}

