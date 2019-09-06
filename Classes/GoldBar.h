#ifndef _GOLD_BAR_H_
#define _GOLD_BAR_H_

#include "cocos2d.h"
USING_NS_CC;

class GoldBar : public Node {
private:
	GoldBar(bool _displayPlusSpr);
public:
	static GoldBar* create(bool _displayPlusSpr);

	bool init() override;
	void onEnter() override;
	void onExit() override;


	void setNumGoldNotification(Ref *ref);
public:
	void setText(int text);
	void setText(std::string text);
	std::string getText();

	const Size &getContentSize() const override; 
	Rect getBoundingBox() const override;

	const bool displayPlusSpr;
private:
	Sprite *goldInfinite;
	Label *goldBarText;

	Sprite *goldBarBack;
	Sprite *goldBarPlus;

public:
	void setIsPressed(const bool _isPressed);
	bool getIsPressed();

private:
	bool isPressed;


};

#endif //_GOLD_BAR_H_