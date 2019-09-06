#include "FieldDesign.h"

#include "ValueMap.h"

#define Z_BUBBLE_UNDEFINED 0;
#define Z_BUBBLE_OTHER 1


FieldDesign::FieldDesign(Rect _fieldRect, int _bubbleWidth):FieldAbstract(_fieldRect, _bubbleWidth){

}


FieldDesign *FieldDesign::create(Rect _fieldRect, int _bubbleWidth){
	FieldDesign *fieldDesign = new FieldDesign(_fieldRect, _bubbleWidth);
	if (fieldDesign && fieldDesign->init()){
		fieldDesign->autorelease();
	}
	else {
		delete fieldDesign;
		fieldDesign = nullptr;
	}

	return fieldDesign;
}





void FieldDesign::addBottomRows(){
	assert(!fieldBubbleView.empty());

	while ((isRowVisible(bottomViewY - 1, addRemoveRowsOffset) || isRowVisible(bottomViewY - 1)) && ((bottomViewY - 1) >= bottomBoundary || !hasBottomBoundary)){
		addRow(bottomViewY - 1);
	}
}
void FieldDesign::addTopRows(){
	assert(!fieldBubbleView.empty());

	while ((isRowVisible(topViewY + 1, -addRemoveRowsOffset) || isRowVisible(topViewY + 1)) && ((topViewY + 1) <= topBoundary || !hasTopBoundary)){
		addRow(topViewY + 1);
	}
}

void FieldDesign::removeBottomRows(){
	if (fieldBubbleView.empty()) {
		return;
	}

	while (!isRowVisible(bottomViewY, addRemoveRowsOffset) && PointApplyTransform(Vec2(0, getVec2YForDec2Y(bottomViewY) + addRemoveRowsOffset), this->getNodeToWorldTransform()).y <= fieldRect.origin.y + fieldRect.size.height){
		removeRow(bottomViewY);
	}
}
void FieldDesign::removeTopRows(){
	if (fieldBubbleView.empty()) {
		return;
	}

	while (!isRowVisible(topViewY, -addRemoveRowsOffset) && PointApplyTransform(Vec2(0, getVec2YForDec2Y(topViewY) - addRemoveRowsOffset), this->getNodeToWorldTransform()).y >= fieldRect.origin.y){
		removeRow(topViewY);
	}
}



void FieldDesign::addRow(int y){
	if (fieldBubbleView.empty()){
		topViewY = bottomViewY = y;
	}
	else {
		if (y > topViewY){
			assert(y == topViewY + 1);
			topViewY = y;
		}

		if (y < bottomViewY){
			assert(y == bottomViewY - 1);
			bottomViewY = y;
		}
	}

	std::vector<Dec2> rowDec2s = getRowDec2s(y);

	for (std::vector<Dec2>::iterator rowDec2sIter = rowDec2s.begin(); rowDec2sIter != rowDec2s.end(); ++rowDec2sIter){
		Dec2 dec2 = *rowDec2sIter;

		Bubble bubble;
		{
			std::map<Dec2, Bubble>::iterator modelBubblesIter = modelBubbles.find(dec2);

			if (modelBubblesIter != modelBubbles.end()){
				Bubble newBubble = modelBubblesIter->second;
				std::vector<Dec2> newBubbleContainer = getBubbleContainer(newBubble.getBubbleType(), dec2 + newBubble.getParent());
				if (isDec2Valid(newBubbleContainer)){
					bubble = newBubble;
				}
			}
		}


		std::map<Dec2, BubbleDesigner *>::iterator fieldBubbleViewIter = fieldBubbleView.find(dec2);
		BubbleDesigner *bubbleDesigner;

		if (fieldBubbleViewIter == fieldBubbleView.end()){
			bubbleDesigner = BubbleDesigner::create(innerRadius, outerRadius, bubble);
			bubbleDesigner->setPosition(getVec2ForDec2(dec2));
			this->addChild(bubbleDesigner);


			fieldBubbleView[dec2] = bubbleDesigner;
		}
		else {
			bubbleDesigner = fieldBubbleViewIter->second;
		}

		setBubbleDesignerBubble(bubbleDesigner, bubble);
	}
}


void FieldDesign::setBubbleDesignerBubble(BubbleDesigner *bubbleDesigner, Bubble bubble){
	bubbleDesigner->setBubble(bubble);

	int zOrder;
	if (BubbleType::UNDEFINED == bubble.getBubbleType() || Dec2(0, 0) != bubble.getParent()){
		zOrder = Z_BUBBLE_UNDEFINED;
	}
	else {
		zOrder = Z_BUBBLE_OTHER;
	}

	bubbleDesigner->setLocalZOrder(zOrder);
}

