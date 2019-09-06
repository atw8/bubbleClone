#ifndef _GAME_LAYER_H_
#define _GAME_LAYER_H_

#include "cocos2d.h"
USING_NS_CC;

#include "SaddleView.h"

#include "TopBoundary.h"

#include "FieldGame.h"

#include "PredictionLine.h"

#include "Clock.h"

#include "GameMode.h"
#include "PropBtn.h"

#include "EventGameListener.h"

#include "Global.h"

#include "GoldBar.h"

class GameLayer : public Layer, public EventGameListener {
private:
	GameLayer(LEVEL_TYPE _levelType, std::string _levelName, int _levelNumber, std::function<void()> _returnLastInterface);
public:
	static GameLayer *create(LEVEL_TYPE _levelType, std::string _levelName, int _levelNumber, std::function<void()> _returnLastInterface);
	static Scene *createScene(LEVEL_TYPE _levelType, std::string _levelName, int _levelNumber, std::function<void()> _returnLastInterface);


	const LEVEL_TYPE levelType;
	const std::string levelName;
	const int levelNumber;
		
	
	void onEnter() override;
	void onExit() override;

	bool onTouchBegan(Touch *touch, Event *unused_event) override;
	void onTouchMoved(Touch *touch, Event *unused_event) override;
	void onTouchEnded(Touch *touch, Event *unused_event) override;
	void onTouchCancelled(Touch *touch, Event *unused_event) override;


	void updateField(float dt);
private:
	void addFrontShootBubble();
private:
	float topFallBoundary;
	float bottomFallBoundary;
	float leftFallBoundary;
	float rightFallBoundary;

	TopBoundary *topBoundary;
	
	FieldGame *fieldGame;



	Touch *main_touch;
	EventListenerTouchOneByOne *listener;

	SaddleView *saddleView;

public:
	void eventFallCallback(BubbleView *fallBubble, Vec2 velocity);


	void animalWitchDestroyCallback(BubbleView *bubbleView);
	void callbackFallAnimalHelper2(BubbleType bubbleType, Vec2 position);
	
	std::map<BubbleType, int> inTransBubbles;
	inline int getNumOfTransBubble(BubbleType bubbleType);
	inline void addNumOfTransBubble(BubbleType bubbleType, int val);
	inline void incrNumOfTransBubble(BubbleType bubbleType);
	inline void decrNumOfTransBubble(BubbleType bubbleType);
	inline void setNumOfTransBubble(BubbleType bubbleType, int _numOfTransBubble);
	inline int getTotalNumTransBubble();
	inline int getTotalNumAnimalTransBubble();
	
private:
	std::set<BubbleView *> fallBubbles;
public:
	void updateFallBubbles(float dt);


private:
	EventListenerPhysicsContact *physicsContact;
public:
	bool onContactBegin(PhysicsContact &physicsContact);
	void onContactSeparate(PhysicsContact &physicsContact);


private:
	PredictionLine *predictionLine;
public:
	void setPredictionLineBubbleTypes(BubbleType bubbleType);
	void startSchedulePredictionLine();
	void stopSchedulePredictionLine();
	void updatePredictionLine(float dt);


private:
	void loadArmature();
	void unloadArmature();

	
private:

	
	std::vector<PropBtn *> propBtns;
	void propBtnCallback(PropType propType);

	Clock *myClock;

	GameMode gameMode;
	Sprite *rotateCircle;

	GoldBar *goldBar;
	void goldBarCallback();
public:
	void updateTime(float dt);

private:
	int maxNumOfShotBubbles;
	void normalizeShootBubbles();


private:
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);


	void scheduleUpdates();
	void unscheduleUpdates();

private:
	Sprite *pauseBtn;
	void pauseBtnCallback();

	void pauseReturnGame();
	void pauseRestartGame();

	void popUpLayer(Layer *layer);


private:
	bool isMovingSaddleView;
	void setIsMovingSaddleView(Vec2 location);
	void setIsMovingSaddleViewRest();

	
	
	void updatePopEndGame1Bubbles(float dt);

private:
	Sprite *background;

	void setFieldGameStateCallback(FieldGameState fieldGameState);

public:
	void executeVisit(FieldGameEventElement *eventElement);

	void visitEvent(EventFall *eventFall) override;
	void visitEvent(EventNormalExplosion *eventNormalExplosion) override;
	void visitEvent(EventChromeExplosion *eventChromeExplosion) override;
	void visitEvent(EventWitchDestroy *eventWitchDestoy) override;
	void visitEvent(EventWitchChangeColor *eventWitchChangeColor) override;
	void visitEvent(EventSetBubble *eventSetBubble) override;

	void visitEvent(EventEnd2 *eventEnd2) override;

	void visitEvent(EventNull *eventNull) override;

	void visitEvent(EventWood1Break *eventWood1Break) override;
	void visitEvent(EventWood2Break *eventWood2Break) override;

	void visitEvent(EventIceBreak *eventIceBreak) override;

	void visitEvent(EventFog *eventFog) override;

	void visitEvent(EventFission *eventFission) override;

	void visitEvent(EventDestroy *eventDestroy) override;
private:
	void updateActiveBasicBubbleTypes();
	void updateTopBoundary();


	const std::function<void()> returnLastInterface;
};


#endif //_GAME_LAYER_H_
