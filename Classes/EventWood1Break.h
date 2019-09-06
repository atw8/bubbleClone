#ifndef _EVENT_WOOD_1_BREAK_H_
#define _EVENT_WOOD_1_BREAK_H_

#include "FieldGameEventElement.h"
#include "Dec2.h"

class EventWood1Break : public FieldGameEventElement {
public:
	EventWood1Break(float _totalElapsedTime, Dec2 _dec2);

	void accept(EventGameListener *eventGameListener) override;
};


#endif //_EVENT_WOOD_1_BREAK_H_