void FieldDesign::removeRow(int y){
	assert(y == topViewY || y == bottomViewY);

	if (y == topViewY){
		topViewY--;
	}
	else if (y == bottomViewY){
		bottomViewY++;
	}


	std::vector<Dec2> rowDec2s = getRowDec2s(y);
	for (std::vector<Dec2>::iterator rowDec2sIter = rowDec2s.begin(); rowDec2sIter != rowDec2s.end(); ++rowDec2sIter){
		Dec2 dec2 = *rowDec2sIter;

		std::map<Dec2, BubbleDesigner *>::iterator fieldBubbleViewIter = fieldBubbleView.find(dec2);
		if (fieldBubbleViewIter != fieldBubbleView.end()){
			BubbleDesigner *bubbleDesigner = fieldBubbleViewIter->second;
			bubbleDesigner->removeFromParentAndCleanup(true);

			fieldBubbleView.erase(fieldBubbleViewIter);
		}

	}
}


void FieldDesign::initRows(){

	Vec2 vec2 = PointApplyTransform(fieldRect.origin + Vec2(fieldRect.size.width / 2.0f, fieldRect.size.height / 2.0f), this->getWorldToNodeTransform());
	Dec2 dec2 = getDec2ForVec2(vec2);


	addRow(dec2.y);
	addTopRows();
	addBottomRows();
}

void FieldDesign::outitRows(){
	while (fieldBubbleView.empty()){
		return;
	}

	for (int y = bottomViewY; y <= topViewY; y++){
		removeRow(y);
	}
}




void FieldDesign::setBubble(Vec2 vec2, Bubble bubble){
	setBubble(getDec2ForVec2(vec2), bubble);
}
void FieldDesign::setBubble(const Dec2 dec2, Bubble bubble){
	assert(bubble.getParent() == Dec2(0, 0));

	{
		std::map<Dec2, Bubble>::iterator modelBubblesIter = modelBubbles.find(dec2);
		if (modelBubblesIter != modelBubbles.end()){
			Bubble _bubble = modelBubblesIter->second;
			if (bubble == _bubble){
				return;
			}
		}
	}

	std::vector<Dec2> bubbleContainer = getBubbleContainer(bubble.getBubbleType(), dec2);
	if (!isDec2Valid(bubbleContainer)){
		return;
	}

	for (Dec2 bubbleCont : bubbleContainer){
		Bubble bubbleDelete;
		{
			std::map<Dec2, Bubble>::iterator modelBubblesIter = modelBubbles.find(bubbleCont);
			if (modelBubblesIter != modelBubbles.end()){
				bubbleDelete = modelBubblesIter->second;
			}
		}

		if (BubbleType::UNDEFINED != bubbleDelete.getBubbleType()){
			std::vector<Dec2> bubbleContainerDelete = getBubbleContainer(bubbleDelete.getBubbleType(), bubbleCont + bubbleDelete.getParent());
			for (Dec2 bubbleContainrDel : bubbleContainerDelete){
				eraseBubbleModel(bubbleContainrDel);

				BubbleDesigner *bubbleDesigner = fieldBubbleView[bubbleContainrDel];
				setBubbleDesignerBubble(bubbleDesigner, Bubble(BubbleType::UNDEFINED));
			}
		}


	}

	for (Dec2 bubbleCont : bubbleContainer){
		Bubble newBubble = Bubble(bubble.getBubbleType(), bubble.getBubbleTypeWitchFog(), dec2 - bubbleCont);
		if (BubbleType::UNDEFINED != bubble.getBubbleType()){
			setBubbleModel(bubbleCont, newBubble);
		}

		BubbleDesigner *bubbleDesigner = fieldBubbleView[bubbleCont];
		setBubbleDesignerBubble(bubbleDesigner, newBubble);
	}

	calculateTopBottomModelY();
}


void FieldDesign::calculateTopBottomModelY(){

	std::vector<Dec2> fieldBubbleModelDec2s;
	for (std::pair<Dec2, Bubble> modelBubblesPair : modelBubbles){
		Dec2 dec2 = modelBubblesPair.first;
		Bubble bubble = modelBubblesPair.second;

		std::vector<Dec2> bubbleContainer = getBubbleContainer(bubble.getBubbleType(), dec2 + bubble.getParent());
		if (isDec2Valid(bubbleContainer)){
			fieldBubbleModelDec2s.push_back(dec2);
		}
	}

	modelBubblesVisible = !fieldBubbleModelDec2s.empty();


	if (modelBubblesVisible){
		std::vector<Dec2>::iterator fieldBubbleModelDec2sIter = fieldBubbleModelDec2s.begin();
		{
			Dec2 dec2 = *fieldBubbleModelDec2sIter;
			bottomModelY = dec2.y;
			topModelY = dec2.y;
		}

		for (; fieldBubbleModelDec2sIter != fieldBubbleModelDec2s.end(); ++fieldBubbleModelDec2sIter){
			Dec2 dec2 = *fieldBubbleModelDec2sIter;
			if (bottomModelY > dec2.y){
				bottomModelY = dec2.y;
			}

			if (topModelY < dec2.y){
				topModelY = dec2.y;
			}
		}
	}


}



