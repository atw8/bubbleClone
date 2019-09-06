#include "GameModeLayer.h"
#include "Global.h"


GameModeLayer::GameModeLayer(std::function<void(GameMode)> _updateGameModeFunc, GameMode _gameMode, int _bubbleWidth)
:updateGameModeFunc(_updateGameModeFunc), gameMode(_gameMode), bubbleWidth(_bubbleWidth){

	main_touch = nullptr;
}

GameModeLayer* GameModeLayer::create(std::function<void(GameMode)> _updateGameModeFunc, GameMode _gameMode, int _bubbleWidth){
	GameModeLayer *gameModeLayer = new GameModeLayer(_updateGameModeFunc, _gameMode, _bubbleWidth);
	if (gameModeLayer && gameModeLayer->init()){
		gameModeLayer->autorelease();
	}
	else {
		delete gameModeLayer;
		gameModeLayer = nullptr;
	}

	return gameModeLayer;
}

Scene *GameModeLayer::createScene(std::function<void(GameMode)> _updateGameModeFunc, GameMode _gameMode, int _bubbleWidth){
	Scene *pScene = Scene::create();
	pScene->addChild(GameModeLayer::create(_updateGameModeFunc, _gameMode, _bubbleWidth));

	return pScene;
}

void GameModeLayer::onEnter(){
	LayerColor::onEnter();
	this->setColor(Color3B::GREEN);
	this->setOpacity(128);


	keyboardListener->onKeyPressed = std::bind(&GameModeLayer::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
	keyboardListener->onKeyReleased = std::bind(&GameModeLayer::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);

	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = std::bind(&GameModeLayer::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchMoved = std::bind(&GameModeLayer::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchEnded = std::bind(&GameModeLayer::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchCancelled = std::bind(&GameModeLayer::onTouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
	listener->setSwallowTouches(true);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	Button *buttonMove = Button::create("Move Mode", Size(320, 70), 40);
	buttonMove->setPosition(Vec2(getContWidth(this)/2.0f - 180.0f, getContHeight(this) / 2.0f + 300.0f));
	this->addChild(buttonMove);

	Button *buttonTime = Button::create("Time Mode", Size(320, 70), 40);
	buttonTime->setPosition(Vec2(getContWidth(this) / 2.0f + 180.0f, getContHeight(this) / 2.0f + 300.0f));
	this->addChild(buttonTime);

	gameModeButtons[GameModeType::MOVE] = buttonMove;
	gameModeButtons[GameModeType::TIME] = buttonTime;

	gameModeButtons[gameMode.getGameModeType()]->setIsSelected(true);





	std::function<std::string(std::string)> normalizeGameAmountFunc = [](std::string str){
	
		int strSize = 0;
		for (std::string::iterator strIter = str.begin(); strIter != str.end(); ){
			char s = *strIter;
			if (s >= '0' && s <= '9' && strSize < 4){
				strSize++;
				strIter++;
			}
			else {
				strIter = str.erase(strIter);
			}
		}

		return str;
	};


	Size gameModeAmountBtnSize(300, 70);
	gameModeAmountBtn = CustomNumber::createWithPlace(gameModeAmountBtnSize, 40, normalizeGameAmountFunc, std::bind(&GameModeLayer::gameModeAmountCallback, this, std::placeholders::_1));
	gameModeAmountBtn->setString(to_string(gameMode.getGameModeAmount()));
	gameModeAmountBtn->setPosition(Vec2(getContWidth(this) / 2.0f, getContHeight(this) / 2.0f - 100.0f + 300.0f));
	this->addChild(gameModeAmountBtn);


	Label *clearTopLabel = Label::createWithSystemFont("Clear Top Condition", "Arial", 40);
	clearTopLabel->setPosition(Vec2(getContWidth(this) / 2.0f, getContHeight(this) / 2.0f - 100.0f + 200.0f));
	this->addChild(clearTopLabel);

	std::function<std::string(std::string)> normalizeClearTop = [this](std::string str){
		for (std::string::iterator strIter = str.begin(); strIter != str.end();){
			char s = *strIter;
			if (s >= '0' && s <= '9'){
				strIter++;
			}
			else {
				strIter = str.erase(strIter);
			}
		}

		int strNum = std::atoi(str.c_str());
		strNum = std::min(strNum, bubbleWidth);
		
		str = to_string(strNum);

		return str;
	};

	Size clearTopBtnSize(300, 70);
	clearTopBtn = CustomNumber::createWithPlace(clearTopBtnSize, 40, normalizeClearTop, std::bind(&GameModeLayer::gameModeClearTopCallback, this, std::placeholders::_1));
	clearTopBtn->setPosition(Vec2(getContWidth(this) / 2.0, clearTopLabel->getPositionY() - 80.0f));
	clearTopBtn->setString(to_string(gameMode.getGameModeClearTop()));
	this->addChild(clearTopBtn);


	backBtn = Sprite::create("backBtn.png");
	backBtn->setPosition(Vec2(getContWidth(backBtn) / 2.0f, getContHeight(this) - getContHeight(backBtn) / 2.0f));
	this->addChild(backBtn);
}


bool GameModeLayer::onTouchBegan(Touch *touch, Event *unused_event){
	if (nullptr != main_touch){
		return true;
	}
	main_touch = touch;

	gameModeAmountBtn->detachWithIME();
	clearTopBtn->detachWithIME();



	Vec2 location = touch->getLocation();

	if (gameModeButtons[GameModeType::MOVE]->getBoundingBox().containsPoint(location)){
		gameModeButtons[GameModeType::MOVE]->setIsPressed(true);
	}
	else if (gameModeButtons[GameModeType::TIME]->getBoundingBox().containsPoint(location)){
		gameModeButtons[GameModeType::TIME]->setIsPressed(true);
	}
	else if (gameModeAmountBtn->getBoundingBox().containsPoint(location)){
		gameModeAmountBtn->setIsPressed(true);
	}
	else if (clearTopBtn->getBoundingBox().containsPoint(location)){
		clearTopBtn->setIsPressed(true);
	}
	else if (backBtn->getBoundingBox().containsPoint(location)) {
		backBtn->setScale(0.9f);
	}

	return true;
}
void GameModeLayer::onTouchMoved(Touch *touch, Event *unused_event){
	if (touch != main_touch){
		return;
	}

	Vec2 location = touch->getLocation();
	
	gameModeButtons[GameModeType::MOVE]->setIsPressed(false);
	gameModeButtons[GameModeType::TIME]->setIsPressed(false);
	gameModeAmountBtn->setIsPressed(false);
	clearTopBtn->setIsPressed(false);
	backBtn->setScale(1.0f);

	if (gameModeButtons[GameModeType::MOVE]->getBoundingBox().containsPoint(location)){
		gameModeButtons[GameModeType::MOVE]->setIsPressed(true);
	}
	else if (gameModeButtons[GameModeType::TIME]->getBoundingBox().containsPoint(location)){
		gameModeButtons[GameModeType::TIME]->setIsPressed(true);
	}
	else if (gameModeAmountBtn->getBoundingBox().containsPoint(location)){
		gameModeAmountBtn->setIsPressed(true);
	}
	else if (clearTopBtn->getBoundingBox().containsPoint(location)){
		clearTopBtn->setIsPressed(true);
	}
	else if (backBtn->getBoundingBox().containsPoint(location)) {
		backBtn->setScale(0.9f);
	}

}
void GameModeLayer::onTouchEnded(Touch *touch, Event *unused_event){
	if (touch != main_touch){
		return;
	}
	main_touch = nullptr;

	Vec2 location = touch->getLocation();

	gameModeButtons[GameModeType::MOVE]->setIsPressed(false);
	gameModeButtons[GameModeType::TIME]->setIsPressed(false);
	gameModeAmountBtn->setIsPressed(false);
	clearTopBtn->setIsPressed(false);
	backBtn->setScale(1.0f);

	if (gameModeButtons[GameModeType::MOVE]->getBoundingBox().containsPoint(location)){
		gameModeTypeCallback(GameModeType::MOVE);
	}
	else if (gameModeButtons[GameModeType::TIME]->getBoundingBox().containsPoint(location)){
		gameModeTypeCallback(GameModeType::TIME);
	}
	else if (gameModeAmountBtn->getBoundingBox().containsPoint(location)){
		gameModeAmountBtn->attachWithIME();
	}
	else if (clearTopBtn->getBoundingBox().containsPoint(location)){
		clearTopBtn->attachWithIME();
	}
	else if (backBtn->getBoundingBox().containsPoint(location)) {
		backBtnCallback();
	}
}
void GameModeLayer::onTouchCancelled(Touch *touch, Event *unused_event){
	onTouchEnded(touch, unused_event);
}

void GameModeLayer::gameModeTypeCallback(GameModeType gameModeType){
	if (gameModeType == gameMode.getGameModeType()){
		return;
	}
	gameModeButtons[GameModeType::MOVE]->setIsSelected(false);
	gameModeButtons[GameModeType::TIME]->setIsSelected(false);

	gameModeButtons[gameModeType]->setIsSelected(true);

	gameMode.setGameModeType(gameModeType);


	callUpdateGameModeFunc();
}
void GameModeLayer::gameModeAmountCallback(std::string gameModeAmountStr){
	int gameModeAmount = atoi(gameModeAmountStr.c_str());

	if (gameModeAmount <= 0) {
		return;
	}

	gameMode.setGameModeAmount(gameModeAmount);


	callUpdateGameModeFunc();
}

void GameModeLayer::gameModeClearTopCallback(std::string clearTopAmountStr){
	if (clearTopAmountStr.empty()){
		return;
	}

	int clearTopAmount = atoi(clearTopAmountStr.c_str());

	gameMode.setGameModeClearTop(clearTopAmount);

	callUpdateGameModeFunc();
}

void GameModeLayer::callUpdateGameModeFunc(){
	if (updateGameModeFunc){
		updateGameModeFunc(gameMode);
	}
}

void GameModeLayer::backBtnCallback() {
	this->removeFromParentAndCleanup(true);
}

void GameModeLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){

}
void GameModeLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (EventKeyboard::KeyCode::KEY_BACK == keyCode){
		gameModeAmountBtn->detachWithIME();
		clearTopBtn->detachWithIME(); 

		backBtnCallback();
	}
}