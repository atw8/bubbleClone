#ifndef _EVENT_WITCH_CHANGE_COLOR_H_
#define _EVENT_WITCH_CHANGE_COLOR_H_

#include "FieldGameEventElement.h"
#include "Dec2.h"

class EventWitchChangeColor : public FieldGameEventElement {
public:
	EventWitchChangeColor(float _totalElapsedTime, Dec2 _dec2);

	void accept(EventGameListener *eventGameListener) override;
};

#endif //_EVENT_WITCH_CHANGE_COLOR_H_