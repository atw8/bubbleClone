#include "ShopLayerWidget.h"
#include "Global.h"

ShopLayerWidget::ShopLayerWidget(ShopStruct _shopStruct, std::string _goldIconStr) :shopStruct(_shopStruct), goldIconStr(_goldIconStr){

}

ShopLayerWidget* ShopLayerWidget::create(ShopStruct _shopStruct, std::string _goldIconStr){
	ShopLayerWidget *shopLayerWidget = new ShopLayerWidget(_shopStruct, _goldIconStr);
	if (shopLayerWidget && shopLayerWidget->initWithFile("shopLayer/bar.png")){
		shopLayerWidget->autorelease();
	}
	else {
		delete shopLayerWidget;
		shopLayerWidget = nullptr;
	}

	return shopLayerWidget;
}

void ShopLayerWidget::onEnter(){
	Sprite::onEnter();

	Sprite *goldIcon = Sprite::create(goldIconStr);
	goldIcon->setPosition(Vec2(60.0f, getContHeight(this) / 2.0f + 10.0f));
	this->addChild(goldIcon);

	Sprite *goldX = Sprite::create("shopLayer/shopGold_X.png");
	goldX->setPosition(Vec2(130.0f, getContHeight(this) / 2.0f));
	this->addChild(goldX);


	Label *goldLabel = Label::createWithCharMap("shopLayer/numbersGold.png", 84, 84, '0');
	goldLabel->setString(to_string(shopStruct.gold));
	goldLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
	goldLabel->setPosition(goldX->getPosition() + Vec2(10.0f, 0.0f));
	this->addChild(goldLabel);

	


	button = Sprite::create("shopLayer/buyBtn.png");
	button->setPosition(Vec2(getContWidth(this) - getContWidth(button) / 2.0f - 10.0f, getContHeight(this) / 2.0f));
	this->addChild(button);
	{
		const float xOffset = 10.0f;

		int price = shopStruct.price;

		Sprite *dotSpr = Sprite::create("shopLayer/point.png");
		dotSpr->setAnchorPoint(Vec2(0.5f, 1.0f));
		dotSpr->setPosition(Vec2(getContWidth(button) / 2.0f - 20.0f, getContHeight(button) / 2.0f));
		button->addChild(dotSpr);

		int kuai = price / 100;
		std::string kuaiStr = to_string(kuai);
		Label *kuaiLabel = Label::createWithCharMap("shopLayer/numbersGold2.png", 35, 32, '0');
		kuaiLabel->setString(kuaiStr);
		kuaiLabel->setAnchorPoint(Vec2(1.0f, 0.5f));
		kuaiLabel->setPosition(dotSpr->getPosition() + Vec2(-xOffset, 0.0f));
		button->addChild(kuaiLabel);

		int mao = price % 100;
		std::string maoStr = to_string(mao);
		if (mao < 10){
			maoStr = "0" + maoStr;
		}
		Label *maoLabel = Label::createWithCharMap("shopLayer/numbersGold2.png", 35, 32, '0');
		maoLabel->setString(maoStr);
		maoLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
		maoLabel->setPosition(dotSpr->getPosition() + Vec2(xOffset, 0.0f));
		button->addChild(maoLabel);

		Sprite *yuan = Sprite::create("shopLayer/yuan.png");
		yuan->setPosition(maoLabel->getPosition() + Vec2(90.0f, 0.0f));
		button->addChild(yuan);
	}


}


float ShopLayerWidget::getWidgetWidth(){
	return 636.0f;
}
float ShopLayerWidget::getWidgetHeight(){
	return 140.0f;
}