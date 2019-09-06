#ifndef _BUBBLE_DESIGNER_H_
#define _BUBBLE_DESIGNER_H_

#include "BubbleView.h"

class BubbleDesigner : public BubbleView {
private:
	BubbleDesigner(float _innerRadius, float _outerRadius, Bubble _bubble);
public:
	static BubbleDesigner* create(float _innerRadius, float _outerRadius, Bubble _bubble); 

	const float outerRadius;

	virtual void onEnter();
};


#endif //_BUBBLE_DESIGNER_H_