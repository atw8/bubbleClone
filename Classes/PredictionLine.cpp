#include "PredictionLine.h"

PredictionLine::PredictionLine(float _pointOffset, float _minBubbleRadius, float _maxBubbleRadius) :PredictionLineAbstract(_pointOffset), minBubbleRadius(_minBubbleRadius), maxBubbleRadius(_maxBubbleRadius){

}

PredictionLine *PredictionLine::create(float _pointOffset, float _minBubbleRadius, float _maxBubbleRadius){
	PredictionLine *predictionLine = new PredictionLine(_pointOffset, _minBubbleRadius, _maxBubbleRadius);
	if (predictionLine && predictionLine->init()){
		predictionLine->autorelease();
	}
	else {
		delete predictionLine;
		predictionLine = nullptr;
	}

	return predictionLine;
}

void PredictionLine::deleteNodeEvent(PredictionLineNode * dNode){
	BubbleView *bubbleView = colorNodes[dNode];
	bubbleView->removeFromParentAndCleanup(true);

	colorNodes.erase(colorNodes.find(dNode));
}
void PredictionLine::addNodeEvent(PredictionLineNode *aNode){
	float bubbleRadius = minBubbleRadius + (maxBubbleRadius - minBubbleRadius)*rand_0_1();


	BubbleView *bubbleView = BubbleView::create(bubbleRadius, Bubble(bubbleTypes[rand()%bubbleTypes.size()]));
	bubbleView->setPosition(getVec2ForLinePosition(aNode->lineLength));
	this->addChild(bubbleView);

	colorNodes[aNode] = bubbleView;
}
void PredictionLine::changeNodeEvent(PredictionLineNode *cNode){
	BubbleView *bubbleView = colorNodes[cNode];
	bubbleView->setPosition(getVec2ForLinePosition(cNode->lineLength));
}


void PredictionLine::setBubbleTypes(std::vector<BubbleType> _bubbleTypes){
	bubbleTypes = _bubbleTypes;

	for (std::pair<PredictionLineNode *, BubbleView *> colorNodesPair : colorNodes){
		BubbleView *bubbleView = colorNodesPair.second;
		bubbleView->setBubble(Bubble((bubbleTypes[rand() % bubbleTypes.size()])));
	}
}
std::vector<BubbleType> PredictionLine::getBubbleTypes(){
	return bubbleTypes;
}