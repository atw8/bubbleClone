#ifndef _EVENT_NORMAL_EXPLOSION_H_
#define _EVENT_NORMAL_EXPLOSION_H_

#include "FieldGameEventElement.h"
#include "Dec2.h"

class EventNormalExplosion : public FieldGameEventElement {
public:
	EventNormalExplosion(float totalElapsedTime, Dec2 dec2);

	void accept(EventGameListener *eventGameListener);
};

#endif //_EVENT_NORMAL_EXPLOSION_H_