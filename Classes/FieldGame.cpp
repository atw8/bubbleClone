#include "FieldGame.h"

#include "Global.h"


#include "Constants.h"

#define SCHEDULE_MOVE_FIELD_NORMAL "SCHEDULE_MOVE_FIELD_NORMAL"

#define SCHEDULE_END_GAME2_BUBBLES "SCHEDULE_END_GAME2_BUBBLES"
#define SCHEDULE_END_GAME2_BUBBLES_INTERVAL 0.3f

#include <array>

FieldGame::FieldGame(Rect _fieldRect, int _bubbleWidth, FieldGameCallbacks _fieldGameCallbacks) :FieldAbstract(_fieldRect, _bubbleWidth), totalElapsedTime(0.0f), callbacks(_fieldGameCallbacks){
	totalElapsedTime = 0.0f;
	fieldGameEventManger = new FieldGameEventManager();
}

FieldGame::~FieldGame(){
	for (std::pair<Dec2, JitterManager *> jitterManagerPair : jitterManagers) {
		delete jitterManagerPair.second;
	}
	jitterManagers.clear();

	delete fieldGameEventManger;
}

FieldGame *FieldGame::create(Rect _fieldRect, int _bubbleWidth, FieldGameCallbacks _fieldGameCallbacks) {
	FieldGame *fieldGame = new FieldGame(_fieldRect, _bubbleWidth, _fieldGameCallbacks);
	if (fieldGame && fieldGame->init()){
		fieldGame->autorelease();
	}
	else {
		delete fieldGame;
		fieldGame = nullptr;
	}

	return fieldGame;
}

void FieldGame::outitGame(bool completely){
	FieldAbstract::outitGame(completely);

	this->unschedule(SCHEDULE_MOVE_FIELD_NORMAL);


	modelBubbles.clear();
	for (std::pair<Dec2, BubbleView*> viewBubblesPair : viewBubbles){
		BubbleView *bubbleView = viewBubblesPair.second;
		bubbleView->removeFromParentAndCleanup(true);
	}
	viewBubbles.clear();

	for (ShootBubble *shootBubble : shootBubbles){
		shootBubble->removeFromParentAndCleanup(true);
	}
	shootBubbles.clear();

	fallGraph.clear();

	fieldGameEventManger->outit();
	

	for (std::pair<Dec2, JitterManager *> jitterManagerPair: jitterManagers) {
		delete jitterManagerPair.second;
	}
	jitterManagers.clear();

}
void FieldGame::initGame(){
	FieldAbstract::initGame();

	setFieldGameState(FieldGameState::START);
	
	for (std::pair<Dec2, Bubble> modelBubblesPair : modelBubbles){
		Dec2 dec2 = modelBubblesPair.first;
		Bubble bubble = modelBubblesPair.second;

		BubbleView *bubbleView = BubbleView::create(innerRadius, bubble);
		bubbleView->setPosition(getVec2ForDec2(dec2));

		this->addChild(bubbleView);

		viewBubbles[dec2] = bubbleView;
	}
	
	this->setPositionY(0.0f);


	this->schedule(std::bind(&FieldGame::moveFieldNormal, this, std::placeholders::_1), SCHEDULE_MOVE_FIELD_NORMAL);
	updateActiveBasicBubbleTypes();
}



float FieldGame::getStablePositionY(){
	float ret;
	if (!modelBubblesVisible){
		ret = 0.0f;
	}
	else {
		float bottomVec2Y = getVec2YForDec2Y(bottomModelY);
		float topVec2Y = getVec2YForDec2Y(topModelY);

		if ((topVec2Y + innerRadius) - (bottomVec2Y - innerRadius) < fieldRect.size.height){
			ret = 0.0f;
		}
		else {
			ret = -getVec2YForDec2Y(bottomModelY) + fieldRect.origin.y + (floatMod(std::sqrt(3.0f)*innerRadius, fieldRect.size.height));
		}

	}

	return ret;
}

void FieldGame::setPosition(const Vec2 &postion){
	Layer::setPosition(postion);
}



void FieldGame::calculateTopBottomModelY(){
	modelBubblesVisible = !modelBubbles.empty();

	if (modelBubblesVisible){
		std::map<Dec2, Bubble>::iterator modelBubblesIter = modelBubbles.begin();

		{
			Dec2 dec2 = modelBubblesIter->first;
			bottomModelY = topModelY = dec2.y;
		}

		for (; modelBubblesIter != modelBubbles.end(); ++modelBubblesIter){
			Dec2 dec2 = modelBubblesIter->first;

			if (bottomModelY > dec2.y){
				bottomModelY = dec2.y;
			}

			if (topModelY < dec2.y){
				topModelY = dec2.y;
			}
		}
	}
}

const FieldGameState FieldGame::getFieldGameState(){
	return fieldGameState;
}

void FieldGame::setFieldGameState(const FieldGameState _fieldGameState){
	if (_fieldGameState == fieldGameState){
		return;
	}

	fieldGameState = _fieldGameState;
	switch (fieldGameState){
	case FieldGameState::NORMAL:
		for (std::pair<Dec2, Bubble> modelBubblePair : modelBubbles){
			Dec2 dec2 = modelBubblePair.first;
			Bubble bubble = modelBubblePair.second;

			int pivotRank = fallGraph.getPivotRank(dec2);
			if (-1 == pivotRank){
				fieldGameEventManger->pushEvent(new EventFall(totalElapsedTime + EVENT_FALL_DELAY, dec2));
			}

		}
		break;
	case FieldGameState::END1:
		while (!shootBubbles.empty()){
			ShootBubble *shootBubble = *shootBubbles.begin();
			Vec2 shootBubbleVelocity = shootBubble->getVelocity();

			eventFallCreate(shootBubble, shootBubbleVelocity);
			shootBubbles.erase(shootBubbles.begin());
		}
		break;
	case FieldGameState::END2:
		fieldGameEventManger->pushEvent(new EventEnd2(totalElapsedTime + EVENT_END2_DELAY));
		break;
	}

	callbacks.setFieldGameState(fieldGameState);
}



