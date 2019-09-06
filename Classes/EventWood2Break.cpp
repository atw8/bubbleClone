#include "EventWood2Break.h"

EventWood2Break::EventWood2Break(float _totalElapsedTime, Dec2 _dec2) :FieldGameEventElement(_totalElapsedTime, _dec2, EventType::REMOVE){

}

void EventWood2Break::accept(EventGameListener *eventGameListener){
	eventGameListener->visitEvent(this);
}