#ifndef _EVENT_END_2_H_
#define _EVENT_END_2_H_

#include "FieldGameEventElement.h"
#include "Dec2.h"

class FieldGame;

class EventEnd2 : public FieldGameEventElement {
public:
	EventEnd2(float totalElapsedTime);

	void accept(EventGameListener *eventGameListener);
};

#endif //_EVENT_END_2_H_