#include "FieldAbstract.h"
#include "Global.h"

#include "ValueMap.h"

#include <assert.h>


#define Z_BUBBLE_UNDEFINED 0;
#define Z_BUBBLE_OTHER 1

float FieldAbstract::calculateInnerRadius(float fieldWidth, int bubbleWidth){
	return (fieldWidth / bubbleWidth) / 2.0f;
}
float FieldAbstract::calculateOuterRadius(float fieldWidth, int bubbleWidth){
	return calculateInnerRadius(fieldWidth, bubbleWidth) * (2.0f / std::sqrt(3.0f));
}


FieldAbstract::FieldAbstract(Rect _fieldRect, int _bubbleWidth) :fieldRect(_fieldRect), bubbleWidth(_bubbleWidth){
	modelBubblesVisible = false;
}

FieldAbstract::~FieldAbstract(){
}



bool FieldAbstract::init(){
	if (!Layer::init()){
		return false;
	}


	hasTopBoundary = true;
	hasBottomBoundary = false;

	topBoundary = 0;
	bottomBoundary = -10;

	setBubbleWidth(bubbleWidth, true);


	return true;
	
}



std::vector<Dec2> FieldAbstract::getRowDec2s(int y){
	int offset = isLongLRow(y) ? 0 : 1;

	std::vector<Dec2> rowDec2s;
	for (int i = leftBoundary + offset; i <= rightBoundary - offset; i += 2){
		Dec2 dec2(i, y);
		rowDec2s.push_back(dec2);
	}

	return rowDec2s;
}

bool FieldAbstract::isLongLRow(int y){
	return std::abs(y) % 2 == std::abs(topBoundary) % 2;
}

float FieldAbstract::getOuterRadius(){
	return outerRadius;
}
float FieldAbstract::getInnerRadius(){
	return innerRadius;
}




bool FieldAbstract::isDec2Valid(std::vector<Dec2> dec2s){
	bool ret = true;

	for (std::vector<Dec2>::iterator dec2sIter = dec2s.begin(); dec2sIter != dec2s.end() && ret; ++dec2sIter){
		ret = isDec2Valid(*dec2sIter);
	}

	return ret;
}


bool FieldAbstract::isDec2Valid(Dec2 dec2){
	bool ret = (dec2.x >= leftBoundary && dec2.x <= rightBoundary);
	ret = ret && (!hasTopBoundary || dec2.y <= topBoundary);
	ret = ret && (!hasBottomBoundary || dec2.y >= bottomBoundary);

	if (ret){

		dec2.x = std::abs(dec2.x);
		dec2.y = std::abs(dec2.y);



		if (isLongLRow(dec2.y)){
			ret = dec2.x % 2 == 0;
		}
		else {
			ret = dec2.x % 2 == 1;
		}



	}


	return ret;
}


Vec2 FieldAbstract::getVec2ForDec2(Dec2 dec2){
	Vec2 vec2(getVec2XForDec2X(dec2.x), getVec2YForDec2Y(dec2.y));
	return vec2;
}

float FieldAbstract::getVec2YForDec2Y(int dec2Y){
	float vec2Y = (fieldRect.origin.y + fieldRect.size.height - outerRadius) + ((float)dec2Y - (float)topBoundary)*std::sqrt(3.0f)*innerRadius;

	return vec2Y;
}
float FieldAbstract::getVec2XForDec2X(int dec2X){
	float vec2X = (fieldRect.origin.x + innerRadius) + ((float)dec2X - (float)leftBoundary)*innerRadius;

	return vec2X;
}


