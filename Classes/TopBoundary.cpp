#include "TopBoundary.h"
#include "Global.h"


TopBoundary::TopBoundary(Rect _topBoundaryRect)
:topBoundaryRect(_topBoundaryRect){
	
	nodeWitch = nullptr;
	nodeAnimal = nullptr;
	nodeClearTop = nullptr;

	numOfWitchesClear = 0;
	numOfClearTopClear = 0;
	numOfAnimalClear = 0;

	numOfWitchesTotal = 0;
	numOfClearTopTotal = 0;
	numOfAnimalTotal = 0;
}
TopBoundary* TopBoundary::create(Rect _topBoundaryRect){
	TopBoundary *topBoundary = new TopBoundary(_topBoundaryRect);
	if (topBoundary && topBoundary->init()){
		topBoundary->autorelease();
	}
	else {
		delete topBoundary;
		topBoundary = nullptr;
	}

	return topBoundary;
}

Scene *TopBoundary::createScene(Rect _topBoundaryRect){
	Scene *pScene = Scene::create();
	pScene->addChild(TopBoundary::create(_topBoundaryRect));

	return pScene;
}

void TopBoundary::onEnter(){
	Layer::onEnter();

	topBoundarySpr = Sprite::create("shared/topBoundary.png");
	topBoundarySpr->setScaleX(topBoundaryRect.size.width / getContWidth(topBoundarySpr));
	topBoundarySpr->setScaleY(topBoundaryRect.size.height / getContHeight(topBoundarySpr));
	topBoundarySpr->setAnchorPoint(Vec2(0.0f, 0.0f));
	topBoundarySpr->setPosition(topBoundaryRect.origin);
	this->addChild(topBoundarySpr);




}

void TopBoundary::createNodeWitch() {
	if (nullptr != nodeWitch) {
		return;
	}

	nodeWitch = TopBoundaryNode::create("target_witch.png", numOfWitchesTotal);
	topBoundarySpr->addChild(nodeWitch);

	positionNodes();

}
void TopBoundary::createNodeAnimal() {
	if (nullptr != nodeAnimal) {
		return;
	}

	nodeAnimal = TopBoundaryNode::create("target_animals.png", numOfAnimalTotal);
	topBoundarySpr->addChild(nodeAnimal);

	positionNodes();

}
void TopBoundary::createNodeClearTop() {
	if (nullptr != nodeClearTop) {
		return;
	}

	nodeClearTop = TopBoundaryNode::create("target_clearTop.png", numOfClearTopTotal);
	topBoundarySpr->addChild(nodeClearTop);

	positionNodes();
}

void TopBoundary::deleteNodeWitch() {
	if (nullptr == nodeWitch) {
		return;
	}

	nodeWitch->removeFromParentAndCleanup(true);
	nodeWitch = nullptr;

	positionNodes();
}
void TopBoundary::deleteNodeAnimal() {
	if (nullptr == nodeAnimal) {
		return;
	}

	nodeAnimal->removeFromParentAndCleanup(true);
	nodeAnimal = nullptr;

	positionNodes();
}
void TopBoundary::deleteNodeClearTop() {
	if (nullptr == nodeClearTop) {
		return;
	}

	nodeClearTop->removeFromParentAndCleanup(true);
	nodeClearTop = nullptr;

	positionNodes();
}

void TopBoundary::positionNodes() {
	float xOffset = getContWidth(topBoundarySpr);
	float separator = 20.0f;

	if (nullptr != nodeClearTop) {
		Rect boundBox = nodeClearTop->getBoundingBox();
		nodeClearTop->setPosition(Vec2(xOffset - boundBox.size.width / 2.0f - separator, getContHeight(topBoundarySpr) / 2.0f));


		xOffset -= boundBox.size.width + separator;
	}

	if (nullptr != nodeAnimal) {
		Rect boundBox = nodeAnimal->getBoundingBox();
		nodeAnimal->setPosition(Vec2(xOffset - boundBox.size.width / 2.0f - separator, getContHeight(topBoundarySpr) / 2.0f));


		xOffset -= boundBox.size.width + separator;
	}

	if (nullptr != nodeWitch) {
		Rect boundBox = nodeWitch->getBoundingBox();
		nodeWitch->setPosition(Vec2(xOffset - boundBox.size.width / 2.0f - separator, getContHeight(topBoundarySpr) / 2.0f));


		xOffset -= boundBox.size.width + separator;
	}
}



