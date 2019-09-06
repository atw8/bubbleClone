#include "LevelsInnerLayer.h"
#include "Global.h"


LevelsInnerLayer::LevelsInnerLayer(Rect _mainLayerLevelsRect) :layerRect(_mainLayerLevelsRect), pressButton(nullptr), draggingSensativity(200.0f), minXOffset(20.0f), yOffset(150.0f){
	innerLayerMinY = 0.0f;
	innerLayerMaxY = 0.0f;
}

LevelsInnerLayer *LevelsInnerLayer::create(Rect _mainLayerLevelsRect){
	LevelsInnerLayer *layerLevelsInner = new LevelsInnerLayer(_mainLayerLevelsRect);

	if (layerLevelsInner && layerLevelsInner->init()){
		layerLevelsInner->autorelease();
	}
	else {
		delete layerLevelsInner;
		layerLevelsInner = nullptr;
	}

	return layerLevelsInner;
}

void LevelsInnerLayer::onEnter(){
	Layer::onEnter();
	
	/*
	DrawNode *dNode = DrawNode::create();
	dNode->drawRect(layerRect.origin, layerRect.origin + Vec2(layerRect.size.width, layerRect.size.height), Color4F::WHITE);
	this->addChild(dNode, 100);
	*/

	clipNode = ClippingNode::create();
	{
		DrawNode *clipNodeStencil = DrawNode::create();

		clipNodeStencil->drawSolidRect(layerRect.origin, layerRect.origin + Vec2(layerRect.size.width, layerRect.size.height), Color4F::WHITE);

		clipNode->setStencil(clipNodeStencil);
	}
	this->addChild(clipNode);

	innerLayer = Layer::create();
	clipNode->addChild(innerLayer);


}



void LevelsInnerLayer::addLevel(LevelButton *levelButton){
	innerLayer->addChild(levelButton);

	levelButtons.push_back(levelButton);

	updateInnerLayerMax();
}

void LevelsInnerLayer::removeLevel(LevelButton *levelButton){
	levelButtons.erase(std::find(levelButtons.begin(), levelButtons.end(), levelButton));

	levelButton->removeFromParentAndCleanup(true);

	pressButton = nullptr;
}


void LevelsInnerLayer::removeAllLevels() {

	for (LevelButton *levelButton : levelButtons) {
		levelButton->removeFromParentAndCleanup(true);
	}

	levelButtons.clear();

	updateInnerLayerMax();
}
void LevelsInnerLayer::positionLevelButtons() {
	numOfRows = 0;

	int firstButton = 0;
	int lastButton;
	for (int i = 0; i < levelButtons.size(); i++) {
		lastButton = i;
		if (!canPositionRow(firstButton, lastButton)) {
			positionRow(firstButton, lastButton - 1);
			
			firstButton = lastButton;
		}
	}

	positionRow(firstButton, lastButton);

}


float LevelsInnerLayer::geButtonsXOffset(int firstButton, int lastButton) {
	float emptySpace = layerRect.size.width - getButtonsWidth(firstButton, lastButton);
	float xOffset = (emptySpace) / ((lastButton - firstButton) + 2.0f);

	return xOffset;
}

float LevelsInnerLayer::getButtonsWidth(int firstButton, int lastButton) {
	float ret = 0.0f;
	for (int i = firstButton; i <= lastButton; i++) {
		ret += levelButtons.at(i)->getButtonWidth();
	}

	return ret;
}


float LevelsInnerLayer::getButtonsHeight(int firstButton, int lastButton) {
	float ret = 0.0f;
	for (int i = firstButton; i <= lastButton; i++) {
		ret += levelButtons.at(i)->getButtonHeight();
	}

	return ret;
}


bool LevelsInnerLayer::canPositionRow(int firstButton, int lastButton) {
	bool ret = geButtonsXOffset(firstButton, lastButton) > minXOffset;
	
	return ret;
}

void LevelsInnerLayer::positionRow(int firstButton, int lastButton) {
	numOfRows++;


	float positionY = layerRect.origin.y + layerRect.size.height - (numOfRows*yOffset) + 50.0f; 
	
	if (firstButton == lastButton) {
		float positionX = layerRect.origin.x + layerRect.size.width / 2.0f;
		levelButtons[firstButton]->setPosition(Vec2(positionX, positionY));
	}
	else {


		float positionX = layerRect.origin.x;

		float xOffset = geButtonsXOffset(firstButton, lastButton);

		for (int i = firstButton; i <= lastButton; i++) {
			LevelButton *levelButton = levelButtons.at(i);

			positionX += xOffset;
			positionX += levelButton->getButtonWidth() / 2.0f;
			
			levelButton->setPosition(Vec2(positionX, positionY));

			positionX += levelButton->getButtonWidth() / 2.0f;
		}
	}

	updateInnerLayerMax();

}

