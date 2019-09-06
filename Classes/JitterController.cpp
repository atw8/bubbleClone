#include "JitterController.h"

JitterController::JitterController(float _startTime, Vec2 _startDirection, float _magnitude, float _period, float _decayconstant, float _timeOffset)
:startTime(_startTime), startDirection(_startDirection), magnitude(_magnitude), pediod(_period), decayConstant(_decayconstant), timeOffset(_timeOffset){

}

Vec2 JitterController::getPosition(float time){
	float t = time - startTime;

	return startDirection*magnitude*std::sin((t + timeOffset) * (2.0f*M_PI / pediod))*(1.0f - t*decayConstant);
}


Vec2 JitterController::getVelocity(float time) {
	float t = time - startTime;

	return startDirection*magnitude*((2.0f*M_PI / pediod)* std::cos((t + timeOffset)*(2.0f*M_PI / pediod)) * (1.0f - t*decayConstant) - decayConstant*std::sin((t + timeOffset) * (2.0f*M_PI / pediod)));
}


bool JitterController::isFinished(float time) {
	return 1.0f - (time - startTime)*decayConstant < 0.0f;
}

float JitterController::getTimeOffset(float diff, float magnitude, float period) {
	float timeOffset = std::asin(diff / magnitude) /  ( 2.0f*M_PI / period );

	return timeOffset;
}