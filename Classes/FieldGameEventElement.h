#ifndef _FIELD_GAME_EVENT_ELEMENT_H_
#define _FIELD_GAME_EVENT_ELEMENT_H_


#include "Dec2.h"
#include "EventGameListener.h"


enum class EventType{
	ADD,
	REMOVE,
	MODIFY,

	OTHER,
};


class FieldGameEventElement {
protected:
	FieldGameEventElement(float _totalElapsedTime, Dec2 _dec2, EventType eventType);
public:
	const Dec2 dec2;
	const EventType eventType;
	const float totalElapsedTime;

	virtual void accept(EventGameListener *eventGameListener) = 0;
};

#endif //_FILE_GAME_EVENT_H_