#include "SaddleView.h"
#include "Global.h"
#include <random>

#include "RotatePivotTo.h"


#define Z_NOT_FRONT_SHOOT_BUBBLES -2
#define Z_FRONT_SHOOT_BUBBLES -1
#define Z_SADDLE 0 

#include "Constants.h"

#define SCHEDULE_MOVE_SADDLE_TO_REST_STATE "SCHEDULE_MOVE_SADDLE_TO_REST_STATE"
#define MOVE_SADDLE_TO_REST_STATE_SPEED 1000.0f

#define ACTION_MOVE_SADDLE 12312312





SaddleView::SaddleView(SaddleViewStruct _saddleViewStruct)
:saddleStruct(_saddleViewStruct){

}

SaddleView *SaddleView::create(SaddleViewStruct _saddleViewStruct){
	SaddleView *saddleView = new SaddleView(_saddleViewStruct);
	if (saddleView && saddleView->init()){
		saddleView->autorelease();
	}
	else {
		delete saddleView;
		saddleView = nullptr;
	}

	return saddleView;
}


void SaddleView::onEnter(){
	Layer::onEnter();


	saddleNode = SaddleSpr::create(saddleStruct);
	this->addChild(saddleNode, Z_SADDLE);
}




void SaddleView::setSaddleNodePosition(Vec2 location){
	stopMoveSaddleToRestState();

	saddleNode->setSprPosition(location);

	if (!shootBubbles.empty()){
		ShootBubble *bubble0 = shootBubbles.front();
		bubble0->stopActionByTag(ACTION_MOVE_SADDLE);
		bubble0->setPosition(convertPolarToCartesian(getShootBubblePolarPosition(0)));
	}
}

void SaddleView::setSaddleNodePositionRest(){
	startMoveSaddleToRestState();
}


bool SaddleView::canSetSaddleNodePosition(Vec2 location){
	return saddleNode->canSetSprPosition(location);
}





void SaddleView::pushBubbleFront(ShootBubble * shootBubble){
	pushBubbleFront(shootBubble, saddleStruct.reserveBubblesCenter);
}
void SaddleView::pushBubbleFront(ShootBubble *shootBubble, Vec2 location){
	if (!shootBubbles.empty()){
		ShootBubble *bubble0 = shootBubbles.front();
		bubble0->setLocalZOrder(Z_NOT_FRONT_SHOOT_BUBBLES);
	}
	shootBubble->setPosition(location);
	this->addChild(shootBubble, Z_FRONT_SHOOT_BUBBLES);


	shootBubbles.push_front(shootBubble);
	moveShootBubblesToDefaultPosition();
}	



void SaddleView::pushBubbleBack(ShootBubble *shootBubble){
	pushBubbleBack(shootBubble, saddleStruct.reserveBubblesCenter);
}
void SaddleView::pushBubbleBack(ShootBubble *shootBubble, Vec2 location){
	int zOrder;
	if (shootBubbles.empty()){
		zOrder = Z_FRONT_SHOOT_BUBBLES;
	}
	else {
		zOrder = Z_NOT_FRONT_SHOOT_BUBBLES;
	}

	shootBubble->setPosition(location);
	this->addChild(shootBubble, zOrder);


	shootBubbles.push_back(shootBubble);
	moveShootBubblesToDefaultPosition();
}



ShootBubble *SaddleView::popBubbleFront(){
	ShootBubble *bubble0 = shootBubbles.front();
	bubble0->stopActionByTag(ACTION_MOVE_SADDLE);
	bubble0->setPosition(convertPolarToCartesian(getShootBubblePolarPosition(0)));

	populateBubbleWithData(bubble0);



	shootBubbles.pop_front();


	

	moveShootBubblesToDefaultPosition();

	return bubble0;
}
ShootBubble *SaddleView::popBubbleBack(){
	ShootBubble *bubbleBack = shootBubbles.back();
	shootBubbles.pop_back();

	moveShootBubblesToDefaultPosition();

	return bubbleBack;
}




void SaddleView::populateBubbleWithData(ShootBubble *bubble){
	Vec2 sprDirection = saddleNode->getSprDirection();
	float sprStretch = saddleNode->getSprStretch();

	float speed = SHOOT_BUBBLE_MIN_VELOCITY + (SHOOT_BUBBLE_MAX_VELOCITY - SHOOT_BUBBLE_MIN_VELOCITY)*sprStretch;
	float shootBubbleRatio = SHOOT_BUBBLE_MIN_RADIUS + (SHOOT_BUBBLE_MAX_RADIUS - SHOOT_BUBBLE_MIN_RADIUS)*(1.0f - sprStretch);

	bubble->setShootBubbleRadiusRelativeToInnerRadius(shootBubbleRatio);
	bubble->setVelocity(sprDirection * speed);
}





void SaddleView::rotateBubbles(){
	if (shootBubbles.empty()){
		return;
	}

	ShootBubble *bubble0 = shootBubbles.front();
	shootBubbles.pop_front();

	shootBubbles.push_back(bubble0);

	moveShootBubblesToDefaultPosition();
	
}


