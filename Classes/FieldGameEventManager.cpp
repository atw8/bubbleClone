#include "FieldGameEventManager.h"


FieldGameEventManager::FieldGameEventManager(){

}
FieldGameEventManager::~FieldGameEventManager(){
	outit();
}

void FieldGameEventManager::outit(){
	for (FieldGameEventElement *event : fieldGameEvents){
		delete event;
	}
	fieldGameEvents.clear();
}

void FieldGameEventManager::pushEvent(FieldGameEventElement *event){
	std::list<FieldGameEventElement *>::iterator whereIter = fieldGameEvents.end();
	for (std::list<FieldGameEventElement *>::iterator fieldGameEventsIter = fieldGameEvents.begin(); fieldGameEventsIter != fieldGameEvents.end() && whereIter == fieldGameEvents.end(); ++fieldGameEventsIter) {
		FieldGameEventElement *fieldGameEvent2 = *fieldGameEventsIter;
		if (fieldGameEvent2->totalElapsedTime > event->totalElapsedTime) {
			whereIter = fieldGameEventsIter;
		}
	}

	fieldGameEvents.insert(whereIter, event);
}
FieldGameEventElement *FieldGameEventManager::popEvent(float totalElapsedTime){
	FieldGameEventElement *ret = nullptr;

	if (!fieldGameEvents.empty()){
		FieldGameEventElement *event = fieldGameEvents.front();
		if (event->totalElapsedTime <= totalElapsedTime){
			ret = event;

			fieldGameEvents.pop_front();
		}
	}

	return ret;
}

int FieldGameEventManager::getNumOfFieldGameEvents(){
	return fieldGameEvents.size();
}