Dec2 FieldAbstract::getDec2ForVec2(Vec2 vec2){
	float dec2X = (float)leftBoundary + (vec2.x - fieldRect.origin.x - innerRadius) / innerRadius;
	float dec2Y = (float)topBoundary + (vec2.y - fieldRect.origin.y - fieldRect.size.height + outerRadius) / (std::sqrt(3.0f)*innerRadius);

	std::list<Dec2> dec2s;
	dec2s.push_back(Dec2(std::ceil(dec2X), std::ceil(dec2Y)));
	dec2s.push_back(Dec2(std::ceil(dec2X), std::floor(dec2Y)));
	dec2s.push_back(Dec2(std::floor(dec2X), std::ceil(dec2Y)));
	dec2s.push_back(Dec2(std::floor(dec2X), std::floor(dec2Y)));

	for (std::list<Dec2>::iterator dec2sIter = dec2s.begin(); dec2sIter != dec2s.end();){
		Dec2 dec2 = *dec2sIter;
		if (!isDec2Valid(dec2)){
			dec2sIter = dec2s.erase(dec2sIter);
		}
		else {
			dec2sIter++;
		}
	}

	assert(!dec2s.empty());
	std::list<Dec2>::iterator dec2sIter = dec2s.begin();
	Dec2 ret = *dec2sIter;
	float minLength = vec2.distanceSquared(getVec2ForDec2(ret));

	dec2sIter++;
	for (; dec2sIter != dec2s.end(); dec2sIter++){
		Dec2 newRet = *dec2sIter;
		float newMinLength = vec2.distanceSquared(getVec2ForDec2(newRet));
		if (newMinLength < minLength){
			ret = newRet;
			minLength = newMinLength;
		}
	}

	return ret;
}


int FieldAbstract::getBubbleWidth(){
	return bubbleWidth;
}
void FieldAbstract::setBubbleWidth(int _bubleWidth, bool force){
	if (bubbleWidth == _bubleWidth && !force){
		return;
	}

	outitGame(false);

	bubbleWidth = _bubleWidth;
	outerRadius = calculateOuterRadius(fieldRect.size.width, bubbleWidth);
	innerRadius = calculateInnerRadius(fieldRect.size.width, bubbleWidth);

	{
		int boundaryOffset;
		if (bubbleWidth % 2 == 1){
			boundaryOffset = 0;
		}
		else {
			if ((bubbleWidth / 2) % 2 == 1){
				boundaryOffset = 1;
			}
			else if ((bubbleWidth / 2) % 2 == 0){
				boundaryOffset = -1;
			}
		}

		leftBoundary = boundaryOffset - (bubbleWidth - 1);
		rightBoundary = boundaryOffset + (bubbleWidth - 1);
	}


	initGame();
}


void FieldAbstract::getLevelBoardMap(rapidjson::Writer<rapidjson::StringBuffer> &writer, bool saveWitch, bool saveFog){
	using namespace rapidjson;

	writer.Key(VALUEMAP_HAS_TOP_BOUNDARY);
	writer.Bool(hasTopBoundary);

	writer.Key(VALUEMAP_HAS_BOTTOM_BOUNDARY);
	writer.Bool(hasBottomBoundary);


	writer.Key(VALUEMAP_TOP_BOUNDARY);
	writer.Int(topBoundary);

	writer.Key(VALUEMAP_BOTTOM_BOUNDARY);
	writer.Int(bottomBoundary);

	writer.Key(VALUEMAP_BUBBLE_WIDTH);
	writer.Int(bubbleWidth);


	writer.Key(VALUEMAP_BOARD_VECTOR);
	writer.StartArray();

	for (std::pair<Dec2, Bubble> modelBubblesPair : modelBubbles){


		Dec2 dec2 = modelBubblesPair.first;
		Bubble bubble = modelBubblesPair.second;

		if (!isDec2Valid(getBubbleContainer(bubble.getBubbleType(), dec2 + bubble.getParent()))){
			continue;
		}

		if (BubbleType::UNDEFINED == bubble.getBubbleType()){
			continue;
		}

		writer.StartObject();

		writer.Key(VALUEMAP_DEC2_X);
		writer.Int(dec2.x);
		writer.Key(VALUEMAP_DEC2_Y);
		writer.Int(dec2.y);
		writer.Key(VALUEMAP_BUBBLE_TYPE);
		writer.Int((int)bubble.getBubbleType());

		Dec2 parent = bubble.getParent();
		if (0 != parent.x){
			writer.Key(VALUEMAP_PARENT_DEC2_X);
			writer.Int(parent.x);
		}

		if (0 != parent.y){
			writer.Key(VALUEMAP_PARENT_DEC2_Y);
			writer.Int(parent.y);
		}

		if (BubbleType::UNDEFINED != bubble.getBubbleTypeWitchFog()){
			bool saveWitchFogBubble = false;
			
			if (BubbleType::WITCH == bubble.getBubbleType()){
				saveWitchFogBubble = saveWitch;
			}
			else if (BubbleType::FOG == bubble.getBubbleType()){
				saveWitchFogBubble = saveFog;
			}

			if (saveWitchFogBubble){
				writer.Key(VALUEMAP_BUBBLE_WITCH_FOG_TYPE);
				writer.Int((int)bubble.getBubbleTypeWitchFog());
			}

		}

		writer.EndObject();
	}

	writer.EndArray();
}