void FieldGame::updateField(float dt){
	
	totalElapsedTime += dt;

	//Update the position of all the shoot bubbles
	for (std::set<ShootBubble *>::iterator shootBubblesIter = shootBubbles.begin(); shootBubblesIter != shootBubbles.end();){
		
		ShootBubble *shootBubble = *shootBubblesIter;

		Vec2 lastshootBubblePosition = shootBubble->getPosition();
		
		shootBubble->updateVelocity(dt);
		Vec2 shootBubblePosition = shootBubble->getPosition();
		Vec2 shootBubbleVelocity = shootBubble->getVelocity();
		Vec2 shootBubbleDirection = shootBubbleVelocity.getNormalized();

		float shootBubbleRadius = shootBubble->getShootBubbleRadius();

		std::vector<Vec2> controlPoints;
		{
			const Vec2 increment = shootBubbleDirection*shootBubbleRadius*0.5f;
			
			Vec2 controlPoint = lastshootBubblePosition + increment;
			while ((shootBubblePosition - controlPoint).dot(shootBubbleDirection) > 0){
				controlPoints.push_back(controlPoint);
				controlPoint += increment;
			}

			controlPoints.push_back(shootBubblePosition);
		}



		bool eraseShootBubble = false;
		bool isBreak = false;
		for (std::vector<Vec2>::iterator controlPointsIter = controlPoints.begin(); controlPointsIter != controlPoints.end() && !isBreak; ++controlPointsIter){
			Vec2 controlPoint = *controlPointsIter;

			shootBubble->setPosition(controlPoint);
		
			if (isOverTopEdge(controlPoint, shootBubbleVelocity, shootBubbleRadius)){
				shootBubble->removeFromParentAndCleanup(true);

				fieldGameEventManger->pushEvent(new EventNull(totalElapsedTime));
				eraseShootBubble = true;
				isBreak = true;
			}
			else if (isOverBottomEdge(controlPoint, shootBubbleVelocity, shootBubbleRadius)){
				eventFallCreate(shootBubble, shootBubbleVelocity);

				eraseShootBubble = true;
				isBreak = true;
			}
			else if (isOverLeftEdge(controlPoint, shootBubbleVelocity, shootBubbleRadius)){
				std::pair<Vec2, Vec2> leftEdgeProp = leftEdgeCollision(controlPoint, shootBubbleVelocity, shootBubbleRadius);

				shootBubble->setPosition(leftEdgeProp.first);
				shootBubble->setVelocity(leftEdgeProp.second);

				eraseShootBubble = false;
				isBreak = true;
			}
			else if (isOverRightEdge(controlPoint, shootBubbleVelocity, shootBubbleRadius)){
				std::pair<Vec2, Vec2> rightEdgeProp = rightEdgeCollision(controlPoint, shootBubbleVelocity, shootBubbleRadius);

				shootBubble->setPosition(rightEdgeProp.first);
				shootBubble->setVelocity(rightEdgeProp.second);
				
				eraseShootBubble = false;
				isBreak = true;
			}
			else if (isOverTopBoundary(controlPoint, shootBubbleVelocity) || isOverBottomBoundary(controlPoint, shootBubbleVelocity) || isOverHexagon(controlPoint, shootBubbleRadius)){
				
				
				//getCollisionHexagons(const std::set<Dec2> &dec2s, const Vec2 &position, const float &radius)
				std::set<Dec2> firstOrderHexagons = getHexagonOfOrder(1, getDec2ForVec2(controlPoint));
				std::set<Dec2> undefinedHexagons = getUndefinedHexagons(firstOrderHexagons);
				
				std::function<bool(Dec2, Dec2)> closestHexFunc = std::bind([this](Dec2 decA, Dec2 decB, Vec2 vec2){
					float distA = getVec2ForDec2(decA).distanceSquared(vec2);
					float distB = getVec2ForDec2(decB).distanceSquared(vec2);

					return distA > distB;
				}, std::placeholders::_1, std::placeholders::_2, controlPoint);

				std::set<Dec2>::iterator closestHexagonIter = std::max_element(undefinedHexagons.begin(), undefinedHexagons.end(), closestHexFunc);
				if (undefinedHexagons.empty()){
					getDec2ForVec2(controlPoint);
				}
				assert(closestHexagonIter != undefinedHexagons.end());
	

				fieldGameEventManger->pushEvent(new EventSetBubble(0.0f, *closestHexagonIter, shootBubble));
				//shootBubble->removeFromParentAndCleanup(true);
				
				eraseShootBubble = true;
				isBreak = true;
			}
		}


		if (eraseShootBubble){
			shootBubblesIter = shootBubbles.erase(shootBubblesIter);
		}
		else {
			shootBubblesIter++;
		}
	}
	



	//Check if this shoot bubble collides with any other shootBubbles
	for (ShootBubble *bubbleA : shootBubbles){
		std::set<ShootBubble *> bubbleAs;
		bubbleAs.insert(bubbleA);
		for (ShootBubble *bubbleB : shootBubbles){
			if (bubbleAs.find(bubbleB) != bubbleAs.end()) {
				continue;
			}

			Vec2 posDiff = (bubbleA->getPosition() - bubbleB->getPosition());
			Vec2 posDiffNorm = posDiff.getNormalized();

			if (posDiff.length() > bubbleA->innerRadius + bubbleB->innerRadius) {
				continue;
			}

			Vec2 velDiff = (bubbleA->getVelocity() - bubbleB->getVelocity());
			Vec2 velDiffNorm = velDiff.getNormalized();

			float vn = velDiffNorm.dot(posDiffNorm);


			if (vn > 0.0f) {
				continue; //Balls are moving away from each other
			}

			Vec2 normVec = posDiffNorm;
			Vec2 tangVec(-normVec.y, normVec.x);


			float bubbleANormOld = normVec.dot(bubbleA->getVelocity());
			float bubbleATangOld = tangVec.dot(bubbleA->getVelocity());

			float bubbleBNormOld = normVec.dot(bubbleB->getVelocity());
			float bubbleBTangOld = tangVec.dot(bubbleB->getVelocity());


			float bubbleATangNew = bubbleATangOld;
			float bubbleBTangNew = bubbleBTangOld;

			float bubbleANormNew = bubbleBNormOld;
			float bubbleBNormNew = bubbleANormOld;


			Vec2 bubbleAVelocity = bubbleATangNew*tangVec + bubbleANormNew*normVec;
			Vec2 bubbleBVelocity = bubbleBTangNew*tangVec + bubbleBNormNew*normVec;

			bubbleA->setVelocity(bubbleAVelocity);
			bubbleB->setVelocity(bubbleBVelocity);
		}
	}



	//Update the fieldgame events
	FieldGameEventElement *event = nullptr;
	while (nullptr != (event = fieldGameEventManger->popEvent(totalElapsedTime))){
		bool canExecute;
		
		switch (event->eventType){
		case EventType::ADD:
			canExecute = modelBubbles.find(event->dec2) == modelBubbles.end();
			break;
		case EventType::MODIFY:
		case EventType::REMOVE:
			canExecute = modelBubbles.find(event->dec2) != modelBubbles.end();
			break;
		case EventType::OTHER:
			canExecute = true;
			break;
		}

		if (canExecute){
			event->accept(this);
			callbacks.executeVisit(event);
		}
		else {
			fieldGameEventManger->pushEvent(new EventNull(totalElapsedTime));
		}



		delete event;
	}





	//Update the jitter manager
	for (std::map<Dec2, JitterManager *>::iterator jitterManagersIter = jitterManagers.begin(); jitterManagersIter != jitterManagers.end();) {
		Dec2 dec2 = jitterManagersIter->first;
		JitterManager *jitterManager = jitterManagersIter->second;;
		jitterManager->setTotalElapseTime(totalElapsedTime);


		Vec2 position = jitterManager->getPosition();
		if (jitterManager->isJitterControllersEmpty()) {
			delete jitterManager;
			jitterManagersIter = jitterManagers.erase(jitterManagersIter);
		}
		else {
			jitterManagersIter++;
		}


		BubbleView *bubbleView = viewBubbles[dec2];
		bubbleView->setPosition(position);
	}

}

