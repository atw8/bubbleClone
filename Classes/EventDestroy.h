#ifndef _EVENT_DESTROY_H_
#define _EVENT_DESTROY_H_

#include "FieldGameEventElement.h"
#include "Dec2.h"

class EventDestroy : public FieldGameEventElement {
public:
	EventDestroy(float _totalElapsedTime, Dec2 _dec2);

	void accept(EventGameListener *eventGameListener) override;
};


#endif //_EVENT_DESTROY_H_