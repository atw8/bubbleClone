#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "cocos2d.h"
USING_NS_CC;

class Button : public Node {
private:
	Button(std::string _buttonStr, Size _buttonSize, float _fontSize);
public:
	static Button* create(std::string _buttonStr, Size _buttonSize, float _fontSize);


	bool init();
	void onEnter();

public:
	void setIsPressed(bool _isPressed);
	bool getIsPressed();

	void setIsSelected(bool _isSelected);
	bool getIsSelected();

	Rect getBoundingBox();
private:
	bool isPressed;

	bool isSelected;
public:
	void setButtonStr(std::string _buttonStr);
	std::string getButtonStr();
private:
	std::string buttonStr;
	
	
	Size buttonSize;
	DrawNode *normalBox;

	DrawNode *selectBox;

	Label *buttonLabel;
	
	const int fontSize;


};

#endif //_BUTTON_H_