int FieldGame::getNumOfShootBubble(){
	return shootBubbles.size();
}

void FieldGame::addShootBubble(ShootBubble *shootBubble){
	assert(shootBubble->getParent() == nullptr);


	this->addChild(shootBubble);

	if (FieldGameState::NORMAL == this->getFieldGameState()){
		shootBubbles.insert(shootBubble);
	}
	else {
		Vec2 shootBubbleVelocity = shootBubble->getVelocity()*0.4f;

		eventFallCreate(shootBubble, shootBubbleVelocity);
	}

	
}


std::set<Dec2> FieldGame::getValidHexagons(const std::set<Dec2> &dec2s) {
	static std::function<bool(Dec2)> helperFunc = [this](Dec2 dec2) {
		return isDec2Valid(dec2);
	};

	return getHelperHexagons(dec2s, helperFunc);
}

std::vector<Dec2> FieldGame::getValidHexagons(const std::vector<Dec2> &dec2s){
	static std::function<bool(Dec2)> helperFunc = [this](Dec2 dec2) {
		return isDec2Valid(dec2);
	};

	return getHelperHexagons(dec2s, helperFunc);
}

std::set<Dec2> FieldGame::collapseToParent(const std::set<Dec2> &dec2s){
	std::set<Dec2> ret;
	for (Dec2 dec2 : dec2s){
		Bubble bubble = modelBubbles[dec2];
		ret.insert(bubble.getParent() + dec2);
	}

	return ret;
}

std::set<Dec2> FieldGame::getCollisionHexagons(const std::set<Dec2> &dec2s, const Vec2 &position, const float &radius){
	std::function<bool(Dec2)> helperFunc = std::bind([this](Dec2 dec2, Vec2 position, float radius, float innerRadius){
		return (position - getVec2ForDec2(dec2)).length() <= radius + innerRadius;
	}, std::placeholders::_1, position, radius, innerRadius);

	return getHelperHexagons(dec2s, helperFunc);
}
std::set<Dec2> FieldGame::getNotCollisionHexagons(const std::set<Dec2> &dec2s, const Vec2 &position, const float &radius){
	std::function<bool(Dec2)> helperFunc = std::bind([this](Dec2 dec2, Vec2 position, float radius, float innerRadius){
		return (position - getVec2ForDec2(dec2)).length() > radius + innerRadius;
	}, std::placeholders::_1, position, radius, innerRadius);

	return getHelperHexagons(dec2s, helperFunc);
}

std::set<Dec2> FieldGame::getOrderHexagons(const std::set<Dec2> &dec2s, int order, Dec2 decB){
	std::function<bool(Dec2)> helperFunc = std::bind([this](Dec2 decA, int order, Dec2 decB){
		return getHexagonDifference(decA, decB) == order;
	}, std::placeholders::_1, order, decB);

	return getHelperHexagons(dec2s, helperFunc);
}




std::set<Dec2> FieldGame::getSameBubbleTypeHexagons(const std::set<Dec2> &dec2s, BubbleType bubbleType){
	std::function<bool(Dec2)> helperFunc = std::bind([this](Dec2 dec2, BubbleType bubbleType){
		std::map<Dec2, Bubble>::iterator modelBubblesIter = modelBubbles.find(dec2);

		BubbleType bubbleType2;
		if (modelBubbles.end() == modelBubblesIter){
			bubbleType2 = BubbleType::UNDEFINED;
		}
		else {
			bubbleType2 = modelBubblesIter->second.getBubbleType();
		}

		return bubbleType == bubbleType2;
	}, std::placeholders::_1, bubbleType);

	return getHelperHexagons(dec2s, helperFunc);
}


std::set<Dec2> FieldGame::getPivotReachebleHexagons(const std::set<Dec2> &dec2s){
	std::function<bool(Dec2)> helperFunc = [this](Dec2 dec2){
		return fallGraph.getPivotRank(dec2) != -1;
	};

	return getHelperHexagons(dec2s, helperFunc);
}
std::set<Dec2> FieldGame::getPivotUnreachebleHexagons(const std::set<Dec2> &dec2s){
	std::function<bool(Dec2)> helperFunc = [this](Dec2 dec2){
		return fallGraph.getPivotRank(dec2) == -1;
	};

	return getHelperHexagons(dec2s, helperFunc);
}

std::set<Dec2> FieldGame::getBubbleSameColorChain(Dec2 dec2, BubbleType bubbleType, int maxSize){
	std::function<bool(Dec2)> helperFunc = std::bind([this](Dec2 dec2, BubbleType bubbleType){
		std::map<Dec2, Bubble>::iterator modelBubblesIter = modelBubbles.find(dec2);

		BubbleType bubbleType2;
		if (modelBubbles.end() == modelBubblesIter){
			bubbleType2 = BubbleType::UNDEFINED;
		}
		else {
			bubbleType2 = modelBubblesIter->second.getBubbleType();
		}

		return bubbleType == bubbleType2;
	}, std::placeholders::_1, bubbleType);

	return getBubbleChain(dec2, helperFunc, maxSize);
}

std::set<Dec2> FieldGame::getBubbleChain(Dec2 dec2, std::function<bool(Dec2)> _helperFunc, int maxSize){
	std::function<bool(Dec2, const std::set<Dec2> &, const std::function<bool(Dec2)> &)> helperFunc;
	helperFunc = [](Dec2 dec2, const std::set<Dec2> &bubbleChain, const std::function < bool(Dec2)> & _helperFunc){
		return _helperFunc(dec2) && bubbleChain.find(dec2) == bubbleChain.end();
	};

	std::set<Dec2> bubbleChain;

	std::set<Dec2> frontier;
	frontier.insert(dec2);

	for (int i = 0; (maxSize == -1 || bubbleChain.size() <= maxSize) && !frontier.empty(); i++){
		frontier = getHelperHexagons(expandSetByHexagon(1, frontier), std::bind(helperFunc, std::placeholders::_1, bubbleChain, _helperFunc));

		bubbleChain.insert(frontier.begin(), frontier.end());

	}

	return bubbleChain;
}