void FieldAbstract::setLevelBoardMap(rapidjson::Document &document){
	outitGame(true);



	hasTopBoundary = document[VALUEMAP_HAS_TOP_BOUNDARY].GetBool();
	hasBottomBoundary = document[VALUEMAP_HAS_BOTTOM_BOUNDARY].GetBool();

	topBoundary = document[VALUEMAP_TOP_BOUNDARY].GetInt();
	bottomBoundary = document[VALUEMAP_BOTTOM_BOUNDARY].GetInt();

	bubbleWidth = document[VALUEMAP_BUBBLE_WIDTH].GetInt();

	innerRadius = calculateInnerRadius(fieldRect.size.width, bubbleWidth);
	outerRadius = calculateOuterRadius(fieldRect.size.width, bubbleWidth);

	{
		int boundaryOffset;
		if (bubbleWidth % 2 == 1){
			boundaryOffset = 0;
		}
		else {
			if ((bubbleWidth / 2) % 2 == 1){
				boundaryOffset = 1;
			}
			else if ((bubbleWidth / 2) % 2 == 0){
				boundaryOffset = -1;
			}
		}

		leftBoundary = boundaryOffset - (bubbleWidth - 1);
		rightBoundary = boundaryOffset + (bubbleWidth - 1);
	}



	rapidjson::Value &boardVector = document[VALUEMAP_BOARD_VECTOR];


	for (rapidjson::Value::ValueIterator boardVectorIter = boardVector.Begin(); boardVectorIter != boardVector.End(); ++boardVectorIter){
		rapidjson::Value &boardVectorElement = *boardVectorIter;


		int dec2X = boardVectorElement[VALUEMAP_DEC2_X].GetInt();
		int dec2Y = boardVectorElement[VALUEMAP_DEC2_Y].GetInt();
		Dec2 dec2(dec2X, dec2Y);


		BubbleType bubbleType = (BubbleType)boardVectorElement[VALUEMAP_BUBBLE_TYPE].GetInt();

		BubbleType bubbleTypeWitchFog = BubbleType::UNDEFINED;
		rapidjson::Value::MemberIterator boardVectorWitchFogIter = boardVectorElement.FindMember(VALUEMAP_BUBBLE_WITCH_FOG_TYPE);
		if (boardVectorElement.MemberEnd() != boardVectorWitchFogIter){
			bubbleTypeWitchFog = (BubbleType)boardVectorWitchFogIter->value.GetInt();
		}
		else {
			bubbleTypeWitchFog = Bubble::getRandomBasicBubbleType();
		}


		Dec2 parent(0, 0);
		rapidjson::Value::MemberIterator boardVectorParentDec2XIter = boardVectorElement.FindMember(VALUEMAP_PARENT_DEC2_X);
		if (boardVectorElement.MemberEnd() != boardVectorParentDec2XIter){
			parent.x = boardVectorParentDec2XIter->value.GetInt();
		}

		rapidjson::Value::MemberIterator boardVectorParentDec2YIter = boardVectorElement.FindMember(VALUEMAP_PARENT_DEC2_Y);
		if (boardVectorElement.MemberEnd() != boardVectorParentDec2YIter){
			parent.y = boardVectorParentDec2YIter->value.GetInt();
		}


		setBubbleModel(dec2, Bubble(bubbleType, bubbleTypeWitchFog, parent));
	}



	initGame();
}


