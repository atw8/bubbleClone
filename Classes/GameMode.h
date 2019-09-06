#ifndef _GAME_MODE_H_
#define _GAME_MODE_H_

enum class GameModeType {
	TIME,
	MOVE,
};

class GameMode {
public:
	GameMode();
	GameMode(GameModeType _gameModeType, int _gameModeAmount, int _clearTop);

private:
	GameModeType gameModeType;
	int gameModeAmount;

	int clearTop;
public:
	GameModeType getGameModeType();
	void setGameModeType(GameModeType _gameModeType);



	void decrGameModeAmount();
	void incrGameModeAmount();
	void addGameModeAmount(int val);

	void setGameModeAmount(int _gameModeAmount);
	int getGameModeAmount();



	void setGameModeClearTop(int _clearTop);
	int getGameModeClearTop();
};

#endif //_GAME_MODE_H_