#ifndef _PREDICTION_LINE_H_
#define _PREDICTION_LINE_H_

#include "cocos2d.h"
USING_NS_CC;

#include "PredictionLineAbstract.h"

#include "BubbleView.h"

class PredictionLine : public Layer, public PredictionLineAbstract {
private:
	PredictionLine(float _pointOffset, float _minBubbleRadius, float _maxBubbleRadius);
public:
	static PredictionLine *create(float _pointOffset, float _minBubbleRadius, float _maxBubbleRadius);

private:
	std::map<PredictionLineNode *, BubbleView *> colorNodes;
protected:
	void deleteNodeEvent(PredictionLineNode * dNode);
	void addNodeEvent(PredictionLineNode *aNode);
	void changeNodeEvent(PredictionLineNode *cNode);

public:
	const float minBubbleRadius;
	const float maxBubbleRadius;

	void setBubbleTypes(std::vector<BubbleType> _bubbleTypes);
	std::vector<BubbleType> getBubbleTypes();
private:
	std::vector<BubbleType> bubbleTypes;
};

#endif //_PREDICTION_LINE_H_