int FieldGame::getHexagonDifference(Dec2 decA, Dec2 decB){
	Dec2 diff = decA - decB;
	diff.x = std::abs(diff.x);
	diff.y = std::abs(diff.y);
	
	int ret = 0;
	{
		ret += std::min(diff.x, diff.y);
		diff += Dec2(-1, -1)*std::min(diff.x, diff.y);
	}

	{
		ret += diff.x / 2;
		diff += Dec2(-1, 0)*diff.x;
	}

	{
		ret += diff.y;
		diff += Dec2(0, -1)*diff.y;
	}


	return ret;

}


void FieldGame::visitEvent(EventWitchChangeColor *eventWitchChangeColor){
	const Dec2 dec2 = eventWitchChangeColor->dec2;

	std::vector<Dec2> witchDec2s = getBubbleContainer(BubbleType::WITCH, dec2);

	Bubble witchNode = modelBubbles[dec2];
	BubbleType oldBubbleType = witchNode.getBubbleTypeWitchFog();
	BubbleType newBubbleType = oldBubbleType;
	do {
		newBubbleType = Bubble::getRandomBasicBubbleType();
	} while (oldBubbleType == newBubbleType);

	for (Dec2 witchDec2 : witchDec2s){
		Bubble witchModel = modelBubbles[witchDec2];
		eraseBubbleModel(witchDec2);
		witchModel.setBubbleTypeWitch(newBubbleType, witchModel.getParent());
		setBubbleModel(witchDec2, witchModel);

		BubbleView *bubbleView = viewBubbles[witchDec2];
		bubbleView->setBubble(witchModel);
	}
}

void FieldGame::visitEvent(EventWitchDestroy *eventWitchDestoy){
	const Dec2 dec2 = eventWitchDestoy->dec2;

	callbacks.animalWitchDestroyCallback(viewBubbles[dec2]);

	std::vector<Dec2> witchDec2s = getBubbleContainer(BubbleType::WITCH, dec2);

	for (Dec2 witchDec2 : witchDec2s){
		
		if (dec2 != witchDec2){
			BubbleView *bubbleView = viewBubbles[witchDec2];
			bubbleView->removeFromParentAndCleanup(true);
	

		}

		eraseBubbleModel(witchDec2);
		viewBubbles.erase(viewBubbles.find(witchDec2));

		createFallEvents(witchDec2);
	}

	
}

void FieldGame::visitEvent(EventChromeExplosion *chromeExplosion){
	Dec2 dec2 = chromeExplosion->dec2;

	Bubble bubble = modelBubbles[dec2];
	BubbleView *bubbleView = viewBubbles[dec2];

	eraseBubbleModel(dec2);
	bubbleView->removeFromParentAndCleanup(true);
	viewBubbles.erase(viewBubbles.find(dec2));

	std::set<Dec2> hexagonFront = getHexagonFrontOfOrder(1, dec2);
	std::set<Dec2> definedHexagons = getDefinedHexagons(hexagonFront);
	for (Dec2 definedHexagon : definedHexagons){
		Bubble bubble = modelBubbles[definedHexagon];
		switch (bubble.getBubbleType()){
		case BubbleType::RED:
		case BubbleType::BLUE:
		case BubbleType::GREEN:
		case BubbleType::YELLOW:
		case BubbleType::PURPLE:
		{
			EventNormalExplosion *eventNormalExplosion = new EventNormalExplosion(totalElapsedTime + EVENT_NORMAL_EXPLOSION_DELAY, definedHexagon);
			fieldGameEventManger->pushEvent(eventNormalExplosion);
		}
			break;
		default:
			break;
		}
	}

	createFallEvents(dec2);
}

void FieldGame::visitEvent(EventNormalExplosion *normalExplosion){
	Dec2 dec2 = normalExplosion->dec2;

	Bubble bubble = modelBubbles[dec2];
	BubbleView *bubbleView = viewBubbles[dec2];


	BubbleType bubbleType = bubble.getBubbleType();

	{
		eraseBubbleModel(dec2);
		viewBubbles.erase(viewBubbles.find(dec2));

		bubbleView->runAction(Sequence::create(ScaleTo::create(EVENT_NORMAL_EXPLOSION_SCALE_0_DELAY, 0.0f), RemoveSelf::create(true), nullptr));
	}

	std::set<Dec2> hexagonFront = getHexagonFrontOfOrder(1, dec2);

	std::set<Dec2> bubbleChainOrder1 = getSameBubbleTypeHexagons(getHexagonFrontOfOrder(1, dec2), bubbleType);
	for (Dec2 bubChainOrder1 : bubbleChainOrder1){
		EventNormalExplosion *eventNormalExplosion = new EventNormalExplosion(totalElapsedTime + EVENT_NORMAL_EXPLOSION_DELAY, bubChainOrder1);
		fieldGameEventManger->pushEvent(eventNormalExplosion);
	}

	createFallEvents(dec2);
}


