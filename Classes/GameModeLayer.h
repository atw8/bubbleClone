#ifndef _GAME_MODE_LAYER_H_
#define _GAME_MODE_LAYER_H_

#include "Bubble.h"

#include "cocos2d.h"
USING_NS_CC;

#include "GameMode.h"
#include "Button.h"
#include "CustomNumber.h"


class GameModeLayer : public LayerColor {
private:
	GameModeLayer(std::function<void(GameMode)> _updateGameModeFunc, GameMode _gameMode, int _bubbleWidth);
public:
	static GameModeLayer* create(std::function<void(GameMode)> _updateGameModeFunc, GameMode _gameMode, int _bubbleWidth);
	static Scene *createScene(std::function<void(GameMode)> _updateGameModeFunc, GameMode _gameMode, int _bubbleWidth);

	void onEnter();

private:
	EventListenerTouchOneByOne *listener;
	Touch *main_touch;

	std::function<void(GameMode)> updateGameModeFunc;
	GameMode gameMode;
	const int bubbleWidth;
public:
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void onTouchCancelled(Touch *touch, Event *unused_event);


private:
	std::map<GameModeType, Button *> gameModeButtons;
	CustomNumber *gameModeAmountBtn;
	Sprite *backBtn;

	CustomNumber* clearTopBtn;

	void gameModeTypeCallback(GameModeType gameModeType);
	void gameModeAmountCallback(std::string gameModeAmountStr);
	void gameModeClearTopCallback(std::string clearTopAmountStr);

	void callUpdateGameModeFunc();

	void backBtnCallback();


public:
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};


#endif //_GAME_MODE_LAYER_H_