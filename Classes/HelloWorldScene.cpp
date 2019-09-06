#include "HelloWorldScene.h"

USING_NS_CC;

#include "Global.h"
#include "BubbleView.h"
#include "DDUserDefault.h"

#include "GameModeLayer.h"



#include "ValueMap.h"

#include "GameLayer.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"


#include "LevelLayer.h"


#include "AudioManager.h"


#define SCHEDULE_CHANGE_WITCH_COLOR "SCHEDULE_CHANGE_WITCH_COLOR"
#define SCHEDULE_CHANGE_WITCH_COLOR_INTERVAL 1.5f

#define FIELD_WIDTH_SCROLL_BAR_MIN 5
#define FIELD_WIDTH_SCROLL_BAR_MAX 20


#define FIELD_MOVE_SENSITIVITY 50.0f


#define Z_BACKGROUND -1
#define Z_POPUP_LAYER 100




HelloWorld::HelloWorld(LEVEL_TYPE _levelType, std::string _levelName) :levelType(_levelType), levelName(_levelName),
main_touch(nullptr), pressedBubbleButton(nullptr), selectedBubbleButton(nullptr), touchType(TouchType::UNDEFINED), fieldMoveSensitivity(FIELD_MOVE_SENSITIVITY),
gameMode(DDUserDefault::getInstance()->getDesignerGameMode()){
	
}

Scene* HelloWorld::createScene(LEVEL_TYPE _levelType, std::string _levelName){
    Scene *scene = Scene::create();
	scene->addChild(HelloWorld::create(_levelType, _levelName));

    return scene;
}

HelloWorld *HelloWorld::create(LEVEL_TYPE _levelType, std::string _levelName){
	HelloWorld *helloWorld = new HelloWorld(_levelType, _levelName);
	if (helloWorld && helloWorld->init()){
		helloWorld->autorelease();
	}
	else {
		delete helloWorld;
		helloWorld = nullptr;
	}

	return helloWorld;
}

/*

void getLeftRightBoundary(){

	for (int bubbleWidth = 1; bubbleWidth < 20; bubbleWidth++){
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


		int leftBoundary, rightBoundary;
		leftBoundary = boundaryOffset - (bubbleWidth - 1);
		rightBoundary = boundaryOffset + (bubbleWidth - 1);


		CCLOG("%d the left right boundary is %d %d", bubbleWidth, leftBoundary, rightBoundary);
	}

}
*/

