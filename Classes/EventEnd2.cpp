#include "EventEnd2.h"

#include "FieldGame.h"

EventEnd2::EventEnd2(float _totalElapsedTime) :FieldGameEventElement(_totalElapsedTime, Dec2(0, 0), EventType::OTHER){

}

void EventEnd2::accept(EventGameListener *eventGameListener){
	eventGameListener->visitEvent(this);
}