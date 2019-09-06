#include "ScrollBar.h"

#define Z_BACK_COMPONENT 0
#define Z_SCROLL_COMPONENT 1


ScrollBar::ScrollBar(float _backTopY, float _backBottomY, float _scrollSize, float _scrollPosition, Color4F _scrollComponentColor, Color4F _backComponentColor) 
:scrollComponent(nullptr), backComponent(nullptr),
backTopY(_backTopY), backBottomY(_backBottomY), scrollSize(_scrollSize), scrollPosition(_scrollPosition),
scrollComponentColor(_scrollComponentColor), backComponentColor(_backComponentColor){


}
ScrollBar* ScrollBar::create(float _backTopY, float _backBottomY, float _scrollSize, float _scrollPosition, Color4F _scrollComponentColor, Color4F _backComponentColor){
	ScrollBar *scrollBar = new ScrollBar(_backTopY, _backBottomY, _scrollSize, _scrollPosition, _scrollComponentColor, _backComponentColor);
	if (scrollBar && scrollBar->init()){
		scrollBar->autorelease();
	}
	else {
		delete scrollBar;
		scrollBar = nullptr;
	}

	return scrollBar;
}

void ScrollBar::onEnter(){
	Node::onEnter();

	backComponent = DrawNode::create();
	this->addChild(backComponent, Z_BACK_COMPONENT);

	scrollComponent = DrawNode::create();
	this->addChild(scrollComponent, Z_SCROLL_COMPONENT);

	setContentSize(getContentSize());
}
void ScrollBar::setContentSize(Size contentSize){
	Node::setContentSize(contentSize);

	if (nullptr != scrollComponent && nullptr != backComponent){
		scrollComponent->clear();
		backComponent->clear();

		std::vector<Vec2> componentVec2;
		componentVec2.push_back(Vec2(-contentSize.width / 2.0f, -contentSize.height / 2.0f));
		componentVec2.push_back(Vec2(contentSize.width / 2.0f, -contentSize.height / 2.0f));
		componentVec2.push_back(Vec2(contentSize.width / 2.0f, contentSize.height / 2.0f));
		componentVec2.push_back(Vec2(-contentSize.width / 2.0f, contentSize.height / 2.0f));

		backComponent->drawSolidPoly(&componentVec2[0], componentVec2.size(), backComponentColor);
		scrollComponent->drawSolidPoly(&componentVec2[0], componentVec2.size(), scrollComponentColor);

		updateScrollComponentSize();
		setScrollPosition(getScrollPosition());
	}

}

float ScrollBar::getBackTopY(){
	return backTopY;
}
void ScrollBar::setBackTopY(float _backTopY){
	assert(_backTopY > backBottomY);
	backTopY = _backTopY;

	updateScrollComponentSize();
}

float ScrollBar::getBackBottomY(){
	return backBottomY;
}
void ScrollBar::setBackBottomY(float _backBottomY){
	assert(_backBottomY < backTopY);
	backBottomY = _backBottomY;

	updateScrollComponentSize();
}

float ScrollBar::getBackSize(){
	return backTopY - backBottomY;
}


float ScrollBar::getScrollSize(){
	return scrollSize;
}
void ScrollBar::setScrollSize(float _scrollSize){
	assert(_scrollSize > 0 && _scrollSize <= (backTopY - backBottomY));
	scrollSize = _scrollSize;

	updateScrollComponentSize();
}

float ScrollBar::getScrollPosition(){
	return scrollPosition;
}


void ScrollBar::addScrollPosition(float val){
	setScrollPosition(getScrollPosition() + val);
}

void ScrollBar::setScrollPosition(float _scrollPosition){
	float maxScrollPosition = backTopY - scrollSize / 2.0f;
	float minScrollPosition = backBottomY + scrollSize / 2.0f;

	_scrollPosition = std::min(maxScrollPosition, _scrollPosition);
	_scrollPosition = std::max(minScrollPosition, _scrollPosition);

	scrollPosition = _scrollPosition;


	float scrollRatio = (scrollPosition - backBottomY) / (backTopY - backBottomY);

	scrollComponent->setPositionY(scrollRatio*getContentSize().height - getContentSize().height/2.0f);
}

void ScrollBar::updateScrollComponentSize(){
	scrollComponent->setScaleY(scrollSize / getBackSize());
	setScrollPosition(getScrollPosition());
}