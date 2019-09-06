#include "RotatePivotTo.h"

RotatePivotTo::RotatePivotTo(float _endAngle, float _endR, Vec2 _pivot, RotatePivotToType _rotatePivotToType) :endAngle(_endAngle), endR(_endR), pivot(_pivot), rotatePivotToType(_rotatePivotToType){

}

RotatePivotTo* RotatePivotTo::create(float duration, float _endAngle, float _endR, Vec2 _pivot, RotatePivotToType _rotatePivotToType){
	RotatePivotTo *rotatePivotTo = new RotatePivotTo(_endAngle, _endR, _pivot, _rotatePivotToType);
	if (rotatePivotTo && rotatePivotTo->initWithDuration(duration)){
		rotatePivotTo->autorelease();
	}
	else {
		delete rotatePivotTo;
		rotatePivotTo = nullptr;
	}

	return rotatePivotTo;
}

void RotatePivotTo::startWithTarget(Node *target){
	ActionInterval::startWithTarget(target);

	startAngle = (target->getPosition() - pivot).getAngle();
	startR = (_target->getPosition() - pivot).length();

	switch (rotatePivotToType){
	case RotatePivotToType::CW:
		while (endAngle > startAngle){
			endAngle -= 2.0f*M_PI;
		}

		while (startAngle - endAngle > 2.0f*M_PI){
			endAngle += 2.0f*M_PI;
		}
		break;
	case RotatePivotToType::CCW:
		while (endAngle < startAngle){
			endAngle += 2.0f*M_PI;
		}

		while (endAngle - startAngle > 2.0f*M_PI){
			endAngle -= 2.0f*M_PI;
		}
		break;
	}
}


void RotatePivotTo::update(float time){
	float angle = startAngle + (endAngle - startAngle)*time;
	float R = startR + (endR - startR)*time;

	Vec2 position = pivot + R*Vec2(std::cos(angle), std::sin(angle));
	_target->setPosition(position);
}




float RotatePivotTo::getAngleDiff(float startAngle, float endAngle, RotatePivotToType rotatePivotToType){
	switch (rotatePivotToType){
	case RotatePivotToType::CW:
		while (endAngle > startAngle){
			endAngle -= 2.0f*M_PI;
		}

		while (startAngle - endAngle > 2.0f*M_PI){
			endAngle += 2.0f*M_PI;
		}

		break;
	case RotatePivotToType::CCW:
		while (endAngle < startAngle){
			endAngle += 2.0f*M_PI;
		}

		while (endAngle - startAngle > 2.0f*M_PI){
			endAngle -= 2.0f*M_PI;
		}
		break;
	}

	return std::abs(endAngle - startAngle);
}