float FieldDesign::getScrollSize(){
	return fieldRect.size.height;
}

float FieldDesign::getScrollPosition(){
	Vec2 vec2 = fieldRect.origin + Vec2(fieldRect.size.width / 2.0f, fieldRect.size.height / 2.0f);
	vec2 = PointApplyTransform(vec2, this->getWorldToNodeTransform());

	return vec2.y;
}

void FieldDesign::setScrollPosition(float scrollPosition){
	float lastScrollPosition = getScrollPosition();
	this->setPositionY(getPositionY() + lastScrollPosition - scrollPosition);
}


void FieldDesign::setPositionY(float positionY){
	float lastPositionY = getPositionY();

	if (hasBottomBoundary) {
		float maxPositionY = -(getBackBottomY() - fieldRect.origin.y);
		positionY = std::min(maxPositionY, positionY);
	}

	if (hasTopBoundary) {
		float minPositionY = -(getBackTopY() - (fieldRect.origin.y + fieldRect.size.height));
		positionY = std::max(minPositionY, positionY);
	}



	Layer::setPositionY(positionY);

	if (positionY > lastPositionY){
		removeTopRows();
		if (!fieldBubbleView.empty()){
			addBottomRows();
		}
		else {
			initRows();
		}
	}
	else {
		removeBottomRows();
		if (!fieldBubbleView.empty()){
			addTopRows();
		}
		else {
			initRows();
		}
	}
}






