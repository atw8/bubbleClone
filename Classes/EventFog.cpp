#include "EventFog.h"


EventFog::EventFog(float _totalElapsedTime, Dec2 _dec2):FieldGameEventElement(_totalElapsedTime, _dec2, EventType::MODIFY){

}

void EventFog::accept(EventGameListener *eventGameListener){
	eventGameListener->visitEvent(this);
}