/*


FieldAbstract *FieldAbstract::create(Rect _fieldRect, int _bubbleWidth){
	FieldAbstract *field = new FieldAbstract(_fieldRect, _bubbleWidth);
	if (field && field->init()){
		field->autorelease();
	}
	else {
		delete field;
		field = nullptr;
	}

	return field;
}
Scene *FieldAbstract::createScene(Rect _fieldRect, int _bubbleWidth){
	Scene *pScene = Scene::create();
	pScene->addChild(FieldAbstract::create(_fieldRect, _bubbleWidth));

	return pScene;
}
*/




bool FieldAbstract::getModelBubblesVisible(){
	return modelBubblesVisible;
}

std::vector<Dec2> FieldAbstract::getBubbleContainer(BubbleType bubbleType, Dec2 dec2){
	std::vector<Dec2> tangents;

	switch (bubbleType){
	case BubbleType::RED:
	case BubbleType::BLUE:
	case BubbleType::GREEN:
	case BubbleType::YELLOW:
	case BubbleType::PURPLE:
	case BubbleType::ICE:
	case BubbleType::WOOD_1:
	case BubbleType::WOOD_2:
	case BubbleType::FOG:
	case BubbleType::FISION:
	case BubbleType::UNDEFINED:
	case BubbleType::ANIMAL1:
	case BubbleType::STONE:
		tangents.push_back(Dec2(0, 0));
		break;
	case BubbleType::ANIMAL3:
	case BubbleType::WITCH:
		tangents.push_back(Dec2(0, 0));
		tangents.push_back(Dec2(-2, 0));
		tangents.push_back(Dec2(-1, -1));
		tangents.push_back(Dec2(1, -1));
		tangents.push_back(Dec2(2, 0));
		tangents.push_back(Dec2(1, 1));
		tangents.push_back(Dec2(-1, 1));
		break;
	case BubbleType::ANIMAL2:
		tangents.push_back(Dec2(0, 0));
		tangents.push_back(Dec2(-1, -1));
		tangents.push_back(Dec2(1, -1));
		break;
	default:
		throw("every bubbleType needs to be defined here");
		break;
	}

	std::vector<Dec2> ret;
	for (Dec2 tangent : tangents){
		ret.push_back(dec2 + tangent);
	}

	return ret;
}


bool FieldAbstract::isRowVisible(int dec2Y){
	return isRowVisible(dec2Y, 0.0f);
}
bool FieldAbstract::isRowVisible(int dec2Y, float offsetY){
	float vec2Y = getVec2YForDec2Y(dec2Y);
	vec2Y += offsetY;
	float vec2X = fieldRect.origin.x + fieldRect.size.width / 2.0f;

	Vec2 vec2(vec2X, vec2Y);

	vec2 = PointApplyTransform(vec2, this->getNodeToWorldTransform());



	return fieldRect.containsPoint(vec2);
}


void FieldAbstract::outitGame(bool completely){

}
void FieldAbstract::initGame(){
	calculateTopBottomModelY();
}



std::set<Dec2> FieldAbstract::getHexagonFrontOfOrder(int order, Dec2 offset){
	order = std::abs(order);


	std::vector<Dec2> lineDec2;
	lineDec2.push_back(Dec2(2 * order, 0));

	for (int i = 0; i < order; i++){
		Dec2 lineEndDec2 = *(lineDec2.end() - 1);
		lineDec2.push_back(lineEndDec2 + Dec2(-1, 1));
	}
	for (int i = 0; i < order - 1; i++){
		Dec2 lineEndDec2 = *(lineDec2.end() - 1);
		lineDec2.push_back(lineEndDec2 + Dec2(-2, 0));
	}

	std::vector<Dec2> tangents;
	tangents.push_back(Dec2(1, 1));
	tangents.push_back(Dec2(1, -1));
	tangents.push_back(Dec2(-1, 1));
	tangents.push_back(Dec2(-1, -1));

	std::set<Dec2> ret;
	for (Dec2 lDec2 : lineDec2){
		for (Dec2 tangent : tangents){
			ret.insert(lDec2*tangent + offset);
		}
	}

	return ret;
}

std::set<Dec2> FieldAbstract::getHexagonFrontOfOrder(int order){
	return getHexagonFrontOfOrder(order, Dec2(0, 0));
}


