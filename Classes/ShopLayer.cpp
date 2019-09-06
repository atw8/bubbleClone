#include "ShopLayer.h"
#include "Global.h"

#include "GlobalBuy.h"
#include "DDUserDefault.h"
#include "GoldBar.h"

ShopLayer::ShopLayer():main_touch(nullptr), moveSensitivity(50.0f){

}

ShopLayer* ShopLayer::create(){
	ShopLayer *shopLayer = new ShopLayer();
	if (shopLayer && shopLayer->init()){
		shopLayer->autorelease();
	}
	else {
		delete shopLayer;
		shopLayer = nullptr;
	}

	return shopLayer;
}
Scene* ShopLayer::createScene(){
	Scene *pScene = Scene::create();
	pScene->addChild(ShopLayer::create());

	return pScene;
}

void ShopLayer::onEnter(){
	LayerColor::onEnter();	
	this->setColor(Color3B::GREEN);
	this->setOpacity(128);



	background = Sprite::create("shopLayer/background.png");
	background->setPosition(Vec2(getContWidth(this) / 2.0f, getContHeight(this) / 2.0f));
	this->addChild(background);

	GoldBar *goldBar = GoldBar::create(false);
	goldBar->setPosition(Vec2(getContWidth(this) / 2.0f, getContHeight(this) - getContHeight(goldBar) / 2.0f));
	this->addChild(goldBar);
	goldBar->setText(DDUserDefault::getInstance()->getNumOfGold());



	
	{
		Size innerSize = Size(640, 630);
		Vec2 innerOrigin = Vec2(getContWidth(background) / 2.0f, 45.0f) + Vec2(-innerSize.width / 2.0f, 0.0f);

		innerRect.size = innerSize;
		innerRect.origin = innerOrigin;
	}



	

	shopLayerInner = ShopLayerInner::create(std::bind(&ShopLayer::shopBtnCallback, this, std::placeholders::_1));
	shopLayerInner->setPosition(Vec2(innerRect.size.width / 2.0f, innerRect.size.height) + innerRect.origin);


	ClippingNode *innerClip = ClippingNode::create();
	{
		DrawNode *clippingNodeStencil = DrawNode::create();
		std::vector<Vec2> clippingNodeStencilVec2;
		clippingNodeStencilVec2.push_back(innerRect.origin + Vec2(0.0f, 0.0f));
		clippingNodeStencilVec2.push_back(innerRect.origin + Vec2(innerRect.size.width, 0.0f));
		clippingNodeStencilVec2.push_back(innerRect.origin + Vec2(innerRect.size.width, innerRect.size.height));
		clippingNodeStencilVec2.push_back(innerRect.origin + Vec2(0.0f, innerRect.size.height));

		clippingNodeStencil->drawSolidPoly(&clippingNodeStencilVec2[0], clippingNodeStencilVec2.size(), Color4F::WHITE);

		innerClip->setStencil(clippingNodeStencil);
		innerClip->addChild(shopLayerInner);
	}
	background->addChild(innerClip);

	minYShopLayerInner = shopLayerInner->getPositionY();
	if (shopLayerInner->getWidgetsHeight() < innerRect.size.height){
		maxYShopLayerInner = minYShopLayerInner;
	}
	else {
		maxYShopLayerInner = shopLayerInner->getPositionY() + shopLayerInner->getWidgetsHeight() - innerRect.size.height;
	}


	backBtn = Sprite::create("backBtn.png");
	backBtn->setPosition(Vec2(getContWidth(backBtn) / 2.0f, getContHeight(this) - getContHeight(backBtn) / 2.0f));
	this->addChild(backBtn);


	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = std::bind(&ShopLayer::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchMoved = std::bind(&ShopLayer::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchEnded = std::bind(&ShopLayer::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchCancelled = std::bind(&ShopLayer::onTouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	keyboardListener->onKeyPressed = std::bind(&ShopLayer::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
	keyboardListener->onKeyReleased = std::bind(&ShopLayer::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);
}


void ShopLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *unused_event){

}
void ShopLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *unused_event){
	if (EventKeyboard::KeyCode::KEY_BACK == keyCode){
		backBtnCallback();
	}
}



bool ShopLayer::onTouchBegan(Touch *touch, Event *unused_event){
	if (nullptr != main_touch){
		return true;
	}
	main_touch = touch;

	Vec2 location = touch->getLocation();

	if (innerRect.containsPoint(PointApplyTransform(location, background->getWorldToNodeTransform()))){
		touchType = TouchType::BUTTONSINNER;
	}else {
		touchType = TouchType::BUTTONSOUTER;
	}

	switch (touchType){
	case TouchType::BUTTONSINNER:
		shopLayerInner->onTouchBegan(PointApplyTransform(location, shopLayerInner->getWorldToNodeTransform()));
		break;
	case TouchType::BUTTONSOUTER:
		if (backBtn->getBoundingBox().containsPoint(location)){
			backBtn->setScale(0.9f);
		}
		break;
	}

	

	return true;
}
void ShopLayer::onTouchMoved(Touch *touch, Event *unused_event){
	if (touch != main_touch){
		return;
	}

	Vec2 location = touch->getLocation();

	if (TouchType::BUTTONSINNER == touchType){
		if (std::abs(location.y - touch->getStartLocation().y) > moveSensitivity){
			touchType = TouchType::DRAGINNER;
			shopLayerInner->setShopLayerWidgetsScale(1.0f);
		}
	}

	switch (touchType){
	case TouchType::BUTTONSINNER:
		shopLayerInner->onTouchMoved(PointApplyTransform(location, shopLayerInner->getWorldToNodeTransform()));
		break;
	case TouchType::BUTTONSOUTER:
		backBtn->setScale(1.0f);
		if (backBtn->getBoundingBox().containsPoint(location)){
			backBtn->setScale(0.9f);
		}
		break;
	case TouchType::DRAGINNER:
	{
								 Vec2 diff = location - touch->getPreviousLocation();

								 float yShopLayerInner = shopLayerInner->getPositionY() + diff.y;
								 yShopLayerInner = std::max(yShopLayerInner, minYShopLayerInner);
								 yShopLayerInner = std::min(yShopLayerInner, maxYShopLayerInner);

								 shopLayerInner->setPositionY(yShopLayerInner);
	}
		break;
	}




}
void ShopLayer::onTouchEnded(Touch *touch, Event *unused_event){
	if (touch != main_touch){
		return;
	}
	main_touch = nullptr;

	Vec2 location = touch->getLocation();

	switch (touchType){
	case TouchType::BUTTONSINNER:
		shopLayerInner->onTouchEnded(PointApplyTransform(location, shopLayerInner->getWorldToNodeTransform()));
		break;
	case TouchType::BUTTONSOUTER:
		backBtn->setScale(1.0f);
		if (backBtn->getBoundingBox().containsPoint(location)){
			backBtnCallback();
		}
		break;
	case TouchType::DRAGINNER:
		break;
	}


}
void ShopLayer::onTouchCancelled(Touch *touch, Event *unused_event){
	onTouchEnded(touch, unused_event);
}

void ShopLayer::backBtnCallback(){
	this->removeFromParentAndCleanup(true);
}

void ShopLayer::shopBtnCallback(ShopStruct shopStruct){
	
	GlobalBuy::getInstance()->buy(shopStruct, std::bind(&ShopLayer::buyBack, this, std::placeholders::_1, std::placeholders::_2));
}

void ShopLayer::buyBack(ShopStruct shopStruct, bool isSuccess){
	if (!isSuccess){
		return;
	}


	DDUserDefault::getInstance()->addNumOfGold(shopStruct.gold);
}