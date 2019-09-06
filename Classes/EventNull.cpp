#include "EventNull.h"

EventNull::EventNull(float totalElapsedTime):FieldGameEventElement(totalElapsedTime, Dec2(0, 0), EventType::OTHER){

}

void EventNull::accept(EventGameListener *eventGameListener){
	eventGameListener->visitEvent(this);
}