void FieldDesign::setOldLevelBoardMap(ValueMap oldLevelBoardMap){
	outitGame(true);
	
	hasTopBoundary = true;
	hasBottomBoundary = false;

	topBoundary = 0;
	bottomBoundary = -100;
	bubbleWidth = oldLevelBoardMap[PLIST_WIDTH].asInt();

	std::vector<std::set<Dec2>> containers;
	ValueVector plistContainer = oldLevelBoardMap[PLIST_CONTAINER].asValueVector();
	for (Value plistContainerElem_ : plistContainer){
		ValueVector plistContainerElem = plistContainerElem_.asValueVector();

		std::set<Dec2> subContainer;

		for (Value plistContainerElemElem_ : plistContainerElem){
			ValueMap plistContainerElemElem = plistContainerElemElem_.asValueMap();
			int dec2X = plistContainerElemElem[PLIST_CONTAINER_WIDTH_COORD].asInt() - (bubbleWidth - 1);
			int dec2Y = -plistContainerElemElem[PLIST_CONTAINER_HEIGHT_COORD].asInt();

			Dec2 dec2(dec2X, dec2Y);

			subContainer.insert(dec2);
		}

		containers.push_back(subContainer);
	}

	std::function<Dec2(std::set<Dec2>)> getContainerHead = [](std::set<Dec2> subContainer){
		assert(subContainer.size() == 3 || subContainer.size() == 7);
		Dec2 ret;

		if (subContainer.size() == 3){
			std::set<Dec2>::iterator subContainerIter = subContainer.begin();
			ret = *subContainerIter;
			for (; subContainerIter != subContainer.end(); ++subContainerIter){
				Dec2 newRet = *subContainerIter;
				if (newRet.y > ret.y){
					ret = newRet;
				}
			}
		}
		else if (subContainer.size() == 7){
			Dec2 total;
			for (std::set<Dec2>::iterator subContainerIter = subContainer.begin(); subContainerIter != subContainer.end(); ++subContainerIter){
				total += *subContainerIter;
			}

			ret = total / subContainer.size();
		}

		return ret;
	};

	ValueVector boardVector = oldLevelBoardMap[PLIST_DATA].asValueVector();
	for (Value boardVectorElem_ : boardVector){
		ValueMap boardVectorElem = boardVectorElem_.asValueMap();

		int dec2X = boardVectorElem[PLIST_DATA_WIDTHCOORD].asInt() - (bubbleWidth - 1);
		int dec2Y = -boardVectorElem[PLIST_DATA_HEIGHTCOORD].asInt();
		Dec2 dec2(dec2X, dec2Y);


		BubbleType bubbleType;
		BubbleType bubbleTypeWitch = BubbleType::UNDEFINED;
		Dec2 parent;

		switch (boardVectorElem[PLIST_DATA_BALLSELECTED].asInt()){
		case 3:
			bubbleType = BubbleType::BLUE;
			break;
		case 4:
			bubbleType = BubbleType::GREEN;
			break;
		case 5:
			bubbleType = BubbleType::PURPLE;
			break;
		case 6:
			bubbleType = BubbleType::YELLOW;
			break;
		case 7:
			bubbleType = BubbleType::RED;
			break;
		case 8:
			bubbleType = BubbleType::WOOD_1;
			break;
		case 9:
			bubbleType = BubbleType::ICE;
			break;
		case 10:
			bubbleType = BubbleType::STONE;
			break;
		case 11:
			bubbleType = BubbleType::FOG;
			break;
		case 13:
			bubbleType = BubbleType::ANIMAL1;
			break;
		case 18:
			bubbleType = BubbleType::FISION;
			break;
		case 14:
		{
				   bubbleType = BubbleType::ANIMAL2;
				   bool foundParent = false;
				   for (std::vector<std::set<Dec2> >::iterator containersIter = containers.begin(); containersIter != containers.end() && !foundParent; containersIter++){
					   std::set<Dec2> container = *containersIter;
					   if (container.find(dec2) != container.end()){
						   parent = getContainerHead(container) - dec2;
						   foundParent = true;
					   }
				   }

				   assert(foundParent);
		}
			break;
		case 15:
		{
				   bubbleType = BubbleType::ANIMAL3;
				   bool foundParent = false;
				   for (std::vector<std::set<Dec2> >::iterator containersIter = containers.begin(); containersIter != containers.end() && !foundParent; containersIter++){
					   std::set<Dec2> container = *containersIter;
					   if (container.find(dec2) != container.end()){
						   parent = getContainerHead(container) - dec2;
						   foundParent = true;
					   }
				   }

				   assert(foundParent);
		}
			break;
		case 17:
		{
				   bubbleType = BubbleType::WITCH;
				   bubbleTypeWitch = Bubble::getRandomBasicBubbleType();
				   bool foundParent = false;

				   for (std::vector<std::set<Dec2> >::iterator containersIter = containers.begin(); containersIter != containers.end() && !foundParent; containersIter++){
					   std::set<Dec2> container = *containersIter;
					   if (container.find(dec2) != container.end()){
						   parent = getContainerHead(container) - dec2;
						   foundParent = true;
					   }
				   }

				   assert(foundParent);

		}
			break;
		default:
			CCLOG("bubbleType is %d", boardVectorElem[PLIST_DATA_BALLSELECTED].asInt());
			throw("undefined bubbleType");
			break;
		}

		Bubble bubble(bubbleType, bubbleTypeWitch, parent);

		setBubbleModel(dec2, bubble);
	}


	initGame();
}


void FieldDesign::outitGame(bool completely){
	FieldAbstract::outitGame(completely);

	if (completely){
		while (!modelBubbles.empty()){
			std::map<Dec2, Bubble>::iterator modelBubblesIter = modelBubbles.begin();
			Dec2 dec2 = modelBubblesIter->first;

			eraseBubbleModel(dec2);
		}
	}
	outitRows();
}
void FieldDesign::initGame(){
	//FieldAbstract::initGame();

	addRemoveRowsOffset = outerRadius + 3.0f*std::sqrt(3.0f)*innerRadius + outerRadius;
	
	calculateTopBottomModelY();
	setPositionY(getPositionY());
	initRows();



	std::set<Dec2> headBubbles;


	for (std::pair<Dec2, Bubble> modelBubblesPair : modelBubbles) {
		Dec2 dec2 = modelBubblesPair.first;
		Bubble bubble = modelBubblesPair.second;

		BubbleType bubbleType = bubble.getBubbleType();
		switch (bubbleType) {
		case BubbleType::WITCH:
		case BubbleType::ANIMAL1:
		case BubbleType::ANIMAL2:
		case BubbleType::ANIMAL3:
		{
			if (!isDec2Valid(getBubbleContainer(bubble.getBubbleType(), dec2 + bubble.getParent()))) {
				headBubbles.insert(dec2 + bubble.getParent());
			}

		}

			break;
		}
	}


	for (Dec2 headBubble : headBubbles) {
		Bubble bubble = modelBubbles[headBubble];
		std::vector<Dec2> headBubbleContainer = getBubbleContainer(bubble.getBubbleType(), headBubble + bubble.getParent());

		for (Dec2 headBubb : headBubbleContainer) {
			eraseBubbleModel(headBubb);
		}
	}
}