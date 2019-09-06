#include "PropBtn.h"
#include "Global.h"

PropBtn::PropBtn(PropType _propType, GameModeType _gameModeType, int _cost):propType(_propType), gameModeType(_gameModeType), cost(_cost){

}

PropBtn* PropBtn::create(PropType _propType, GameModeType _gameModeType, int _cost){
	PropBtn *propBtn = new PropBtn(_propType, _gameModeType, _cost);

	if (propBtn && propBtn->initWithFile("balls/propBack.png")){
		propBtn->autorelease();
	}
	else {
		delete propBtn;
		propBtn = nullptr;
	}

	return propBtn;
}

void PropBtn::onEnter(){
	Sprite::onEnter();


	std::string propBackInnerStr;
	switch (propType){
	case PropType::ADD_RESERVE:
		propBackInnerStr = "balls/prop_magicWand.png";
		break;
	case PropType::ADD_TIMEMOVE:
		switch (gameModeType){
		case GameModeType::MOVE:
			propBackInnerStr = "balls/prop_addMove.png";
			break;
		case GameModeType::TIME:
			propBackInnerStr = "balls/prop_addTime.png";
			break;
		}
		break;
	case PropType::CHROME:
		propBackInnerStr = "balls/pop_chrome.png";
		break;
	}

	Sprite *propBackInner = Sprite::create(propBackInnerStr);
	propBackInner->setPosition(Vec2(getContWidth(this) / 2.0f, getContHeight(this) / 2.0f));
	this->addChild(propBackInner);


	Label *costLabel = Label::createWithCharMap("numbers/number1.png", 34, 34, '0');
	costLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	costLabel->setString(to_string(cost));
	costLabel->setPosition(Vec2(getContWidth(this) / 2.0f, 20.0f));
	this->addChild(costLabel);

}
