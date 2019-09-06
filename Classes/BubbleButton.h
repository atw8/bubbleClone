#ifndef _BUBBLE_TYPE_BUTTON_H_
#define _BUBBLE_TYPE_BUTTON_H_

#include "Bubble.h"
#include "BubbleView.h"

#include "cocos2d.h"
USING_NS_CC;

class BubbleButton : public BubbleView {
private:
	BubbleButton(float _innerRadius, float _outerRadius, Bubble _bubble);
public:
	static BubbleButton *create(float _innerRadius, float _outerRadius, Bubble _bubble);

	void onEnter();


	virtual bool containsPoint(Vec2 location);
private:
	bool isPressed;
	std::map<bool, float> isPressedScale;
	
	bool isSelected;
	DrawNode *isSelectedDrawNode;


public:
	bool getIsSelected();
	void setIsSelected(bool _isSelected);

	bool getIsPressed();
	void setIsPressed(bool _isPressed);
	void setIsPressedInner();




	virtual void setBubbleInner(Bubble bubble, bool forceInit);

private:
	float getShapeWidth();

	float outerRadius;


};

#endif //_BUBBLE_TYPE_BUTTON_H_