void FieldGame::visitEvent(EventSetBubble *eventSetBubble){
	Dec2 dec2 = eventSetBubble->dec2;
	ShootBubble *shootBubble = eventSetBubble->shootBubble;


	Bubble bubble = shootBubble->getBubble();

	setBubbleModel(dec2, bubble);
	viewBubbles[dec2] = shootBubble;

	std::set<Dec2> hexagonFront = getHexagonFrontOfOrder(1, dec2);
	std::set<Dec2> witchHexagons = collapseToParent(getSameBubbleTypeHexagons(hexagonFront, BubbleType::WITCH));

	switch (bubble.getBubbleType()){
	case BubbleType::CHROME:
	{
							   EventChromeExplosion *eventChromeExplosion = new EventChromeExplosion(totalElapsedTime + EVENT_CHROME_EXPLOSION_DELAY, dec2);
							   fieldGameEventManger->pushEvent(eventChromeExplosion);

							   for (Dec2 witchHexagon : witchHexagons){
								   EventWitchDestroy *eventWitchDestroy = new EventWitchDestroy(totalElapsedTime + EVENT_WITCH_DESTROY_DELAY, witchHexagon);
								   fieldGameEventManger->pushEvent(eventWitchDestroy);
							   }
	}
		break;
	case BubbleType::BLUE:
	case BubbleType::GREEN:
	case BubbleType::RED:
	case BubbleType::PURPLE:
	case BubbleType::YELLOW:
	{
							   std::set<Dec2> bubbleChain = getBubbleSameColorChain(dec2, bubble.getBubbleType(), MIN_SAME_COLOR_DESTRUCTION);

							   if (bubbleChain.size() >= MIN_SAME_COLOR_DESTRUCTION){
								   EventNormalExplosion *eventNormalExplosion = new EventNormalExplosion(totalElapsedTime + EVENT_NORMAL_EXPLOSION_DELAY, dec2);
								   fieldGameEventManger->pushEvent(eventNormalExplosion);
							   }

							   for (Dec2 witchHexagon : witchHexagons){
								   Bubble witch = modelBubbles[witchHexagon];
								   if (witch.getBubbleTypeWitchFog() == bubble.getBubbleType()){
									   EventWitchDestroy *eventWitchDestroy = new EventWitchDestroy(totalElapsedTime + EVENT_WITCH_DESTROY_DELAY, witchHexagon);
									   fieldGameEventManger->pushEvent(eventWitchDestroy);
								   }
								   else {
									   EventWitchChangeColor *eventWitchChangeColor = new EventWitchChangeColor(totalElapsedTime + EVENT_WITCH_CHANGE_COLOR_DELAY, witchHexagon);
									   fieldGameEventManger->pushEvent(eventWitchChangeColor);
								   }
							   }
	}

		break;
	}


	std::set<Dec2> wood1Hexagons = getSameBubbleTypeHexagons(hexagonFront, BubbleType::WOOD_1);
	for (Dec2 wood1Hexagon : wood1Hexagons){
		fieldGameEventManger->pushEvent(new EventWood1Break(totalElapsedTime + EVENT_WOOD1_BREAK_DELAY, wood1Hexagon));
	}

	std::set<Dec2> wood2Hexagons = getSameBubbleTypeHexagons(hexagonFront, BubbleType::WOOD_2);
	for (Dec2 wood2Hexagon : wood2Hexagons){
		fieldGameEventManger->pushEvent(new EventWood2Break(totalElapsedTime + EVENT_WOOD2_BREAK_DELAY, wood2Hexagon));
	}

	std::set<Dec2> iceHexagons = getSameBubbleTypeHexagons(hexagonFront, BubbleType::ICE);
	for (Dec2 iceHexagon : iceHexagons){
		fieldGameEventManger->pushEvent(new EventIceBreak(totalElapsedTime + EVENT_ICE_BREAK_DELAY, iceHexagon));
	}

	std::set<Dec2> fogHexagons = getSameBubbleTypeHexagons(hexagonFront, BubbleType::FOG);
	for (Dec2 fogHexagon : fogHexagons){
		fieldGameEventManger->pushEvent(new EventFog(totalElapsedTime + EVENT_FOG_DELAY, fogHexagon));
	}

	std::set<Dec2> fissionHexagons = getSameBubbleTypeHexagons(hexagonFront, BubbleType::FISION);
	for (Dec2 fissionHexagon : fissionHexagons){
		fieldGameEventManger->pushEvent(new EventFission(totalElapsedTime + EVENT_FISSION_DELAY, fissionHexagon));
	}


	float posDiff = (shootBubble->getPosition() - getVec2ForDec2(dec2)).length();
	float timeOffset = JitterController::getTimeOffset(posDiff, posDiff, JITTER_PERIOD);
	createJitter(dec2, (shootBubble->getPosition() - getVec2ForDec2(dec2)).getNormalized(), posDiff, JITTER_PERIOD, timeOffset);
	createJitter(dec2, shootBubble->getDirection(), JITTER_NORMAL_EXPLOSION_MAGNITUDE, JITTER_PERIOD, 0.0f);


	createJitterRipple(dec2, JITTER_NORMAL_EXPLOSION_RADIUS, JITTER_NORMAL_EXPLOSION_MAGNITUDE*((JITTER_NORMAL_EXPLOSION_RADIUS - 1.0f) / JITTER_NORMAL_EXPLOSION_RADIUS));
}


void FieldGame::visitEvent(EventFall *eventFall){
	Dec2 dec2 = eventFall->dec2;
	

	Bubble bubble = modelBubbles[dec2];
	BubbleView *bubbleView = viewBubbles[dec2];

	eraseBubbleModel(dec2);
	viewBubbles.erase(viewBubbles.find(dec2));

	switch (bubble.getBubbleType()){
	case BubbleType::ANIMAL1:
	case BubbleType::ANIMAL2:
	case BubbleType::ANIMAL3:
		if (bubble.getParent() == Dec2(0, 0)){
			callbacks.animalWitchDestroyCallback(bubbleView);
			
		}
		else {
			bubbleView->removeFromParentAndCleanup(true);
		}
		break;
	default:
		eventFallCreate(bubbleView, getViewBubbleVelocity(dec2));
		break;
	}
	

	createFallEvents(dec2);
}


void FieldGame::visitEvent(EventEnd2 *eventEnd2){
	if (!modelBubblesVisible){
		return;
	}

	std::vector<Dec2> bottomRowDec2s = getDefinedHexagons(getRowDec2s(bottomModelY));
	for (Dec2 bottomRowDec2 : bottomRowDec2s){
		fieldGameEventManger->pushEvent(new EventFall(totalElapsedTime, bottomRowDec2));
	}

	fieldGameEventManger->pushEvent(new EventEnd2(totalElapsedTime + EVENT_END2_DELAY));
}

void FieldGame::visitEvent(EventNull *eventNull){

}

void FieldGame::visitEvent(EventWood1Break *eventWood1Break){
	Dec2 dec2 = eventWood1Break->dec2;

	setBubbleModel(dec2, Bubble(BubbleType::WOOD_2));
	viewBubbles[dec2]->setBubble(Bubble(BubbleType::WOOD_2));
}
void FieldGame::visitEvent(EventWood2Break *eventWood2Break){
	Dec2 dec2 = eventWood2Break->dec2;
	
	viewBubbles[dec2]->removeFromParentAndCleanup(true);
	viewBubbles.erase(viewBubbles.find(dec2));

	eraseBubbleModel(dec2);


	createFallEvents(dec2);
}

void FieldGame::visitEvent(EventIceBreak *eventIceBreak){
	Dec2 dec2 = eventIceBreak->dec2;

	viewBubbles[dec2]->removeFromParentAndCleanup(true);
	viewBubbles.erase(viewBubbles.find(dec2));

	eraseBubbleModel(dec2);

	createFallEvents(dec2);
}

void FieldGame::visitEvent(EventFog *eventFog){
	Dec2 dec2 = eventFog->dec2;

	Bubble bubble = modelBubbles[dec2];

	Bubble newBubble;
	if (BubbleType::UNDEFINED == bubble.getBubbleTypeWitchFog()){
		newBubble = Bubble(getRandomActiveBasicBubbleType());
	}
	else {
		newBubble = Bubble(bubble.getBubbleTypeWitchFog());
	}

	setBubbleModel(dec2, newBubble);
	viewBubbles[dec2]->setBubble(newBubble);
}

