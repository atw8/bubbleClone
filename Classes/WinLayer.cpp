#include "WinLayer.h"
#include "Global.h"

WinLayer::WinLayer(std::function<void()> _returnLastInterface) :returnLastInterface(_returnLastInterface), main_touch(nullptr){

}

WinLayer* WinLayer::create(std::function<void()> _returnLastInterface){
	WinLayer *winLayer = new WinLayer(_returnLastInterface);

	if (winLayer && winLayer->init()){
		winLayer->autorelease();
	}
	else {
		delete winLayer;
		winLayer = nullptr;
	}

	return winLayer;
}
void WinLayer::onEnter(){
	LayerColor::onEnter();
	this->setColor(Color3B::BLACK);
	this->setOpacity(0);
	
	Sprite *winSign = Sprite::create("winLayer/winSign.png");
	winSign->setPosition(Vec2(getContWidth(this) / 2.0f, getContHeight(this) / 2.0f));
	this->addChild(winSign);

	winSign->setOpacity(0.0f);
	winSign->runAction(FadeIn::create(0.6f));

	this->runAction(FadeTo::create(0.3f, 255 * (2.0f / 3.0f)));



	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = std::bind(&WinLayer::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchMoved = std::bind(&WinLayer::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchEnded = std::bind(&WinLayer::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchCancelled = std::bind(&WinLayer::onTouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
	listener->setSwallowTouches(true);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
}


bool WinLayer::onTouchBegan(Touch *touch, Event *unused_event){
	if (nullptr != main_touch){
		return true;
	}
	main_touch = touch;



	return true;
}
void WinLayer::onTouchMoved(Touch *touch, Event *unused_event){
	if (touch != main_touch){
		return;
	}
}
void WinLayer::onTouchEnded(Touch *touch, Event *unused_event){
	if (touch != main_touch){
		return;
	}
	main_touch = nullptr;

	returnLastInterface();
}
void WinLayer::onTouchCancelled(Touch *touch, Event *unused_event){
	onTouchEnded(touch, unused_event);
}