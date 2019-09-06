#ifndef _LEVEL_BUTTON_DESIGN_H_
#define _LEVEL_BUTTON_DESIGN_H_

#include "LevelButton.h"

class LevelButtonDesign : public LevelButton {
private:
	LevelButtonDesign(std::string _levelName);
public:
	static LevelButtonDesign *create(std::string _levelName);
public:

	void onEnter() override;

	float getButtonWidth() override;
	float getButtonHeight() override;


	void setIsPressed(bool _isPressed) override;

	void setDeleteButtonOn(bool _deleteButtonOn) override;
private:
	Sprite *spr;
	Sprite *delSpr;

	const float xScale;

};


#endif //_LEVEL_BUTTON_DESIGN_H_