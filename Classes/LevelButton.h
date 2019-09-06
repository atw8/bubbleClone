#ifndef _LEVEL_BUTTON_H_
#define _LEVEL_BUTTON_H_

#include "cocos2d.h"
USING_NS_CC;

#define LEVEL_BUTTON_NOT_PRESS_SCALE 0.8f
#define LEVEL_BUTTON_PRESS_SCALE (LEVEL_BUTTON_NOT_PRESS_SCALE*0.9f)

enum class LevelButtonType {
	GAME_LOCK,
	GAME_UNLOCK,
	DESIGN,
	DESIGN_NEW,
};

class LevelButton : public Node{
protected:
	LevelButton(int _levelNumber, std::string _levelName, LevelButtonType _levelButtonType);
public:
	static LevelButton *create(int _levelNumber, std::string _levelName, LevelButtonType _levelButtonType);

	const int levelNumber;
	const std::string levelName;
	const LevelButtonType levelButtonType;


	virtual void setIsPressed(bool _isPressed) = 0;
	bool getIsPressed();

	virtual float getButtonWidth() = 0;
	virtual float getButtonHeight() = 0;
	Rect getButtonRect();

	

	virtual void setDeleteButtonOn(bool _deleteButtonOn);
protected:
	bool isPressed;

	bool deleteButtonOn;
};


#endif //_LEVEL_BUTTON_H_