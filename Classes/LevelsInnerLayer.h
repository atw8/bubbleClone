#ifndef _LAYER_LEVELS_INNER_H_
#define _LAYER_LEVELS_INNER_H_

#include "LevelButton.h"

#include "cocos2d.h"
USING_NS_CC;

class LevelsInnerLayer : public Layer{
private:
	LevelsInnerLayer(Rect _mainLayerLevelsRect);

public:
	static LevelsInnerLayer *create(Rect _mainLayerLevelsRect);

	void onEnter() override;

	const Rect layerRect;

	void addLevel(LevelButton *levelButton);
	void removeLevel(LevelButton *levelButton);
	void removeAllLevels();
	
	void positionLevelButtons();

	int numOfRows;

	float getButtonsWidth(int firstButton, int lastButton);
	float getButtonsHeight(int firstButton, int lastButton);
	float geButtonsXOffset(int firstButton, int lastButton);

	bool canPositionRow(int firstButton, int lastButton);
	void positionRow(int firstButton, int lastButton);

	void updateInnerLayerMax();

private:
	std::vector<LevelButton *> levelButtons;

	ClippingNode *clipNode;
	Layer *innerLayer;

	LevelButton *pressButton;

	bool isDragging;
	float draggingSensativity;
public:
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

public:
	void setPressButton(LevelButton *button);
	LevelButton *getPressButton();
private:
	float innerLayerMinY;
	float innerLayerMaxY;

	const float minXOffset;
	const float yOffset;

public:
	void setDeleteButtonOn(bool deleteButtonOn);
};

#endif //_LAYER_LEVELS_INNER_H_