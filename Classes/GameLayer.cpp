#include "GameLayer.h"
#include "Global.h"

#include "FieldAbstract.h"
#include "Constants.h"

#include "ValueMap.h"

#include "PauseLayer.h"

#include "WinLayer.h"

#include "LoseLayer.h"

#include "PipeNode.h"

#include "ShopLayer.h"

#include "DDUserDefault.h"


#define Z_ROTATE_CIRCLE 1

#define Z_BACKGROUND -10

#define Z_PIPE 20

#define Z_PROP_BTN 22

#define Z_SADDLE 2

#define Z_TOP_BOUNDARY 10

#define Z_BACK_BTN 11
#define Z_GOLD_BAR 12

#define Z_FIELD_GAME 4
#define Z_FALL_BUBBLE 7

#define Z_PREDICTION_LINE  5

#define Z_CLOCK 1



#define Z_FLY_ANIMAL 11
#define Z_POPUP_LAYER 100



#define SCHEDULE_MOVE_PIPES 

#define SCHEDULE_UPDATE_FIELD "SCHEDULE_UPDATE"
#define SCHEDULE_UPDATE_FIELD_INTERVAL 0.0f

#define SCHEDULE_FALL_BUBBLES_UPDATE "SCHEDULE_FALL_BUBBLES_UPDATE"
#define SCHEDULE_FALL_BUBBLES_UPDATE_INTERVAL 0.0f

#define SCHEDULE_UPDATE_PREDICTION_LINE "SCHEDULE_UPDATE_PREDICTION_LINE"
#define SCHEDULE_UPDATE_PREDICTION_LINE_INTERVAL 1.0f/24.0f

#define SCHEDULE_TIME "SCHEDULE_TIME"
#define SCHEDULE_TIME_INTERVAL 1.0f

#define SCHEDULE_POP_END_GAME_1_BUBBLES "SCHEDULE_POP_END_GAME_1_BUBBLES"
#define SCHEDULE_POP_END_GAME_1_BUBBLES_INTERVAL 0.5f


#define TAG_BACKGROUND 1
#define TAG_FALL_BUBBLE 2


#define PHYSICS_GRAVITY Vec2(0, -1000.0f)





const static PhysicsMaterial fallBubbleMaterial(0.5f, 0.8f, 0.1f);
const static PhysicsMaterial gameLayerMaterial(0.5f, 0.8f, 0.1f);
const static PhysicsMaterial pipeNodeMaterial(0.5f, 0.8f, 0.1f);

GameLayer::GameLayer(LEVEL_TYPE _levelType, std::string _levelName, int _levelNumber, std::function<void()> _returnLastInterface)
:levelType(_levelType), levelName(_levelName), returnLastInterface(_returnLastInterface), levelNumber(_levelNumber){
	
	maxNumOfShotBubbles = 2;
	isMovingSaddleView = false;
	main_touch = nullptr;

}

GameLayer *GameLayer::create(LEVEL_TYPE _levelType, std::string _levelName, int _levelNumber, std::function<void()> _returnLastInterface){
	GameLayer *gameLayer = new GameLayer(_levelType, _levelName, _levelNumber, _returnLastInterface);
	if (gameLayer && gameLayer->init()){
		gameLayer->autorelease();
	}
	else {
		delete gameLayer;
		gameLayer = nullptr;
	}

	return gameLayer;
}
Scene *GameLayer::createScene(LEVEL_TYPE _levelType, std::string _levelName, int _levelNumber, std::function<void()> _returnLastInterface){
	Scene *pScene = Scene::createWithPhysics();
	PhysicsWorld *physicsWorld = pScene->getPhysicsWorld();


	physicsWorld->setSubsteps(2);
	physicsWorld->setGravity(PHYSICS_GRAVITY);
	//physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	//physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);

	pScene->addChild(GameLayer::create(_levelType, _levelName, _levelNumber, _returnLastInterface));

	return pScene;
}

