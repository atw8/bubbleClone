#ifndef _SCROLL_BAR_INTEGER_H_
#define _SCROLL_BAR_INTEGER_H_

#include "cocos2d.h"
USING_NS_CC;

class ScrollBarInteger : public Node{
private:
	ScrollBarInteger(int _scrollBottom, int _scrollTop, int _scroll);
public:
	static ScrollBarInteger* create(int _scrollBottom, int _scrollTop, int _scroll);

	void onEnter();

	void setContentSize(Size contentSize);

private:
	int scrollBottom;
	int scrollTop;
	int scroll;


	DrawNode *backComponent;
	DrawNode *scrollComponent;


	inline float getVec2XForScroll(int i);
public:
	int getScrollBottom();
	int getScrollTop();
	int getScroll();

	void setScrollBottom(int _scrollBottom, bool force = false);
	void setScrollTop(int _scrollTop, bool force = false);
	void setScroll(int _scroll, bool force = false);

	void setScrollFloat(float positionX);

};

#endif //_SCROLL_BAR_INTEGER_H_