void LevelsInnerLayer::updateInnerLayerMax() {
	innerLayerMaxY = ((numOfRows + 1.0f)*yOffset) - layerRect.size.height;
	innerLayerMaxY = std::max(innerLayerMaxY, 0.0f);


	float innerLayerPosY = innerLayer->getPositionY();
	innerLayerPosY = std::min(innerLayerMaxY, innerLayerPosY);
	innerLayerPosY = std::max(innerLayerMinY, innerLayerPosY);

	innerLayer->setPositionY(innerLayerPosY);
}




bool LevelsInnerLayer::onTouchBegan(Touch *touch, Event *unused_event){
	isDragging = false;

	Vec2 location = PointApplyTransform(touch->getLocation(), innerLayer->getWorldToNodeTransform());
	for (std::vector<LevelButton *>::iterator levelButtonsIter = levelButtons.begin(); levelButtonsIter != levelButtons.end() && nullptr == pressButton; levelButtonsIter++){
		LevelButton *levelButton = *levelButtonsIter;
		if (levelButton->getButtonRect().containsPoint(location)){

			setPressButton(levelButton);
		}
	}


	return true;
}
void LevelsInnerLayer::onTouchMoved(Touch *touch, Event *unused_event){

	setPressButton(nullptr);

	if (!isDragging){
		isDragging = std::abs(touch->getLocation().y - touch->getStartLocation().y) >= draggingSensativity;
	}

	if (isDragging){
		float yDiff = (touch->getLocation() - touch->getPreviousLocation()).y;
		float innerLayerPosY = innerLayer->getPositionY() + yDiff;

		innerLayerPosY = std::min(innerLayerMaxY, innerLayerPosY);
		innerLayerPosY = std::max(innerLayerMinY, innerLayerPosY);
		
		innerLayer->setPositionY(innerLayerPosY);
	}
	else {
		Vec2 location = PointApplyTransform(touch->getLocation(), innerLayer->getWorldToNodeTransform());
		for (std::vector<LevelButton *>::iterator levelButtonsIter = levelButtons.begin(); levelButtonsIter != levelButtons.end() && nullptr == pressButton; levelButtonsIter++){
			LevelButton *levelButton = *levelButtonsIter;
			if (levelButton->getButtonRect().containsPoint(location)){

				setPressButton(levelButton);
			}
		}
	}

}
void LevelsInnerLayer::onTouchEnded(Touch *touch, Event *unused_event){
	setPressButton(nullptr);

	if (!isDragging){
		isDragging = std::abs(touch->getLocation().y - touch->getStartLocation().y) >= draggingSensativity;
	}

	if (isDragging){
		float yDiff = (touch->getLocation() - touch->getPreviousLocation()).y;
		float innerLayerPosY = innerLayer->getPositionY() + yDiff;

		innerLayerPosY = std::min(innerLayerMaxY, innerLayerPosY);
		innerLayerPosY = std::max(innerLayerMinY, innerLayerPosY);

		innerLayer->setPositionY(innerLayerPosY);
	}
	else {
		Vec2 location = PointApplyTransform(touch->getLocation(), innerLayer->getWorldToNodeTransform());
		for (std::vector<LevelButton *>::iterator levelButtonsIter = levelButtons.begin(); levelButtonsIter != levelButtons.end() && nullptr == pressButton; levelButtonsIter++){
			LevelButton *levelButton = *levelButtonsIter;
			if (levelButton->getButtonRect().containsPoint(location)){
				setPressButton(levelButton);
				levelButton->setIsPressed(false);
			}
		}
	}



}


void LevelsInnerLayer::setPressButton(LevelButton *_pressButton){
	if (nullptr != pressButton){
		pressButton->setIsPressed(false);
	}

	pressButton = _pressButton;

	if (nullptr != pressButton){
		pressButton->setIsPressed(true);
	}
}
LevelButton *LevelsInnerLayer::getPressButton(){
	return pressButton;
}

void LevelsInnerLayer::setDeleteButtonOn(bool deleteButtonOn){
	for (LevelButton *levelButton : levelButtons){
		levelButton->setDeleteButtonOn(deleteButtonOn);
	}
}