#ifndef _FIELD_ABSTRACT_
#define _FIELD_ABSTRACT_

#include "cocos2d.h"
USING_NS_CC;

#include "Bubble.h"
#include "BubbleDesigner.h"
#include "Dec2.h"

#include "Graph.h"


#include "json/writer.h"
#include "json/stringbuffer.h"



class FieldAbstract : public Layer {
protected:
	FieldAbstract(Rect _fieldRect, int _bubbleWidth);
public:
	static float calculateInnerRadius(float fieldWidth, int bubbleWidth);
	static float calculateOuterRadius(float fieldWidth, int bubbleWidth);

	~FieldAbstract();

	virtual bool init();

protected:
	std::map<Dec2, Bubble> modelBubbles;
	
public:
	const Rect fieldRect;
protected:
	Vec2 getVec2ForDec2(Dec2 dec2);
	float getVec2YForDec2Y(int dec2Y);
	float getVec2XForDec2X(int dec2X);
	Dec2 getDec2ForVec2(Vec2 vec2);

	inline bool isLongLRow(int y);

	bool isDec2Valid(std::vector<Dec2> dec2s);
	bool isDec2Valid(Dec2 dec2);

	std::vector<Dec2> getRowDec2s(int y);

protected:
	int leftBoundary;
	int rightBoundary;

	int topBoundary;
	bool hasTopBoundary;

	int bottomBoundary;
	bool hasBottomBoundary;

	int bubbleWidth;
	float innerRadius;
	float outerRadius;

public:
	float getOuterRadius();
	float getInnerRadius();

	
public:
	int getBubbleWidth();
	virtual void setBubbleWidth(int _bubleWidth, bool force = false);
	
protected:
	virtual void outitGame(bool completely) = 0;
	virtual void initGame() = 0;


protected:
	bool isRowVisible(int dec2Y);
	bool isRowVisible(int dec2Y, float offsetY);


public:
	void getLevelBoardMap(rapidjson::Writer<rapidjson::StringBuffer> &writer, bool saveWitch, bool saveFog);
	void setLevelBoardMap(rapidjson::Document &document);


	bool getModelBubblesVisible();
protected:
	std::vector<Dec2> getBubbleContainer(BubbleType bubbleType, Dec2 dec2);


	bool modelBubblesVisible;
	int bottomModelY;
	int topModelY;

	virtual void calculateTopBottomModelY() = 0;

protected:
	std::set<Dec2> getHexagonFrontOfOrder(int order, Dec2 offset);
	std::set<Dec2> getHexagonFrontOfOrder(int order);

	std::set<Dec2> expandSetByHexagon(int order, const std::set<Dec2> &set);

	std::set<Dec2> getHexagonOfOrder(int order, Dec2 offset);
	std::set<Dec2> getHexagonOfOrder(int order);



	std::set<Dec2> getUndefinedHexagons(const std::set<Dec2> &dec2s);
	
	std::vector<Dec2> getDefinedHexagons(const std::vector<Dec2> &dec2s);
	std::set<Dec2> getDefinedHexagons(const std::set<Dec2> &dec2s);
private:
	inline std::function<bool(Dec2)> getDefinedHexagonsHelperFunc();

protected:
	std::set<Dec2> getHelperHexagons(const std::set<Dec2> &dec2s, const std::function<bool(Dec2)> &helperFunc);
	std::vector<Dec2> getHelperHexagons(const std::vector<Dec2> &dec2s, const std::function<bool(Dec2)> &helperFunc);


protected:

	virtual void setBubbleModel(Dec2 dec2, Bubble bubble); 
	virtual void eraseBubbleModel(Dec2 dec2);

	Graph fallGraph;

public:

	float getBackTopY();
	float getBackBottomY();

	virtual int getBubbleNum(Bubble bubble);
	int getBubbleNumBasic(BubbleType bubbleTypeBasic);
	int getBubbleNumWitch();
	int getBubbleNumAnimal();

	
	int getNumOfEmptyElementsInTopRow();
	int getNumOfEmptyElementsInBottomRow();
private:
	int getNumOfEmptyElementsInRow(int row);
private:
	std::map<Bubble, int> bubblesNumMap;
	
};



#endif //_FIELD_ABSTRACT_