#ifndef _DD_USER_DEFAULT_H_
#define _DD_USER_DEFAULT_H_

#include "GameMode.h"

#include "ShopStruct.h"
#include "PropType.h"


#include <string>
#include <vector>

#define NOTIFICATION_setNumOfGold "NOTIFICATION_setNumOfGold" 

class DDUserDefault {
private:
	DDUserDefault();
public:
	static DDUserDefault *getInstance();


	//Game Related Crap
	std::vector<ShopStruct> getShopStructs();

public:
	int getNumOfGold();
	void addNumOfGold(int val);
private:
	void setNumOfGold(int _gold);
	inline std::string getNumOfGoldStr();



	//Bubble Designer Related Crap;
public:
	GameMode getDesignerGameMode();
	void setDesignerGameMode(GameMode gameMode);

private:
	GameModeType getDesignerGameModeType();
	void setDesignerGameModeType(GameModeType gameModeType);
	inline std::string getDesignerGameModeTypeStr();

	int getDesignerGameModeAmount();
	void setDesignerGameModeAmount(int amount);
	inline std::string getDesignerGameModeAmountStr();

public:
	int getDesignerBubbleWidth();
	void setDesignerBubbleWidth(int _bubbleWidth);
private:
	inline std::string getDesignerBubbleWidthStr();


public:
	int getPropCost(PropType propType);



public:
	bool getIsMusicPaused();
	void setIsMusicPaused(bool _isMusicPaused);

	bool getIsSoundPaused();
	void setIsSoundPaused(bool _isSoundPaused);

private:
	std::string getIsMusicPausedStr();
	std::string getIsSoundPausedStr();

public:
	int getLevelFrontier();
	void setLevelFrontier(int _levelFrontier);
private:

	std::string getLevelFrontierStr();

};

#endif //_DD_USER_DEFAULT_H_