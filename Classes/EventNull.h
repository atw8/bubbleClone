#ifndef _EVENT_NULL_H_
#define _EVENT_NULL_H_

#include "FieldGameEventElement.h"
#include "Dec2.h"

class EventNull : public FieldGameEventElement {
public:
	EventNull(float totalElapsedTime);

	void accept(EventGameListener *eventGameListener) override;
};

#endif //_EVENT_NULL_H_