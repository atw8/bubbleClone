#include "EventNormalExplosion.h"

#include "FieldGame.h"

EventNormalExplosion::EventNormalExplosion(float totalElapsedTime, Dec2 dec2) :FieldGameEventElement(totalElapsedTime, dec2, EventType::REMOVE){

}


void EventNormalExplosion::accept(EventGameListener *eventGameListener){
	eventGameListener->visitEvent(this);
}
