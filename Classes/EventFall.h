#ifndef _EVENT_FALL_H_
#define _EVENT_FALL_H_

#include "FieldGameEventElement.h"
#include "Dec2.h"

class FieldGame;

class EventFall : public FieldGameEventElement {
public:
	EventFall(float totalElapsedTime, Dec2 _dec2);

	void accept(EventGameListener *eventGameListener);
};


#endif //_EVENT_FALL_H_