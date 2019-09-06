#ifndef _LOSE_LAYER_H_
#define _LOSE_LAYER_H_

#include "cocos2d.h"
USING_NS_CC;

class LoseLayer : public LayerColor{
private:
	LoseLayer(std::function<void()> _returnLastInterface);
public:
	static LoseLayer* create(std::function<void()> _returnLastInterface);

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

#endif //_LOSE_LAYER_H_