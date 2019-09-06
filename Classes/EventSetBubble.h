#ifndef _EVENT_SET_BUBBLE_H_
#define _EVENT_SET_BUBBLE_H_

#include "FieldGameEventElement.h"
#include "Dec2.h"

#include "ShootBubble.h"


class EventSetBubble : public FieldGameEventElement {
public:
	EventSetBubble(float _totalElapsedTime, Dec2 _dec2, ShootBubble *_shootBubble);

	void accept(EventGameListener *eventGameListener) override;

	ShootBubble *shootBubble;
};

#endif //_EVENT_SET_BUBBLE_H_