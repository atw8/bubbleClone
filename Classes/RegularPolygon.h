#ifndef _REGULAR_POLYGON_H_
#define _REGULAR_POLYGON_H_

#include "cocos2d.h"
USING_NS_CC;

class RegularPolygon : public DrawNode {
protected:
	RegularPolygon(float _radius, int _numOfVertices, float _initialAngle);
public:
	~RegularPolygon();

	static RegularPolygon *create(float _radius, int _numOfVertices, float _initialAngle);

	virtual void onEnter();

private:
	const float radius;
	const int numOfVertices;
	const float initialAngle;

};

#endif //_REGULAR_POLYGON_H_