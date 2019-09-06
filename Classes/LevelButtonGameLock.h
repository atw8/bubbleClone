#ifndef _LEVEL_BUTTON_GAME_LOCK_H_
#define _LEVEL_BUTTON_GAME_LOCK_H_

#include "LevelButton.h"

class LevelButtonGameLock : public LevelButton {
private:
	LevelButtonGameLock();
public:
	static LevelButtonGameLock *create();

	void onEnter() override;

	void setIsPressed(bool _isPressed) override;
	float getButtonWidth() override;
	float getButtonHeight() override;

private:
	Sprite *spr;
};


#endif //_LEVEL_BUTTON_GAME_LOCK_H_