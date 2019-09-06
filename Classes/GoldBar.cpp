#include "GoldBar.h"
#include "Global.h"

#include "DDUserDefault.h"

GoldBar::GoldBar(bool _displayPlusSpr):displayPlusSpr(_displayPlusSpr), isPressed(false){

}
GoldBar* GoldBar::create(bool _displayPlusSpr){
	GoldBar *goldBar = new GoldBar(_displayPlusSpr);
	if (goldBar && goldBar->init()){
		goldBar->autorelease();
	}
	else {
		delete goldBar;
		goldBar = nullptr;
	}

	return goldBar;
}

bool GoldBar::init(){
	if (!Node::init()){
		return false;
	}



	goldBarBack = Sprite::create("balls/map_goldBack.png");
	this->addChild(goldBarBack);

	if (displayPlusSpr){
		goldBarPlus = Sprite::create("balls/map_goldPlus.png");
		goldBarPlus->setPosition(Vec2(getContWidth(goldBarBack) - getContWidth(goldBarPlus) / 2.0f, getContHeight(goldBarBack) / 2.0f));
		goldBarBack->addChild(goldBarPlus, 1);
	}
	else {
		goldBarPlus = nullptr;
	}
	

	goldBarText = Label::createWithCharMap("numbers/numbers_goldCoinMap.png", 20, 25, '0');
	goldBarText->setAnchorPoint(Vec2(0.0f, 0.5f));
	goldBarText->setPosition(Vec2(30.0f, getContHeight(goldBarBack)/2.0f));
	goldBarText->setScale(1.3f);
	goldBarBack->addChild(goldBarText, 1);



	return true;
}

void GoldBar::onEnter(){
	Node::onEnter();
	
	NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(GoldBar::setNumGoldNotification), NOTIFICATION_setNumOfGold, nullptr);
	setNumGoldNotification(nullptr);
}

void GoldBar::onExit(){
	Node::onExit();

	NotificationCenter::getInstance()->removeObserver(this, NOTIFICATION_setNumOfGold);
}

void GoldBar::setNumGoldNotification(Ref *ref){
	setText(DDUserDefault::getInstance()->getNumOfGold());
}

void GoldBar::setText(int text){
	setText(to_string(text));
}
void GoldBar::setText(std::string text){
	goldBarText->setString(text);
}
std::string GoldBar::getText(){
	return goldBarText->getString();
}


const Size &GoldBar::getContentSize() const {
	return goldBarBack->getContentSize();
}

Rect GoldBar::getBoundingBox() const {
	return RectApplyTransform(goldBarBack->getBoundingBox(), this->getNodeToParentTransform());
}


void GoldBar::setIsPressed(const bool _isPressed){
	if (_isPressed == isPressed){
		return;
	}
	isPressed = _isPressed;

	if (!displayPlusSpr){
		return;
	}


	float scale;
	if (isPressed){
		scale = 0.9f;
	}
	else {
		scale = 1.0f;
	}
	goldBarPlus->setScale(scale);
}
bool GoldBar::getIsPressed(){
	return isPressed;
}