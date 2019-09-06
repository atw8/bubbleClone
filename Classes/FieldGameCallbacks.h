#ifndef _FIELD_GAME_CALLBACKS_H_
#define _FIELD_GAME_CALLBACKS_H_

#include <functional>

class BubbleView;

#include "cocos2d.h"
USING_NS_CC;

enum class FieldGameState;

class FieldGameEventElement;

class FieldGameCallbacks {
public:
	std::function<void(BubbleView *, Vec2)> eventBubbleFallCallback;
	std::function<void(BubbleView *)> animalWitchDestroyCallback;

	std::function<void(FieldGameState)> setFieldGameState;

	std::function<void(FieldGameEventElement *)> executeVisit;

};


#endif //_FIELD_GAME_CALLBACKS_H_