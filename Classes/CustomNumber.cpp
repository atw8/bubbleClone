#include "CustomNumber.h"

#include "Global.h"


CustomNumber::CustomNumber(std::function<std::string(std::string)> _normalizeStrFunc, std::function<void(std::string)> _setStringCallback): isPressed(false), 
normalizeStrFunc(_normalizeStrFunc), setStringCallback(_setStringCallback){
}

CustomNumber::~CustomNumber(){

}

CustomNumber* CustomNumber::createWithPlace(const Size& dimensions, float fontSize, std::function<std::string(std::string)> _normalizeStrFunc, std::function<void(std::string)> _setStringCallback){
	CustomNumber *ret = new (std::nothrow) CustomNumber(_normalizeStrFunc, _setStringCallback);
	if (ret && ret->initWithPlaceHolder("...", dimensions, TextHAlignment::CENTER, "Arial", fontSize)){
		ret->setColor(Color3B::BLACK);
		ret->autorelease();
	}
	else {
		delete ret;
		ret = nullptr;
	}


	return ret;
}


void CustomNumber::onEnter(){
	TextFieldTTF::onEnter();

	isSelectedDrawNode = DrawNode::create();
	this->addChild(isSelectedDrawNode);

	drawIsSelectedDrawNode();
}

void CustomNumber::setString(const std::string& text){
	std::string newText = normalizeStrFunc(text);

	TextFieldTTF::setString(newText);

	if (nullptr != setStringCallback){
		setStringCallback(newText);
	}
	
}


void CustomNumber::drawIsSelectedDrawNode(){
	Vec2 origin = Vec2(0.0f, 0.0f);
	Vec2 destination = Vec2(getContWidth(this), getContHeight(this));

	Color4F color;
	if (isPressed){
		color = Color4F::GREEN;
	}
	else {
		color = Color4F::RED;
	}

	isSelectedDrawNode->clear();
	isSelectedDrawNode->drawRect(origin, destination, color);
}

bool CustomNumber::getIsPressed(){
	return isPressed;
}

void CustomNumber::setIsPressed(bool _isPressed){
	if (isPressed == _isPressed){
		return;
	}

	isPressed = _isPressed;

	drawIsSelectedDrawNode();
}

