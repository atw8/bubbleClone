#ifndef _PAUSE_LAYER_H_
#define _PAUSE_LAYER_H_

#include "cocos2d.h"
USING_NS_CC;

class PauseLayer : public LayerColor {
private:
	PauseLayer(std::function<void()> _restartGameFunc, std::function<void()> _returnGameFunc, std::function<void()> _returnMapFunc);
public:
	static PauseLayer *create(std::function<void()> _restartGameFunc, std::function<void()> _returnGameFunc, std::function<void()> _returnMapFunc);
	static Scene *createScene(std::function<void()> _restartGameFunc, std::function<void()> _returnGameFunc, std::function<void()> _returnMapFunc);
	
	void onEnter() override;

private:
	EventListenerTouchOneByOne *listener;
	Touch *main_touch;
public:
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void onTouchCancelled(Touch *touch, Event *unused_event);
private:
	void restartGameBtnCallback();
	void returnGameBtnCallback();
	void returnMapBtnCallback();

	Sprite *background;

	Sprite *restartGameBtn;
	Sprite *returnGameBtn;
	Sprite *returnMapBtn;

	Sprite *backBtn;

	const std::function<void()> restartGameFunc;
	const std::function<void()> returnGameFunc;
	const std::function<void()> returnMapFunc;

public:
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};

#endif //_PAUSE_LAYER_H_