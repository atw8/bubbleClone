#include "EventChromeExplosion.h"

EventChromeExplosion::EventChromeExplosion(float totalElapsedTime, Dec2 dec2):FieldGameEventElement(totalElapsedTime, dec2, EventType::REMOVE){
}

void EventChromeExplosion::accept(EventGameListener *eventGameListener){
	eventGameListener->visitEvent(this);
}