int SaddleView::getNumOfShootBubbles(){
	return shootBubbles.size();
}

void SaddleView::moveShootBubblesToDefaultPosition(){
	int index = 0;
	for (ShootBubble *bubble : shootBubbles){
		Vec2 bubbleOldCartesian = bubble->getPosition();
		std::pair<float, float> bubbleOldPolar = convertCartesianToPolar(bubbleOldCartesian);
		
		float oldR = bubbleOldPolar.first;
		float oldAngle = bubbleOldPolar.second;
		
		std::pair<float, float> bubbleNewPolar = getShootBubbleDefaultPolarPosition(index);

		float newR = bubbleNewPolar.first;
		float newAngle = bubbleNewPolar.second;

		float rDiff = std::abs(oldR - newR);
		
		float angleDiff;
		RotatePivotToType rotatePivotToType;
		{
			float angleDiffCW = RotatePivotTo::getAngleDiff(oldAngle, newAngle, RotatePivotToType::CW);
			float angleDiffCCW = RotatePivotTo::getAngleDiff(oldAngle, newAngle, RotatePivotToType::CCW);
			if (angleDiffCW < angleDiffCCW){
				angleDiff = angleDiffCW;
				rotatePivotToType = RotatePivotToType::CW;
			}
			else {
				angleDiff = angleDiffCCW;
				rotatePivotToType = RotatePivotToType::CCW;
			}
		}
		
		
		if (oldR == 0.0f){
			oldAngle = newAngle;
		}
		
		const float TIME_DIST_CONST = 500.0f;

		Vector<FiniteTimeAction *> seqVector;
		if (newR < oldR){
			float timeMove = rDiff / TIME_DIST_CONST;
			MoveTo *moveTo = MoveTo::create(timeMove, convertPolarToCartesian(std::make_pair(newR, oldAngle)));

			float timeRotate = (angleDiff*newR) / TIME_DIST_CONST;
			RotatePivotTo *rotatePivotTo = RotatePivotTo::create(timeRotate, newAngle, newR, saddleStruct.reserveBubblesCenter, rotatePivotToType);

			seqVector.pushBack(moveTo);
			seqVector.pushBack(rotatePivotTo);
		}
		else
		{
			float timeRotate = (angleDiff * oldR) / TIME_DIST_CONST;
			RotatePivotTo *rotatePivotTo = RotatePivotTo::create(timeRotate, newAngle, oldR, saddleStruct.reserveBubblesCenter, rotatePivotToType);

			float timeMove = rDiff / TIME_DIST_CONST;
			MoveTo *moveTo = MoveTo::create(timeMove, convertPolarToCartesian(bubbleNewPolar));

			seqVector.pushBack(rotatePivotTo);
			seqVector.pushBack(moveTo);
		}

		Sequence *seq = Sequence::create(seqVector);
		seq->setTag(ACTION_MOVE_SADDLE);

		bubble->stopActionByTag(ACTION_MOVE_SADDLE);
		bubble->runAction(seq);

		index++;
	}
}



std::pair<float, float> SaddleView::getShootBubbleDefaultPolarPosition(int index){

	std::pair<float, float> ret;

	Vec2 bubble0Carteesian = saddleNode->getSprRestPosition() - (saddleStruct.innerRadius*0.5f)* saddleStruct.gravity;
	
	if (index == 0){
		ret = convertCartesianToPolar(bubble0Carteesian);
	}
	else {
		
		float R;
		{
			float cosAngle = std::cos(2.0f*M_PI / (shootBubbles.size()));

			R = (std::sqrt(2)*saddleStruct.innerRadius) / std::sqrt(1.0f - cosAngle);
			R = std::max(saddleStruct.minRotateCircleRadius + saddleStruct.innerRadius, R);
		}

		float angle;
		{
			float bubble0Angle = convertCartesianToPolar(bubble0Carteesian).second;
			angle = (2.0f*M_PI / shootBubbles.size()) * index + bubble0Angle;
			while (angle >= 2.0f*M_PI){
				angle -= 2.0f*M_PI;
			}

			while (angle < 0.0f){
				angle += 2.0f*M_PI;
			}
		}

		ret = std::make_pair(R, angle);
	}

	return ret;
}
std::pair<float, float> SaddleView::getShootBubblePolarPosition(int index){
	std::pair<float, float> ret;
	
	if (index == 0){
		Vec2 bubble0Carteesian;
		{
			Vec2 sprDirection = saddleNode->getSprDirection();
			bubble0Carteesian = saddleNode->getSprPosition() + (saddleStruct.innerRadius*0.5f)*sprDirection;
		}

		ret = convertCartesianToPolar(bubble0Carteesian);
	}
	else {
		ret = getShootBubbleDefaultPolarPosition(index);
	}

	return ret;
}