void FieldGame::visitEvent(EventFission *eventFission){
	Dec2 dec2 = eventFission->dec2;

	std::vector<Dec2> rowDec2s = getDefinedHexagons(getRowDec2s(dec2.y));

	for (Dec2 rowDec2 : rowDec2s){
		fieldGameEventManger->pushEvent(new EventDestroy(totalElapsedTime, rowDec2));
	}
}

void FieldGame::visitEvent(EventDestroy *eventDestroy){
	Dec2 dec2 = eventDestroy->dec2;
	
	eraseBubbleModel(dec2);
	viewBubbles[dec2]->removeFromParentAndCleanup(true);
	viewBubbles.erase(viewBubbles.find(dec2));


	createFallEvents(dec2);
}

void FieldGame::createFallEvents(Dec2 dec2){
	std::set<Dec2> hexagonFront = getHexagonFrontOfOrder(1, dec2);
	std::set<Dec2> pivotUnreacheble = getPivotUnreachebleHexagons(getDefinedHexagons(hexagonFront));

	for (Dec2 pUnreacheble : pivotUnreacheble){
		Bubble bubble = modelBubbles[pUnreacheble];

		if (BubbleType::FISION == bubble.getBubbleType()){
			EventFission *eventFission = new EventFission(totalElapsedTime, pUnreacheble);
			fieldGameEventManger->pushEvent(eventFission);
		}
		else {
			EventFall *eventFall = new EventFall(totalElapsedTime + EVENT_FALL_DELAY, pUnreacheble);
			fieldGameEventManger->pushEvent(eventFall);
		}
	}
}




void FieldGame::eventFallCreate(BubbleView *bubbleView, Vec2 velocity){
	if (nullptr == callbacks.eventBubbleFallCallback){
		bubbleView->removeFromParentAndCleanup(true);
	}
	else {
		bubbleView->retain();
		bubbleView->removeFromParentAndCleanup(false);
		callbacks.eventBubbleFallCallback(bubbleView, velocity);
		bubbleView->release();
	}

}

void FieldGame::setBubbleModel(Dec2 dec2, Bubble bubble){
	FieldAbstract::setBubbleModel(dec2, bubble);

	calculateTopBottomModelY();

	if (getBubbleNum(bubble) == 1){
		updateActiveBasicBubbleTypes();
	}


	

}

void FieldGame::eraseBubbleModel(Dec2 dec2){
	Bubble bubble = modelBubbles[dec2];

	FieldAbstract::eraseBubbleModel(dec2);

	std::map<Dec2, JitterManager *>::iterator jitterManagersIter = jitterManagers.find(dec2);
	if (jitterManagers.end() != jitterManagersIter) {
		delete jitterManagersIter->second;
		jitterManagers.erase(jitterManagersIter);
	}

	calculateTopBottomModelY();

	if (getBubbleNum(bubble) == 0) {
		updateActiveBasicBubbleTypes();
	}

}


void FieldGame::updateActiveBasicBubbleTypes(){
	activeBasicBubbleTypes.clear();

	for (int i = (int)BubbleType::FIRST_BASIC_BUBBLE_TYPE; i <= (int)BubbleType::LAST_BASIC_BUBBLE_TYPE; i++){
		BubbleType basicBubbleType = (BubbleType)i;

		if (getBubbleNumBasic(basicBubbleType) > 0) {
			activeBasicBubbleTypes.push_back(basicBubbleType);
		}
	}

	if (activeBasicBubbleTypes.empty()){
		for (int i = (int)BubbleType::FIRST_BASIC_BUBBLE_TYPE; i <= (int)BubbleType::LAST_BASIC_BUBBLE_TYPE; i++){
			activeBasicBubbleTypes.push_back((BubbleType)i);
		}
	}
}

BubbleType FieldGame::getRandomActiveBasicBubbleType(){
	static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> distribution(0, activeBasicBubbleTypes.size() - 1);


	return activeBasicBubbleTypes[distribution(generator)];
}

const std::vector<BubbleType> & FieldGame::getActiveBasicBubbleTypes(){
	return activeBasicBubbleTypes;
}

void FieldGame::createJitterRipple(Dec2 dec2, const int numJitterRadiuses, const float maxMagnitude){
	std::vector<std::set<Dec2> > jitterArcs;

	jitterArcs.push_back(std::set<Dec2>());
	jitterArcs[0].insert(dec2);

	{

		std::set<Dec2> jitterElems;
		jitterElems.insert(jitterArcs[0].begin(), jitterArcs[0].end());

		for (int i = 0; i < numJitterRadiuses; i++){
			
			std::set<Dec2> lastJitterArc = jitterArcs[i];
			std::set<Dec2> jitterArc;

			for (Dec2 front : lastJitterArc){
				std::set<Dec2> definedHexagons = getDefinedHexagons(getHexagonFrontOfOrder(1, front));

				for (Dec2 hexagon : definedHexagons){
					if (jitterElems.find(hexagon) == jitterElems.end()){
						jitterArc.insert(hexagon);
					}
				}
			}

			jitterArcs.push_back(jitterArc);
			jitterElems.insert(jitterArc.begin(), jitterArc.end());
		}
	}



	const float minMagnitude = maxMagnitude / (numJitterRadiuses - 0.0f);

	for (int i = 1; i < jitterArcs.size(); i++){
		std::set<Dec2> lastJitterArc = jitterArcs[i - 1];
		std::set<Dec2> jitterArc = jitterArcs[i];

		for (Dec2 jArc : jitterArc){
			std::set<Dec2> definedHexagons = getDefinedHexagons(getHexagonFrontOfOrder(1, jArc));

			std::set<Dec2> jitterArcOrigin; 
			for (Dec2 hexagon : definedHexagons){
				if (lastJitterArc.find(hexagon) != lastJitterArc.end()){
					jitterArcOrigin.insert(hexagon);
				}
			}
			
			Vec2 totalVec2;
			for (Dec2 jArcOrigin : jitterArcOrigin){
				totalVec2 += getRelationShipVec2(jArcOrigin, jArc);
			}



			float magnitude = maxMagnitude + (minMagnitude - maxMagnitude)*(i / (numJitterRadiuses - 1.0f));
			createJitter(jArc, totalVec2.getNormalized(), magnitude, JITTER_PERIOD, 0.0f);
		}
	}
}

