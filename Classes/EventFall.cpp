#include "EventFall.h"

#include "FieldGame.h"

EventFall::EventFall(float _totalElapsedTime, Dec2 _dec2) :FieldGameEventElement(_totalElapsedTime, _dec2, EventType::REMOVE){

}

void EventFall::accept(EventGameListener *eventGameListener){
	eventGameListener->visitEvent(this);
}