Vec2 SaddleView::convertPolarToCartesian(std::pair<float, float> polarCoord){
	float r = polarCoord.first;
	float angle = polarCoord.second;
	
	Vec2 ret;
	ret = saddleStruct.reserveBubblesCenter + r * Vec2(std::cos(angle), std::sin(angle));

	return ret;
}

std::pair<float, float> SaddleView::convertCartesianToPolar(Vec2 carteesianCoord){
	float r = (carteesianCoord - saddleStruct.reserveBubblesCenter).getLength();
	float angle = (carteesianCoord - saddleStruct.reserveBubblesCenter).getAngle();

	return std::make_pair(r, angle);
}






void SaddleView::startMoveSaddleToRestState(){
	if (isMoveSaddleToRestState()){
		return;
	}

	this->schedule(std::bind(&SaddleView::updateMoveSaddletoRestState, this, std::placeholders::_1), SCHEDULE_MOVE_SADDLE_TO_REST_STATE);
}

void SaddleView::stopMoveSaddleToRestState(){
	if (!isMoveSaddleToRestState()){
		return;
	}

	this->unschedule(SCHEDULE_MOVE_SADDLE_TO_REST_STATE);
}


bool SaddleView::isMoveSaddleToRestState(){
	return this->isScheduled(SCHEDULE_MOVE_SADDLE_TO_REST_STATE);
}

void SaddleView::updateMoveSaddletoRestState(float dt){
	Vec2 saddleNodeRestPos = saddleNode->getSprRestPosition();
	Vec2 saddleNodePos = saddleNode->getSprPosition();

	Vec2 saddleVec = saddleNodeRestPos - saddleNodePos;

	
	Vec2 displacement = saddleVec.getNormalized()*MOVE_SADDLE_TO_REST_STATE_SPEED*dt;

	if (displacement.length() >= saddleVec.length()){
		saddleNodePos = saddleNodeRestPos;
		stopMoveSaddleToRestState();
	}
	else {
		saddleNodePos += displacement;
	}

	saddleNode->setSprPosition(saddleNodePos);
	if (!shootBubbles.empty()){
		ShootBubble *bubble0 = shootBubbles.front();
		if (nullptr == bubble0->getActionByTag(ACTION_MOVE_SADDLE)) {
			bubble0->setPosition(convertPolarToCartesian(getShootBubblePolarPosition(0)));
		}
	}
}

void SaddleView::updateActiveBasicBubbleTypes(const std::vector<BubbleType> &activeBasicBubbleTypes){

	if (activeBasicBubbleTypes.empty()){
		for (std::list<ShootBubble *>::iterator shootBubblesIter = shootBubbles.begin(); shootBubblesIter != shootBubbles.end();){
			ShootBubble *shootBubble = *shootBubblesIter;

			BubbleType bubbleType = shootBubble->getBubble().getBubbleType();
			if (bubbleType >= BubbleType::FIRST_BASIC_BUBBLE_TYPE && bubbleType <= BubbleType::LAST_BASIC_BUBBLE_TYPE){
				shootBubble->removeFromParentAndCleanup(true);
				shootBubblesIter = shootBubbles.erase(shootBubblesIter);
			}
			else {
				shootBubblesIter++;
			}
		}

		moveShootBubblesToDefaultPosition();
	}
	else {
		for (ShootBubble *shootBubble : shootBubbles){
			BubbleType bubbleType = shootBubble->getBubble().getBubbleType();
			if (bubbleType >= BubbleType::FIRST_BASIC_BUBBLE_TYPE && bubbleType <= BubbleType::LAST_BASIC_BUBBLE_TYPE){
				std::vector<BubbleType>::const_iterator activeBasicBubbleTypesIter = std::find(activeBasicBubbleTypes.begin(), activeBasicBubbleTypes.end(), bubbleType);

				if (activeBasicBubbleTypesIter == activeBasicBubbleTypes.end()){
					std::vector<BubbleType>::const_iterator newBubbleTypeIter = activeBasicBubbleTypes.begin();
					std::advance(newBubbleTypeIter, rand() % activeBasicBubbleTypes.size());

					BubbleType newBubbleType = *newBubbleTypeIter;
					Bubble newBubble(newBubbleType);

					shootBubble->setBubble(newBubble);
				}
			}
		}
	}
	
}




ShootBubble *SaddleView::peekBubbleFront(){
	ShootBubble *ret;
	if (shootBubbles.empty()){
		ret = nullptr;
	}
	else {
		ret = shootBubbles.front();
		
		ret->stopActionByTag(ACTION_MOVE_SADDLE);
		ret->setPosition(convertPolarToCartesian(getShootBubblePolarPosition(0)));

		populateBubbleWithData(ret);
	}

	return ret;
}
ShootBubble *SaddleView::peekBubbleBack(){
	ShootBubble *ret;
	if (shootBubbles.empty()){
		ret = nullptr;
	}
	else {
		ret = shootBubbles.back();
	}

	return ret;
}


