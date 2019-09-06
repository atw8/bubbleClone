#include "GameMode.h"

#include "DDUserDefault.h"

GameMode::GameMode(GameModeType _gameModeType, int _gameModeAmount, int _clearTop) :gameModeType(_gameModeType), gameModeAmount(_gameModeAmount), clearTop(_clearTop){

}

GameMode::GameMode():GameMode(GameModeType::MOVE, 0, 0){

}

GameModeType GameMode::getGameModeType(){
	return gameModeType;
}
void GameMode::setGameModeType(GameModeType _gameModeType){
	gameModeType = _gameModeType;
}

void GameMode::decrGameModeAmount(){
	addGameModeAmount(-1);
}
void GameMode::incrGameModeAmount(){
	addGameModeAmount(1);
}
void GameMode::addGameModeAmount(int val){
	setGameModeAmount(getGameModeAmount() + val);
}

void GameMode::setGameModeAmount(int _gameModeAmount){
	gameModeAmount = _gameModeAmount;
}
int GameMode::getGameModeAmount(){
	return gameModeAmount;
}

void GameMode::setGameModeClearTop(int _clearTop){
	clearTop = _clearTop;
}
int GameMode::getGameModeClearTop(){
	return clearTop;
}