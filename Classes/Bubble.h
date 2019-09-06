#ifndef _BUBBLE_H_
#define _BUBBLE_H_

#include "Dec2.h"
#include <vector>

enum class BubbleType {
	CHROME = -1,

	RED = 0,
	BLUE = 1,
	GREEN = 2,
	YELLOW = 3,
	PURPLE = 4,

	FIRST_BASIC_BUBBLE_TYPE = RED,
	LAST_BASIC_BUBBLE_TYPE = PURPLE,

	ICE = 5,
	WOOD_1 = 6,
	WOOD_2 = 7,
	FOG = 8,
	FISION = 9,
	STONE = 10,


	WITCH = 11,

	ANIMAL1 = 12,
	ANIMAL2 = 13,
	ANIMAL3 = 14,

	FIRST_ANIMAL_TYPE = ANIMAL1,
	LAST_ANIMAL_TYPE = ANIMAL3,


	UNDEFINED = 15,
};



class Bubble {
public:
	Bubble(BubbleType _bubbleType, BubbleType _bubbleTypeWitchFog, Dec2 _parent);
	Bubble(BubbleType _bubbleType, BubbleType _bubbleTypeWitchFog);

	Bubble(BubbleType _bubbleType, Dec2 _parent);
	Bubble(BubbleType _bubbleType);
	Bubble();


	BubbleType getBubbleType() const;
	void setBubbleType(BubbleType _bubbleType, Dec2 _parent);

	void setBubbleTypeWitch(BubbleType _witchColor, Dec2 _parent);


	void setBubbleTypeFog(BubbleType _fogColor);

	BubbleType getBubbleTypeWitchFog() const;

	Dec2 getParent() const;
private:
	BubbleType bubbleType;

	BubbleType bubbleTypeWitchFog;

	Dec2 parent;
public:
	bool operator==(const Bubble& b) const;
	bool operator<(const Bubble& b) const;
	
	static BubbleType getRandomBasicBubbleType();
	static const std::vector<BubbleType> & getBasicBubbleTypes();
};




#endif //_BUBBLE_H_