void HelloWorld::onEnter(){
	Layer::onEnter();


	keyboardListener->onKeyPressed = std::bind(&HelloWorld::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
	keyboardListener->onKeyReleased = std::bind(&HelloWorld::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);

	loadArmature();

	





	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = std::bind(&HelloWorld::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchMoved = std::bind(&HelloWorld::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchEnded = std::bind(&HelloWorld::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchCancelled = std::bind(&HelloWorld::onTouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	background = Sprite::create("shared/background_1.png");
	background->setAnchorPoint(Vec2(0.5f, 0.0f));
	background->setPosition(Vec2(getContWidth(this) / 2.0f, 0.0f));
	background->setScaleX(getContWidth(this) / getContWidth(background));
	if (getContHeight(background) < getContHeight(this)){
		background->setScaleY(getContHeight(this) / getContHeight(background));
	}
	this->addChild(background, Z_BACKGROUND);



	Rect topBoundaryRect;
	fieldRect;
	{
		Vec2 topBoundaryOrigin = Vec2(0.0f, getContHeight(this) - 120.0f);
		Size topBoundarySize = Size(getContWidth(this), 120.0f);
		topBoundaryRect = Rect(topBoundaryOrigin, topBoundarySize);

		Vec2 fieldRectOrigin = PointApplyTransform(Vec2(0.0f, 380), background->getNodeToWorldTransform()) + Vec2(0.0f, 220.0f);
		Size fieldRectSize = Size(getContWidth(this) - 40.0f, topBoundaryRect.origin.y - fieldRectOrigin.y);
		fieldRect = Rect(fieldRectOrigin, fieldRectSize);
	}
	topBoundary = TopBoundary::create(topBoundaryRect);
	this->addChild(topBoundary);

	//Add the backButton
	backBtn = Sprite::create("backBtn.png");
	backBtn->setPosition(Vec2(getContWidth(backBtn) / 2.0f, getContHeight(this) - getContHeight(backBtn) / 2.0f) + 0.0f*Vec2(1.0f, -1.0f));
	this->addChild(backBtn);


	//Add the levelName crap
	std::function<std::string(std::string)> normalizeLevelNameLabel = [](std::string normStr) {
		std::string ret;
		for (char c : normStr) {
			if (c >= '0' && c <= '9' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c == '.') {
				ret.push_back(c);
			}
		}

		return ret;
	};
	Size levelNameLabelSize(550, 70);
	levelNameLabel = CustomNumber::createWithPlace(levelNameLabelSize, 40, normalizeLevelNameLabel, nullptr);
	levelNameLabel->setPosition(Vec2(((backBtn->getPositionX() + getContWidth(backBtn) / 2.0f) + getContWidth(this)) / 2.0f, backBtn->getPositionY()));
	this->addChild(levelNameLabel);
	levelNameLabel->setString("");


	//Add the fieldDesign
	fieldDesign = FieldDesign::create(fieldRect, DDUserDefault::getInstance()->getDesignerBubbleWidth());
	
	ClippingNode *fieldClip = ClippingNode::create();
	{
		DrawNode *clippingNodeStencil = DrawNode::create();
		std::vector<Vec2> clippingNodeStencilVec2;
		clippingNodeStencilVec2.push_back(fieldRect.origin + Vec2(0.0f, 0.0f));
		clippingNodeStencilVec2.push_back(fieldRect.origin + Vec2(fieldRect.size.width, 0.0f));
		clippingNodeStencilVec2.push_back(fieldRect.origin + Vec2(fieldRect.size.width, fieldRect.size.height));
		clippingNodeStencilVec2.push_back(fieldRect.origin + Vec2(0.0f, fieldRect.size.height));
		clippingNodeStencil->drawSolidPoly(&clippingNodeStencilVec2[0], clippingNodeStencilVec2.size(), Color4F::WHITE);

		fieldClip->setStencil(clippingNodeStencil);
		fieldClip->addChild(fieldDesign);
	}
	this->addChild(fieldClip);


	//THE SCROLL BAR CODE
	Vec2 scrollRectOrigin = fieldRect.origin + Vec2(fieldRect.size.width, 0.0f);
	Size scrollRectSize = Size(getContWidth(this) - fieldRect.size.width, fieldRect.size.height);
	
	scrollRect = Rect(scrollRectOrigin, scrollRectSize);

	scrollBar = ScrollBar::create(fieldDesign->getBackTopY(), fieldDesign->getBackBottomY(), fieldDesign->getScrollSize(), fieldDesign->getScrollPosition(), Color4F::BLACK, Color4F::WHITE);
	scrollBar->setPosition(scrollRectOrigin + Vec2(scrollRectSize.width / 2.0f, scrollRectSize.height / 2.0f));
	scrollBar->setContentSize(scrollRectSize);
	this->addChild(scrollBar);

	
	class BubbleButtonPosition {
	public:
		BubbleButtonPosition(Bubble _bubble, Vec2 _vec2Position) :bubble(_bubble), vec2Position(_vec2Position){

		}

		Bubble bubble;
		Vec2 vec2Position;
	};


	std::vector<BubbleButtonPosition> bubbleButtonPositions;

	bubbleButtonPositions.push_back(BubbleButtonPosition(Bubble(), Vec2(0, 0)));
	bubbleButtonPositions.push_back(BubbleButtonPosition(Bubble(BubbleType::RED), Vec2(1, 0)));
	bubbleButtonPositions.push_back(BubbleButtonPosition(Bubble(BubbleType::GREEN), Vec2(2, 0)));
	bubbleButtonPositions.push_back(BubbleButtonPosition(Bubble(BubbleType::YELLOW), Vec2(3, 0)));
	bubbleButtonPositions.push_back(BubbleButtonPosition(Bubble(BubbleType::PURPLE), Vec2(4, 0)));
	bubbleButtonPositions.push_back(BubbleButtonPosition(Bubble(BubbleType::BLUE), Vec2(5, 0)));
	bubbleButtonPositions.push_back(BubbleButtonPosition(Bubble(BubbleType::WOOD_1), Vec2(6, 0)));
	bubbleButtonPositions.push_back(BubbleButtonPosition(Bubble(BubbleType::WOOD_2), Vec2(7, 0)));


	bubbleButtonPositions.push_back(BubbleButtonPosition(Bubble(BubbleType::FOG), Vec2(0, -1)));
	bubbleButtonPositions.push_back(BubbleButtonPosition(Bubble(BubbleType::FISION), Vec2(1, -1)));
	bubbleButtonPositions.push_back(BubbleButtonPosition(Bubble(BubbleType::ICE), Vec2(2, -1)));
	bubbleButtonPositions.push_back(BubbleButtonPosition(Bubble(BubbleType::STONE), Vec2(3, -1)));


	bubbleButtonPositions.push_back(BubbleButtonPosition(Bubble(BubbleType::WITCH, Bubble::getRandomBasicBubbleType()), Vec2(1, -3)));
	bubbleButtonPositions.push_back(BubbleButtonPosition(Bubble(BubbleType::ANIMAL3), Vec2(3.5, -3)));
	bubbleButtonPositions.push_back(BubbleButtonPosition(Bubble(BubbleType::ANIMAL2), Vec2(5.625, -2.5)));
	bubbleButtonPositions.push_back(BubbleButtonPosition(Bubble(BubbleType::ANIMAL1), Vec2(7, -3)));

	

	float innerRadius = FieldAbstract::calculateInnerRadius(getContWidth(this), 8);
	float outerRadius = FieldAbstract::calculateOuterRadius(getContWidth(this), 8);
	float innerRadiusMove = innerRadius;
	float outerRadiusMove = outerRadius;

	innerRadius *= 0.8f;
	outerRadius *= 0.8f;


	std::function<Vec2(Vec2)> bubblePositionFunc = [&](Vec2 vec2Func){
		Vec2 vec2Return;
		vec2Return.x = innerRadiusMove + 2.0f*innerRadiusMove*vec2Func.x;
		vec2Return.y = fieldRect.origin.y - outerRadiusMove + 1.5f*outerRadiusMove*vec2Func.y;

		return vec2Return;
	};

	//Vec2 position(innerRadiusMove, fieldRect.origin.y - field->outerRadius - outerRadius);
	for (BubbleButtonPosition bubbleButtonPosition : bubbleButtonPositions){
		Bubble bubble = bubbleButtonPosition.bubble;
		Vec2 vec2Position = bubbleButtonPosition.vec2Position;
		vec2Position = bubblePositionFunc(vec2Position);
		

		BubbleButton *bubbleButton = BubbleButton::create(innerRadius, outerRadius, bubble);
		bubbleButton->setPosition(vec2Position);
		this->addChild(bubbleButton);

		bubbleButtons[bubble.getBubbleType()] = bubbleButton;
	}

	gameModeBtn = Button::create(getGameModeBtnStr(), Size(320, 70), 40);
	gameModeBtn->setPosition(bubblePositionFunc(Vec2(5.5, -1)));
	this->addChild(gameModeBtn);


	fieldWidthBar = ScrollBarInteger::create(FIELD_WIDTH_SCROLL_BAR_MIN, FIELD_WIDTH_SCROLL_BAR_MAX, fieldDesign->getBubbleWidth());
	Size fieldBarRectSize(getContWidth(this)*0.9f, 80.0f);
	Vec2 fieldBarRectOrigin((getContWidth(this) - fieldBarRectSize.width) / 2.0f, 100.0f);
	fieldWidthBarRect = Rect(fieldBarRectOrigin, fieldBarRectSize);

	fieldWidthBar->setContentSize(fieldBarRectSize);
	fieldWidthBar->setPosition(fieldBarRectOrigin + Vec2(fieldBarRectSize.width / 2.0f, fieldBarRectSize.height / 2.0f));
	this->addChild(fieldWidthBar);


	
	saveBtn = Button::create("Save", Size(110, 50), 40);
	saveBtn->setPosition(Vec2(70, 50));
	this->addChild(saveBtn);

	shareBtn = Button::create("Share", Size(120, 50), 40);
	shareBtn->setPosition(saveBtn->getPosition() + Vec2(getContWidth(saveBtn) / 2.0f + getContWidth(shareBtn) / 2.0f + 50.0f, 0.0f));
	this->addChild(shareBtn);

	testButton = Button::create("Test", Size(120.0f, 50.0f), 40);
	testButton->setPosition(shareBtn->getPosition() + Vec2(getContWidth(shareBtn) / 2.0f + getContWidth(testButton) / 2.0f + 50.0f, 0.0f));
	this->addChild(testButton);

	this->schedule(std::bind(&HelloWorld::changeWitchColor, this, std::placeholders::_1), SCHEDULE_CHANGE_WITCH_COLOR_INTERVAL, SCHEDULE_CHANGE_WITCH_COLOR);


	/*
	std::vector<int> oldLevelIntegers;
	for (int i = 1; i <= 60; i++){
		if ((i % 5 != 0 || i == 5) && i != 6 && i != 7){
			oldLevelIntegers.push_back(i);
		}
	}

	int i = 0;
	for (int oldLevelInteger : oldLevelIntegers){


		std::string oldLevelStr;

		if (oldLevelInteger >= 10){
			oldLevelStr = "E:\\bubbleLevelDesigner\\Resources\\0" + to_string(oldLevelInteger) + ".plist";
		}
		else {
			oldLevelStr = "E:\\bubbleLevelDesigner\\Resources\\00" + to_string(oldLevelInteger) + ".plist";
		}

		if (oldLevelInteger == 3){
			CCLOG("we are here");
		}
		

		ValueMap levelValueMap = FileUtils::getInstance()->getValueMapFromFile(oldLevelStr);
		setLevelValueMapOld(levelValueMap);

		i++;

		std::string newLevelStr;
		if (i >= 10){
			newLevelStr = "TRY_TRY_0" + to_string(i);
		}
		else {
			newLevelStr = "TRY_TRY_00" + to_string(i);
		}

		levelNameLabel->setString(newLevelStr);
		saveBtnFunc();
	}
	*/
	/*
	ValueMap levelValueMap = FileUtils::getInstance()->getValueMapFromFile("E:\\bubbleLevelDesigner\\Resources\\008.plist");
	setLevelValueMapOld(levelValueMap);
	levelNameLabel->setString("xxxx");
	saveBtnFunc();
	*/
	//loadBtnFuncCallback("E:\\bubbleLevelDesigner\\Resources\\GAMEXXOO\\GAMEXXOO_012.json");

	/*
	for (int i = 1; i <= 47; i++){
		std::string levelStr;
		if (i >= 10){
			levelStr = "E:\\bubbleLevelDesigner\\Resources\\GAMEXXOO\\GAMEXXOO_0" + to_string(i) + ".json";
		}
		else {
			levelStr = "E:\\bubbleLevelDesigner\\Resources\\GAMEXXOO\\GAMEXXOO_00" + to_string(i) + ".json";
		}

		loadBtnFuncCallback(levelStr);

		std::vector<BubbleType> bubbleTypes;

		bubbleTypes.push_back(BubbleType::ICE);
		bubbleTypes.push_back(BubbleType::WOOD_1);
		bubbleTypes.push_back(BubbleType::WOOD_2);
		bubbleTypes.push_back(BubbleType::FOG);
		bubbleTypes.push_back(BubbleType::FISION);
		bubbleTypes.push_back(BubbleType::STONE);
		bubbleTypes.push_back(BubbleType::WITCH);
		bubbleTypes.push_back(BubbleType::ANIMAL1);
		bubbleTypes.push_back(BubbleType::ANIMAL2);
		bubbleTypes.push_back(BubbleType::ANIMAL3);

		for (BubbleType bubbleType : bubbleTypes){
			if (fieldDesign->getBubbleNum(Bubble(bubbleType)) > 0){
				CCLOG("has a bubbleType ");
			}
		}
	}
	*/
	//loadBtnFuncCallback("E:\\bubbleLevelDesigner\\Resources\\GAMEXXOO\\GAMEXXOO_004.json");


	//loadBtnFuncCallback("E:\\bubbleLevelDesigner\\Resources\\GAMEXXOO\\GAMEXXOO_044.plist");


	loadBtnFuncCallback(levelType, levelName);
}


void HelloWorld::loadBtnFuncCallback(LEVEL_TYPE _levelType, std::string _levelName){
	levelType = _levelType;
	levelName = _levelName;

	levelNameLabel->setString(getFileNameForFullPath(levelName));


	std::string fullPath = Global::getInstance()->getFullPathForLevelDirectoryAndLevelName(levelType, levelName);
	
	if (FileUtils::getInstance()->isFileExist(fullPath)){
		std::string valueMap = FileUtils::getInstance()->getStringFromFile(fullPath);
		setLevelValueMap(valueMap);
	}
}


void HelloWorld::saveBtnFunc() {
	AudioManager::getInstance()->playSound(SoundType::CLICK);

    saveBtnFuncHelper(Global::getInstance()->getFullPathForLevelDirectoryAndLevelName(LEVEL_TYPE::CUSTOM_LEVELS, levelNameLabel->getString()));
}

void HelloWorld::saveBtnFuncHelper(std::string fullPath){
	std::string levelValueMap = getLevelValueMap();

	FileUtils::getInstance()->writeStringToFile(levelValueMap, fullPath);
}



void HelloWorld::shareBtnCallback() {
	AudioManager::getInstance()->playSound(SoundType::CLICK);
}

void HelloWorld::testBtnCallback(){
	AudioManager::getInstance()->playSound(SoundType::CLICK);

	std::string testLevelName = levelNameLabel->getString();

    saveBtnFuncHelper(Global::getInstance()->getFullPathForLevelDirectoryAndLevelName(LEVEL_TYPE::TEST_LEVELS, testLevelName));


	std::function<void()> _returnLastInterface = std::bind([](LEVEL_TYPE levelType, std::string levelName){
		Scene *pScene = HelloWorld::createScene(levelType, levelName);
		
		Director::getInstance()->replaceScene(pScene);

    }, LEVEL_TYPE::TEST_LEVELS, testLevelName);

    Scene *pScene = GameLayer::createScene(LEVEL_TYPE::TEST_LEVELS, testLevelName, 0, _returnLastInterface);
	Director::getInstance()->replaceScene(pScene);
}


std::string HelloWorld::getLevelValueMap(){
	using namespace rapidjson;

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();

	writer.Key(VALUEMAP_VERSION);
	writer.Double(VALUEMAP_VERSION_NUMBER);

	writer.Key(VALUEMAP_GAMEMODE_GAME_TYPE);
	writer.Int((int)gameMode.getGameModeType());

	writer.Key(VALUEMAP_GAMEMODE_GAME_AMOUNT);
	writer.Int(gameMode.getGameModeAmount());

	writer.Key(VALUEMAP_GAMEMODE_CLEAR_TOP);
	writer.Int(gameMode.getGameModeClearTop());

	fieldDesign->getLevelBoardMap(writer, false, false);

	writer.EndObject();



	return s.GetString();

}

void HelloWorld::setLevelValueMap(std::string levelValueMap){
	rapidjson::Document document;

	document.Parse(levelValueMap.c_str());

	
	
	GameModeType gameModeType = (GameModeType)document[VALUEMAP_GAMEMODE_GAME_TYPE].GetInt();
	int gameModeAmount = document[VALUEMAP_GAMEMODE_GAME_AMOUNT].GetInt();
	int gameModeClearTop = document[VALUEMAP_GAMEMODE_CLEAR_TOP].GetInt();

	gameMode.setGameModeType(gameModeType);
	gameMode.setGameModeAmount(gameModeAmount);
	gameMode.setGameModeClearTop(gameModeClearTop);

	gameModeBtn->setButtonStr(getGameModeBtnStr());


	fieldDesign->setLevelBoardMap(document);

	fieldWidthBar->setScroll(fieldDesign->getBubbleWidth());
	scrollBar->setBackBottomY(fieldDesign->getBackBottomY());
	scrollBar->setBackTopY(fieldDesign->getBackTopY());
	scrollBar->setScrollPosition(fieldDesign->getScrollPosition());

	updateTopBoundary();
}




void HelloWorld::setLevelValueMapOld(ValueMap oldLevelValueMap){
	GameModeType gameModeType;
	if (oldLevelValueMap[PLIST_TIMEMOVE_BOOL].asBool()){
		gameModeType = GameModeType::MOVE;
	}
	else {
		gameModeType = GameModeType::TIME;
	}
	int gameModeAmount = oldLevelValueMap[PLIST_TIMEMOVE].asInt();
	gameMode.setGameModeType(gameModeType);
	gameMode.setGameModeAmount(gameModeAmount);

	


	gameModeBtn->setButtonStr(getGameModeBtnStr());
	
	fieldDesign->setOldLevelBoardMap(oldLevelValueMap);

	{
		int clearTopAmount;
		if (GameModeType::TIME == gameModeType){
			clearTopAmount = 11;
		}
		else {
			if (fieldDesign->getBubbleNum(Bubble(BubbleType::ANIMAL1)) == 0
				&& fieldDesign->getBubbleNum(Bubble(BubbleType::ANIMAL2)) == 0
				&& fieldDesign->getBubbleNum(Bubble(BubbleType::ANIMAL3)) == 0
				&& fieldDesign->getBubbleNumWitch() == 0){
				clearTopAmount = 6;
			}
			else {
				clearTopAmount = 0;
			}
		}

		
		gameMode.setGameModeClearTop(clearTopAmount);
	}

	fieldWidthBar->setScroll(fieldDesign->getBubbleWidth());
	scrollBar->setBackBottomY(fieldDesign->getBackBottomY());
	scrollBar->setBackTopY(fieldDesign->getBackTopY());
	scrollBar->setScrollPosition(fieldDesign->getScrollPosition());

	updateTopBoundary();
}



void HelloWorld::onExit(){
	DDUserDefault::getInstance()->setDesignerGameMode(gameMode);
	DDUserDefault::getInstance()->setDesignerBubbleWidth(fieldDesign->getBubbleWidth());


	unloadArmature();


	Layer::onExit();
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event){

	if (main_touch != nullptr){
		return true;
	}
	main_touch = touch;
	
	levelNameLabel->detachWithIME();


	Vec2 location = touch->getLocation();
	if (fieldRect.containsPoint(location)){
		touchType = TouchType::FIELD_PRESS;
	}
	else if (scrollRect.containsPoint(location)){
		touchType = TouchType::SCROLL_BAR;
	}
	else {
		if (fieldWidthBarRect.containsPoint(location)){
			touchType = TouchType::BUTTONS_SCROLLBAR;
		}
		else {
			touchType = TouchType::BUTTONS;
		}
		
	}

	switch (touchType){
	case TouchType::FIELD_PRESS:
		break;
	case TouchType::FIELD_MOVE:
		break;
	case TouchType::SCROLL_BAR:
		break;
	case TouchType::BUTTONS:

		if (levelNameLabel->getBoundingBox().containsPoint(location)){
			levelNameLabel->setIsPressed(true);
		}else if (backBtn->getBoundingBox().containsPoint(location)){
			backBtn->setScale(0.9f);
		}
		else if (gameModeBtn->getBoundingBox().containsPoint(location)){
			gameModeBtn->setIsPressed(true);
		}
		else if (saveBtn->getBoundingBox().containsPoint(location)) {
			saveBtn->setIsPressed(true);
		}
		else if (shareBtn->getBoundingBox().containsPoint(location)) {
			shareBtn->setIsPressed(true);
		}
		else if (testButton->getBoundingBox().containsPoint(location)){
			testButton->setIsPressed(true);
		}
		else {
			for (std::map<BubbleType, BubbleButton *>::iterator bubbleButtonsIter = bubbleButtons.begin(); bubbleButtonsIter != bubbleButtons.end() && nullptr == pressedBubbleButton; ++bubbleButtonsIter){
				BubbleButton *bubbleButton = bubbleButtonsIter->second;
				if (bubbleButton->containsPoint(location)){
					pressedBubbleButton = bubbleButton;
					pressedBubbleButton->setIsPressed(true);
				}
			}
		}


		break;
	case TouchType::BUTTONS_SCROLLBAR:
		fieldWidthBar->setScrollFloat(PointApplyTransform(location, fieldWidthBar->getWorldToNodeTransform()).x);
		break;
	}

	return true;
}
void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event){
	if (main_touch != touch){
		return;
	}

	Vec2 location = touch->getLocation();

	switch (touchType){
	case TouchType::FIELD_PRESS:
		if (location.distance(touch->getStartLocation()) > fieldMoveSensitivity){
			touchType = TouchType::FIELD_MOVE;
		}
		break;
	case TouchType::FIELD_MOVE:
		fieldDesign->setPositionY(fieldDesign->getPositionY() + (location.y - touch->getPreviousLocation().y));
		scrollBar->setBackBottomY(fieldDesign->getBackBottomY());
		scrollBar->setBackTopY(fieldDesign->getBackTopY());
		scrollBar->setScrollPosition(fieldDesign->getScrollPosition());
		break;
	case TouchType::SCROLL_BAR:
		scrollBar->addScrollPosition((location.y - touch->getPreviousLocation().y) * (scrollBar->getBackSize()/scrollRect.size.height));
		
		fieldDesign->setScrollPosition(scrollBar->getScrollPosition());
		scrollBar->setBackBottomY(fieldDesign->getBackBottomY());
		scrollBar->setBackTopY(fieldDesign->getBackTopY());
		scrollBar->setScrollPosition(fieldDesign->getScrollPosition());
		break;
	case TouchType::BUTTONS:
		
		if (nullptr != pressedBubbleButton){
			pressedBubbleButton->setIsPressed(false);
			pressedBubbleButton = nullptr;
		}
		gameModeBtn->setIsPressed(false);
		saveBtn->setIsPressed(false);
		shareBtn->setIsPressed(false);
		testButton->setIsPressed(false);


		backBtn->setScale(1.0f);

		levelNameLabel->setIsPressed(false);

		if (levelNameLabel->getBoundingBox().containsPoint(location)){
			levelNameLabel->setIsPressed(true);
		}else if(backBtn->getBoundingBox().containsPoint(location)){
			backBtn->setScale(0.9f);
		}
		else if (gameModeBtn->getBoundingBox().containsPoint(location)){
			gameModeBtn->setIsPressed(true);
		}
		else if (saveBtn->getBoundingBox().containsPoint(location)) {
			saveBtn->setIsPressed(true);
		}
		else if (shareBtn->getBoundingBox().containsPoint(location)) {
			shareBtn->setIsPressed(true);
		}
		else if (testButton->getBoundingBox().containsPoint(location)){
			testButton->setIsPressed(true);
		}
		else {
			for (std::map<BubbleType, BubbleButton *>::iterator bubbleButtonsIter = bubbleButtons.begin(); bubbleButtonsIter != bubbleButtons.end() && nullptr == pressedBubbleButton; ++bubbleButtonsIter){
				BubbleButton *bubbleButton = bubbleButtonsIter->second;
				if (bubbleButton->containsPoint(location)){
					pressedBubbleButton = bubbleButton;
					pressedBubbleButton->setIsPressed(true);
				}
			}
		}


		break;
	case TouchType::BUTTONS_SCROLLBAR:
		fieldWidthBar->setScrollFloat(PointApplyTransform(location, fieldWidthBar->getWorldToNodeTransform()).x);
		break;
	}
}
void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event){
	if (main_touch != touch){
		return;
	}

	Vec2 location = touch->getLocation();


	switch (touchType){
	case TouchType::FIELD_PRESS:
		if (nullptr != selectedBubbleButton){
			AudioManager::getInstance()->playSound(SoundType::CLICK);

			fieldDesign->setBubble(PointApplyTransform(location, fieldDesign->getWorldToNodeTransform()), selectedBubbleButton->getBubble());

			scrollBar->setBackBottomY(fieldDesign->getBackBottomY());
			scrollBar->setBackTopY(fieldDesign->getBackTopY());
			scrollBar->setScrollPosition(fieldDesign->getScrollPosition());

			updateTopBoundary();
		}
		break;
	case TouchType::FIELD_MOVE:
		break;
	case TouchType::SCROLL_BAR:
		break;
	case TouchType::BUTTONS:
		if (nullptr != pressedBubbleButton){
			pressedBubbleButton->setIsPressed(false);
			pressedBubbleButton = nullptr;
		}
		gameModeBtn->setIsPressed(false);
		saveBtn->setIsPressed(false);
		shareBtn->setIsPressed(false);
		testButton->setIsPressed(false);

		backBtn->setScale(1.0f);

		levelNameLabel->setIsPressed(false);

		if (nullptr != selectedBubbleButton){
			selectedBubbleButton->setIsSelected(false);
			selectedBubbleButton = nullptr;
		}


		if (levelNameLabel->getBoundingBox().containsPoint(location)){
			levelNameLabel->attachWithIME();
		}else if (backBtn->getBoundingBox().containsPoint(location)){
			backBtnCallback();
		}else if (gameModeBtn->getBoundingBox().containsPoint(location)){
			gameModeBtnCallback();
		}
		else if (saveBtn->getBoundingBox().containsPoint(location)) {
			saveBtnFunc();
		}
		else if (shareBtn->getBoundingBox().containsPoint(location)) {
			shareBtnCallback();
		}
		else if (testButton->getBoundingBox().containsPoint(location)){
			testBtnCallback();
		}
		else {
			for (std::map<BubbleType, BubbleButton *>::iterator bubbleButtonsIter = bubbleButtons.begin(); bubbleButtonsIter != bubbleButtons.end() && nullptr == selectedBubbleButton; ++bubbleButtonsIter){
				BubbleButton *bubbleButton = bubbleButtonsIter->second;
				if (bubbleButton->containsPoint(location)){
					AudioManager::getInstance()->playSound(SoundType::CLICK);

					selectedBubbleButton = bubbleButton;
					selectedBubbleButton->setIsSelected(true);

				}
			}
		}



		break;
	case TouchType::BUTTONS_SCROLLBAR:
		AudioManager::getInstance()->playSound(SoundType::CLICK);

		fieldWidthBar->setScrollFloat(PointApplyTransform(location, fieldWidthBar->getWorldToNodeTransform()).x);
		fieldDesign->setBubbleWidth(fieldWidthBar->getScroll());

		scrollBar->setBackBottomY(fieldDesign->getBackBottomY());
		scrollBar->setBackTopY(fieldDesign->getBackTopY());
		scrollBar->setScrollPosition(fieldDesign->getScrollPosition());

		gameMode.setGameModeClearTop(std::min(gameMode.getGameModeClearTop(), fieldDesign->getBubbleWidth()));
		updateTopBoundary();
		break;
	}


	touchType = TouchType::UNDEFINED;
	main_touch = nullptr;
}
void HelloWorld::onTouchCancelled(Touch *touch, Event *unused_event){
	onTouchEnded(touch, unused_event);
}


#define WITCH_BUBBLE_ARMATURE "balls/witch_all0.ExportJson"
#define ANIMAL1_ARMATURE "balls/animal1_btn0.ExportJson"
#define ANIMAL2_ARMATURE "balls/YL_btn0.ExportJson"
#define ANIMAL3_ARMATURE "balls/animal3_btn0.ExportJson"


void HelloWorld::loadArmature(){
	ArmatureDataManager::getInstance()->addArmatureFileInfo(WITCH_BUBBLE_ARMATURE);
	ArmatureDataManager::getInstance()->addArmatureFileInfo(ANIMAL1_ARMATURE);
	ArmatureDataManager::getInstance()->addArmatureFileInfo(ANIMAL2_ARMATURE);
	ArmatureDataManager::getInstance()->addArmatureFileInfo(ANIMAL3_ARMATURE);
}
void HelloWorld::unloadArmature(){
	ArmatureDataManager::getInstance()->removeArmatureFileInfo(WITCH_BUBBLE_ARMATURE);
	ArmatureDataManager::getInstance()->removeArmatureFileInfo(ANIMAL1_ARMATURE);
	ArmatureDataManager::getInstance()->removeArmatureFileInfo(ANIMAL2_ARMATURE);
	ArmatureDataManager::getInstance()->removeArmatureFileInfo(ANIMAL3_ARMATURE);
}


void HelloWorld::changeWitchColor(float dt){
	BubbleButton *bubbleButtonWitch = bubbleButtons[BubbleType::WITCH];

	Bubble bubble = bubbleButtonWitch->getBubble();
	BubbleType bubbleWitchType;
	do {
		bubbleWitchType = Bubble::getRandomBasicBubbleType();
	} while (bubbleWitchType == bubble.getBubbleTypeWitchFog());

	bubble.setBubbleTypeWitch(bubbleWitchType, Dec2(0, 0));

	bubbleButtonWitch->setBubble(bubble);
	bubbleButtonWitch->setIsPressedInner();
}


std::string HelloWorld::getGameModeBtnStr(){
	std::string ret;
	switch (gameMode.getGameModeType()){
	case GameModeType::MOVE:
		ret += "Move Mode: ";
		break;
	case GameModeType::TIME:
		ret += "Time Mode: ";
		break;
	}

	ret += to_string(gameMode.getGameModeAmount());

	return ret;
}


void HelloWorld::gameModeBtnCallback(){
	AudioManager::getInstance()->playSound(SoundType::CLICK);

	GameModeLayer *gameModeLayer = GameModeLayer::create(std::bind(&HelloWorld::gameModeLayerCallback, this, std::placeholders::_1), gameMode, fieldDesign->getBubbleWidth());
	popUpLayer(gameModeLayer);
	

}

void HelloWorld::gameModeLayerCallback(GameMode _gameMode){
	gameMode = _gameMode;
	updateTopBoundary();
	gameModeBtn->setButtonStr(getGameModeBtnStr());
}


void HelloWorld::backBtnCallback(){
	AudioManager::getInstance()->playSound(SoundType::CLICK);

	Director::getInstance()->replaceScene(LevelLayer::createScene(LevelState::DESIGNER));
}

void HelloWorld::popUpLayer(Layer *layer){
	layer->setOnExitCallback([this](){
		keyboardListener->onKeyPressed = std::bind(&HelloWorld::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
		keyboardListener->onKeyReleased = std::bind(&HelloWorld::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);
	});
	this->addChild(layer, Z_POPUP_LAYER);
}


void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){

}
void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (EventKeyboard::KeyCode::KEY_BACK == keyCode){
		levelNameLabel->detachWithIME();
	}
}


void HelloWorld::updateTopBoundary() {
	topBoundary->setNumOfWitchesTotal(fieldDesign->getBubbleNumWitch());
	topBoundary->setNumOAnimalTotal(fieldDesign->getBubbleNumAnimal());
	topBoundary->setNumOfClearTopTotal(gameMode.getGameModeClearTop());
}
