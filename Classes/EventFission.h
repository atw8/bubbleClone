#ifndef _EVENT_FISSION_H_
#define _EVENT_FISSION_H_

#include "FieldGameEventElement.h"
#include "Dec2.h"

class EventFission : public FieldGameEventElement {
public:
	EventFission(float _totalElapsedTime, Dec2 _dec2);

	void accept(EventGameListener *eventGameListener) override;
};


#endif //_EVENT_FISSION_H_