void FieldGame::createJitter(Dec2 dec2, Vec2 direction, float magnitude, float period, float timeOffset) {
	const float decayConstant = 0.9f;
	

	JitterController *jitterController = new JitterController(totalElapsedTime, direction, magnitude, period, decayConstant, timeOffset);

	std::map<Dec2, JitterManager *>::iterator jitterManagersIter = jitterManagers.find(dec2);
	if (jitterManagers.end() == jitterManagersIter) {
		jitterManagers[dec2] = new JitterManager(getVec2ForDec2(dec2), totalElapsedTime);

		jitterManagersIter = jitterManagers.find(dec2);
	}

	JitterManager *jitterManager = jitterManagersIter->second;
	jitterManager->addJitterController(jitterController);
}

Vec2 FieldGame::getRelationShipVec2(Dec2 dec2From, Dec2 dec2To){
	Vec2 vec2From = getVec2ForDec2(dec2From);
	Vec2 vec2To = getVec2ForDec2(dec2To);

	Vec2 vec2Diff = vec2To - vec2From;
	vec2Diff.normalize();

	return vec2Diff;
}


Vec2 FieldGame::getViewBubblePosition(Dec2 dec2) {
	std::map<Dec2, JitterManager*>::iterator jitterManagersIter = jitterManagers.find(dec2);
	
	Vec2 ret;
	if (jitterManagers.end() != jitterManagersIter) {
		JitterManager *jitterManager = jitterManagersIter->second;
		ret = jitterManager->getPosition();
	}
	else {
		ret = getVec2ForDec2(dec2);
	}

	return ret;
}
Vec2 FieldGame::getViewBubbleVelocity(Dec2 dec2) {
	std::map<Dec2, JitterManager*>::iterator jitterManagersIter = jitterManagers.find(dec2);

	Vec2 ret;
	if (jitterManagers.end() != jitterManagersIter) {
		JitterManager *jitterManager = jitterManagersIter->second;
		ret = jitterManager->getVelocity();
	}
	else {
		ret = Vec2(0.0f, 0.0f);
	}

	return ret;
}

void FieldGame::moveFieldNormal(float dt) {
	float vecPositionY = getMoveFieldVecPositionY(dt);
	if (vecPositionY != 0.0f) {

		
		this->setPositionY(this->getPositionY() + vecPositionY);

		for (ShootBubble *shootBubble : shootBubbles) {
			shootBubble->setPositionY(shootBubble->getPositionY() - vecPositionY);
		}
	}
	else {
		if (fieldGameState == FieldGameState::START){
			setFieldGameState(FieldGameState::NORMAL);
		}
	}
}

float FieldGame::getMoveFieldVecPositionY(float dt) {
	float stablePositionY = getStablePositionY();
	float currentPositionY = this->getPositionY();
	if (stablePositionY == currentPositionY) {
		return 0.0f;
	}

	float diffPositionY = stablePositionY - currentPositionY;
	float vecNormPositionY;
	if (diffPositionY > 0.0f) {
		vecNormPositionY = 1.0f;
	}
	else if (diffPositionY < 0.0f) {
		vecNormPositionY = -1.0f;
	}

	float velocity = vecNormPositionY*((std::abs(diffPositionY) / MOVE_FIELD_VELOCITY_CONSTANT) + 1.0f)*MOVE_FIELD_VELOCITY_0;
	float displacement = velocity*dt;
	/*
	switch (getFieldGameState()){
	case FieldGameState::START:
		vecPositionY = vecNormPositionY*MOVE_FIELD_VELOCITY_START*dt;
		break;
	default:
		vecPositionY = vecNormPositionY*MOVE_FIELD_VELOCITY_NORMAL*dt;
		break;
	}
	*/


	if (vecNormPositionY == 1.0f) {
		if (displacement > diffPositionY) {
			displacement = diffPositionY;
		}
	}
	else if (vecNormPositionY == -1.0f) {
		if (displacement < diffPositionY) {
			displacement = diffPositionY;
		}
	}


	return displacement;
}


std::vector<Vec2> FieldGame::getControlPoints(Vec2 currentPoint, Vec2 direction, float innerRadius, float shotBubbleRadius){
	currentPoint = PointApplyTransform(currentPoint, this->getWorldToNodeTransform());

	std::vector<Vec2> ret;
	ret.push_back(currentPoint);


	const int numOfControlPoints = 3;
	const float lastControlPointOffset = innerRadius*2.0f;

	float maxControlPointsLength = FLT_MAX;
	float controlPointsLength = 0.0f;

	bool isBreak = false;
	do {
		bool addCurrentPoint = false;

		controlPointsLength += shotBubbleRadius;
		currentPoint += shotBubbleRadius*direction;

		
		if (isOverLeftEdge(currentPoint, direction, innerRadius)){
			std::pair<Vec2, Vec2> leftEdgeProp = leftEdgeCollision(currentPoint, direction, innerRadius);

			currentPoint = leftEdgeProp.first;
			direction = leftEdgeProp.second;

			addCurrentPoint = true;
		}
		else if (isOverRightEdge(currentPoint, direction, innerRadius)){
			std::pair<Vec2, Vec2> rightEdgeProp = rightEdgeCollision(currentPoint, direction, innerRadius);

			currentPoint = rightEdgeProp.first;
			direction = rightEdgeProp.second;

			addCurrentPoint = true;
		}
		else if (isOverTopEdge(currentPoint, direction, innerRadius)){
			std::pair<Vec2, Vec2> topEdgeProp = topEdgeCollision(currentPoint, direction, innerRadius);

			currentPoint = topEdgeProp.first;
			direction = topEdgeProp.second;

			addCurrentPoint = true;
			isBreak = true;
		}
		else if (isOverBottomEdge(currentPoint, direction, innerRadius)){
			std::pair<Vec2, Vec2> bottomEdgeProp = bottomEdgeCollision(currentPoint, direction, innerRadius);

			currentPoint = bottomEdgeProp.first;
			direction = bottomEdgeProp.second;

			addCurrentPoint = true;
			isBreak = true;
		}
		else if (isOverTopBoundary(currentPoint, direction)){
			currentPoint = topBoundaryCollision(currentPoint, direction);

			addCurrentPoint = true;
			isBreak = true;
		}
		else if (isOverBottomBoundary(currentPoint, direction)){
			currentPoint = bottomBoundaryCollision(currentPoint, direction);

			addCurrentPoint = true;
			isBreak = true;
		}
		else if (isOverHexagon(currentPoint, shotBubbleRadius)){
			currentPoint = hexagonCollision(currentPoint, direction, shotBubbleRadius) + (shotBubbleRadius - innerRadius*PREDICTION_LINE_MAX_BUBBLE_RADIUS_MULTIPLIER)*direction;

			addCurrentPoint = true;
			isBreak = true;
		}
		else if (controlPointsLength > maxControlPointsLength){
			addCurrentPoint = true;
			isBreak = true;
		}


		if (addCurrentPoint){
			ret.push_back(currentPoint);

			if (ret.size() == numOfControlPoints - 1){
				maxControlPointsLength = controlPointsLength + lastControlPointOffset;
			}
			else if (ret.size() == numOfControlPoints){
				isBreak = true;
			}
		}

	} while (!isBreak);


	for (int i = 0; i < ret.size(); i++){
		ret[i] = PointApplyTransform(ret[i], this->getNodeToWorldTransform());
	}
	

	return ret;
}

