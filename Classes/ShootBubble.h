#ifndef _SHOOT_BUBBLE_H_
#define _SHOOT_BUBBLE_H_

#include "BubbleView.h"

class ShootBubble : public BubbleView {
private:
	ShootBubble(float _innerRadius, Bubble _bubble);
public:
	static ShootBubble *create(float _innerRadius, Bubble _bubble);

	void onEnter();

private:
	Vec2 velocity;

	float shootBubbleRadius;

public:
	void updateVelocity(float dt);

	void addVelocity(Vec2 val);
	Vec2 getVelocity();
	void setVelocity(Vec2 _velocity);

	Vec2 getDirection();


	float getShootBubbleRadius();
	void setShootBubbleRadiusRelativeToInnerRadius(float ratio);
private:
	inline void setShootBubbleRadius(float _shootBubbleRadius);

};

#endif //_SHOOT_BUBBLE_H_