#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "cocos2d.h"
USING_NS_CC;

class Clock : public Sprite {
private:
	Clock(int _timeMove);
public:
	static Clock* create(int _timeMove);

	void onEnter() override;

	


	int getTimeMove();
	void setTimeMove(int _timeMove);
private:
	Label *timeMoveLabel;
	int timeMove;

};


#endif //_CLOCK_H_