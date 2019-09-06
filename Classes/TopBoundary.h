#ifndef _TOP_BOUNDARY_H_
#define _TOP_BOUNDARY_H_

#include "cocos2d.h"
USING_NS_CC;

#include "TopBoundaryNode.h"

class TopBoundary : public Layer {
private:
	TopBoundary(Rect _topBoundaryRect);
public:
	static TopBoundary* create(Rect _topBoundaryRect);
	static Scene* createScene(Rect _topBoundaryRect);

	void onEnter();
private:
	int numOfWitchesTotal;
	int numOfAnimalTotal;
	int numOfClearTopTotal;
public:
	void setNumOfWitchesTotal(int _numOfWitchesTotal);
	void setNumOAnimalTotal(int _numOfAnimalTotal);
	void setNumOfClearTopTotal(int _numOfClearTopTotal);

	int getNumOfWitchesTotal();
	int getNumOfAnimalTotal();
	int getNumOfClearTopTotal();

private:
	int numOfWitchesClear;
	int numOfAnimalClear;
	int numOfClearTopClear;
public:
	void setNumOfWitchesClear(int _numOfWitchesClear);
	void setNumOfAnimalClear(int _numOfAnimalClear);
	void setNumOfClearTopClear(int _numOfClearTopClear);

	int getNumOfWitchesClear();
	int getNumOfAnimalClear();
	int getNumOfClearTopClear();

	Vec2 getWitchesFlyPosition();
	Vec2 getAnimalFlyPosition();
	Vec2 getClearTopFlyPosition();

	bool isWin();
private:
	const Rect topBoundaryRect;


	Sprite *topBoundarySpr;

	TopBoundaryNode *nodeWitch;
	TopBoundaryNode *nodeAnimal;
	TopBoundaryNode *nodeClearTop;


	void createNodeWitch();
	void createNodeAnimal();
	void createNodeClearTop();

	void deleteNodeWitch();
	void deleteNodeAnimal();
	void deleteNodeClearTop();

	void positionNodes();
};

#endif //_TOP_BOUNDARY_H_