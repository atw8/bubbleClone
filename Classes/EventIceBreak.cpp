#include "EventIceBreak.h"

EventIceBreak::EventIceBreak(float _totalElapsedTime, Dec2 _de2):FieldGameEventElement(_totalElapsedTime, _de2, EventType::REMOVE){

}

void EventIceBreak::accept(EventGameListener *eventGameListener){
	eventGameListener->visitEvent(this);
}