#include "Clock.h"
#include "Global.h"

Clock::Clock(int _timeMove){
	timeMove = _timeMove;
}

Clock* Clock::create(int _timeMove){
	Clock *myClock = new Clock(_timeMove);
	if (myClock && myClock->initWithFile("balls/clock.png")){
		myClock->autorelease();
	}
	else {
		delete myClock;
		myClock = nullptr;
	}

	return myClock;
}

void Clock::onEnter(){
	Sprite::onEnter();

	timeMoveLabel = Label::createWithCharMap("numbers/numbers_moveTime.png", 23, 23, '0');
	timeMoveLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	timeMoveLabel->setPosition(Vec2(getContWidth(this)/2.0f, getContHeight(this)/2.0f) + Vec2(0.0f, 5.0f));
	timeMoveLabel->setString(to_string(timeMove));
	this->addChild(timeMoveLabel);
}





void Clock::setTimeMove(int _timeMove){
	if (timeMove == _timeMove){
		return;
	}

	timeMove = _timeMove;
	timeMoveLabel->setString(to_string(timeMove));
}

int Clock::getTimeMove(){
	return timeMove;
}