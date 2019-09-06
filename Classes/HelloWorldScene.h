#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

#include "FieldDesign.h"
#include "Bubble.h"
#include "BubbleButton.h"
#include "ScrollBar.h"

#include "ScrollBarInteger.h"
#include "Button.h"

#include "CustomNumber.h"
#include "GameMode.h"

#include "TopBoundary.h"

#include "Global.h"

class HelloWorld : public Layer{
private:
	HelloWorld(LEVEL_TYPE _levelType, std::string _levelName);
public:
	static Scene* createScene(LEVEL_TYPE _levelType, std::string _levelName);
	static HelloWorld *create(LEVEL_TYPE _levelType, std::string _levelName);


	void onEnter();
	void onExit();
    


private:
	Rect fieldRect;
	FieldDesign *fieldDesign;
	ClippingNode *fieldClip;

private:
	Rect scrollRect;
	ScrollBar *scrollBar;

	//Touch Related functionality
	EventListenerTouchOneByOne *listener;
private:
	Touch *main_touch;
	enum class TouchType {
		FIELD_PRESS,
		FIELD_MOVE,
		SCROLL_BAR,

		BUTTONS,
		BUTTONS_SCROLLBAR,
		UNDEFINED,
	};

	const float fieldMoveSensitivity;
	TouchType touchType;



public:
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void onTouchCancelled(Touch *touch, Event *unused_event);

private:
	TopBoundary *topBoundary;

	BubbleButton *pressedBubbleButton;
	BubbleButton *selectedBubbleButton;
	
	std::map<BubbleType, BubbleButton *> bubbleButtons;
	
	
	ScrollBarInteger *fieldWidthBar;
	Rect fieldWidthBarRect;

	Sprite *background;
	

	Button *gameModeBtn;
	std::string getGameModeBtnStr();
	void gameModeBtnCallback();

	void gameModeLayerCallback(GameMode _gameMode);

	Sprite *backBtn;

	Button *saveBtn;
	Button *shareBtn;

	Button *testButton;

	void saveBtnFunc();
	void saveBtnFuncHelper(std::string fullPath);
	void shareBtnCallback();
	void testBtnCallback();



public:
	void loadBtnFuncCallback(LEVEL_TYPE _levelType, std::string _levelName);
private:
	void backBtnCallback();

	void loadArmature();
	void unloadArmature();


	CustomNumber *levelNameLabel;
public:
	void changeWitchColor(float dt);
private:
	GameMode gameMode;

	std::string getLevelValueMap();
	void setLevelValueMap(std::string levelValueMap);


	void setLevelValueMapOld(ValueMap oldLevelValueMap);


	LEVEL_TYPE levelType;
	std::string levelName;


	void updateTopBoundary();
private:
	void popUpLayer(Layer *layer);

	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
};

#endif // __HELLOWORLD_SCENE_H__
