#include "LevelButtonGameUnlock.h"
#include "Global.h"

LevelButtonGameUnlock::LevelButtonGameUnlock(int _levelNumber, std::string _levelName) :LevelButton(_levelNumber, _levelName, LevelButtonType::GAME_UNLOCK){

}
LevelButtonGameUnlock *LevelButtonGameUnlock::create(int _levelNumber, std::string _levelName){
	LevelButtonGameUnlock *levelButtonGameUnlock = new LevelButtonGameUnlock(_levelNumber, _levelName);
	if (levelButtonGameUnlock && levelButtonGameUnlock->init()){
		levelButtonGameUnlock->autorelease();
	}
	else {
		delete levelButtonGameUnlock;
		levelButtonGameUnlock = nullptr;
	}

	return levelButtonGameUnlock;
}

void LevelButtonGameUnlock::onEnter(){
	LevelButton::onEnter();


	spr = Sprite::create("levelLayer/button_design.png");
	this->addChild(spr);

	spr->setScale(LEVEL_BUTTON_NOT_PRESS_SCALE);



	Label *numberLabel = Label::createWithSystemFont(to_string(levelNumber), "Arial", 50);
	numberLabel->setPosition(Vec2(getContWidth(this) / 2.0f, getContHeight(this) / 2.0f + 10.0f));
	numberLabel->setColor(Color3B::WHITE);
	this->addChild(numberLabel);

	
	//Add the stars
	int numOfStarsUnlocked = random(0, 4);
	Sprite *star1 = Sprite::create(numOfStarsUnlocked > 0? "levelLayer/yellowStar_small.png" : "levelLayer/greyStar_small.png");
	Sprite *star2 = Sprite::create(numOfStarsUnlocked > 1? "levelLayer/yellowStar_small.png" : "levelLayer/greyStar_small.png");
	Sprite *star3 = Sprite::create(numOfStarsUnlocked > 2? "levelLayer/yellowStar_big.png" : "levelLayer/greyStar_big.png");


	star3->setPosition(Vec2(getContWidth(spr) / 2.0f, 20.0f));
	star3->setScale(1.5f);
	spr->addChild(star3);

	star1->setPosition(Vec2(getContWidth(spr) / 2.0f - 45.0f, 30.0f));
	star1->setScale(1.5f);
	spr->addChild(star1);

	star2->setPosition(Vec2(getContWidth(spr) / 2.0f + 45.0f, 30.0f));
	star2->setScale(1.5f);
	spr->addChild(star2);
}

float LevelButtonGameUnlock::getButtonWidth(){
	return getContWidth(spr)*LEVEL_BUTTON_NOT_PRESS_SCALE;
}
float LevelButtonGameUnlock::getButtonHeight(){
	return getContHeight(spr)*LEVEL_BUTTON_NOT_PRESS_SCALE;
}

void LevelButtonGameUnlock::setIsPressed(bool _isPressed){
	LevelButton::setIsPressed(_isPressed);

	float scale;
	if (deleteButtonOn){
		scale = LEVEL_BUTTON_NOT_PRESS_SCALE;
	}
	else {
		if (isPressed){
			scale = LEVEL_BUTTON_PRESS_SCALE;
		}
		else {
			scale = LEVEL_BUTTON_NOT_PRESS_SCALE;
		}
	}

	spr->setScale(scale);


}