#include "BubbleDesigner.h"
#include "RegularPolygon.h"


BubbleDesigner::BubbleDesigner(float _innerRadius, float _outerRadius, Bubble _bubble):BubbleView(_innerRadius, _bubble), outerRadius(_outerRadius){

}
BubbleDesigner* BubbleDesigner::create(float _innerRadius, float _outerRadius, Bubble _bubble){
	BubbleDesigner *bubbleDesigner = new BubbleDesigner(_innerRadius, _outerRadius, _bubble);

	if (bubbleDesigner && bubbleDesigner->init()){
		bubbleDesigner->autorelease();
	}
	else {
		delete bubbleDesigner;
		bubbleDesigner = nullptr;
	}

	return bubbleDesigner;
}

void BubbleDesigner::onEnter(){
	BubbleView::onEnter();

	RegularPolygon *regularPolygon = RegularPolygon::create(outerRadius, 6, CC_DEGREES_TO_RADIANS(-90.0f));
	this->addChild(regularPolygon, -10);
}