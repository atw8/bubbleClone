#include "EventWitchDestroy.h"


EventWitchDestroy::EventWitchDestroy(float _totalElapsedTime, Dec2 _dec2):FieldGameEventElement(_totalElapsedTime, _dec2, EventType::REMOVE){

}

void EventWitchDestroy::accept(EventGameListener *eventGameListener){
	eventGameListener->visitEvent(this);
}