#ifndef _EVENT_CHROME_EXPLOSION_H_
#define _EVENT_CHROME_EXPLOSION_H_

#include "FieldGameEventElement.h"
#include "Dec2.h"

class EventChromeExplosion : public FieldGameEventElement {
public:
	EventChromeExplosion(float totalElapsedTime, Dec2 dec2);

	void accept(EventGameListener *eventGameListener);
};

#endif //_EVENT_CHROME_EXPLOSION_H_