#ifndef _SCROLL_BAR_H_
#define _SCROLL_BAR_H_

#include "cocos2d.h"
USING_NS_CC;

class ScrollBar : public Node {
private:
	ScrollBar(float backTopY, float _backBottomY, float _scrollSize, float _scrollPosition, Color4F _scrollComponentColor, Color4F _backComponentColor);
public:
	static ScrollBar* create(float _backTopY, float _backBottomY, float _scrollSize, float _scrollPosition, Color4F _scrollComponentColor, Color4F _backComponentColor);

	void onEnter();
	void setContentSize(Size contentSize);

private:
	DrawNode *scrollComponent;
	DrawNode *backComponent;


	float backTopY;
	float backBottomY;
	float scrollSize;
	float scrollPosition;

public:
	float getBackTopY();
	void setBackTopY(float _backTopY);

	float getBackBottomY();
	void setBackBottomY(float _backBottomY);

	float getScrollSize();
	void setScrollSize(float _scrollSize);

	float getScrollPosition();
	void addScrollPosition(float val);
	void setScrollPosition(float _scrollPosition);


	float getBackSize();
private:
	void updateScrollComponentSize();

	Color4F scrollComponentColor;
	Color4F backComponentColor;
};

#endif //_SCROLL_BAR_H_