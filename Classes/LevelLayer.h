#ifndef _LEVEL_LAYER_H_
#define _LEVEL_LAYER_H_

#include "cocos2d.h"
USING_NS_CC;

#include "LevelsInnerLayer.h"

#include "TwoTextureSprite.h"

enum class LevelState {
	GAME,
	DESIGNER,
};

class LevelLayer : public Layer {
private:
	LevelLayer(LevelState _levelState);
public:
	static Scene *createScene(LevelState _levelState);
	static LevelLayer *create(LevelState _levelState);

	void onEnter() override;

	void onExit() override;

private:

	void updateLevelState(LevelState _levelState, bool isAnimation);
	Sprite *bg;
	EventListenerTouchOneByOne *listener;
	

	std::map<LevelState, float> levelStateT;
	
	TwoTextureSprite *buttonDesigner;
	TwoTextureSprite *buttonGame;

	LevelState levelState;

	std::map<LevelState, Vec2> levelsDesignerPosition;
	LevelsInnerLayer *levelsDesigner;

	std::map<LevelState, Vec2> levelsGamePosition;
	LevelsInnerLayer *levelsGame;

	std::map<LevelButtonType, int> numOfButtonTypesMap;

	bool deleteButtonOn;
	Sprite *buttonDelete;
public:
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void onTouchCancelled(Touch *touch, Event *unused_event);

public:
	Touch *main_touch;
};

#endif //_LEVEL_LAYER_H_