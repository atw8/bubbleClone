#ifndef _LEVEL_BUTTON_GAME_UNLOCK_H_
#define _LEVEL_BUTTON_GAME_UNLOCK_H_

#include "LevelButton.h"

class LevelButtonGameUnlock : public LevelButton {
private:
	LevelButtonGameUnlock(int _levelNumber, std::string _levelName);
public:
	static LevelButtonGameUnlock *create(int _levelNumber, std::string _levelName);
public:

	void onEnter() override;


	float getButtonWidth() override;
	float getButtonHeight() override;

	void setIsPressed(bool _isPressed) override;
private:
	Sprite *spr;
};


#endif //_LEVEL_BUTTON_GAME_UNLOCK_H_