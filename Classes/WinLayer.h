#ifndef _WIN_LAYER_H_
#define _WIN_LAYER_H_

#include "cocos2d.h"
USING_NS_CC;


class WinLayer : public LayerColor {
private:
	WinLayer(std::function<void()> _returnLastInterface);
public:
	static WinLayer* create(std::function<void()> _returnLastInterface);

	void onEnter() override;


	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void onTouchCancelled(Touch *touch, Event *unused_event);

private:
	EventListenerTouchOneByOne *listener;
	Touch *main_touch;

	std::function<void()> returnLastInterface;
};

#endif //_WIN_LAYER_H_