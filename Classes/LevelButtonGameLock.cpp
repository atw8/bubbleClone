#include "LevelButtonGameLock.h"
#include "Global.h"


LevelButtonGameLock::LevelButtonGameLock():LevelButton(0, "", LevelButtonType::GAME_LOCK){

}

LevelButtonGameLock *LevelButtonGameLock::create(){
	LevelButtonGameLock *levelButtonGameLock = new LevelButtonGameLock();
	if (levelButtonGameLock && levelButtonGameLock->init()){
		levelButtonGameLock->autorelease();
	}
	else {
		delete levelButtonGameLock;
		levelButtonGameLock = nullptr;
	}

	return levelButtonGameLock;
}

void LevelButtonGameLock::onEnter(){
	LevelButton::onEnter();

	spr = Sprite::create("levelLayer/button_locked.png");
	spr->setScale(LEVEL_BUTTON_NOT_PRESS_SCALE);
	this->addChild(spr);
}

void LevelButtonGameLock::setIsPressed(bool _isPressed){
	LevelButton::setIsPressed(_isPressed);
}
float LevelButtonGameLock::getButtonWidth(){
	return getContWidth(spr)*LEVEL_BUTTON_NOT_PRESS_SCALE;
}
float LevelButtonGameLock::getButtonHeight(){
	return getContHeight(spr)*LEVEL_BUTTON_NOT_PRESS_SCALE;
}