#ifndef _SHOP_LAYER_H_
#define _SHOP_LAYER_H_

#include "cocos2d.h"
USING_NS_CC;

#include "ShopLayerInner.h"

class ShopLayer : public LayerColor {
private:
	enum class TouchType{
		DRAGINNER,
		BUTTONSINNER,
		BUTTONSOUTER,
	};
	
	ShopLayer();
private:
	TouchType touchType;

	const float moveSensitivity;

	float minYShopLayerInner;
	float maxYShopLayerInner;

	Rect innerRect;
public:
	static ShopLayer* create();
	static Scene* createScene();

	void onEnter() override;


	
private:
	Sprite *background;
	ShopLayerInner *shopLayerInner;

	Sprite *backBtn;


	EventListenerTouchOneByOne *listener;
	Touch *main_touch;
public:
	bool onTouchBegan(Touch *touch, Event *unused_event) override;
	void onTouchMoved(Touch *touch, Event *unused_event) override;
	void onTouchEnded(Touch *touch, Event *unused_event) override;
	void onTouchCancelled(Touch *touch, Event *unused_event) override;

public:
	void shopBtnCallback(ShopStruct shopStruct);

	void buyBack(ShopStruct shopStruct, bool isSuccess);

	void backBtnCallback();
	
public:
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event *unused_event) override;
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *unused_event) override;
	
	

};

#endif //_SHOP_LAYER_H_