std::set<Dec2> FieldAbstract::getHexagonOfOrder(int order, Dec2 offset){
	order = std::abs(order);

	std::set<Dec2> ret;
	for (int i = 0; i <= order; i++){
		std::set<Dec2> hexagonFront = getHexagonFrontOfOrder(i, offset);
		ret.insert(hexagonFront.begin(), hexagonFront.end());
	}

	return ret;
}
std::set<Dec2> FieldAbstract::getHexagonOfOrder(int order){
	return getHexagonOfOrder(order, Dec2(0, 0));
}


std::set<Dec2> FieldAbstract::expandSetByHexagon(int order, const std::set<Dec2> &set){
	std::set<Dec2> ret;
	for (Dec2 s : set){
		std::set<Dec2> _ret = getHexagonOfOrder(order, s);
		ret.insert(_ret.begin(), _ret.end());
	}

	return ret;
}

void FieldAbstract::setBubbleModel(Dec2 dec2, Bubble bubble){
	BubbleType bubbleType = bubble.getBubbleType();

	if (BubbleType::UNDEFINED != bubbleType){
		std::map<Dec2, Bubble>::iterator modelBubblesIter = modelBubbles.find(dec2);

		if (modelBubblesIter == modelBubbles.end()){
			bool isPivot = (dec2.y == topBoundary) || (BubbleType::WITCH == bubble.getBubbleType());
			if (bubbleType >= BubbleType::FIRST_ANIMAL_TYPE && bubbleType <= BubbleType::LAST_ANIMAL_TYPE){
				isPivot = false;
			}

			fallGraph.addVert(dec2, isPivot);
			std::set<Dec2> frontHexagons = getDefinedHexagons(getHexagonFrontOfOrder(1, dec2));
			for (Dec2 frontHexagon : frontHexagons){
				fallGraph.addEdge(frontHexagon, dec2);
			}
		}

		std::map<Bubble, int>::iterator bubblesNumMapIter = bubblesNumMap.find(bubble);
		if (bubblesNumMap.end() == bubblesNumMapIter) {
			bubblesNumMap[bubble] = 1;
		}
		else {
			bubblesNumMap[bubble]++;
		}
	}


	modelBubbles[dec2] = bubble;
}

void FieldAbstract::eraseBubbleModel(Dec2 dec2){
	std::map<Dec2, Bubble>::iterator modelBubblesIter = modelBubbles.find(dec2);
	Bubble bubble = modelBubblesIter->second;
	
	modelBubbles.erase(modelBubblesIter);

	if (BubbleType::UNDEFINED != bubble.getBubbleType()){
		fallGraph.removeVert(dec2);

		bubblesNumMap[bubble]--;
		if (bubblesNumMap[bubble] == 0) {
			bubblesNumMap.erase(bubblesNumMap.find(bubble));
		}
	}


}

std::set<Dec2> FieldAbstract::getHelperHexagons(const std::set<Dec2> &dec2s, const std::function<bool(Dec2)> &helperFunc){
	std::set<Dec2> ret;
	for (Dec2 dec2 : dec2s){
		if (helperFunc(dec2)){
			ret.insert(dec2);
		}
	}

	return ret;
}

std::vector<Dec2> FieldAbstract::getHelperHexagons(const std::vector<Dec2> &dec2s, const std::function<bool(Dec2)> &helperFunc){
	std::vector<Dec2> ret;
	for (Dec2 dec2 : dec2s){
		if (helperFunc(dec2)){
			ret.push_back(dec2);
		}
	}

	return ret;
}

std::set<Dec2> FieldAbstract::getUndefinedHexagons(const std::set<Dec2> &dec2s){
	std::function<bool(Dec2)> helperFunc = [this](Dec2 dec2){
		return modelBubbles.find(dec2) == modelBubbles.end();
	};

	return getHelperHexagons(dec2s, helperFunc);
}

std::vector<Dec2> FieldAbstract::getDefinedHexagons(const std::vector<Dec2> &dec2s){
	std::function<bool(Dec2)> helperFunc = getDefinedHexagonsHelperFunc();

	return getHelperHexagons(dec2s, helperFunc);
}

std::set<Dec2> FieldAbstract::getDefinedHexagons(const std::set<Dec2> &dec2s){
	std::function<bool(Dec2)> helperFunc = getDefinedHexagonsHelperFunc();

	return getHelperHexagons(dec2s, helperFunc);
}

