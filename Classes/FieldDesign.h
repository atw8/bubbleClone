#ifndef _FIELD_DESIGN_H_
#define _FIELD_DESIGN_H_

#include "FieldAbstract.h"



class FieldDesign : public FieldAbstract {
protected:
	FieldDesign(Rect _fieldRect, int _bubbleWidth);
public:
	static FieldDesign *create(Rect _fieldRect, int _bubbleWidth);




	std::map<Dec2, BubbleDesigner *> fieldBubbleView;


protected:
	void outitGame(bool completely) override;
	void initGame() override;






private:
	int bottomViewY;
	int topViewY;

	void addRow(int y);
	void removeRow(int y);



public:
	void setBubble(Vec2 vec2, Bubble bubble);
	void setBubble(const Dec2 dec2, Bubble bubble);

	
private:
	void calculateTopBottomModelY() override;
private:
	void addBottomRows();
	void addTopRows();

	void removeBottomRows();
	void removeTopRows();

	void initRows();
	void outitRows();

	void setBubbleDesignerBubble(BubbleDesigner *bubbleDesigner, Bubble bubble);

public:
	float getScrollSize();
	float getScrollPosition();
	void setScrollPosition(float scrollPosition);

	void setPositionY(float positionY);

private:
	float addRemoveRowsOffset;




public:

	void setOldLevelBoardMap(ValueMap oldLevelBoardMap);
};

#endif //_FIELD_DESIGN_H_
