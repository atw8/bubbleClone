#ifndef _PROP_BTN_H_
#define _PROP_BTN_H_

#include "PropType.h"
#include "GameMode.h"

#include "cocos2d.h"
USING_NS_CC;



class PropBtn : public Sprite {
private:
	PropBtn(PropType _propType, GameModeType _gameModeType, int _cost);
public:
	static PropBtn* create(PropType _propType, GameModeType _gameModeType, int _cost);

	void onEnter() override;



	const PropType propType;
	const GameModeType gameModeType;
	const int cost;

};


#endif //_PROP_BTN_H_