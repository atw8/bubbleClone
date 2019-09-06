#include "ScrollBarInteger.h"
#include "Global.h"


ScrollBarInteger::ScrollBarInteger(int _scrollBottom, int _scrollTop, int _scroll):
scrollBottom(_scrollBottom), scrollTop(_scrollTop), scroll(_scroll),
backComponent(nullptr), scrollComponent(nullptr){

	scroll = std::min(scroll, scrollTop);
	scroll = std::max(scroll, scrollBottom);
}

ScrollBarInteger* ScrollBarInteger::create(int _scrollBottom, int _scrollTop, int _scroll){
	ScrollBarInteger *scrollBarInteger = new ScrollBarInteger(_scrollBottom, _scrollTop, _scroll);
	if (scrollBarInteger && scrollBarInteger->init()){
		scrollBarInteger->autorelease();
	}
	else {
		delete scrollBarInteger;
		scrollBarInteger = nullptr;
	}

	return scrollBarInteger;
}
void ScrollBarInteger::onEnter(){
	Node::onEnter();

	backComponent = DrawNode::create();
	this->addChild(backComponent, 0);

	scrollComponent = DrawNode::create();
	this->addChild(scrollComponent, 1);

	setContentSize(getContentSize());
}

void ScrollBarInteger::setContentSize(Size contentSize){
	Node::setContentSize(contentSize);

	if (backComponent == nullptr || scrollComponent == nullptr){
		return;
	}
	backComponent->clear();
	backComponent->removeAllChildrenWithCleanup(true);
	scrollComponent->clear();
	scrollComponent->removeAllChildrenWithCleanup(true);

	std::vector<Vec2> backComponentVec2s;
	backComponentVec2s.push_back(Vec2(-contentSize.width / 2.0f, -(1.0f / 20.0f)*contentSize.height));
	backComponentVec2s.push_back(Vec2(contentSize.width / 2.0f, -(1.0f / 20.0f)*contentSize.height));
	backComponentVec2s.push_back(Vec2(contentSize.width / 2.0f, (1.0f / 20.0f)*contentSize.height));
	backComponentVec2s.push_back(Vec2(-contentSize.width / 2.0f, (1.0f / 20.0f)*contentSize.height));

	backComponent->drawSolidPoly(&backComponentVec2s[0], backComponentVec2s.size(), Color4F::WHITE);


	const float backLineWidth = 5.0f;
	for (int i = scrollBottom; i <= scrollTop; i++){
		Vec2 vec2(getVec2XForScroll(i), 0.0f);

		DrawNode *backLine = DrawNode::create();
		backLine->setPosition(vec2);
		backComponent->addChild(backLine);

		std::vector<Vec2> backLineVec2s;
		backLineVec2s.push_back(Vec2(-backLineWidth / 2.0f, -(1.0f / 4.0f)*contentSize.height));
		backLineVec2s.push_back(Vec2(backLineWidth / 2.0f, -(1.0f / 4.0f)*contentSize.height));
		backLineVec2s.push_back(Vec2(backLineWidth / 2.0f, (1.0f / 4.0f)*contentSize.height));
		backLineVec2s.push_back(Vec2(-backLineWidth / 2.0f, (1.0f / 4.0f)*contentSize.height));

		backLine->drawSolidPoly(&backLineVec2s[0], backLineVec2s.size(), Color4F::WHITE);

		Label *backLineLabel = Label::createWithSystemFont(to_string(i), "Arial", 20);
		backLineLabel->setAnchorPoint(Vec2(0.5f, 0.0f));
		backLineLabel->setPosition(Vec2(0.0f, (1.0f / 4.0f)*contentSize.height ));
		backLine->addChild(backLineLabel);
	}

	
	const float scrollComponentWidth = backLineWidth*4.0f;
	std::vector<Vec2> scrollComponentVec2s;
	scrollComponentVec2s.push_back(Vec2(-scrollComponentWidth / 2.0f, -contentSize.height / 2.0f));
	scrollComponentVec2s.push_back(Vec2(scrollComponentWidth / 2.0f, -contentSize.height / 2.0f));
	scrollComponentVec2s.push_back(Vec2(scrollComponentWidth / 2.0f, contentSize.height / 2.0f));
	scrollComponentVec2s.push_back(Vec2(-scrollComponentWidth / 2.0f, contentSize.height / 2.0f));

	scrollComponent->drawSolidPoly(&scrollComponentVec2s[0], scrollComponentVec2s.size(), Color4F::RED);

	setScroll(getScroll(), true);
}

float ScrollBarInteger::getVec2XForScroll(int i){
	Size contentSize = getContentSize();

	return -contentSize.width / 2.0f + contentSize.width*((float)(i - scrollBottom) / (float)(scrollTop - scrollBottom));
}

int ScrollBarInteger::getScrollBottom(){
	return scrollBottom;
}
int ScrollBarInteger::getScrollTop(){
	return scrollTop;
}
int ScrollBarInteger::getScroll(){
	return scroll;
}

void ScrollBarInteger::setScrollBottom(int _scrollBottom, bool force){
	if (scrollBottom == _scrollBottom && !force){
		return;
	}
	scrollBottom = _scrollBottom;

	setContentSize(getContentSize());
}
void ScrollBarInteger::setScrollTop(int _scrollTop, bool force){
	if (scrollTop == _scrollTop && !force){
		return;
	}
	scrollTop = _scrollTop;

	setContentSize(getContentSize());
}
void ScrollBarInteger::setScroll(int _scroll, bool force){
	if (scroll == _scroll && !force){
		return;
	}
	scroll = _scroll;

	scroll = std::min(scroll, scrollTop);
	scroll = std::max(scroll, scrollBottom);

	if (scrollComponent == nullptr){
		return;
	}

	scrollComponent->setPositionX(getVec2XForScroll(scroll));
}

void ScrollBarInteger::setScrollFloat(float positionX){
	float _scroll = (float)scrollBottom + ((float)scrollTop - (float)scrollBottom)*( (positionX + getContWidth(this)/2.0f)/getContWidth(this));

	setScroll((int) (_scroll + 0.5f));
}