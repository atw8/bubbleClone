#ifndef _JITTER_MANAGER_H_
#define _JITTER_MANAGER_H_

#include "JitterController.h"

class JitterManager {
public:
	JitterManager(Vec2 _stablePosition, float _totalElapsedTime);
	~JitterManager();

	void setTotalElapseTime(float _totalElapsedTime);

	Vec2 getPosition();
	Vec2 getVelocity();


	void addJitterController(JitterController *jitterController);
	bool isJitterControllersEmpty();
private:
	std::set<JitterController *> jitterControllers;


	const Vec2 stablePosition;
	float totalElapsedTime;
};


#endif //_JITTER_MANAGER_H_