#include "EventFission.h"

EventFission::EventFission(float _totalElapsedTime, Dec2 _dec2):FieldGameEventElement(_totalElapsedTime, _dec2, EventType::OTHER){

}

void EventFission::accept(EventGameListener *eventGameListener){
	eventGameListener->visitEvent(this);
}