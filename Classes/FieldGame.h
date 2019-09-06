#ifndef _FIELD_GAME_H_
#define _FIELD_GAME_H_

#include <list>

#include "FieldAbstract.h"

#include "ShootBubble.h"

#include "FieldGameEventManager.h"

#include "FieldGameEventElement.h"
#include "EventWitchChangeColor.h"
#include "EventNormalExplosion.h"
#include "EventFall.h"
#include "EventChromeExplosion.h"
#include "EventWitchDestroy.h"
#include "EventSetBubble.h"

#include "EventEnd2.h"
#include "EventNull.h"

#include "EventWood1Break.h"
#include "EventWood2Break.h"

#include "EventFog.h"

#include "EventIceBreak.h"

#include "EventFission.h"

#include "EventDestroy.h"

#include "JitterManager.h"

#include "Graph.h"

#include "FieldGameCallbacks.h"

enum class FieldGameState {
	START,
	NORMAL,
	
	END1,
	END2,
	END3,

	LOOSE1,
};

class FieldGame : public FieldAbstract, public EventGameListener {
private:
	FieldGame(Rect _fieldRect, int _bubbleWidth, FieldGameCallbacks fieldGameCallbacks);
public:
	~FieldGame();
	static FieldGame* create(Rect _fieldRect, int _bubbleWidth, FieldGameCallbacks fieldGameCallbacks);
	
	void updateField(float dt);

private:
	std::map<Dec2, BubbleView*> viewBubbles;
protected:
	void outitGame(bool completely) override;
	void initGame() override;


	float getStablePositionY();
public:

	void setPosition(const Vec2 &postion) override;

private:
	void calculateTopBottomModelY() override;

	FieldGameState fieldGameState;
public:
	void setFieldGameState(const FieldGameState _fieldGameState);
	const FieldGameState getFieldGameState();
	

private:
	std::set<ShootBubble *> shootBubbles;
public:
	void addShootBubble(ShootBubble *shootBubble);
	int getNumOfShootBubble();

private:
	std::set<Dec2> collapseToParent(const std::set<Dec2> &dec2s);
	std::set<Dec2> getCollisionHexagons(const std::set<Dec2> &dec2s, const Vec2 &position, const float &radius);
	std::set<Dec2> getNotCollisionHexagons(const std::set<Dec2> &dec2s, const Vec2 &position, const float &radius);

	std::set<Dec2> getValidHexagons(const std::set<Dec2> &dec2s);
	std::vector<Dec2> getValidHexagons(const std::vector<Dec2> &dec2s);
	
	std::set<Dec2> getOrderHexagons(const std::set<Dec2> &dec2s, int order, Dec2 dec2B = Dec2(0, 0));

	std::set<Dec2> getSameBubbleTypeHexagons(const std::set<Dec2> &dec2s, BubbleType bubbleType);

	std::set<Dec2> getPivotReachebleHexagons(const std::set<Dec2> &dec2s);
	std::set<Dec2> getPivotUnreachebleHexagons(const std::set<Dec2> &dec2s);





	std::set<Dec2> getBubbleSameColorChain(Dec2 dec2, BubbleType bubbleType, int order = -1);
	std::set<Dec2> getBubbleChain(Dec2 dec2, std::function<bool(Dec2)>, int order = -1);


	float totalElapsedTime;

	int getHexagonDifference(Dec2 decA, Dec2 decB);


public:
	void visitEvent(EventWitchChangeColor *eventWitchChangeColor) override;

	void visitEvent(EventWitchDestroy *eventWitchDestoy) override;
	void visitEvent(EventChromeExplosion *chromeExplosion) override;
	void visitEvent(EventNormalExplosion *normalExplosion) override;
	
	void visitEvent(EventSetBubble *eventSetBubble) override;
	
	void visitEvent(EventFall *eventFall) override;

	void visitEvent(EventEnd2 *eventEnd2) override;
	
	void visitEvent(EventNull *eventNull) override;

	void visitEvent(EventWood1Break *eventWood1Break) override;
	void visitEvent(EventWood2Break *eventWood2Break) override;

	void visitEvent(EventIceBreak *eventIceBreak) override;

	void visitEvent(EventFog *eventFog) override;

	void visitEvent(EventFission *eventFission) override;

	void visitEvent(EventDestroy *eventDestroy) override;

	void createFallEvents(Dec2 dec2);

	void eventFallCreate(BubbleView *bubbleView, Vec2 velocity);
	
	const FieldGameCallbacks callbacks;
private:
	inline void updateActiveBasicBubbleTypes();
	std::vector<BubbleType> activeBasicBubbleTypes;
public:
	BubbleType getRandomActiveBasicBubbleType();
	const std::vector<BubbleType> & getActiveBasicBubbleTypes();

private:
	FieldGameEventManager *fieldGameEventManger;
protected:
	void setBubbleModel(Dec2 dec2, Bubble bubble);
	void eraseBubbleModel(Dec2 dec2);

private:
	void createJitterRipple(Dec2 dec2, const int numJitterRadiuses, const float maxMagnitude);
	void createJitter(Dec2 dec2, Vec2 direction, float magnitude, float period, float timeOffset);
	

	Vec2 getRelationShipVec2(Dec2 dec2From, Dec2 dec2To);

	std::map<Dec2, JitterManager *> jitterManagers;
	Vec2 getViewBubblePosition(Dec2 dec2);
	Vec2 getViewBubbleVelocity(Dec2 dec2);
	
public:
	void moveFieldNormal(float dt);
private:
	float getMoveFieldVecPositionY(float dt);

public:
	std::vector<Vec2> getControlPoints(Vec2 currentPoint, Vec2 direction, float innerRadius, float shootBubbleRadius);
private:
	inline bool isOverLeftEdge(Vec2 point, Vec2 velocity, float radius);
	inline bool isOverRightEdge(Vec2 point, Vec2 velocity, float radius); 
	inline bool isOverTopEdge(Vec2 point, Vec2 velocity, float radius);
	inline bool isOverBottomEdge(Vec2 point, Vec2 velocity, float radius);

	inline bool isOverHexagon(Vec2 point, float radius);

	inline bool isOverTopBoundary(Vec2 point, Vec2 velocity);
	inline bool isOverBottomBoundary(Vec2 point, Vec2 velocity);

	inline std::pair<Vec2, Vec2> leftEdgeCollision(Vec2 point, Vec2 velocity, float radius);
	inline std::pair<Vec2, Vec2> rightEdgeCollision(Vec2 point, Vec2 velocity, float radius);
	inline std::pair<Vec2, Vec2> topEdgeCollision(Vec2 point, Vec2 velocity, float radius);
	inline std::pair<Vec2, Vec2> bottomEdgeCollision(Vec2 point, Vec2 velocity, float radius);
	Vec2 hexagonCollision(Vec2 point, Vec2 direction, float radius);

	inline Vec2 topBoundaryCollision(Vec2 point, Vec2 velocity);
	inline Vec2 bottomBoundaryCollision(Vec2 point, Vec2 velocity);

public:
	int getBubbleNum(Bubble bubble) override;

	int getNumOfEvents();

};


#endif //_FIELD_GAME_H_