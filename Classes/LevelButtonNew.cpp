#include "LevelButtonNew.h"
#include "Global.h"

LevelButtonNew::LevelButtonNew():LevelButton(0, "", LevelButtonType::DESIGN_NEW){

}
LevelButtonNew *LevelButtonNew::create(){
	LevelButtonNew *levelButtonNew = new LevelButtonNew();
	if (levelButtonNew && levelButtonNew->init()){
		levelButtonNew->autorelease();
	}
	else {
		delete levelButtonNew;
		levelButtonNew = nullptr;
	}

	return levelButtonNew;
}

void LevelButtonNew::onEnter(){
	LevelButton::onEnter();


	spr = Sprite::create("levelLayer/button_design_new.png");
	this->addChild(spr);

	spr->setScale(LEVEL_BUTTON_NOT_PRESS_SCALE);
	

}

float LevelButtonNew::getButtonWidth(){
	return getContWidth(spr)*LEVEL_BUTTON_NOT_PRESS_SCALE;
}
float LevelButtonNew::getButtonHeight(){
	return getContHeight(spr)*LEVEL_BUTTON_NOT_PRESS_SCALE;
}

void LevelButtonNew::setIsPressed(bool _isPressed){
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