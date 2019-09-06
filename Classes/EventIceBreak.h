#ifndef _EVENT_ICE_BREAK_H_
#define _EVENT_ICE_BREAK_H_

#include "FieldGameEventElement.h"
#include "Dec2.h"

class EventIceBreak : public FieldGameEventElement {
public:
	EventIceBreak(float _totalElapsedTime, Dec2 _de2);

	void accept(EventGameListener *eventGameListener) override;
};

#endif //_EVENT_ICE_BREAK_H_