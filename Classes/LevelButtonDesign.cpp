#include "LevelButtonDesign.h"
#include "Global.h"

#include "LevelLayerGlobal.h"

#define LEVEL_BUTTON_DELETE_PRESS_SCALE 0.8f
#define LEVEL_BUTTON_DELETE_NOT_PRESS_SCALE 1.0f

LevelButtonDesign::LevelButtonDesign(std::string _levelName) :LevelButton(0, _levelName, LevelButtonType::DESIGN), xScale(2.5f){

}
LevelButtonDesign *LevelButtonDesign::create(std::string _levelName){
	LevelButtonDesign *levelButtonDesign = new LevelButtonDesign(_levelName);
	
	if (levelButtonDesign && levelButtonDesign->init()){
		levelButtonDesign->autorelease();
	}
	else {
		delete levelButtonDesign;
		levelButtonDesign = nullptr;
	}

	return levelButtonDesign;
}
void LevelButtonDesign::onEnter(){
	LevelButton::onEnter();

	spr = Sprite::create("levelLayer/button_design.png");
	this->addChild(spr);

	spr->setScaleX(xScale*LEVEL_BUTTON_NOT_PRESS_SCALE);
	spr->setScaleY(LEVEL_BUTTON_NOT_PRESS_SCALE);


	Label *nameLabel = Label::createWithSystemFont('"' + levelName + '"', "Arial", 30);
	nameLabel->setPosition(Vec2(getContWidth(this) / 2.0f, getContHeight(this) / 2.0f));
	nameLabel->setColor(Color3B::BLACK);
	nameLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(nameLabel);


	delSpr = Sprite::create("levelLayer/delete.png");
	delSpr->setPosition(Vec2(xScale*LEVEL_BUTTON_NOT_PRESS_SCALE*getContWidth(delSpr), getContHeight(delSpr)) + Vec2(-20.0f, -35.0f));
	delSpr->setScale(LEVEL_BUTTON_DELETE_NOT_PRESS_SCALE);
	this->addChild(delSpr);

	if (deleteButtonOn){
		delSpr->setOpacity(255);
	}
	else {
		delSpr->setOpacity(0);
	}
	
	nameLabel->setScale(LEVEL_BUTTON_NOT_PRESS_SCALE);
}

float LevelButtonDesign::getButtonWidth(){
	return xScale*getContWidth(spr)*LEVEL_BUTTON_NOT_PRESS_SCALE;
}
float LevelButtonDesign::getButtonHeight(){
	return getContHeight(spr)*LEVEL_BUTTON_NOT_PRESS_SCALE;
}

void LevelButtonDesign::setIsPressed(bool _isPressed){
	LevelButton::setIsPressed(_isPressed);

	
	if (deleteButtonOn){
		if (isPressed){
			delSpr->setScale(LEVEL_BUTTON_DELETE_PRESS_SCALE);
		}
		else {
			delSpr->setScale(LEVEL_BUTTON_DELETE_NOT_PRESS_SCALE);
		}
	}
	else {
		if (isPressed){
			spr->setScaleX(xScale*LEVEL_BUTTON_PRESS_SCALE);
			spr->setScaleY(LEVEL_BUTTON_PRESS_SCALE);
		}
		else {
			spr->setScaleX(xScale*LEVEL_BUTTON_NOT_PRESS_SCALE);
			spr->setScaleY(LEVEL_BUTTON_NOT_PRESS_SCALE);
		}
	}


}

void LevelButtonDesign::setDeleteButtonOn(bool _deleteButtonOn){
	LevelButton::setDeleteButtonOn(_deleteButtonOn);

	delSpr->stopAllActions();
	if (deleteButtonOn){
		delSpr->runAction(FadeIn::create(TRANSITION_TIME));
	}
	else {
		delSpr->runAction(FadeOut::create(TRANSITION_TIME));
	}

}