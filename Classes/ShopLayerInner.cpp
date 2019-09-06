#include "ShopLayerInner.h"
#include "DDUserDefault.h"
#include "Global.h"

ShopLayerInner::ShopLayerInner(std::function<void(ShopStruct)> _shopBtnCallback) :widgetXOffset(10.0f), shopBtnCallback(_shopBtnCallback){

}

ShopLayerInner* ShopLayerInner::create(std::function<void(ShopStruct)> _shopBtnCallback){
	ShopLayerInner *shopLayerInner = new ShopLayerInner(_shopBtnCallback);
	if (shopLayerInner && shopLayerInner->init()){
		shopLayerInner->autorelease();
	}
	else {
		delete shopLayerInner;
		shopLayerInner = nullptr;
	}

	return shopLayerInner;
}


void ShopLayerInner::onEnter(){
	Layer::onEnter();

	std::vector<ShopStruct> shopStructs = DDUserDefault::getInstance()->getShopStructs();

	std::function<bool(ShopStruct, ShopStruct)> shopStructsSort = [](ShopStruct s1, ShopStruct s2){
		return s1.price < s2.price;
	};
	std::sort(shopStructs.begin(), shopStructs.end(), shopStructsSort);

	const float widgetWdith = ShopLayerWidget::getWidgetWidth();
	const float widgetHeight = ShopLayerWidget::getWidgetHeight();


	Vec2 widgetPosition(0.0f, -(widgetHeight + widgetXOffset) / 2.0f);



	int goldIconStrIndex = 0;
	for (ShopStruct shopStruct : shopStructs){
		std::string goldIconStr;

		switch (goldIconStrIndex){
		case 0:
			goldIconStr = "shopLayer/gold_1.png";
			break;
		case 1:
			goldIconStr = "shopLayer/gold_2.png";
			break;
		case 2:
		default:
			goldIconStr = "shopLayer/gold_3.png";
			break;
		}
		ShopLayerWidget *shopLayerWidget = ShopLayerWidget::create(shopStruct, goldIconStr);
		shopLayerWidget->setPosition(widgetPosition);
		this->addChild(shopLayerWidget);

		goldIconStrIndex++;
		widgetPosition.y -= (widgetHeight + widgetXOffset);


		shopLayerWidgets.push_back(shopLayerWidget);
	}
}

float ShopLayerInner::getWidgetsWidth(){
	return ShopLayerWidget::getWidgetWidth();
}
float ShopLayerInner::getWidgetsHeight(){
	float ret = shopLayerWidgets.size()*(ShopLayerWidget::getWidgetHeight() + widgetXOffset) - widgetXOffset;

	return ret;
}


void ShopLayerInner::onTouchBegan(Vec2 location){
	for (ShopLayerWidget *shopLayerWidget : shopLayerWidgets){
		Sprite *button = shopLayerWidget->button;
		if (button->getBoundingBox().containsPoint(PointApplyTransform(location, shopLayerWidget->getParentToNodeTransform()))){
			button->setScale(0.9f);
		}
	}
}
void ShopLayerInner::onTouchMoved(Vec2 location){
	setShopLayerWidgetsScale(1.0f);

	for (ShopLayerWidget *shopLayerWidget : shopLayerWidgets){
		Sprite *button = shopLayerWidget->button;
		
		if (button->getBoundingBox().containsPoint(PointApplyTransform(location, shopLayerWidget->getParentToNodeTransform()))){
			button->setScale(0.9f);
		}
	}
}
void ShopLayerInner::onTouchEnded(Vec2 location){
	setShopLayerWidgetsScale(1.0f);

	for (ShopLayerWidget *shopLayerWidget : shopLayerWidgets){
		Sprite *button = shopLayerWidget->button;

		if (button->getBoundingBox().containsPoint(PointApplyTransform(location, shopLayerWidget->getParentToNodeTransform()))){
			shopBtnCallback(shopLayerWidget->shopStruct);
			break;
		}
	}
}

void ShopLayerInner::setShopLayerWidgetsScale(float scale){
	for (ShopLayerWidget *shopLayerWidget : shopLayerWidgets){
		Sprite *button = shopLayerWidget->button;

		button->setScale(scale);
	}
}
