#include "PauseLayer.h"

#include "Global.h"

PauseLayer::PauseLayer(std::function<void()> _restartGameFunc, std::function<void()> _returnGameFunc, std::function<void()> _returnMapFunc) :main_touch(nullptr),
restartGameFunc(_restartGameFunc),
returnGameFunc(_returnGameFunc),
returnMapFunc(_returnMapFunc)
{

}
PauseLayer *PauseLayer::create(std::function<void()> _restartGameFunc, std::function<void()> _returnGameFunc, std::function<void()> _returnMapFunc){
	PauseLayer *pauseLayer = new PauseLayer(_restartGameFunc, _returnGameFunc, _returnMapFunc);
	if (pauseLayer && pauseLayer->init()){
		pauseLayer->autorelease();
	}
	else {
		delete pauseLayer;
		pauseLayer = nullptr;
	}

	return pauseLayer;
}

Scene *PauseLayer::createScene(std::function<void()> _restartGameFunc, std::function<void()> _returnGameFunc, std::function<void()> _returnMapFunc){
	Scene *pScene = Scene::create();
	pScene->addChild(PauseLayer::create(_restartGameFunc, _returnGameFunc, _returnMapFunc));

	return pScene;
}

void PauseLayer::onEnter(){
	LayerColor::onEnter();
	this->setColor(Color3B::GREEN);
	this->setOpacity(128);

	background = Sprite::create("pauseLayer/background.png");
	background->setPosition(Vec2(getContWidth(this) / 2.0f, getContHeight(this) / 2.0f));
	this->addChild(background);


	float yOffSet = 150.0f;;
	restartGameBtn = Sprite::create("pauseLayer/playAgainBtn.png");
	restartGameBtn->setPosition(Vec2(getContWidth(background) / 2.0f, getContHeight(background) / 2.0f));
	background->addChild(restartGameBtn);

	returnGameBtn = Sprite::create("pauseLayer/returnGameBtn.png");
	returnGameBtn->setPosition(restartGameBtn->getPosition() + Vec2(0.0f, yOffSet));
	background->addChild(returnGameBtn);

	returnMapBtn = Sprite::create("pauseLayer/returnMapBtn.png");
	returnMapBtn->setPosition(restartGameBtn->getPosition() + Vec2(0.0f, -yOffSet));
	background->addChild(returnMapBtn);


	backBtn = Sprite::create("backBtn.png");
	backBtn->setPosition(Vec2(getContWidth(backBtn) / 2.0f, getContHeight(this) - getContHeight(backBtn) / 2.0f));
	this->addChild(backBtn);


	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = std::bind(&PauseLayer::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchMoved = std::bind(&PauseLayer::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchEnded = std::bind(&PauseLayer::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchCancelled = std::bind(&PauseLayer::onTouchCancelled, this, std::placeholders::_1, std::placeholders::_2);

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	
	keyboardListener->onKeyPressed = std::bind(&PauseLayer::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
	keyboardListener->onKeyReleased = std::bind(&PauseLayer::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);
	

}


bool PauseLayer::onTouchBegan(Touch *touch, Event *unused_event){
	if (nullptr != main_touch){
		return true;
	}
	main_touch = touch;


	Vec2 location = PointApplyTransform(touch->getLocation(), background->getWorldToNodeTransform());

	if (returnGameBtn->getBoundingBox().containsPoint(location)){
		returnGameBtn->setScale(0.9f);
	}
	else if (restartGameBtn->getBoundingBox().containsPoint(location)){
		restartGameBtn->setScale(0.9f);
	}
	else if (returnMapBtn->getBoundingBox().containsPoint(location)){
		returnMapBtn->setScale(0.9f);
	}
	else if (backBtn->getBoundingBox().containsPoint(touch->getLocation())){
		backBtn->setScale(0.9f);
	}

	return true;
}
void PauseLayer::onTouchMoved(Touch *touch, Event *unused_event){
	if (touch != main_touch){
		return;
	}

	Vec2 location = PointApplyTransform(touch->getLocation(), background->getWorldToNodeTransform());

	returnGameBtn->setScale(1.0f);
	restartGameBtn->setScale(1.0f);
	returnMapBtn->setScale(1.0f);
	backBtn->setScale(1.0f);

	if (returnGameBtn->getBoundingBox().containsPoint(location)){
		returnGameBtn->setScale(0.9f);
	}
	else if (restartGameBtn->getBoundingBox().containsPoint(location)){
		restartGameBtn->setScale(0.9f);
	}
	else if (returnMapBtn->getBoundingBox().containsPoint(location)){
		returnMapBtn->setScale(0.9f);
	}
	else if (backBtn->getBoundingBox().containsPoint(touch->getLocation())){
		backBtn->setScale(0.9f);
	}
}
void PauseLayer::onTouchEnded(Touch *touch, Event *unused_event){
	if (touch != main_touch){
		return;
	}
	main_touch = nullptr;

	Vec2 location = PointApplyTransform(touch->getLocation(), background->getWorldToNodeTransform());

	returnGameBtn->setScale(1.0f);
	restartGameBtn->setScale(1.0f);
	returnMapBtn->setScale(1.0f);
	backBtn->setScale(1.0f);

	if (returnGameBtn->getBoundingBox().containsPoint(location)){
		returnGameBtnCallback();
	}
	else if (restartGameBtn->getBoundingBox().containsPoint(location)){
		restartGameBtnCallback();
	}
	else if (returnMapBtn->getBoundingBox().containsPoint(location)){
		returnMapBtnCallback();
	}
	else if (backBtn->getBoundingBox().containsPoint(touch->getLocation())){
		returnGameBtnCallback();
	}
}
void PauseLayer::onTouchCancelled(Touch *touch, Event *unused_event){
	onTouchEnded(touch, unused_event);
}

void PauseLayer::returnGameBtnCallback(){
	returnGameFunc();
	this->removeFromParentAndCleanup(true);
}

void PauseLayer::restartGameBtnCallback(){
	restartGameFunc();
	this->removeFromParentAndCleanup(true);
}

void PauseLayer::returnMapBtnCallback(){
	returnMapFunc();
	this->removeFromParentAndCleanup(true);
}


void PauseLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){

}
void PauseLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (EventKeyboard::KeyCode::KEY_BACK == keyCode){
		returnGameBtnCallback();
	}
}