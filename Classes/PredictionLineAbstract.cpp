#include "PredictionLineAbstract.h"

PredictionLineAbstract::PredictionLineAbstract(float _pointOffset) :pointOffset(_pointOffset){

}

void PredictionLineAbstract::updatePredictionLine(float val){
	for (PredictionLineNode *predLineNode : predLinesNodes){
		predLineNode->lineLength += val;
		changeNodeEvent(predLineNode);
	}

	deleteBeginNodes();
	addBeginNodes();

	deleteEndNodes();
	addEndNodes();
}

void PredictionLineAbstract::setControlPoints(std::vector<Vec2> _controlPoints){
	if (controlPoints == _controlPoints){
		return;
	}
	controlPoints = _controlPoints;

	
	deleteBeginNodes();
	addBeginNodes();

	deleteEndNodes();
	addEndNodes();

	for (PredictionLineNode *predLineNode : predLinesNodes){
		changeNodeEvent(predLineNode);
	}
}
std::vector<Vec2> PredictionLineAbstract::getControlPoints(){
	return controlPoints;
}

float PredictionLineAbstract::getMaxLineSize(){
	float ret = 0.0f;

	if (controlPoints.size() < 2){
		ret = -1.0f;
	}
	else {

		ret = 0.0f;

		for (std::vector<Vec2>::iterator controlPointsIter = controlPoints.begin(); controlPointsIter != controlPoints.end() - 1; ++controlPointsIter){
			Vec2 point1 = *controlPointsIter;
			Vec2 point2 = *(controlPointsIter + 1);

			ret += (point1 - point2).length();

		}
	}
	

	return ret;
}

float PredictionLineAbstract::getMinLineSize(){
	return 0.0f;
}


void PredictionLineAbstract::deleteEndNodes(){
	float maxLineSize = getMaxLineSize();

	bool isDeleting = true;
	while (!predLinesNodes.empty() && isDeleting){
		std::list<PredictionLineNode *>::iterator predLinesNodeIter = (--predLinesNodes.end());
		PredictionLineNode *predLineNode = *predLinesNodeIter;
		if (predLineNode->lineLength > maxLineSize){
			deleteNodeEvent(predLineNode);

			delete predLineNode;
			
			predLinesNodes.erase(predLinesNodeIter);
		}
		else {
			isDeleting = false;
		}
	}

}

void PredictionLineAbstract::addEndNodes(){
	float maxLineSize = getMaxLineSize();
	float minLineSize = getMinLineSize();


	if (predLinesNodes.empty()){

		for (float linePosition = maxLineSize; linePosition >= minLineSize; linePosition -= pointOffset){
			PredictionLineNode *predLineNode = new PredictionLineNode();
			predLineNode->lineLength = linePosition;
			predLinesNodes.insert(predLinesNodes.begin(), predLineNode);

			addNodeEvent(predLineNode);
		}

	}
	else {
		for (float maxLinePosition = (*predLinesNodes.rbegin())->lineLength + pointOffset; maxLinePosition <= maxLineSize; maxLinePosition += pointOffset){
			PredictionLineNode *predLineNode = new PredictionLineNode();
			predLineNode->lineLength = maxLinePosition;
			predLinesNodes.insert(predLinesNodes.end(), predLineNode);

			addNodeEvent(predLineNode);
		}
	}

}

void PredictionLineAbstract::deleteBeginNodes(){
	float minLineSize = getMinLineSize();

	bool isDeleting = true;
	while (!predLinesNodes.empty() && isDeleting){
		std::list<PredictionLineNode *>::iterator predLinesNodeIter = predLinesNodes.begin();
		PredictionLineNode *predLineNode = *predLinesNodeIter;
		if (predLineNode->lineLength < minLineSize){
			deleteNodeEvent(predLineNode);
			delete predLineNode;
			predLinesNodes.erase(predLinesNodeIter);
		}
		else {
			isDeleting = false;
		}
	}
}

void PredictionLineAbstract::addBeginNodes(){
	float minLineSize = getMinLineSize();
	float maxLineSize = getMaxLineSize();

	if (predLinesNodes.empty()){

		for (float linePosition = minLineSize; linePosition <= maxLineSize; linePosition += pointOffset){
			PredictionLineNode *predLineNode = new PredictionLineNode();
			predLineNode->lineLength = linePosition;
			predLinesNodes.insert(predLinesNodes.end(), predLineNode);
			
			addNodeEvent(predLineNode);
		}

	}
	else {
		for (float minLinePosition = (*predLinesNodes.begin())->lineLength - pointOffset; minLinePosition >= minLineSize; minLinePosition -= pointOffset){
			PredictionLineNode *predLineNode = new PredictionLineNode();
			predLineNode->lineLength = minLinePosition;
			predLinesNodes.insert(predLinesNodes.begin(), predLineNode);

			addNodeEvent(predLineNode);
		}
	}
}

Vec2 PredictionLineAbstract::getVec2ForLinePosition(float lineLength){
	assert(controlPoints.size() >= 2);
	
	float maxLineSize = getMaxLineSize();
	float minLineSize = getMinLineSize();



	Vec2 pointAPosition;
	Vec2 pointBPosition;

	float pointALineLength;
	float pointBLineLength;

	if (lineLength < minLineSize){
		pointAPosition = *controlPoints.begin();
		pointBPosition = *(controlPoints.begin() + 1);

		pointALineLength = minLineSize;
		pointBLineLength = pointALineLength + (pointAPosition - pointBPosition).length();
	}
	else if(lineLength > maxLineSize){
		pointAPosition = *(controlPoints.end() - 1);
		pointBPosition = *(controlPoints.end() - 2);

		pointBLineLength = maxLineSize;
		pointALineLength = maxLineSize - (pointAPosition - pointBPosition).length();
	}
	else {
		std::vector<Vec2>::iterator controlPointsIter = controlPoints.begin();

		pointAPosition;
		pointBPosition = *controlPointsIter;
		pointALineLength;
		pointBLineLength = minLineSize;

		do {
			controlPointsIter++;

			pointAPosition = pointBPosition;
			pointBPosition = *(controlPointsIter);

			pointALineLength = pointBLineLength;
			pointBLineLength = pointALineLength + (pointAPosition - pointBPosition).length();
		} while (!(pointALineLength <= lineLength && lineLength <= pointBLineLength));
	
	}
	

	return pointAPosition + (pointBPosition - pointAPosition).getNormalized()*(lineLength - pointALineLength);
}