void TopBoundary::setNumOfWitchesTotal(int _numOfWitchesTotal) {
	numOfWitchesTotal = _numOfWitchesTotal;


	if (0 == numOfWitchesTotal) {
		deleteNodeWitch();
	}
	else {
		if (nullptr == nodeWitch) {
			createNodeWitch();
		}

		nodeWitch->setTotal(numOfWitchesTotal);
	}

}
void TopBoundary::setNumOAnimalTotal(int _numOfAnimalTotal) {
	numOfAnimalTotal = _numOfAnimalTotal;

	if (0 == numOfAnimalTotal) {
		deleteNodeAnimal();
	}
	else {
		if (nullptr == nodeAnimal) {
			createNodeAnimal();
		}

		nodeAnimal->setTotal(numOfAnimalTotal);
	}
}
void TopBoundary::setNumOfClearTopTotal(int _numOfClearTopTotal) {
	numOfClearTopTotal = _numOfClearTopTotal;

	if (0 == numOfClearTopTotal) {
		deleteNodeClearTop();
	}
	else {
		if (nullptr == nodeClearTop) {
			createNodeClearTop();
		}

		nodeClearTop->setTotal(numOfClearTopTotal);
	}
}

int TopBoundary::getNumOfWitchesTotal() {
	return numOfWitchesTotal;
}
int TopBoundary::getNumOfAnimalTotal() {
	return numOfAnimalTotal;
}
int TopBoundary::getNumOfClearTopTotal() {
	return numOfClearTopTotal;
	
}

void TopBoundary::setNumOfWitchesClear(int _numOfWitchesClear){
	numOfWitchesClear = std::min(_numOfWitchesClear, numOfWitchesTotal);

	if (nullptr != nodeWitch){
		createNodeWitch();
	}

	nodeWitch->setCurrent(numOfWitchesClear);
}
void TopBoundary::setNumOfAnimalClear(int _numOfAnimalClear){
	numOfAnimalClear = std::min(_numOfAnimalClear, numOfAnimalTotal);

	if (nullptr != nodeAnimal){
		createNodeAnimal();
	}

	nodeAnimal->setCurrent(numOfAnimalClear);

}
void TopBoundary::setNumOfClearTopClear(int _numOfClearTopClear){
	numOfClearTopClear = std::min(_numOfClearTopClear, numOfClearTopTotal);

	if (nullptr != nodeClearTop){
		createNodeClearTop();
	}

	nodeClearTop->setCurrent(numOfClearTopClear);
}

int TopBoundary::getNumOfWitchesClear(){
	return numOfWitchesClear;
}
int TopBoundary::getNumOfAnimalClear(){
	return numOfAnimalClear;
}
int TopBoundary::getNumOfClearTopClear(){
	return numOfClearTopClear;
}

bool TopBoundary::isWin(){
	return numOfWitchesClear >= numOfWitchesTotal && numOfAnimalClear >= numOfAnimalTotal && numOfClearTopClear >= numOfClearTopTotal;
}

Vec2 TopBoundary::getWitchesFlyPosition(){
	return PointApplyTransform(nodeWitch->getPosition(), topBoundarySpr->getNodeToWorldTransform());
}
Vec2 TopBoundary::getAnimalFlyPosition(){
	return PointApplyTransform(nodeAnimal->getPosition(), topBoundarySpr->getNodeToWorldTransform());
}
Vec2 TopBoundary::getClearTopFlyPosition(){
	return PointApplyTransform(nodeClearTop->getPosition(), topBoundarySpr->getNodeToWorldTransform());
}