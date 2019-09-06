#ifndef _FIELD_GAME_EVENT_MANAGER_H_
#define _FIELD_GAME_EVENT_MANAGER_H_

#include <list>
#include <vector>

#include "FieldGameEventElement.h"

class FieldGameEventManager {
public:
	FieldGameEventManager();
	~FieldGameEventManager();

	void pushEvent(FieldGameEventElement *event);
	FieldGameEventElement *popEvent(float totalElapsedTime);


	void outit();


	int getNumOfFieldGameEvents();
private:
	std::list<FieldGameEventElement *> fieldGameEvents;
};

#endif //_FIELD_GAME_EVENT_MANAGER_H_