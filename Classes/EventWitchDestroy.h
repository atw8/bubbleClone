#ifndef _EVENT_WITCH_DESTROY_H_
#define _EVENT_WITCH_DESTROY_H_

#include "FieldGameEventElement.h"
#include "Dec2.h"

class EventWitchDestroy : public FieldGameEventElement {
public:
	EventWitchDestroy(float _totalElapsedTime, Dec2 _dec2);

	void accept(EventGameListener *eventGameListener) override;
};

#endif //_EVENT_WITCH_DESTROY_H_