void GameLayer::onEnter(){
	Layer::onEnter();



	rapidjson::Document document;
	std::string fullPath = Global::getInstance()->getFullPathForLevelDirectoryAndLevelName(levelType, levelName);
	document.Parse(FileUtils::getInstance()->getStringFromFile(fullPath).c_str());

	gameMode.setGameModeType((GameModeType)document[VALUEMAP_GAMEMODE_GAME_TYPE].GetInt());
	gameMode.setGameModeAmount(document[VALUEMAP_GAMEMODE_GAME_AMOUNT].GetInt());
	gameMode.setGameModeClearTop(document[VALUEMAP_GAMEMODE_CLEAR_TOP].GetInt());
	//gameMode.setGameModeAmount(5);

	loadArmature();

	background = Sprite::create("shared/background_1.png");
	background->setAnchorPoint(Vec2(0.5f, 0.0f));
	background->setPosition(Vec2(getContWidth(this) / 2.0f, 0.0f));
	background->setScaleX(getContWidth(this) / getContWidth(background));

	if (getContHeight(background) < getContHeight(this)){
		background->setScaleY(getContHeight(this) / getContHeight(background));
	}
	this->addChild(background, Z_BACKGROUND);






	Rect fieldGameRect;
	Rect topBoundaryRect;
	{
		Vec2 topBoundaryOrigin = Vec2(0.0f, getContHeight(this) - 120.0f);
		Size topBoundarySize = Size(getContWidth(this), 120.0f);
		topBoundaryRect = Rect(topBoundaryOrigin, topBoundarySize);

		Vec2 fieldRectOrigin = PointApplyTransform(Vec2(0.0f, 380), background->getNodeToWorldTransform()) + Vec2(0.0f, 220.0f);
		Size fieldRectSize = Size(getContWidth(this), topBoundaryRect.origin.y - fieldRectOrigin.y);
		fieldGameRect = Rect(fieldRectOrigin, fieldRectSize);
	}


	FieldGameCallbacks callbacks;
	callbacks.eventBubbleFallCallback = std::bind(&GameLayer::eventFallCallback, this, std::placeholders::_1, std::placeholders::_2);
	callbacks.animalWitchDestroyCallback = std::bind(&GameLayer::animalWitchDestroyCallback, this, std::placeholders::_1);
	callbacks.executeVisit = std::bind(&GameLayer::executeVisit, this, std::placeholders::_1);
	callbacks.setFieldGameState = std::bind(&GameLayer::setFieldGameStateCallback, this, std::placeholders::_1);

	fieldGame = FieldGame::create(fieldGameRect, 11, callbacks);
	fieldGame->setLevelBoardMap(document);
	this->addChild(fieldGame, Z_FIELD_GAME);



	topBoundary = TopBoundary::create(topBoundaryRect);
	this->addChild(topBoundary, Z_TOP_BOUNDARY);
	topBoundary->setNumOfWitchesTotal(fieldGame->getBubbleNumWitch());
	topBoundary->setNumOAnimalTotal(fieldGame->getBubbleNumAnimal());
	topBoundary->setNumOfClearTopTotal(gameMode.getGameModeClearTop());


















	{
		const float pointOffset = fieldGame->getInnerRadius()*PREDICTION_LINE_OFFSET_MULTIPLIER;
		const float minBubbleRadius = fieldGame->getInnerRadius()*PREDICTION_LINE_MIN_BUBBLE_RADIUS_MULTIPLIER;
		const float maxBubbleRadius = fieldGame->getInnerRadius()*PREDICTION_LINE_MAX_BUBBLE_RADIUS_MULTIPLIER;

		predictionLine = PredictionLine::create(pointOffset, minBubbleRadius, maxBubbleRadius);
		this->addChild(predictionLine, Z_PREDICTION_LINE);
	}
	





	rotateCircle = Sprite::create("balls/circle.png");
	rotateCircle->setPosition(Vec2(160.0f, 370.0f - 100.0f));
	this->addChild(rotateCircle, Z_ROTATE_CIRCLE);


	SaddleViewStruct saddleViewStruct;
	saddleViewStruct.leftCoord = Vec2(getContWidth(this) / 2.0f - 70.0f, 370.0f);
	saddleViewStruct.rightCoord = Vec2(getContWidth(this) / 2.0f + 70.0f, 370.0f);
	saddleViewStruct.midCoord = (saddleViewStruct.leftCoord + saddleViewStruct.rightCoord) / 2.0f;

	saddleViewStruct.minRotateCircleRadius = (getContWidth(rotateCircle) / 2.0f)*0.9f;
	saddleViewStruct.reserveBubblesCenter = rotateCircle->getPosition();

	saddleViewStruct.innerRadius = fieldGame->getInnerRadius();
	saddleViewStruct.minRadius = 100.0f;
	saddleViewStruct.maxRadius = 200.0f;
	saddleViewStruct.gravity = Vec2(0, -1.0f);
	saddleViewStruct.numOfSaddleBalls = 4;

	saddleViewStruct.hasMinSaddleRotation = true;
	saddleViewStruct.minSaddleRotation = -80.0f;
	saddleViewStruct.hasMaxSaddleRotation = true;
	saddleViewStruct.maxSaddleRotation = 80.0f;

	saddleView = SaddleView::create(saddleViewStruct);
	this->addChild(saddleView, Z_SADDLE);


	normalizeShootBubbles();








	myClock = Clock::create(gameMode.getGameModeAmount());
	myClock->setPosition(Vec2(getContWidth(this) / 2.0f + 190.0f, 270.0f));
	this->addChild(myClock, Z_CLOCK);




	const float offset = 40.0f;

	leftFallBoundary = -offset / 2.0f;
	rightFallBoundary = getContWidth(this) + offset / 2.0f;
	bottomFallBoundary = -offset / 2.0f;
	topFallBoundary = getContHeight(this) + offset / 2.0f + (fieldGame->getBackTopY() - fieldGame->getBackBottomY());



	PhysicsShape *leftFallShape;
	{
		Size leftFallShapeSize(offset, topFallBoundary - bottomFallBoundary);
		Vec2 leftFallShapeOffset(leftFallBoundary - getContWidth(this) / 2.0f, (topFallBoundary - bottomFallBoundary) / 2.0f - getContHeight(this) / 2.0f);

		leftFallShape = PhysicsShapeBox::create(leftFallShapeSize, gameLayerMaterial, leftFallShapeOffset);
	}

	PhysicsShape *rightFallShape;
	{
		Size rightFallShapeSize(offset, topFallBoundary - bottomFallBoundary);
		Vec2 rightFallShapeOffset(rightFallBoundary - getContWidth(this) / 2.0f, (topFallBoundary - bottomFallBoundary) / 2.0f - getContHeight(this) / 2.0f);

		rightFallShape = PhysicsShapeBox::create(rightFallShapeSize, gameLayerMaterial, rightFallShapeOffset);
	}

	PhysicsShape *bottomFallShape;
	{
		Size bottomFallShapeSize(rightFallBoundary - leftFallBoundary, offset);
		Vec2 bottomFallShapeOffset(0.0f, bottomFallBoundary - getContHeight(this) / 2.0f);

		bottomFallShape = PhysicsShapeBox::create(bottomFallShapeSize, gameLayerMaterial, bottomFallShapeOffset);

		bottomFallShape->setContactTestBitmask(0xFFFFFFFF);
	}

	PhysicsShape *topFallShape;
	{
		Size topFallShapeSize(rightFallBoundary - leftFallBoundary, offset);
		Vec2 topFallShapeOffset(0.0f, topFallBoundary - getContHeight(this) / 2.0f);

		topFallShape = PhysicsShapeBox::create(topFallShapeSize, gameLayerMaterial, topFallShapeOffset);
	}

	PhysicsBody *backgroundBody = PhysicsBody::create();
	backgroundBody->setDynamic(false);
	backgroundBody->addShape(leftFallShape);
	backgroundBody->addShape(rightFallShape);
	backgroundBody->addShape(bottomFallShape);
	backgroundBody->addShape(topFallShape);


	background->setPhysicsBody(backgroundBody);
	background->setTag(TAG_BACKGROUND);

	//Add the pipes

	{
		float innerRadius = fieldGame->getInnerRadius();
		float innerDiameter = innerRadius*2.0f;
		int numOfPipes = (int)(getContWidth(this) / (2.0f*innerDiameter));

		const float pipeWidth = getContWidth(this) / numOfPipes;

		for (int i = 0; i < numOfPipes; i++){
			float pipeHeight = 145.0f;
			PipeNode *pipeNode = PipeNode::create(pipeHeight, pipeWidth, pipeNodeMaterial);
			Vec2 pipePosition(pipeWidth*i + pipeWidth / 2.0f, pipeHeight / 2.0f);

			pipeNode->setPosition(pipePosition);
			this->addChild(pipeNode, Z_PIPE);
		}
	}





	//Adding the keyboard listener
	keyboardListener->onKeyPressed = std::bind(&GameLayer::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
	keyboardListener->onKeyReleased = std::bind(&GameLayer::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);

	//Adding the touch listener
	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = std::bind(&GameLayer::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchMoved = std::bind(&GameLayer::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchEnded = std::bind(&GameLayer::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchCancelled = std::bind(&GameLayer::onTouchCancelled, this, std::placeholders::_1, std::placeholders::_2);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);



	physicsContact = EventListenerPhysicsContact::create();
	physicsContact->onContactBegin = std::bind(&GameLayer::onContactBegin, this, std::placeholders::_1);
	physicsContact->onContactSeparate = std::bind(&GameLayer::onContactSeparate, this, std::placeholders::_1);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(physicsContact, this);




	pauseBtn = Sprite::create("backBtn.png");
	pauseBtn->setPosition(Vec2(getContWidth(pauseBtn) / 2.0f, getContHeight(this) - getContHeight(pauseBtn) / 2.0f));
	this->addChild(pauseBtn, Z_BACK_BTN);


	//Add the propBtns
	{
		std::vector<PropType> propTypes;
		propTypes.push_back(PropType::CHROME);
		propTypes.push_back(PropType::ADD_RESERVE);
		propTypes.push_back(PropType::ADD_TIMEMOVE);



		const Vec2 startPosition = Vec2(100.0f, 70.0f);
		const Vec2 endPosition = Vec2(getContWidth(this) - startPosition.x, startPosition.y);
		const Vec2 incrPosition = (endPosition - startPosition) / (propTypes.size() - 1);

		Vec2 position = startPosition;
		for (PropType propType : propTypes){
			PropBtn *propBtn = PropBtn::create(propType, gameMode.getGameModeType(), DDUserDefault::getInstance()->getPropCost(propType));
			propBtn->setPosition(position);
			this->addChild(propBtn, Z_PROP_BTN);

			propBtns.push_back(propBtn);

			position += incrPosition;
		}
	}


	goldBar = GoldBar::create(true);
	goldBar->setPosition(topBoundaryRect.origin + Vec2(topBoundaryRect.size.width / 2.0f, topBoundaryRect.size.height) + Vec2(0.0f, -getContHeight(goldBar)/2.0f));
	this->addChild(goldBar, Z_GOLD_BAR);



	updateTopBoundary();


	scheduleUpdates();
}

void GameLayer::onExit(){
	Layer::onExit();

	unloadArmature();
}



bool GameLayer::onTouchBegan(Touch *touch, Event *unused_event){
	if (nullptr != main_touch){
		return true;
	}
	main_touch = touch;
	Vec2 location = touch->getLocation();



	//check if buttons are pressed
	bool isButtonPressed = false;
	if (!isMovingSaddleView){
		if (goldBar->getBoundingBox().containsPoint(location)){
			goldBar->setIsPressed(true);
			isButtonPressed = true;
		}else if (pauseBtn->getBoundingBox().containsPoint(location)){
			pauseBtn->setScale(0.9f);
			isButtonPressed = true;
		}
		else if (rotateCircle->getBoundingBox().containsPoint(location)){
			rotateCircle->setScale(0.9f);
			isButtonPressed = true;
		}
		else if (FieldGameState::NORMAL == fieldGame->getFieldGameState()){
			for (PropBtn *propBtn : propBtns){
				if (propBtn->getBoundingBox().containsPoint(location)){
					propBtn->setScale(0.9f);
					isButtonPressed = true;
				}
			}
		}
	}



	if (!isButtonPressed){
		if (saddleView->canSetSaddleNodePosition(location)){
			setIsMovingSaddleView(location);
		}
		else {
			setIsMovingSaddleViewRest();
		}
	}


	return true;
}

void GameLayer::onTouchMoved(Touch *touch, Event *unused_event){
	if (touch != main_touch){
		return;
	}
	Vec2 location = touch->getLocation();


	//check if buttons are pressed

	bool isButtonPressed = false;
	if (!isMovingSaddleView){
		pauseBtn->setScale(1.0f);
		rotateCircle->setScale(1.0f);
		goldBar->setIsPressed(false);

		for (PropBtn *propBtn : propBtns){
			propBtn->setScale(1.0f);
		}


		if (goldBar->getBoundingBox().containsPoint(location)){
			goldBar->setIsPressed(true);
			isButtonPressed = true;
		}else if (pauseBtn->getBoundingBox().containsPoint(location)){
			pauseBtn->setScale(0.9f);
			isButtonPressed = true;
		}
		else if (rotateCircle->getBoundingBox().containsPoint(location)){
			rotateCircle->setScale(0.9f);
			isButtonPressed = true;
		}
		else if (FieldGameState::NORMAL == fieldGame->getFieldGameState()){
			for (PropBtn *propBtn : propBtns){
				if (propBtn->getBoundingBox().containsPoint(location)){
					propBtn->setScale(0.9f);
					isButtonPressed = true;
				}
			}
		}

	}

	if (!isButtonPressed){
		if (saddleView->canSetSaddleNodePosition(location)){
			setIsMovingSaddleView(location);
		}
		else {
			setIsMovingSaddleViewRest();
		}
	}



}
void GameLayer::onTouchEnded(Touch *touch, Event *unused_event){
	if (touch != main_touch){
		return;
	}
	main_touch = nullptr;

	Vec2 location = touch->getLocation();


	//check if buttons are pressed
	bool isButtonPressed = false;
	if (!isMovingSaddleView){
		pauseBtn->setScale(1.0f);
		rotateCircle->setScale(1.0f);
		goldBar->setIsPressed(false);

		for (PropBtn *propBtn : propBtns){
			propBtn->setScale(1.0f);
		}

		if (goldBar->getBoundingBox().containsPoint(location)){
			goldBarCallback();
			isButtonPressed = true;
		}else if (pauseBtn->getBoundingBox().containsPoint(location)){
			pauseBtnCallback();
			isButtonPressed = true;
		}
		else if (rotateCircle->getBoundingBox().containsPoint(location)){
			saddleView->rotateBubbles();
			isButtonPressed = true;
		}
		else if (FieldGameState::NORMAL == fieldGame->getFieldGameState()){
			for (PropBtn *propBtn : propBtns){
				if (propBtn->getBoundingBox().containsPoint(location)){
					propBtnCallback(propBtn->propType);
					isButtonPressed = true;
				}
			}
		}
	}


	if (!isButtonPressed && isMovingSaddleView){
		if (saddleView->canSetSaddleNodePosition(location)){
			setIsMovingSaddleView(location);


			if (saddleView->getNumOfShootBubbles() != 0 && FieldGameState::NORMAL == fieldGame->getFieldGameState()){

				if (GameModeType::MOVE == gameMode.getGameModeType()){
					gameMode.decrGameModeAmount();
					myClock->setTimeMove(gameMode.getGameModeAmount());

					if (0 == gameMode.getGameModeAmount()){

					}
				}

				addFrontShootBubble();
			}



			setIsMovingSaddleViewRest();
		}
		else {
			setIsMovingSaddleViewRest();
		}
	}
}

void GameLayer::addFrontShootBubble(){
	ShootBubble *holdBubble = saddleView->popBubbleFront();
	holdBubble->retain();

	holdBubble->setPosition(PointApplyTransform(holdBubble->getPosition(), fieldGame->getWorldToNodeTransform()));
	holdBubble->removeFromParentAndCleanup(false);

	fieldGame->addShootBubble(holdBubble);
	holdBubble->release();


	normalizeShootBubbles();
}

void GameLayer::onTouchCancelled(Touch *touch, Event *unused_event){
	onTouchEnded(touch, unused_event);
}

void GameLayer::updateField(float dt){
	fieldGame->updateField(dt);
}


void GameLayer::eventFallCallback(BubbleView *fallBubble, Vec2 velocity){
	this->addChild(fallBubble, Z_FALL_BUBBLE);

	Vec2 fallBubblePosition = PointApplyTransform(fallBubble->getPosition(), fieldGame->getNodeToWorldTransform());
	fallBubble->setPosition(fallBubblePosition);
	fallBubble->setTag(TAG_FALL_BUBBLE);

	PhysicsBody *physicsBody = PhysicsBody::create();
	physicsBody->setDynamic(true);
	if (FieldGameState::NORMAL != fieldGame->getFieldGameState()){
		if (velocity != Vec2(0.0f, 0.0f)){
			float velocityAngle = CC_DEGREES_TO_RADIANS(random(-END2_VELOCITY_ANGLE, END2_VELOCITY_ANGLE));
			velocity = velocity.rotateByAngle(Vec2(0.0f, 0.0f), velocityAngle);
		}
		else {
			velocity = Vec2(random(-END3_VELOCITY_ZERO, END3_VELOCITY_ZERO), random(-END3_VELOCITY_ZERO, END3_VELOCITY_ZERO));
		}
		
	}
	physicsBody->setVelocity(velocity);

	PhysicsShapeCircle *circle = PhysicsShapeCircle::create(fallBubble->innerRadius, fallBubbleMaterial, Vec2(0.0f, 0.0f));
	physicsBody->addShape(circle);



	
	bool rotationEnable;
	{
		BubbleType bubbleType = fallBubble->getBubble().getBubbleType();
		if (bubbleType >= BubbleType::FIRST_BASIC_BUBBLE_TYPE && bubbleType <= BubbleType::LAST_BASIC_BUBBLE_TYPE){
			rotationEnable = false;
		}
		else {
			rotationEnable = true;
		}
	}
	physicsBody->setRotationEnable(rotationEnable);

	fallBubble->setPhysicsBody(physicsBody);


	fallBubbles.insert(fallBubble);
}


void GameLayer::updateFallBubbles(float dt) {
	for (std::set<BubbleView *>::iterator fallBubblesIter = fallBubbles.begin(); fallBubblesIter != fallBubbles.end();) {
		BubbleView *fallBubble = *fallBubblesIter;

		Vec2 fallBubblePosition = fallBubble->getPosition();
		bool isOutOfBoundary = fallBubblePosition.x < leftFallBoundary || fallBubblePosition.x > rightFallBoundary || fallBubblePosition.y < bottomFallBoundary || fallBubblePosition.y > topFallBoundary;

		if (isOutOfBoundary) {
			fallBubblesIter = fallBubbles.erase(fallBubblesIter);
		}
		else {
			fallBubblesIter++;
		}
	}


	if (fallBubbles.empty() && FieldGameState::END2 == fieldGame->getFieldGameState() && !fieldGame->getModelBubblesVisible()){
		fieldGame->setFieldGameState(FieldGameState::END3);
	}

}

bool GameLayer::onContactBegin(PhysicsContact &physicsContact) {
	PhysicsShape *shapeA = physicsContact.getShapeA();
	PhysicsShape *shapeB = physicsContact.getShapeB();

	PhysicsBody *bodyA = shapeA->getBody();
	PhysicsBody *bodyB = shapeB->getBody();

	Node *nodeA = bodyA->getNode();
	Node *nodeB = bodyB->getNode();

	BubbleView *fallBubble = nullptr;

	if (nodeA->getTag() == TAG_FALL_BUBBLE) {
		fallBubble = (BubbleView *)nodeA;
	}
	else if (nodeA->getTag() == TAG_FALL_BUBBLE) {
		fallBubble = (BubbleView *)nodeB;
	}


	if (nullptr != fallBubble) {
		fallBubble->removeFromParentAndCleanup(true);
		fallBubbles.erase(fallBubble);

	}

	
	if (fallBubbles.empty() && FieldGameState::END2 == fieldGame->getFieldGameState() && !fieldGame->getModelBubblesVisible()){
		fieldGame->setFieldGameState(FieldGameState::END3);
	}

	return true;
}
void GameLayer::onContactSeparate(PhysicsContact &physicsContact) {}

void GameLayer::setPredictionLineBubbleTypes(BubbleType bubbleType){
	std::vector<BubbleType> bubbleTypes;

	switch (bubbleType){
	case BubbleType::CHROME:
		bubbleTypes.push_back(BubbleType::RED);
		bubbleTypes.push_back(BubbleType::BLUE);
		bubbleTypes.push_back(BubbleType::GREEN);
		bubbleTypes.push_back(BubbleType::YELLOW);
		bubbleTypes.push_back(BubbleType::PURPLE);
		break;
	case BubbleType::RED:
	case BubbleType::BLUE:
	case BubbleType::GREEN:
	case BubbleType::YELLOW:
	case BubbleType::PURPLE:
		bubbleTypes.push_back(bubbleType);
		break;
	}

	predictionLine->setBubbleTypes(bubbleTypes);
}

void GameLayer::startSchedulePredictionLine(){
	if (this->isScheduled(SCHEDULE_UPDATE_PREDICTION_LINE)){
		return;
	}


	ShootBubble *bubble0 = saddleView->peekBubbleFront();
	if (nullptr == bubble0){
		return;
	}
	setPredictionLineBubbleTypes(bubble0->getBubble().getBubbleType());


	updatePredictionLine(0.0f);
	this->schedule(std::bind(&GameLayer::updatePredictionLine, this, std::placeholders::_1), SCHEDULE_UPDATE_PREDICTION_LINE_INTERVAL, SCHEDULE_UPDATE_PREDICTION_LINE);

}
void GameLayer::stopSchedulePredictionLine(){
	if (!this->isScheduled(SCHEDULE_UPDATE_PREDICTION_LINE)){
		return;
	}

	predictionLine->setControlPoints(std::vector<Vec2>());
	this->unschedule(SCHEDULE_UPDATE_PREDICTION_LINE);
}

void GameLayer::updatePredictionLine(float dt){
	ShootBubble *holdBubble = saddleView->peekBubbleFront();

	Vec2 controlPoint1 = holdBubble->getPosition() + holdBubble->innerRadius*2.0f*holdBubble->getDirection();
	predictionLine->setControlPoints(fieldGame->getControlPoints(controlPoint1, holdBubble->getDirection(), holdBubble->innerRadius, holdBubble->getShootBubbleRadius()));
	predictionLine->updatePredictionLine(dt * PREDICTION_LINE_SPEED);

}







#define WITCH_BUBBLE_ARMATURE "balls/witch_all0.ExportJson"
#define ANIMAL1_ARMATURE "balls/animal1_btn0.ExportJson"
#define ANIMAL2_ARMATURE "balls/YL_btn0.ExportJson"
#define ANIMAL3_ARMATURE "balls/animal3_btn0.ExportJson"

#define ANIMAL1_EXPLODE_ARMATURE "balls/lanniao_baozha0.ExportJson"
#define ANIMAL2_EXPLODE_ARMATURE "balls/DW_effects0.ExportJson"
#define WITCH_EXPLODE_ARMATURE "balls/nvwu_baozha0.ExportJson"


std::vector<std::string> getArmatureNames(){
	std::vector<std::string> ret;
	ret.push_back(WITCH_BUBBLE_ARMATURE);
	ret.push_back(ANIMAL1_ARMATURE);
	ret.push_back(ANIMAL2_ARMATURE);
	ret.push_back(ANIMAL3_ARMATURE);

	ret.push_back(ANIMAL1_EXPLODE_ARMATURE);
	ret.push_back(ANIMAL2_EXPLODE_ARMATURE);

	ret.push_back(WITCH_EXPLODE_ARMATURE);

	return ret;
}

void GameLayer::loadArmature(){
	std::vector<std::string> armatureNames = getArmatureNames();
	for (std::string armatureName : armatureNames){
		ArmatureDataManager::getInstance()->addArmatureFileInfo(armatureName);
	}
}
void GameLayer::unloadArmature(){

	std::vector<std::string> armatureNames = getArmatureNames();
	for (std::string armatureName : armatureNames){
		ArmatureDataManager::getInstance()->removeArmatureFileInfo(armatureName);
	}
}


void GameLayer::animalWitchDestroyCallback(BubbleView *bubbleView){
	Vec2 bubbleViewWorldPos = PointApplyTransform(bubbleView->getPosition(), fieldGame->getNodeToWorldTransform());
	BubbleType bubbleType = bubbleView->getBubble().getBubbleType();

	incrNumOfTransBubble(bubbleType);


	Armature *armature = nullptr;
	{
		std::string armatureStr;
		switch (bubbleType){
		case BubbleType::ANIMAL1:
			armatureStr = "lanniao_baozha";
			break;
		case BubbleType::ANIMAL2:
			armatureStr = "DW_effects";
			break;
		case BubbleType::ANIMAL3:
			armatureStr = "DW_effects";
			break;
		case BubbleType::WITCH:
			armatureStr = "nvwu_baozha";
			break;
		}


		armature = Armature::create(armatureStr);
	}

	armature->setPosition(bubbleViewWorldPos + bubbleView->getBubbleNodePosition());
	armature->getAnimation()->playWithIndex(0);
	armature->setScale(fieldGame->getInnerRadius() / GRAPHICS_SCALE_CONSTANT);


	bubbleView->retain();
	bubbleView->removeFromParentAndCleanup(false);
	this->addChild(bubbleView, Z_FLY_ANIMAL - 1);
	bubbleView->release();
	bubbleView->setPosition(bubbleViewWorldPos);


	std::function<void(Armature *, MovementEventType, const std::string &)> movementEventFunc = std::bind(
		[this](Armature *armature, MovementEventType movementType, const std::string& movementID, BubbleView *bubbleView){
		if (MovementEventType::COMPLETE == movementType){

			BubbleType bubbleType = bubbleView->getBubble().getBubbleType();

			if (BubbleType::WITCH == bubbleType){
				decrNumOfTransBubble(bubbleType);
				topBoundary->setNumOfWitchesClear(topBoundary->getNumOfWitchesTotal() - fieldGame->getBubbleNumWitch() - getNumOfTransBubble(bubbleType));
				updateTopBoundary();
			}
			else {
				callbackFallAnimalHelper2(bubbleType, armature->getPosition());
			}

			armature->removeFromParentAndCleanup(true);
			bubbleView->removeFromParentAndCleanup(true);

		}
	}, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, bubbleView);


	armature->getAnimation()->setMovementEventCallFunc(movementEventFunc);
	this->addChild(armature, Z_FLY_ANIMAL);
}


void GameLayer::callbackFallAnimalHelper2(BubbleType bubbleType, Vec2 position){
	Vec2 startPosition = position;
	Vec2 endPosition = topBoundary->getAnimalFlyPosition();

	std::string flyAnimalStr;
	switch (bubbleType){
	case BubbleType::ANIMAL1:
		flyAnimalStr = "balls/flyAnimal1.png";
		break;
	case BubbleType::ANIMAL2:
		flyAnimalStr = "balls/flyAnimal2.png";
		break;
	case BubbleType::ANIMAL3:
		flyAnimalStr = "balls/flyAnimal3.png";
		break;
	}

	Sprite *flyAnimal = Sprite::create(flyAnimalStr);
	flyAnimal->setScale(fieldGame->getInnerRadius() / GRAPHICS_SCALE_CONSTANT);
	flyAnimal->setPosition(startPosition);
	this->addChild(flyAnimal, Z_FLY_ANIMAL);




	float duration = (startPosition - endPosition).length() / 1000.0f;



	MoveTo *moveTo = MoveTo::create(duration, endPosition);
	FadeOut *fadeOut = FadeOut::create(duration);
	Spawn *spawn = Spawn::create(moveTo, fadeOut, nullptr);


	std::function<void()> func = std::bind([this](BubbleType bubbleType){
		decrNumOfTransBubble(bubbleType);
		topBoundary->setNumOfAnimalClear(topBoundary->getNumOfAnimalTotal() - fieldGame->getBubbleNumAnimal() - getTotalNumAnimalTransBubble());
		updateTopBoundary();
	}, bubbleType);


	CallFunc *callFunc = CallFunc::create(func);
	RemoveSelf *removeSelf = RemoveSelf::create(true);

	Sequence *seq = Sequence::create(spawn, callFunc, removeSelf, nullptr);


	flyAnimal->runAction(seq);
}









void GameLayer::propBtnCallback(PropType propType){

	int propCost = DDUserDefault::getInstance()->getPropCost(propType);

	if (DDUserDefault::getInstance()->getNumOfGold() < propCost){
		popUpLayer(ShopLayer::create());
	}
	else {
		DDUserDefault::getInstance()->addNumOfGold(-propCost);

		switch (propType){
		case PropType::ADD_RESERVE:
			maxNumOfShotBubbles++;
			normalizeShootBubbles();
			break;
		case PropType::CHROME:
			saddleView->pushBubbleFront(ShootBubble::create(fieldGame->getInnerRadius(), Bubble(BubbleType::CHROME)));
			if (GameModeType::MOVE == gameMode.getGameModeType()){
				gameMode.addGameModeAmount(1);
				myClock->setTimeMove(gameMode.getGameModeAmount());
			}
			break;
		case PropType::ADD_TIMEMOVE:
			gameMode.addGameModeAmount(10);
			myClock->setTimeMove(gameMode.getGameModeAmount());
			break;
		}
	}


}


void GameLayer::goldBarCallback(){
	popUpLayer(ShopLayer::create());
}


void GameLayer::normalizeShootBubbles(){
	if (fieldGame->getActiveBasicBubbleTypes().empty()){
		return;
	}

	std::function<bool()> shouldAddShootBubbles = [this](){
		bool ret;

		switch (fieldGame->getFieldGameState()){
		case FieldGameState::START:
		case FieldGameState::NORMAL:
			ret = saddleView->getNumOfShootBubbles() < maxNumOfShotBubbles && (GameModeType::MOVE != gameMode.getGameModeType() || saddleView->getNumOfShootBubbles() < gameMode.getGameModeAmount());
			break;
		case FieldGameState::END1:
		default:
			ret = saddleView->getNumOfShootBubbles() < maxNumOfShotBubbles && saddleView->getNumOfShootBubbles() < gameMode.getGameModeAmount();
		}

		return ret;
	};



	while (shouldAddShootBubbles()){
		ShootBubble *shootBubble = ShootBubble::create(fieldGame->getInnerRadius(), Bubble(fieldGame->getRandomActiveBasicBubbleType()));
		
		//shootBubble->setScale(0.0f);
		//shootBubble->runAction(ScaleTo::create(1.0f, 1.0f));

		saddleView->pushBubbleBack(shootBubble);
	}

}


void GameLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){

}
void GameLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (EventKeyboard::KeyCode::KEY_BACK == keyCode){
		//popUpLayer(LoseLayer::create());
		pauseBtnCallback();
	}
}


void GameLayer::pauseBtnCallback(){
	std::function<void()> restartGameFunc = std::bind(&GameLayer::pauseRestartGame, this);
	std::function<void()> returnGameFunc = std::bind(&GameLayer::pauseReturnGame, this);
	std::function<void()> returnMapFunc = returnLastInterface;

	popUpLayer(PauseLayer::create(restartGameFunc, returnGameFunc, returnMapFunc));
}


void GameLayer::pauseReturnGame(){

}
void GameLayer::pauseRestartGame(){
	Director::getInstance()->replaceScene(GameLayer::createScene(levelType, levelName, levelNumber, returnLastInterface));
}

void GameLayer::popUpLayer(Layer *layer){
	unscheduleUpdates();

	PhysicsWorld *physicsWorld = this->getScene()->getPhysicsWorld();
	physicsWorld->setAutoStep(false);

	main_touch = nullptr;
	if (isMovingSaddleView){
		setIsMovingSaddleViewRest();
	}
	for (PropBtn *propBtn : propBtns){
		propBtn->setScale(1.0f);
	}
	pauseBtn->setScale(1.0f);

	
	layer->setOnExitCallback([this](){

		PhysicsWorld *physicsWorld = this->getScene()->getPhysicsWorld();
		physicsWorld->setAutoStep(true);

		scheduleUpdates();

		keyboardListener->onKeyPressed = std::bind(&GameLayer::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
		keyboardListener->onKeyReleased = std::bind(&GameLayer::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);
	});


	this->addChild(layer, Z_POPUP_LAYER);
}


void GameLayer::setIsMovingSaddleView(Vec2 location){
	isMovingSaddleView = true;

	saddleView->setSaddleNodePosition(location);
	startSchedulePredictionLine();
}
void GameLayer::setIsMovingSaddleViewRest(){
	isMovingSaddleView = false;

	saddleView->setSaddleNodePositionRest();
	stopSchedulePredictionLine();
}



void GameLayer::updatePopEndGame1Bubbles(float dt){
	addFrontShootBubble();
	if (gameMode.getGameModeAmount() > 0){
		gameMode.decrGameModeAmount();

		myClock->setTimeMove(gameMode.getGameModeAmount());
	}

	normalizeShootBubbles();


	ShootBubble *bubble0 = saddleView->peekBubbleFront();
	if (nullptr != bubble0){
		setPredictionLineBubbleTypes(bubble0->getBubble().getBubbleType());
	}
	else {
		this->unschedule(SCHEDULE_POP_END_GAME_1_BUBBLES);

		stopSchedulePredictionLine();
		fieldGame->setFieldGameState(FieldGameState::END2);
	}
}


void GameLayer::setFieldGameStateCallback(FieldGameState fieldGameState){
	switch (fieldGameState){
	case FieldGameState::NORMAL:
		if (gameMode.getGameModeType() == GameModeType::TIME){
			this->schedule(std::bind(&GameLayer::updateTime, this, std::placeholders::_1), SCHEDULE_TIME_INTERVAL, SCHEDULE_TIME);
		}

		updateTopBoundary();
		break;
	case FieldGameState::END1:
		if (gameMode.getGameModeType() == GameModeType::TIME){
			this->unschedule(SCHEDULE_TIME);
		}

		if (0 == saddleView->getNumOfShootBubbles()){
			stopSchedulePredictionLine();
			fieldGame->setFieldGameState(FieldGameState::END2);
		}
		else {
			this->schedule(std::bind(&GameLayer::updatePopEndGame1Bubbles, this, std::placeholders::_1), SCHEDULE_POP_END_GAME_1_BUBBLES_INTERVAL, SCHEDULE_POP_END_GAME_1_BUBBLES);
		}


		break;
	case FieldGameState::END2:
		if (fallBubbles.empty() && !fieldGame->getModelBubblesVisible()){
			fieldGame->setFieldGameState(FieldGameState::END3);
		}
		break;
	case FieldGameState::END3:
		DDUserDefault::getInstance()->setLevelFrontier(levelNumber + 1);

		popUpLayer(WinLayer::create(returnLastInterface));
		break;
	case FieldGameState::LOOSE1:
		popUpLayer(LoseLayer::create(returnLastInterface));
		break;
	}
}



void GameLayer::scheduleUpdates(){

	this->schedule(std::bind(&GameLayer::updateField, this, std::placeholders::_1), SCHEDULE_UPDATE_FIELD);
	this->schedule(std::bind(&GameLayer::updateFallBubbles, this, std::placeholders::_1), SCHEDULE_FALL_BUBBLES_UPDATE);

	if (gameMode.getGameModeType() == GameModeType::TIME && gameMode.getGameModeAmount() != 0 && FieldGameState::NORMAL == fieldGame->getFieldGameState()){
		this->schedule(std::bind(&GameLayer::updateTime, this, std::placeholders::_1), SCHEDULE_TIME_INTERVAL, SCHEDULE_TIME);
	}

	if (FieldGameState::END1 == fieldGame->getFieldGameState()){
		this->schedule(std::bind(&GameLayer::updatePopEndGame1Bubbles, this, std::placeholders::_1), SCHEDULE_POP_END_GAME_1_BUBBLES_INTERVAL, SCHEDULE_POP_END_GAME_1_BUBBLES);
	}
}

void GameLayer::updateTime(float dt){
	gameMode.decrGameModeAmount();
	myClock->setTimeMove(gameMode.getGameModeAmount());

	if (gameMode.getGameModeAmount() == 0){
		this->unschedule(SCHEDULE_TIME);
		updateTopBoundary();
	}
}

void GameLayer::unscheduleUpdates(){
	this->unschedule(SCHEDULE_UPDATE_FIELD);
	this->unschedule(SCHEDULE_FALL_BUBBLES_UPDATE);

	if (gameMode.getGameModeType() == GameModeType::TIME){
		this->unschedule(SCHEDULE_TIME);
	}

	if (FieldGameState::END1 == fieldGame->getFieldGameState()){
		this->unschedule(SCHEDULE_POP_END_GAME_1_BUBBLES);
	}
}




void GameLayer::executeVisit(FieldGameEventElement *eventElement){
	eventElement->accept(this);
}

void GameLayer::visitEvent(EventFall *eventFall){
	updateActiveBasicBubbleTypes();
	updateTopBoundary();
}
void GameLayer::visitEvent(EventNormalExplosion *eventNormalExplosion){
	updateActiveBasicBubbleTypes();
	updateTopBoundary();
}
void GameLayer::visitEvent(EventChromeExplosion *eventChromeExplosion){
	updateActiveBasicBubbleTypes();
	updateTopBoundary();
}
void GameLayer::visitEvent(EventWitchDestroy *eventWitchDestoy){
	updateActiveBasicBubbleTypes();
	updateTopBoundary();
}
void GameLayer::visitEvent(EventWitchChangeColor *eventWitchChangeColor){
	updateActiveBasicBubbleTypes();
	updateTopBoundary();
}
void GameLayer::visitEvent(EventSetBubble *eventSetBubble){
	updateActiveBasicBubbleTypes();
	updateTopBoundary();
}

void GameLayer::visitEvent(EventEnd2 *eventEnd2){
	updateActiveBasicBubbleTypes();
	updateTopBoundary();
}

void GameLayer::visitEvent(EventNull *eventNull){
	updateActiveBasicBubbleTypes();
	updateTopBoundary();
}

void GameLayer::visitEvent(EventWood1Break *eventWood1Break){
	updateActiveBasicBubbleTypes();
	updateTopBoundary();
}
void GameLayer::visitEvent(EventWood2Break *eventWood2Break){
	updateActiveBasicBubbleTypes();
	updateTopBoundary();
}

void GameLayer::visitEvent(EventIceBreak *eventIceBreak){
	updateActiveBasicBubbleTypes();
	updateTopBoundary();
}

void GameLayer::visitEvent(EventFog *eventFog){
	updateActiveBasicBubbleTypes();
	updateTopBoundary();
}

void GameLayer::visitEvent(EventFission *eventFission){
	updateActiveBasicBubbleTypes();
	updateTopBoundary();
}

void GameLayer::visitEvent(EventDestroy *eventDestroy){
	updateActiveBasicBubbleTypes();
	updateTopBoundary();
}

void GameLayer::updateActiveBasicBubbleTypes(){
	const std::vector<BubbleType> &activeBasicBubbleTypes = fieldGame->getActiveBasicBubbleTypes();
	saddleView->updateActiveBasicBubbleTypes(activeBasicBubbleTypes);
}


void GameLayer::updateTopBoundary(){

	if (topBoundary->getNumOfClearTopTotal() != 0){
		topBoundary->setNumOfClearTopClear(fieldGame->getNumOfEmptyElementsInTopRow());
	}

	switch (fieldGame->getFieldGameState()){
	case FieldGameState::NORMAL:
		if (topBoundary->isWin()){
			fieldGame->setFieldGameState(FieldGameState::END1);
		}
		else {
		if (fieldGame->getNumOfEvents() == 0 && fieldGame->getNumOfShootBubble() == 0 && gameMode.getGameModeAmount() == 0 && getTotalNumTransBubble() == 0){
				fieldGame->setFieldGameState(FieldGameState::LOOSE1);
			}
		}
		break;
	}
}


int GameLayer::getNumOfTransBubble(BubbleType bubbleType){
	std::map<BubbleType, int>::iterator inTransBubblesIter = inTransBubbles.find(bubbleType);

	int ret;
	if (inTransBubbles.end() == inTransBubblesIter){
		ret = 0;
	}
	else {
		ret = inTransBubblesIter->second;
	}

	return ret;
}
void GameLayer::addNumOfTransBubble(BubbleType bubbleType, int val){
	setNumOfTransBubble(bubbleType, getNumOfTransBubble(bubbleType) + val);
}
void GameLayer::incrNumOfTransBubble(BubbleType bubbleType){
	addNumOfTransBubble(bubbleType, 1);
}
void GameLayer::decrNumOfTransBubble(BubbleType bubbleType){
	addNumOfTransBubble(bubbleType, -1);
}
void GameLayer::setNumOfTransBubble(BubbleType bubbleType, int _numOfTransBubble){
	inTransBubbles[bubbleType] = _numOfTransBubble;
}
int GameLayer::getTotalNumTransBubble(){
	int ret = 0;
	for (std::pair<BubbleType, int> inTransBubblesPair : inTransBubbles){
		ret += inTransBubblesPair.second;
	}

	return ret;
}

int GameLayer::getTotalNumAnimalTransBubble(){
	int ret = 0;

	for (int i = (int)BubbleType::FIRST_ANIMAL_TYPE; i <= (int)BubbleType::LAST_ANIMAL_TYPE; i++){
		ret += getNumOfTransBubble((BubbleType)i);
	}

	return ret;
}
