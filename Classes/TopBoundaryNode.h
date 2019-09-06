#ifndef _TOP_BOUNDARY_NODE_H_
#define _TOP_BOUNDARY_NODE_H_

#include "cocos2d.h"
USING_NS_CC;

class TopBoundaryNode : public Node {
private:
	TopBoundaryNode(std::string _sprStr, int _total);
public:
	static TopBoundaryNode* create(std::string _sprStr, int _total);
	

	void setCurrent(int current);
	void setTotal(int _total);

	Rect getBoundingBox() const override;

	void onEnter() override;
private:
	const std::string sprStr;
	
	int total;
	int current;

	Sprite *slash;
	Sprite *spr;
	Label *totalLabel;
	Label *currentLabel;

private:
	std::vector<Vec2> getSuperEllipseCoords(float a, float b, float m, float n, int numOfPoints);
};

#endif //_TOP_BOUNDARY_NODE_H_