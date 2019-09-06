#include "Global.h"

#include "cocos2d.h"
USING_NS_CC;

std::string replaceSubStr(std::string target, std::string oldVal, std::string newVal){
	int pos = 0;
	int match_pos;

	std::string result;

	while ((match_pos = target.find(oldVal, pos)) != std::string::npos){
		result += target.substr(pos, match_pos - pos);
		result += newVal;

		pos = match_pos + newVal.size();
	}

	result += target.substr(pos, std::string::npos);

	return result;
}


std::string getFileNameForFullPath(std::string fullPath){
	
	std::string lastOf;
#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	lastOf = "/";
#elif CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
	lastOf = "\\";
#endif
	
	lastOf = "/";

	int pos;

	pos = fullPath.find_last_of(lastOf);
	return fullPath.substr(pos + 1, std::string::npos);
}

float floatMod(float base, float number){
	while (number < 0.0f){
		number += base;
	}

	while (number >= base){
		number -= base;
	}

	return number;
}

Rect combineBoundingBoxes(const std::vector<Rect> &rects){
	
	Vec2 leftBottomBound;
	Vec2 rightTopBound;

	std::vector<Rect>::const_iterator rectsIter = rects.cbegin();

	if (rectsIter != rects.end()){
		{
			Rect rect = *rectsIter;
			leftBottomBound = rect.origin;
			rightTopBound = rect.origin + Vec2(rect.size.width, rect.size.height);
		}

		for (; rectsIter != rects.end(); rectsIter++){
			Rect rect = *rectsIter;

			leftBottomBound.x = std::min(leftBottomBound.x, rect.origin.x);
			leftBottomBound.y = std::min(leftBottomBound.y, rect.origin.y);
			rightTopBound.x = std::max(rightTopBound.x, rect.origin.x + rect.size.width);
			rightTopBound.y = std::max(rightTopBound.y, rect.origin.y + rect.size.height);
		}
	}



	Rect ret;
	ret.origin = leftBottomBound;
	ret.size = Size(rightTopBound.x - leftBottomBound.x, rightTopBound.y - leftBottomBound.y);


	return ret;
}




Global::Global(){

}
Global *Global::getInstance(){
	static Global gInstance;
	return &gInstance;
}




bool Global::isLeapYear(int year){
	bool ret;

	if (year % 400 == 0){
		ret = true;
	}
	else if (year % 100 == 0){
		ret = false;
	}
	else if (year % 4 == 0){
		ret = true;
	}
	else {
		ret = false;
	}

	return ret;

}

int Global::getNumOfDaysInYear(int year){
	int ret;
	if (isLeapYear(year)){
		ret = 366;
	}
	else {
		ret = 365;
	}


	return ret;
}

int Global::getNumOfDaysInMonth(int month, int year){
	int ret;

	switch (month){
	case 0:
		ret = 31;
	case 1:
		if (isLeapYear(year)){
			ret = 29;
		}
		else {
			ret = 28;
		}
	case 2:
		ret = 31;
	case 3:
		ret = 30;
	case 4:
		ret = 31;
	case 5:
		ret = 30;
	case 6:
		ret = 31;
	case 7:
		ret = 31;
	case 8:
		ret = 30;
	case 9:
		ret = 31;
	case 10:
		ret = 30;
	case 11:
		ret = 31;
	}
    


	return ret;
}



std::vector<std::string> Global::getLevelsForLevelType(LEVEL_TYPE levelType){
    std::string keyword = this->getKeywordForLevelType(levelType);
    
    std::vector<std::string> levels = FileUtils::getInstance()->listFiles(keyword);
    
    for (std::vector<std::string>::iterator strsIter = levels.begin(); strsIter != levels.end();) {
        std::string &str = *strsIter;
        
        //Erase all files which do not have the right extension
        if (str.size() < LEVEL_EXTENSION.size()) {
            strsIter = levels.erase(strsIter);
        }
        else {
            std::string strExt = str.substr(str.size() - LEVEL_EXTENSION.size());
            if (strExt.compare(LEVEL_EXTENSION) == 0) {
                str = str.substr(0, str.size() - LEVEL_EXTENSION.size());
                
                strsIter++;
            }
            else {
                strsIter = levels.erase(strsIter);
            }
        }
        
        str = getFileNameForFullPath(str);
    }
    
    
    
    
    return levels;
}

std::string Global::getFullPathForLevelDirectoryAndLevelName(LEVEL_TYPE levelType, std::string levelName){
    std::string keyword = this->getKeywordForLevelType(levelType);
    
    return keyword + "/" + levelName + LEVEL_EXTENSION;
}

void Global::initLevelTypeFolder(){
    /*
    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    FileUtils::getInstance()->addSearchPath(writablePath);
    */
     
    std::vector<LEVEL_TYPE> levelTypes;
    levelTypes.push_back(LEVEL_TYPE::CUSTOM_LEVELS);
    levelTypes.push_back(LEVEL_TYPE::TEST_LEVELS);
    
    for(std::vector<LEVEL_TYPE>::iterator iter = levelTypes.begin(); iter != levelTypes.end(); iter++){
        LEVEL_TYPE levelType = *iter;
        
        std::string keyword = this->getKeywordForLevelType(levelType);
        
        if (!FileUtils::getInstance()->isDirectoryExist(keyword)){
            FileUtils::getInstance()->createDirectory(keyword);
        }
        
    }
}

std::string Global::getKeywordForLevelType(LEVEL_TYPE levelType){
    std::string ret;
    switch(levelType){
        case LEVEL_TYPE::GAME_LEVELS:
            ret = "GAMEXXOO";
            break;
        case LEVEL_TYPE::CUSTOM_LEVELS:
            ret = "CUSTOM_LEVELS";
            break;
        case LEVEL_TYPE::TEST_LEVELS:
            ret = "TEST_LEVELS";
            break;
    }
    
    return ret;
}
    