bool FieldGame::isOverLeftEdge(Vec2 point, Vec2 velocity, float radius){
	return (velocity.x < 0.0f && point.x <= fieldRect.origin.x + radius);
}
bool FieldGame::isOverRightEdge(Vec2 point, Vec2 velocity, float radius){
	return (velocity.x > 0.0f && point.x >= fieldRect.origin.x + fieldRect.size.width - radius);
}
bool FieldGame::isOverTopEdge(Vec2 point, Vec2 velocity, float radius){
	Vec2 topBoundaryCoord = PointApplyTransform(fieldRect.origin + Vec2(fieldRect.size.width / 2.0f, fieldRect.size.height), this->getWorldToNodeTransform());

	return velocity.y > 0.0f && point.y >= topBoundaryCoord.y + radius;
}
bool FieldGame::isOverBottomEdge(Vec2 point, Vec2 velocity, float radius){
	Vec2 bottomBoundaryCoord = PointApplyTransform(fieldRect.origin + Vec2(fieldRect.size.width / 2.0f, 0.0f), this->getWorldToNodeTransform());

	return velocity.y < 0.0f && point.y <= bottomBoundaryCoord.y - innerRadius;
}


bool FieldGame::isOverTopBoundary(Vec2 point, Vec2 velocity){
	bool ret;
	
	if (hasTopBoundary){
		if (velocity.y > 0.0f && point.y >= getVec2YForDec2Y(topBoundary)){
			ret = true;
		}
		else {
			ret = false;
		}
	}
	else {
		ret = false;
	}

	return ret;
}
bool FieldGame::isOverBottomBoundary(Vec2 point, Vec2 velocity){
	bool ret;

	if (hasBottomBoundary){
		if (velocity.y < 0.0f && point.y <= getVec2YForDec2Y(bottomBoundary)){
			ret = true;
		}
		else {
			ret = false;
		}
	}
	else {
		ret = false;
	}

	return ret;
}


bool FieldGame::isOverHexagon(Vec2 point, float radius){
	std::set<Dec2> firstOrderHexagons = getHexagonOfOrder(1, getDec2ForVec2(point));

	std::set<Dec2> collisionDefinedHexagons = getCollisionHexagons(getDefinedHexagons(firstOrderHexagons), point, radius);
	
	return !collisionDefinedHexagons.empty();
}


std::pair<Vec2, Vec2> FieldGame::leftEdgeCollision(Vec2 point, Vec2 velocity, float radius){
	Vec2 newVelocity = velocity;
	newVelocity.x *= -1.0f;


	Vec2 collisionPoint;
	float time = (fieldRect.origin.x + radius - point.x) / velocity.x;
	collisionPoint = point + time*(velocity);

	std::pair<Vec2, Vec2> ret;
	ret.first = collisionPoint;
	ret.second = newVelocity;

	return ret;
}
std::pair<Vec2, Vec2> FieldGame::rightEdgeCollision(Vec2 point, Vec2 velocity, float radius){
	Vec2 newVelocity = velocity;
	newVelocity.x *= -1.0f;

	Vec2 collisionPoint;
	float time = (fieldRect.origin.x + fieldRect.size.width - radius - point.x) / velocity.x;
	collisionPoint = point + time*velocity;

	std::pair<Vec2, Vec2> ret;
	ret.first = collisionPoint;
	ret.second = newVelocity;

	return ret;
}
std::pair<Vec2, Vec2> FieldGame::topEdgeCollision(Vec2 point, Vec2 velocity, float radius){
	Vec2 topBoundaryCoord = PointApplyTransform(fieldRect.origin + Vec2(fieldRect.size.width / 2.0f, fieldRect.size.height), this->getWorldToNodeTransform());
	
	Vec2 newVelocity = velocity;
	newVelocity.y *= -1.0f;

	Vec2 collisionPoint;
	float time = (topBoundaryCoord.y - point.y) / velocity.y;
	collisionPoint = point + time*velocity;

	std::pair<Vec2, Vec2> ret;
	ret.first = collisionPoint;
	ret.second = newVelocity;

	return ret;
}
std::pair<Vec2, Vec2> FieldGame::bottomEdgeCollision(Vec2 point, Vec2 velocity, float radius){
	Vec2 bottomBoundaryCoord = PointApplyTransform(fieldRect.origin + Vec2(fieldRect.size.width / 2.0f, 0.0f), this->getWorldToNodeTransform());

	Vec2 newVelocity = velocity;
	newVelocity.y *= -1.0f;

	Vec2 collisionPoint;
	float time = (bottomBoundaryCoord.y - point.y) / velocity.y;
	collisionPoint = point + time*velocity;

	std::pair<Vec2, Vec2> ret;
	ret.first = collisionPoint;
	ret.second = newVelocity;

	return ret;
}

Vec2 FieldGame::hexagonCollision(Vec2 point, Vec2 direction, float radius){
	/*
	while (isOverHexagon(point, radius)){
		point -= radius*direction;
	}

	const int precisionNum = 4;
	for (int i = 0; i < precisionNum; i++){
		radius /= 2.0f;

		while (!isOverHexagon(point + radius*direction, radius)){
			point += radius*direction;
		}
	}





	return point;
	*/
	
	const int precisionNum = 4;

	float precRadius = radius;
	for (int i = 0; i < precisionNum; i++){
		precRadius /= 2.0f;

		
		while (isOverHexagon(point - precRadius*direction, radius)){
			point -= precRadius*direction;
		}
	}

	return point;
}


Vec2 FieldGame::topBoundaryCollision(Vec2 point, Vec2 velocity){
	float y = getVec2YForDec2Y(topBoundary);

	float time = (y - point.y) / velocity.y;

	point += velocity*time;

	return point;
}
Vec2 FieldGame::bottomBoundaryCollision(Vec2 point, Vec2 velocity){
	float y = getVec2YForDec2Y(bottomBoundary);

	float time = (y - point.y) / velocity.y;

	point += velocity*time;

	return point;
}

int FieldGame::getBubbleNum(Bubble bubble) {
	int ret = FieldAbstract::getBubbleNum(bubble);

	for (ShootBubble *shootBubble : shootBubbles) {
		if (shootBubble->getBubble() == bubble) {
			ret++;
		}
	}

	return ret;
}



int FieldGame::getNumOfEvents(){
	return fieldGameEventManger->getNumOfFieldGameEvents();
}