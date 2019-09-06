#ifndef _BUBBLE_VIEW_H_
#define _BUBBLE_VIEW_H_

#include "Bubble.h"

#include "cocos2d.h"
USING_NS_CC;

#include "cocostudio/CocoStudio.h"
using namespace cocostudio;


class BubbleView : public Node {
protected:
	BubbleView(float _innerRadius, Bubble _bubble);
public:
	~BubbleView();

	static BubbleView *create(float _innerRadius, Bubble _bubble);

	const float innerRadius;

	virtual void onEnter();
private:
	Bubble bubble;
protected:
	Sprite *bubbleSprite;
	Armature *bubbleArmature;

	Node *bubbleNode;
public:
	void setBubble(Bubble _bubble);
	Bubble getBubble() const;

	Vec2 getBubbleNodePosition();

protected:
	virtual void setBubbleInner(Bubble _bubble, bool forceInit);

};

#endif //_HEXAGON_NOHDE_H_