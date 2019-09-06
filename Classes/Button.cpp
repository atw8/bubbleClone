#include "Button.h"

Button::Button(std::string _buttonStr, Size _buttonSize, float _fontSize) :buttonStr(_buttonStr), buttonSize(_buttonSize), fontSize(_fontSize),
isPressed(false), isSelected(false){

}

Button* Button::create(std::string _buttonStr, Size _buttonSize, float _fontSize){
	Button *button = new Button(_buttonStr, _buttonSize, _fontSize);
	if (button && button->init()){
		button->autorelease();
	}
	else {
		delete button;
		button = nullptr;
	}

	return button;
}

bool Button::init() {
	this->setContentSize(buttonSize);

	return true;
}

void Button::onEnter(){
	Node::onEnter();
	
	std::vector<Vec2> boxVec2s;
	boxVec2s.push_back(Vec2(-buttonSize.width / 2.0f, -buttonSize.height / 2.0f));
	boxVec2s.push_back(Vec2(buttonSize.width / 2.0f, -buttonSize.height/2.0f));
	boxVec2s.push_back(Vec2(buttonSize.width / 2.0f, buttonSize.height / 2.0f));
	boxVec2s.push_back(Vec2(-buttonSize.width / 2.0f, buttonSize.height / 2.0f));

	normalBox = DrawNode::create();
	normalBox->drawPoly(&boxVec2s[0], boxVec2s.size(), true, Color4F::BLACK);
	this->addChild(normalBox);

	selectBox = DrawNode::create();
	selectBox->drawPoly(&boxVec2s[0], boxVec2s.size(), true, Color4F::RED);
	this->addChild(selectBox, 1);

	selectBox->setVisible(isSelected);


	buttonLabel = Label::createWithSystemFont(buttonStr, "Arial", fontSize);
	this->addChild(buttonLabel);
}

void Button::setButtonStr(std::string _buttonStr){
	buttonStr = _buttonStr;

	buttonLabel->setString(buttonStr);
}
std::string Button::getButtonStr(){
	return buttonStr;
}

void Button::setIsPressed(bool _isPressed){
	if (isPressed == _isPressed){
		return;
	}
	isPressed = _isPressed;

	float scale;
	switch (isPressed){
	case true:
		scale = 0.9f;
		break;
	case false:
		scale = 1.0f;
		break;
	}

	this->setScale(scale);
}
bool Button::getIsPressed(){
	return isPressed;
}

Rect Button::getBoundingBox(){
	Rect ret;
	ret.size = buttonSize;
	ret.origin = Vec2(-buttonSize.width / 2.0f, -buttonSize.height / 2.0f);

	ret = RectApplyAffineTransform(ret, getNodeToParentAffineTransform());

	return ret;
}

void Button::setIsSelected(bool _isSelected){
	if (isSelected == _isSelected){
		return;
	}
	isSelected = _isSelected;

	selectBox->setVisible(isSelected);
}
bool Button::getIsSelected(){
	return isSelected;
}