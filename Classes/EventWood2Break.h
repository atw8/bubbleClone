#ifndef _EVENT_WOOD_2_BREAK_H_
#define _EVENT_WOOD_2_BREAK_H_

#include "FieldGameEventElement.h"
#include "Dec2.h"

class EventWood2Break : public FieldGameEventElement {
public:
	EventWood2Break(float _totalElapsedTime, Dec2 _dec2);

	void accept(EventGameListener *eventGameListener) override;
};


#endif //_EVENT_WOOD_2_BREAK_H_