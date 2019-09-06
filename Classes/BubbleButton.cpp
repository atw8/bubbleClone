#include "BubbleButton.h"
#include "Global.h"

BubbleButton::BubbleButton(float _innerRadius, float _outerRadius, Bubble _bubble) :BubbleView(_innerRadius, _bubble), outerRadius(_outerRadius),
isSelected(false), isPressed(false){

}

BubbleButton *BubbleButton::create(float _innerRadius, float _outerRadius, Bubble _bubble){
	BubbleButton *bubbleTypeButton = new BubbleButton(_innerRadius, _outerRadius, _bubble);

	if (bubbleTypeButton && bubbleTypeButton->init()){
		bubbleTypeButton->autorelease();
	}
	else {
		delete bubbleTypeButton;
		bubbleTypeButton = nullptr;
	}

	return bubbleTypeButton;
}

void BubbleButton::onEnter(){
	BubbleView::onEnter();

	isPressedScale[false] = bubbleNode->getScale();
	isPressedScale[true] = isPressedScale[false] * 0.9f;


	isSelectedDrawNode = DrawNode::create();
	std::vector<Vec2> isSelectedDrawNodeVerts;
	float isSelectedDrawNodeScale;
	switch (getBubble().getBubbleType()){
	case BubbleType::WITCH:
	case BubbleType::ANIMAL3:
		isSelectedDrawNodeScale = 0.9f;
		break;
	default:
		isSelectedDrawNodeScale = 1.0f;
		break;
	}

	isSelectedDrawNodeVerts.push_back(Vec2(-outerRadius * getShapeWidth() * isSelectedDrawNodeScale, -outerRadius *getShapeWidth() * isSelectedDrawNodeScale) + bubbleNode->getPosition());
	isSelectedDrawNodeVerts.push_back(Vec2(outerRadius * getShapeWidth() * isSelectedDrawNodeScale, -outerRadius * getShapeWidth() * isSelectedDrawNodeScale) + bubbleNode->getPosition());
	isSelectedDrawNodeVerts.push_back(Vec2(outerRadius * getShapeWidth() * isSelectedDrawNodeScale, outerRadius * getShapeWidth() * isSelectedDrawNodeScale) + bubbleNode->getPosition());
	isSelectedDrawNodeVerts.push_back(Vec2(-outerRadius * getShapeWidth() * isSelectedDrawNodeScale, outerRadius * getShapeWidth() * isSelectedDrawNodeScale) + bubbleNode->getPosition());
	isSelectedDrawNode->drawPoly(&isSelectedDrawNodeVerts[0], isSelectedDrawNodeVerts.size(), true, Color4F::RED);
	this->addChild(isSelectedDrawNode);

	isSelectedDrawNode->setVisible(isSelected);
}



bool BubbleButton::getIsSelected(){
	return isSelected;
}
void BubbleButton::setIsSelected(bool _isSelected){
	if (isSelected == _isSelected){
		return;
	}
	isSelected = _isSelected;


	isSelectedDrawNode->setVisible(isSelected);
}

bool BubbleButton::getIsPressed(){
	return isPressed;
}
void BubbleButton::setIsPressed(bool _isPressed){
	if (isPressed == _isPressed){
		return;
	}
	isPressed = _isPressed;

	setIsPressedInner();
}

void BubbleButton::setIsPressedInner(){
	bubbleNode->setScale(isPressedScale[isPressed]);
}

bool BubbleButton::containsPoint(Vec2 location){
	location = PointApplyTransform(location, this->getParentToNodeTransform());
	
	return location.length() <= innerRadius * getShapeWidth();
}

void BubbleButton::setBubbleInner(Bubble bubble, bool forceInit){
	BubbleView::setBubbleInner(bubble, forceInit);
	
	if (BubbleType::UNDEFINED == bubble.getBubbleType()){
		bubbleSprite->setSpriteFrame(Sprite::create("balls/delete_ball.png")->getSpriteFrame());
		bubbleSprite->setScale(0.95f*(2.0f*innerRadius / getContWidth(bubbleSprite)));
		bubbleSprite->setVisible(true);
	}
}

float BubbleButton::getShapeWidth(){
	float ret = 1.0f;
	switch (getBubble().getBubbleType()){
	case BubbleType::WITCH:
	case BubbleType::ANIMAL3:
		ret = 3.0f;
		break;
	case BubbleType::ANIMAL2:
		ret = 2.0f;
		break;
	default:
		ret = 1.0f;
		break;
	}

	return ret;
}