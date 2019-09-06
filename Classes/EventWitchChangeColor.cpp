#include "EventWitchChangeColor.h"

EventWitchChangeColor::EventWitchChangeColor(float _totalElapsedTime, Dec2 _dec2) :FieldGameEventElement(_totalElapsedTime, _dec2, EventType::MODIFY){

}

void EventWitchChangeColor::accept(EventGameListener *eventGameListener){
	eventGameListener->visitEvent(this);
}