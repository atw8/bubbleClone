#include "EventWood1Break.h"

EventWood1Break::EventWood1Break(float _totalElapsedTime, Dec2 _dec2):FieldGameEventElement(_totalElapsedTime, _dec2, EventType::MODIFY){

}

void EventWood1Break::accept(EventGameListener *eventGameListener){
	eventGameListener->visitEvent(this);
}