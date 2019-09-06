#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <string>
#include <sstream>

#include "cocos2d.h"
USING_NS_CC;


extern EventListenerKeyboard *keyboardListener;
static void defaultKeyboardListenerOnKeyPressed(EventKeyboard::KeyCode, Event*){

}
static void defaultKayboardListenerOnKeyReleased(EventKeyboard::KeyCode, Event*){

}



#include <vector>

static const std::string LEVEL_EXTENSION = ".json";

enum class LEVEL_TYPE {
    GAME_LEVELS = 0,
    CUSTOM_LEVELS = 1,
    TEST_LEVELS = 3,
};


#define getBoundWidth(Sprite) Sprite->getBoundingBox().size.width
#define getBoundHeight(Sprite) Sprite->getBoundingBox().size.height

#define getContWidth(Sprite) Sprite->getContentSize().width
#define getContHeight(Sprite) Sprite->getContentSize().height



template <typename T> static std::string to_string(T value)
{
	std::ostringstream os;
	os << value;
	return os.str();
}



std::string replaceSubStr(std::string target, std::string oldVal, std::string newVal);


std::string getFileNameForFullPath(std::string fullPath);

float floatMod(float base, float number);

Rect combineBoundingBoxes(const std::vector<Rect> &rects);


class Global {
private:
	Global();
public:
	static Global *getInstance();

	bool isLeapYear(int year);
	int getNumOfDaysInYear(int year);
	int getNumOfDaysInMonth(int month, int year);

    
    std::vector<std::string> getLevelsForLevelType(LEVEL_TYPE levelType);
    std::string getFullPathForLevelDirectoryAndLevelName(LEVEL_TYPE levelType, std::string levelName);
    
    void initLevelTypeFolder();
    
private:
    std::string getKeywordForLevelType(LEVEL_TYPE levelType);
    /*
	
	
	std::string getGameLevelDirectory();
	std::string getCustomLevelDirectory();
	std::string getTestLevelDirectory();
	*/
};


#endif //_GLOBAL_H_
