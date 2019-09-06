#include "LoseLayer.h"
#include "Global.h"


LoseLayer::LoseLayer(std::function<void()> _returnLastInterface) :returnLastInterface(_returnLastInterface), main_touch(nullptr){

}

LoseLayer* LoseLayer::create(std::function<void()> _returnLastInterface){
	LoseLayer *loseLayer = new LoseLayer(_returnLastInterface);
	if (loseLayer && loseLayer->init()){
		loseLayer->autorelease();
	}
	else {
		delete loseLayer;
		loseLayer = nullptr;
	}

	return loseLayer;
}

void LoseLayer::onEnter(){
	LayerColor::onEnter();
	this->setColor(Color3B::BLACK);
	this->setOpacity(0);

	Sprite *loseSign = Sprite::create("loseLayer/loseSign.png");
	loseSign->setPosition(Vec2(getContWidth(this) / 2.0f, getContHeight(this) / 2.0f));
	this->addChild(loseSign);

	loseSign->setOpacity(0.0f);
	loseSign->runAction(FadeIn::create(0.6f));

	this->runAction(FadeTo::create(0.3f, 255 * (2.0f / 3.0f)));

	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = std::bind(&LoseLayer::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchMoved = std::bind(&LoseLayer::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchEnded = std::bind(&LoseLayer::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchCancelled = std::bind(&LoseLayer::onTouchCancelled, this, std::placeholders::_1, std::placeholders::_2);

	listener->setSwallowTouches(true);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}


bool LoseLayer::onTouchBegan(Touch *touch, Event *unused_event){
	if (nullptr != main_touch){
		return true;
	}
	main_touch = touch;

	return true;
}
void LoseLayer::onTouchMoved(Touch *touch, Event *unused_event){
	if (touch != main_touch){
		return;
	}
}
void LoseLayer::onTouchEnded(Touch *touch, Event *unused_event){
	if (touch != main_touch){
		return;
	}
	main_touch = nullptr;

	returnLastInterface();
}
void LoseLayer::onTouchCancelled(Touch *touch, Event *unused_event){
	onTouchEnded(touch, unused_event);
}