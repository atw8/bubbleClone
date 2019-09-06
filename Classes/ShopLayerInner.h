#ifndef _SHOP_LAYER_INNER_H_
#define _SHOP_LAYER_INNER_H_

#include "ShopLayerWidget.h"

#include "cocos2d.h"
USING_NS_CC;

class ShopLayerInner : public Layer{
private:
	ShopLayerInner(std::function<void(ShopStruct)> _shopBtnCallback);
public:
	static ShopLayerInner* create(std::function<void(ShopStruct)> _shopBtnCallback);

	void onEnter() override;

private:
	std::vector<ShopLayerWidget *> shopLayerWidgets;
	const float widgetXOffset;

public:
	float getWidgetsHeight();
	float getWidgetsWidth();

	void onTouchBegan(Vec2 location);
	void onTouchMoved(Vec2 location);
	void onTouchEnded(Vec2 location);

	void setShopLayerWidgetsScale(float scale);

private:
	const std::function<void(ShopStruct)> shopBtnCallback;
};


#endif //_SHOP_LAYER_INNER_H_