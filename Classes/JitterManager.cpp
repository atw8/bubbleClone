#include "JitterManager.h"

JitterManager::JitterManager(Vec2 _stablePosition, float _totalElapsedTime):stablePosition(_stablePosition) {
	totalElapsedTime = _totalElapsedTime;
}

JitterManager::~JitterManager() {
	for (JitterController *jitterController : jitterControllers) {
		delete jitterController;
	}

	jitterControllers.clear();
}

void JitterManager::setTotalElapseTime(float _totalElapsedTime) {
	totalElapsedTime = _totalElapsedTime;

	for (std::set<JitterController *>::iterator jitterControllersIter = jitterControllers.begin(); jitterControllersIter != jitterControllers.end();) {
		JitterController *jitterController = *jitterControllersIter;
		
		if (jitterController->isFinished(totalElapsedTime)) {	
			delete jitterController;
			jitterControllersIter = jitterControllers.erase(jitterControllersIter);
		}
		else {
			jitterControllersIter++;
		}
	}
}

Vec2 JitterManager::getPosition() {
	Vec2 position;

	for (JitterController *jitterController : jitterControllers) {
		position += jitterController->getPosition(totalElapsedTime);
	}

	return stablePosition + position;
}

Vec2 JitterManager::getVelocity() {
	Vec2 velocity;

	for (JitterController *jitterController : jitterControllers) {
		velocity += jitterController->getVelocity(totalElapsedTime);
	}

	return velocity;
}

void JitterManager::addJitterController(JitterController *jitterController) {
	jitterControllers.insert(jitterController);
}
bool JitterManager::isJitterControllersEmpty() {
	return jitterControllers.empty();
}