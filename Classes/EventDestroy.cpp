#include "EventDestroy.h"


EventDestroy::EventDestroy(float _totalElapsedTime, Dec2 _dec2):FieldGameEventElement(_totalElapsedTime, _dec2, EventType::REMOVE){

}

void EventDestroy::accept(EventGameListener *eventGameListener){
	eventGameListener->visitEvent(this);
}