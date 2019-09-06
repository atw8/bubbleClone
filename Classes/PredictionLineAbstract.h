#ifndef _PREDICTION_LINE_ABSTRACT_H_
#define _PREDICTION_LINE_ABSTRACT_H_

#include "PredictionLineNode.h"

#include "cocos2d.h"
USING_NS_CC;

#include <vector>

class PredictionLineAbstract {
public:
	PredictionLineAbstract(float _pointOffset);

	void setControlPoints(std::vector<Vec2> _controlPoints);
	std::vector<Vec2> getControlPoints();

	const float pointOffset;

	
	void updatePredictionLine(float val);
	std::list<PredictionLineNode *> predLinesNodes;
private:

	void deleteEndNodes();
	void addEndNodes();

	void deleteBeginNodes();
	void addBeginNodes();
	
	void movePredictionLineNodes(float distance);


	float getMaxLineSize();
	float getMinLineSize();

	std::vector<Vec2> controlPoints;

protected:
	virtual void deleteNodeEvent(PredictionLineNode * dNode) = 0;
	virtual void addNodeEvent(PredictionLineNode *aNode) = 0;
	virtual void changeNodeEvent(PredictionLineNode *cNode) = 0;

	Vec2 getVec2ForLinePosition(float linePosition);
};

#endif //_PREDICTION_LINE_ABSTRACT_H_