std::function<bool(Dec2)> FieldAbstract::getDefinedHexagonsHelperFunc(){
	std::function<bool(Dec2)> helperFunc = [this](Dec2 dec2){
		return modelBubbles.find(dec2) != modelBubbles.end();
	};

	return helperFunc;
}


float FieldAbstract::getBackTopY() {
	float y;

	if (hasTopBoundary) {
		y = getVec2YForDec2Y(topBoundary) + outerRadius;
	}
	else {
		Vec2 vec2 = fieldRect.origin + Vec2(fieldRect.size.width / 2.0f, fieldRect.size.height);
		vec2 = PointApplyTransform(vec2, this->getWorldToNodeTransform());

		if (modelBubblesVisible) {
			float _y = getVec2YForDec2Y(topModelY) + outerRadius;
			if (_y > vec2.y) {
				vec2.y = _y;
			}
		}

		y = vec2.y;
	}

	return y;
}
float FieldAbstract::getBackBottomY() {
	float y;

	if (hasBottomBoundary) {
		y = getVec2YForDec2Y(bottomBoundary) - outerRadius;
	}
	else {
		Vec2 vec2 = fieldRect.origin + Vec2(fieldRect.size.width / 2.0f, 0.0f);
		vec2 = PointApplyTransform(vec2, this->getWorldToNodeTransform());

		if (modelBubblesVisible) {
			float _y = getVec2YForDec2Y(bottomModelY) - outerRadius;
			if (_y < vec2.y) {
				vec2.y = _y;
			}
		}

		y = vec2.y;
	}

	return y;
}


int FieldAbstract::getBubbleNum(Bubble bubble) {
	std::map<Bubble, int>::iterator bubblesNumMapIter = bubblesNumMap.find(bubble);

	int ret;
	if (bubblesNumMap.end() == bubblesNumMapIter) {
		ret = 0;
	}
	else {
		ret = bubblesNumMap[bubble];
	}

	return ret;
}

int FieldAbstract::getBubbleNumBasic(BubbleType bubbleTypeBasic) {
	Bubble bubble(bubbleTypeBasic);
	Bubble bubbleWitch(BubbleType::WITCH, bubbleTypeBasic);
	Bubble bubbleFog(BubbleType::FOG, bubbleTypeBasic);


	return getBubbleNum(bubble) + getBubbleNum(bubbleWitch) + getBubbleNum(bubbleFog);
}

int FieldAbstract::getBubbleNumWitch(){
	int ret = 0;
	
	for (int i = (int)BubbleType::FIRST_BASIC_BUBBLE_TYPE; i <= (int)BubbleType::LAST_BASIC_BUBBLE_TYPE; i++){
		BubbleType bubbleType = (BubbleType)i;

		ret += getBubbleNum(Bubble(BubbleType::WITCH, bubbleType));
	}

	return ret;
}

int FieldAbstract::getBubbleNumAnimal() {
	int ret = 0;

	ret += FieldAbstract::getBubbleNum(Bubble(BubbleType::ANIMAL1));
	ret += FieldAbstract::getBubbleNum(Bubble(BubbleType::ANIMAL2));
	ret += FieldAbstract::getBubbleNum(Bubble(BubbleType::ANIMAL3));

	return ret;
}



int FieldAbstract::getNumOfEmptyElementsInTopRow(){
	assert(hasTopBoundary);

	return getNumOfEmptyElementsInRow(topBoundary);
}
int FieldAbstract::getNumOfEmptyElementsInBottomRow(){
	assert(hasBottomBoundary);

	return getNumOfEmptyElementsInRow(bottomBoundary);
}

int FieldAbstract::getNumOfEmptyElementsInRow(int row){
	std::vector<Dec2> rowDec2s = getRowDec2s(row);

	int ret = 0;
	for (Dec2 rowDec2 : rowDec2s){
		std::map<Dec2, Bubble>::iterator modelBubblesIter = modelBubbles.find(rowDec2);
		if (modelBubbles.end() == modelBubblesIter){
			ret++;
		}
	}

	return ret;
}





