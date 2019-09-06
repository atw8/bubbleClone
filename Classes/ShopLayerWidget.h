#ifndef _SHOP_LAYER_WIDGET_H_
#define _SHOP_LAYER_WIDGET_H_


#include "cocos2d.h"
USING_NS_CC;

#include "ShopStruct.h"




class ShopLayerWidget : public Sprite {
private:
	ShopLayerWidget(ShopStruct _shopStruct, std::string _goldIconStr);
public:
	static ShopLayerWidget* create(ShopStruct _shopStruct, std::string _goldIconStr);
	
	void onEnter() override;

	const ShopStruct shopStruct;
	const std::string goldIconStr;

	static float getWidgetWidth();
	static float getWidgetHeight();

public:
	Sprite *button;
};

#endif //_SHOP_LAYER_WIDGET_H_