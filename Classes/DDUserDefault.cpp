#include "DDUserDefault.h"

#include <ctime>

#include "cocos2d.h"
USING_NS_CC;

DDUserDefault::DDUserDefault(){

}

DDUserDefault *DDUserDefault::getInstance(){
	static DDUserDefault gInstance;

	return &gInstance;
}


GameMode DDUserDefault::getDesignerGameMode(){
	return GameMode(getDesignerGameModeType(), getDesignerGameModeAmount(), 0);
}
void DDUserDefault::setDesignerGameMode(GameMode gameMode){
	setDesignerGameModeType(gameMode.getGameModeType());
	setDesignerGameModeAmount(gameMode.getGameModeAmount());
}

GameModeType DDUserDefault::getDesignerGameModeType(){
	return (GameModeType)UserDefault::getInstance()->getIntegerForKey(getDesignerGameModeTypeStr().c_str(), (int)GameModeType::MOVE);
}
void DDUserDefault::setDesignerGameModeType(GameModeType gameModeType){
	UserDefault::getInstance()->setIntegerForKey(getDesignerGameModeTypeStr().c_str(), (int)gameModeType);
}
std::string DDUserDefault::getDesignerGameModeTypeStr(){
	return "designerGameModeType";
}


int DDUserDefault::getDesignerGameModeAmount(){
	return UserDefault::getInstance()->getIntegerForKey(getDesignerGameModeAmountStr().c_str(), 10);
}
void DDUserDefault::setDesignerGameModeAmount(int amount){
	UserDefault::getInstance()->setIntegerForKey(getDesignerGameModeAmountStr().c_str(), amount);
}
std::string DDUserDefault::getDesignerGameModeAmountStr(){
	return "designerGameModeAmountStr";
}


int DDUserDefault::getDesignerBubbleWidth(){
	return UserDefault::getInstance()->getIntegerForKey(getDesignerBubbleWidthStr().c_str(), 4);
}
void DDUserDefault::setDesignerBubbleWidth(int _bubbleWidth){
	UserDefault::getInstance()->setIntegerForKey(getDesignerBubbleWidthStr().c_str(), _bubbleWidth);
}
std::string DDUserDefault::getDesignerBubbleWidthStr(){
	return "designerBubbleWidthStr";
}







std::vector<ShopStruct> DDUserDefault::getShopStructs(){
	std::vector<ShopStruct> ret;

	ret.push_back(ShopStruct(100, 20));
	
	ret.push_back(ShopStruct(400, 101));

	ret.push_back(ShopStruct(102, 2002));
	ret.push_back(ShopStruct(1000, 3000));

	ret.push_back(ShopStruct(100, 20));
	ret.push_back(ShopStruct(400, 101));
	ret.push_back(ShopStruct(102, 2002));
	ret.push_back(ShopStruct(1000, 3000));
	ret.push_back(ShopStruct(0, 5000));

	return ret;
}


int DDUserDefault::getNumOfGold(){
	return UserDefault::getInstance()->getIntegerForKey(getNumOfGoldStr().c_str(), 0);
}
void DDUserDefault::addNumOfGold(int val){
	setNumOfGold(getNumOfGold() + val);
}

void DDUserDefault::setNumOfGold(int _gold){
	UserDefault::getInstance()->setIntegerForKey(getNumOfGoldStr().c_str(), _gold);

	NotificationCenter::getInstance()->postNotification(NOTIFICATION_setNumOfGold);
}
std::string DDUserDefault::getNumOfGoldStr(){
	return "numOfGoldStr";
}


int DDUserDefault::getPropCost(PropType propType){
	int ret;

	switch (propType){
	case PropType::ADD_RESERVE:
		ret = 10;
		break;
	case PropType::ADD_TIMEMOVE:
		ret = 12;
		break;
	case PropType::CHROME:
		ret = 13;
		break;
	}

	return ret;
}




bool DDUserDefault::getIsMusicPaused() {
	return UserDefault::getInstance()->getBoolForKey(getIsMusicPausedStr().c_str(), false);
}
void DDUserDefault::setIsMusicPaused(bool _isMusicPaused) {
	UserDefault::getInstance()->setBoolForKey(getIsMusicPausedStr().c_str(), _isMusicPaused);
}

bool DDUserDefault::getIsSoundPaused() {
	return UserDefault::getInstance()->getBoolForKey(getIsSoundPausedStr().c_str(), false);
}
void DDUserDefault::setIsSoundPaused(bool _isSoundPaused) {
	UserDefault::getInstance()->setBoolForKey(getIsSoundPausedStr().c_str(), _isSoundPaused);
}

std::string DDUserDefault::getIsMusicPausedStr() {
	return "isMusicPaused";
}
std::string DDUserDefault::getIsSoundPausedStr() {
	return "isSoundPaused";
}



int DDUserDefault::getLevelFrontier(){
	return UserDefault::getInstance()->getIntegerForKey(getLevelFrontierStr().c_str(), 1);
}

void DDUserDefault::setLevelFrontier(int _levelFrontier){
	UserDefault::getInstance()->setIntegerForKey(getLevelFrontierStr().c_str(), std::max(getLevelFrontier(), _levelFrontier));
}


std::string DDUserDefault::getLevelFrontierStr(){
	return "levelFrontierStr";
}