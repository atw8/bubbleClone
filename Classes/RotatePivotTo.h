#ifndef _ROTATE_PIVOT_TO_H_
#define _ROTATE_PIVOT_TO_H_

#include "cocos2d.h"
USING_NS_CC;

enum class RotatePivotToType{
	CW,
	CCW,
};

class RotatePivotTo : public ActionInterval {
private:
	RotatePivotTo(float _endAngle, float _endR, Vec2 _pivot, RotatePivotToType _rotatePivotToType);
public:
	static RotatePivotTo* create(float duration, float _endAngle, float _endR, Vec2 _pivot, RotatePivotToType _rotatePivotToType);

	virtual void update(float time) override;
	virtual void startWithTarget(Node *target) override;

	static float getAngleDiff(float startAngle, float endAngle, RotatePivotToType rotatePivotToType);

private:

	const RotatePivotToType rotatePivotToType;

	float startAngle;
	float startR;
	
	float endAngle;
	float endR;

	const Vec2 pivot;
};


#endif //_ROTATE_PIVOT_TO_H_