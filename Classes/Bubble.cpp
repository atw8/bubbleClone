#include "Bubble.h"

#include <vector>
#include <random>

Bubble::Bubble(BubbleType _bubbleType, BubbleType _bubbleTypeWitchFog, Dec2 _parent){
	switch (_bubbleType) {
	case BubbleType::WITCH:
		setBubbleTypeWitch(_bubbleTypeWitchFog, _parent);
		break;
	case BubbleType::FOG:
		setBubbleTypeFog(_bubbleTypeWitchFog);
		break;
	default:
		setBubbleType(_bubbleType, _parent);
		break;
	}

	
}

Bubble::Bubble(BubbleType _bubbleType, BubbleType _bubbleTypeWitchFog):Bubble(_bubbleType, _bubbleTypeWitchFog, Dec2(0, 0)){
	
}

Bubble::Bubble(BubbleType _bubbleType, Dec2 _parent):Bubble(_bubbleType, BubbleType::UNDEFINED, _parent){
}

Bubble::Bubble(BubbleType _bubbleType) : Bubble(_bubbleType, Dec2(0, 0)){

}
Bubble::Bubble():Bubble(BubbleType::UNDEFINED){

}

BubbleType Bubble::getBubbleType() const{
	return bubbleType;
}
void Bubble::setBubbleType(BubbleType _bubbleType, Dec2 _parent){
	bubbleType = _bubbleType;
	bubbleTypeWitchFog = BubbleType::UNDEFINED;

	parent = _parent;
}

void Bubble::setBubbleTypeWitch(BubbleType _bubbleTypeWitch, Dec2 _parent){
	bubbleType = BubbleType::WITCH;
	bubbleTypeWitchFog = _bubbleTypeWitch;

	parent = _parent;
}
void Bubble::setBubbleTypeFog(BubbleType _bubbleTypeFog) {
	bubbleType = BubbleType::FOG;
	bubbleTypeWitchFog = _bubbleTypeFog;

	parent = Dec2(0, 0);
}

BubbleType Bubble::getBubbleTypeWitchFog() const{
	return bubbleTypeWitchFog;
}

Dec2 Bubble::getParent() const{
	return parent;
}


bool Bubble::operator==(const Bubble& b) const {
	return this->getBubbleType() == b.getBubbleType() && this->getBubbleTypeWitchFog() == b.getBubbleTypeWitchFog() && this->getParent() == b.getParent();
}

bool Bubble::operator<(const Bubble& b) const {
	bool ret;
	if (this->getBubbleType() == b.getBubbleType()) {
		if (this->getBubbleTypeWitchFog() == b.getBubbleTypeWitchFog()) {
			ret = this->getParent() < b.getParent();
		}
		else {
			ret = this->getBubbleTypeWitchFog() < b.getBubbleTypeWitchFog();
		}
	}
	else {
		ret = this->getBubbleType() < b.getBubbleType();
	}

	return ret;
}


std::vector<BubbleType> basicBubbleTypesMaker() {
	std::vector<BubbleType> _basicBubbleTypes;
	_basicBubbleTypes.push_back(BubbleType::RED);
	_basicBubbleTypes.push_back(BubbleType::BLUE);
	_basicBubbleTypes.push_back(BubbleType::GREEN);
	_basicBubbleTypes.push_back(BubbleType::PURPLE);
	_basicBubbleTypes.push_back(BubbleType::YELLOW);

	return _basicBubbleTypes;
}
const static std::vector<BubbleType> basicBubbleTypes(basicBubbleTypesMaker());

BubbleType Bubble::getRandomBasicBubbleType(){
	
	static std::uniform_int_distribution<int> dist(0, basicBubbleTypes.size() - 1);
	static std::default_random_engine mt;

	return basicBubbleTypes[dist(mt)];
}

const std::vector<BubbleType> & Bubble::getBasicBubbleTypes() {
	return basicBubbleTypes;
}