#ifndef _LEVEL_BUTTON_NEW_H_
#define _LEVEL_BUTTON_NEW_H_

#include "LevelButton.h"

class LevelButtonNew : public LevelButton {
private:
	LevelButtonNew();
public:
	static LevelButtonNew *create();
public:

	void onEnter() override;


	float getButtonWidth() override;
	float getButtonHeight() override;

	void setIsPressed(bool _isPressed) override;
private:
	Sprite *spr;
};


#endif //_LEVEL_BUTTON_NEW_H_