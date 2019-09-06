#ifndef _SADDLE_VIEW_H_
#define _SADDLE_VIEW_H_

#include "cocos2d.h"
USING_NS_CC;

#include "ShootBubble.h"

#include "BubbleView.h"


#include "SaddleSpr.h"

#include "SaddleStruct.h"





class SaddleView : public Layer {
private:
	SaddleView(SaddleViewStruct _saddleViewStruct);

	const SaddleViewStruct saddleStruct;
public:
	static SaddleView *create(SaddleViewStruct _saddleViewStruct);

	void onEnter();




public:
	void setSaddleNodePosition(Vec2 location);
	void setSaddleNodePositionRest();
	bool canSetSaddleNodePosition(Vec2 location);


private:
	SaddleSpr *saddleNode;

public:

	void pushBubbleFront(ShootBubble *shootBubble);
	void pushBubbleFront(ShootBubble *shootBubble, Vec2 location);

	void pushBubbleBack(ShootBubble *shootBubble);
	void pushBubbleBack(ShootBubble *shootBubble, Vec2 location);

	ShootBubble *popBubbleFront();
	ShootBubble *popBubbleBack();
	

	ShootBubble *peekBubbleBack();
	ShootBubble *peekBubbleFront();

	int getNumOfShootBubbles();
private:
	std::list<ShootBubble *> shootBubbles;
	void moveShootBubblesToDefaultPosition();
	


	std::pair<float, float> getShootBubbleDefaultPolarPosition(int index);
	std::pair<float, float> getShootBubblePolarPosition(int index);

	Vec2 convertPolarToCartesian(std::pair<float, float> polarCoord);
	std::pair<float, float> convertCartesianToPolar(Vec2 carteesianCoord);
public:	

	void rotateBubbles();


private:
	void startMoveSaddleToRestState();
	void stopMoveSaddleToRestState();
	bool isMoveSaddleToRestState();
	void updateMoveSaddletoRestState(float dt);

public:
	void updateActiveBasicBubbleTypes(const std::vector<BubbleType> &activeBasicBubbleTypes);

private:
	void populateBubbleWithData(ShootBubble *bubble);

};

#endif //_SADDLE_VIEW_H_