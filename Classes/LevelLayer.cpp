#include "LevelLayer.h"

#include "Global.h"

#include "GameLayer.h"
#include "HelloWorldScene.h"

#include "LevelLayerGlobal.h"

#include "AudioManager.h"

#include "DDUserDefault.h"

LevelLayer::LevelLayer(LevelState _levelState)
:main_touch(nullptr), levelState(_levelState), deleteButtonOn(false){

}

Scene *LevelLayer::createScene(LevelState _levelState) {
	Scene *pScene = Scene::create();
	pScene->addChild(LevelLayer::create(_levelState));

	return pScene;
}

LevelLayer *LevelLayer::create(LevelState _levelState){
	LevelLayer *levelLayer = new LevelLayer(_levelState);
	if (levelLayer && levelLayer->init()) {
		levelLayer->autorelease();
	}
	else {
		delete levelLayer;
		levelLayer = nullptr;
	}


	return levelLayer;
}


void LevelLayer::onEnter() {
	Layer::onEnter();

	bg = Sprite::create("shared/background_1.png");
	bg->setScaleX(getContWidth(this) / getContWidth(bg));
	bg->setScaleY(getContHeight(this) / getContHeight(bg));
	bg->setPosition(Vec2(getContWidth(this) / 2.0f, getContHeight(this) / 2.0f));
	this->addChild(bg);
	bg->setZOrder(2);
	levelStateT[LevelState::DESIGNER] = 1.0f;
	levelStateT[LevelState::GAME] = 0.0f;

	buttonGame;
	{
		Texture2D *texture1 = Director::getInstance()->getTextureCache()->addImage("levelLayer/levelHeart_on.png");
		Texture2D *texture2 = Director::getInstance()->getTextureCache()->addImage("levelLayer/levelHeart_off.png");

		buttonGame = TwoTextureSprite::create(texture1, texture2, levelStateT[levelState]);
	}
	buttonGame->setPosition(Vec2(150.0f, getContHeight(bg) - 150.0f));
	bg->addChild(buttonGame);


	buttonDesigner;
	{
		Texture2D *texture1 = Director::getInstance()->getTextureCache()->addImage("levelLayer/levelDesigner_off.png");
		Texture2D *texture2 = Director::getInstance()->getTextureCache()->addImage("levelLayer/levelDesigner_on.png");

		
		buttonDesigner = TwoTextureSprite::create(texture1, texture2, levelStateT[levelState]);
	}
	buttonDesigner->setPosition(Vec2(getContWidth(bg) - 150.0f, getContHeight(bg) - 150.0f));
	bg->addChild(buttonDesigner);





	buttonDelete = Sprite::create("levelLayer/buttonDelete.png");
	buttonDelete->setPosition(Vec2(getContWidth(bg) - getContWidth(buttonDelete) / 2.0f, getContHeight(buttonDelete) / 2.0f + 20.0f));
	bg->addChild(buttonDelete);




	const float levelsRectOffset = 20.0f;
	Rect levelsRect;
	levelsRect.origin.x = levelsRectOffset;
	levelsRect.origin.y = buttonDelete->getPositionY() + getContHeight(buttonDelete) / 2.0f + levelsRectOffset;
	levelsRect.size.width = getContWidth(bg) - 2.0f*levelsRectOffset;
	levelsRect.size.height = (std::min(buttonGame->getPositionY() - getContHeight(buttonGame) / 2.0f, buttonDesigner->getPositionY() - getContHeight(buttonDesigner) / 2.0f)) - levelsRect.origin.y;



	const int levelFrontier = DDUserDefault::getInstance()->getLevelFrontier();


	std::vector<std::string> levelNamesUnlock;
	std::vector<std::string> levelNamesLock;
	{
        Global::getInstance()->getLevelsForLevelType(LEVEL_TYPE::GAME_LEVELS);
		std::vector<std::string> levelNameGames = Global::getInstance()->getLevelsForLevelType(LEVEL_TYPE::GAME_LEVELS);
    
		for (int i = 0; i < levelNameGames.size(); i++){
			std::string levelNameGame = levelNameGames.at(i);
			if (levelFrontier > i){
				levelNamesUnlock.push_back(levelNameGame);
			}
			else {
				levelNamesLock.push_back(levelNameGame);
			}
		}
	}
    std::vector<std::string> levelNamesDesigner = Global::getInstance()->getLevelsForLevelType(LEVEL_TYPE::CUSTOM_LEVELS);


	numOfButtonTypesMap[LevelButtonType::GAME_UNLOCK] = levelNamesUnlock.size();
	numOfButtonTypesMap[LevelButtonType::GAME_LOCK] = levelNamesLock.size();
	numOfButtonTypesMap[LevelButtonType::DESIGN] = levelNamesDesigner.size();
	numOfButtonTypesMap[LevelButtonType::DESIGN_NEW] = 1;



	levelsGame = LevelsInnerLayer::create(levelsRect);
	bg->addChild(levelsGame);
	{

		for (int i = 0; i < levelNamesUnlock.size(); i++){

			levelsGame->addLevel(LevelButton::create(i + 1, levelNamesUnlock.at(i), LevelButtonType::GAME_UNLOCK));
		}

		for (int i = 0; i < levelNamesLock.size(); i++){
			levelsGame->addLevel(LevelButton::create(0, levelNamesLock.at(i), LevelButtonType::GAME_LOCK));
		}
		
	}
	levelsGame->positionLevelButtons();




	levelsDesigner = LevelsInnerLayer::create(levelsRect);
	bg->addChild(levelsDesigner);
	{
		levelsDesigner->addLevel(LevelButton::create(0, "", LevelButtonType::DESIGN_NEW));


		for (int i = 0; i < levelNamesDesigner.size(); i++) {
			levelsDesigner->addLevel(LevelButton::create(0, levelNamesDesigner.at(i), LevelButtonType::DESIGN));
		}


		for (int i = 0; i < levelNamesUnlock.size(); i++){
			levelsDesigner->addLevel(LevelButton::create(0, levelNamesUnlock.at(i), LevelButtonType::GAME_UNLOCK));
		}

		for (int i = 0; i < levelNamesLock.size(); i++){
			levelsDesigner->addLevel(LevelButton::create(0, levelNamesLock.at(i), LevelButtonType::GAME_LOCK));
		}
	}
	levelsDesigner->positionLevelButtons();


	levelsDesignerPosition[LevelState::DESIGNER] = Vec2(0.0f, 0.0f);
	levelsDesignerPosition[LevelState::GAME] = Vec2(0.0f, 0.0f) + Vec2(getContWidth(bg), 0.0f);

	levelsGamePosition[LevelState::DESIGNER] = Vec2(0.0f, 0.0f) + Vec2(-getContWidth(bg), 0.0f);
	levelsGamePosition[LevelState::GAME] = Vec2(0.0f, 0.0f);


	updateLevelState(levelState, false);

	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = std::bind(&LevelLayer::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchMoved = std::bind(&LevelLayer::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchEnded = std::bind(&LevelLayer::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchCancelled = std::bind(&LevelLayer::onTouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	keyboardListener->onKeyPressed = &defaultKayboardListenerOnKeyReleased;
	keyboardListener->onKeyReleased = &defaultKayboardListenerOnKeyReleased;

}


void LevelLayer::onExit(){
	Layer::onExit();

}

void LevelLayer::updateLevelState(LevelState _levelState, bool isAnimation) {
	levelState = _levelState;


	buttonDelete->setVisible(numOfButtonTypesMap[LevelButtonType::DESIGN] != 0);



	if (isAnimation){
		levelsDesigner->stopAllActions();
		levelsGame->stopAllActions();

		Action *levelsDesignerAction = EaseSineInOut::create(MoveTo::create(TRANSITION_TIME, levelsDesignerPosition[levelState]));
		Action *levelsGameAction = EaseSineInOut::create(MoveTo::create(TRANSITION_TIME, levelsGamePosition[levelState]));

		levelsDesigner->runAction(levelsDesignerAction);
		levelsGame->runAction(levelsGameAction);
		

		buttonDesigner->setTSpeed((levelStateT[levelState] - buttonDesigner->getT()) / TRANSITION_TIME);
		buttonGame->setTSpeed((levelStateT[levelState] - buttonGame->getT()) / TRANSITION_TIME);

		buttonDelete->stopAllActions();
		if (LevelState::DESIGNER == levelState){
			buttonDelete->runAction(FadeIn::create(TRANSITION_TIME));
		}
		else if (LevelState::GAME == levelState){
			buttonDelete->runAction(FadeOut::create(TRANSITION_TIME));
		}
	}
	else {
		levelsDesigner->setPosition(levelsDesignerPosition[levelState]);
		levelsGame->setPosition(levelsGamePosition[levelState]);

		buttonDesigner->setT(levelStateT[levelState]);
		buttonGame->setT(levelStateT[levelState]);

		if (LevelState::DESIGNER == levelState){
			buttonDelete->setOpacity(255);
		}
		else if(LevelState::GAME == levelState){
			buttonDelete->setOpacity(0);
		}

	}
}

bool LevelLayer::onTouchBegan(Touch *touch, Event *unused_event) {
	if (nullptr != main_touch) {
		return true;
	}
	main_touch = touch;

	Vec2 location = PointApplyTransform(touch->getLocation(), bg->getWorldToNodeTransform());

	levelsDesigner->setPressButton(nullptr);
	levelsGame->setPressButton(nullptr);

	bool isLoadButtonPressed = false;


	if (LevelState::GAME == levelState){
		if (buttonDesigner->getBoundingBox().containsPoint(location)){
			isLoadButtonPressed = true;
			buttonDesigner->setScale(0.9f);
		}
	}
	else if (LevelState::DESIGNER == levelState){
		if (buttonGame->getBoundingBox().containsPoint(location)){
			isLoadButtonPressed = true;
			buttonGame->setScale(0.9f);
		}
		else if (buttonDelete->getBoundingBox().containsPoint(location)){
			isLoadButtonPressed = true;
			buttonDelete->setScale(0.9f);
		}
	}


	if (!isLoadButtonPressed){
		if (LevelState::GAME == levelState && levelsGame->layerRect.containsPoint(location)){
			levelsGame->onTouchBegan(touch, unused_event);
		}
		else if (LevelState::DESIGNER == levelState && levelsDesigner->layerRect.containsPoint(location)){
			levelsDesigner->onTouchBegan(touch, unused_event);
		}
	}



	return true;

}
void LevelLayer::onTouchMoved(Touch *touch, Event *unused_event) {
	if (touch != main_touch) {
		return;
	}
	Vec2 location = PointApplyTransform(touch->getLocation(), bg->getWorldToNodeTransform());


	levelsDesigner->setPressButton(nullptr);
	levelsGame->setPressButton(nullptr);

	bool isLoadButtonPressed = false;


	if (LevelState::GAME == levelState){
		buttonDesigner->setScale(1.0f);

		if (buttonDesigner->getBoundingBox().containsPoint(location)){
			isLoadButtonPressed = true;
			buttonDesigner->setScale(0.9f);
		}
	}
	else if (LevelState::DESIGNER == levelState){
		buttonGame->setScale(1.0f);
		buttonDelete->setScale(1.0f);


		if (buttonGame->getBoundingBox().containsPoint(location)){
			isLoadButtonPressed = true;
			buttonGame->setScale(0.9f);
		}
		else if (buttonDelete->getBoundingBox().containsPoint(location)){
			isLoadButtonPressed = true;
			buttonDelete->setScale(0.9f);
		}
	}



	if (!isLoadButtonPressed){
		if (LevelState::GAME == levelState && levelsGame->layerRect.containsPoint(location)){
			levelsGame->onTouchMoved(touch, unused_event);
		}
		else if (LevelState::DESIGNER == levelState && levelsDesigner->layerRect.containsPoint(location)){
			levelsDesigner->onTouchMoved(touch, unused_event);
		}
	}
}
void LevelLayer::onTouchEnded(Touch *touch, Event *unused_event) {
	if (touch != main_touch) {
		return;
	}
	main_touch = nullptr;

	Vec2 location = PointApplyTransform(touch->getLocation(), bg->getWorldToNodeTransform());

	levelsDesigner->setPressButton(nullptr);
	levelsGame->setPressButton(nullptr);

	bool isLoadButtonPressed = false;

	if (LevelState::GAME == levelState){
		buttonDesigner->setScale(1.0f);


		if (buttonDesigner->getBoundingBox().containsPoint(location)){
			isLoadButtonPressed = true;

			updateLevelState(LevelState::DESIGNER, true);
		}

	}
	else if (LevelState::DESIGNER == levelState){
		buttonGame->setScale(1.0f);
		buttonDelete->setScale(1.0f);

		if (buttonGame->getBoundingBox().containsPoint(location)){
			isLoadButtonPressed = true;

			updateLevelState(LevelState::GAME, true);
		}
		else if (buttonDelete->getBoundingBox().containsPoint(location)){
			isLoadButtonPressed = true;

			deleteButtonOn = !deleteButtonOn;
			levelsDesigner->setDeleteButtonOn(deleteButtonOn);
		}
	}



	LevelButton *pressButton = nullptr;
	if (!isLoadButtonPressed){
		if (LevelState::GAME == levelState && levelsGame->layerRect.containsPoint(location)){
			levelsGame->onTouchEnded(touch, unused_event);
			pressButton = levelsGame->getPressButton();
		}
		else if (LevelState::DESIGNER == levelState && levelsDesigner->layerRect.containsPoint(location)){
			levelsDesigner->onTouchEnded(touch, unused_event);
			pressButton = levelsDesigner->getPressButton();
		}
	}



	if (nullptr != pressButton && LevelButtonType::GAME_LOCK != pressButton->levelButtonType) {
		AudioManager::getInstance()->playSound(SoundType::CLICK);

		if (LevelState::DESIGNER == levelState && deleteButtonOn && LevelButtonType::DESIGN == pressButton->levelButtonType){

			std::string fullPath = Global::getInstance()->getFullPathForLevelDirectoryAndLevelName(LEVEL_TYPE::CUSTOM_LEVELS, pressButton->levelName);
			FileUtils::getInstance()->removeFile(fullPath);

			levelsDesigner->removeLevel(pressButton);

		}
		else {
            LEVEL_TYPE levelType;
			std::string levelName;
			switch (pressButton->levelButtonType) {
			case LevelButtonType::DESIGN:
                    levelType = LEVEL_TYPE::CUSTOM_LEVELS;
                    levelName = pressButton->levelName;
				break;
			case LevelButtonType::DESIGN_NEW:
                    levelType = LEVEL_TYPE::CUSTOM_LEVELS;
                    levelName = "";
				break;
			case LevelButtonType::GAME_LOCK:
			case LevelButtonType::GAME_UNLOCK:
                    levelType = LEVEL_TYPE::GAME_LEVELS;
                    levelName = pressButton->levelName;
				break;
			}


			Scene *newScene;

			if (LevelState::GAME == levelState) {
				std::function<void()> _returnLastInterface = []() {
					Scene *pScene = LevelLayer::createScene(LevelState::GAME);
					Director::getInstance()->replaceScene(pScene);
				};

				newScene = GameLayer::createScene(levelType, levelName, pressButton->levelNumber, _returnLastInterface);
			}
			else if (LevelState::DESIGNER == levelState) {
				newScene = HelloWorld::createScene(levelType, levelName);
			}

			Director::getInstance()->replaceScene(newScene);
		}

	}
}
void LevelLayer::onTouchCancelled(Touch *touch, Event *unused_event) {
	onTouchEnded(touch, unused_event);
}
