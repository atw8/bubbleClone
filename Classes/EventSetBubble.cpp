#include "EventSetBubble.h"

EventSetBubble::EventSetBubble(float _totalElapsedTime, Dec2 _dec2, ShootBubble *_shootBubble) 
:FieldGameEventElement(_totalElapsedTime, _dec2, EventType::ADD), shootBubble(_shootBubble){

}

void EventSetBubble::accept(EventGameListener *eventGameListener){
	eventGameListener->visitEvent(this);
}