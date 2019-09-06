#include "LevelButton.h"
#include "Global.h"



#include "LevelButtonNew.h"
#include "LevelButtonDesign.h"
#include "LevelButtonGameUnlock.h"
#include "LevelButtonGameLock.h"


LevelButton::LevelButton(int _levelNumber, std::string _levelName, LevelButtonType _levelButtonType)
	:levelNumber(_levelNumber), levelName(_levelName), levelButtonType(_levelButtonType), isPressed(false), deleteButtonOn(false){

}

LevelButton *LevelButton::create(int _levelNumber, std::string _levelName, LevelButtonType _levelButtonType) {
	
	LevelButton *levelButton;
	switch (_levelButtonType){
	case LevelButtonType::DESIGN_NEW:
		levelButton = LevelButtonNew::create();
		break;
	case LevelButtonType::DESIGN:
		levelButton = LevelButtonDesign::create(_levelName);
		break;
	case LevelButtonType::GAME_UNLOCK:
		levelButton = LevelButtonGameUnlock::create(_levelNumber, _levelName);
		break;
	case LevelButtonType::GAME_LOCK:
		levelButton = LevelButtonGameLock::create();
		break;
	}
	
	return levelButton;
}


void LevelButton::setIsPressed(bool _isPressed) {
	isPressed = _isPressed;
}

bool LevelButton::getIsPressed() {
	return isPressed;
}


Rect LevelButton::getButtonRect(){
	Rect ret;
	ret.size.width = getButtonWidth();
	ret.size.height = getButtonHeight();
	ret.origin = this->getPosition() + Vec2(-ret.size.width/2.0f, -ret.size.height/2.0f);


	return ret;

}

void LevelButton::setDeleteButtonOn(bool _deleteButtonOn){
	deleteButtonOn = _deleteButtonOn;
}