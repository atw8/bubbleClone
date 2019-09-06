#ifndef _EVENT_FOG_H_
#define _EVENT_FOG_H_

#include "FieldGameEventElement.h"
#include "Dec2.h"


class EventFog : public FieldGameEventElement {
public:
	EventFog(float _totalElapsedTime, Dec2 _dec2);

	void accept(EventGameListener *eventGameListener) override;
};


#endif //_EVENT_FOG_H_