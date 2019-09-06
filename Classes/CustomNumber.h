#ifndef _CUSTOM_NUMBER_H_
#define _CUSTOM_NUMBER_H_

#include "cocos2d.h"
USING_NS_CC;

class CustomNumber : public TextFieldTTF {
private:
	CustomNumber(std::function<std::string(std::string)> _normalizeStrFunc, std::function<void(std::string)> _setStringCallback);
public:
	~CustomNumber();

	static CustomNumber* createWithPlace(const Size& dimensions, float fontSize, std::function<std::string(std::string)> _normalizeStrFunc, std::function<void(std::string)> _setStringCallback);

	virtual void setString(const std::string& text);
	


	void onEnter();
private:

	bool isPressed;
	DrawNode *isSelectedDrawNode;


	void drawIsSelectedDrawNode();
public:
	void setIsPressed(bool _isSelected);
	bool getIsPressed();

private:
	const std::function<std::string(std::string)> normalizeStrFunc;
	const std::function<void(std::string)> setStringCallback;
};

#endif //_CUSTOM_NUMBER_H_