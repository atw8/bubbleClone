#ifndef _EVENT_GAME_LISTENER_H_
#define _EVENT_GAME_LISTENER_H_

class EventFall;
class EventNormalExplosion;
class EventChromeExplosion;
class EventWitchDestroy;
class EventWitchChangeColor;
class EventSetBubble;

class EventEnd2;

class EventNull;

class EventWood1Break;
class EventWood2Break;

class EventIceBreak;

class EventFog;

class EventFission;

class EventDestroy;

class EventGameListener {
public:
	EventGameListener();
	~EventGameListener();

	virtual void visitEvent(EventFall *eventFall) = 0;
	virtual void visitEvent(EventNormalExplosion *eventNormalExplosion) = 0;
	virtual void visitEvent(EventChromeExplosion *eventChromeExplosion) = 0;
	virtual void visitEvent(EventWitchDestroy *eventWitchDestoy) = 0;

	virtual void visitEvent(EventWitchChangeColor *eventWitchChangeColor) = 0;

	virtual void visitEvent(EventSetBubble *eventSetBubble) = 0;

	virtual void visitEvent(EventEnd2 *eventEnd2) = 0;

	virtual void visitEvent(EventNull *eventNull) = 0;

	virtual void visitEvent(EventWood1Break *eventWood1Break) = 0;
	virtual void visitEvent(EventWood2Break *eventWood2Break) = 0;

	virtual void visitEvent(EventIceBreak *eventIceBreak) = 0;

	virtual void visitEvent(EventFog *eventFog) = 0;

	virtual void visitEvent(EventFission *eventFission) = 0;

	virtual void visitEvent(EventDestroy *eventDestroy) = 0;
};

#endif //_EVENT_GAME_LISTENER_H_