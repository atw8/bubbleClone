#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include "cocos2d.h"
USING_NS_CC;

enum class MusicType{
	UNDEFINED,

	MAIN_MUSIC,
};

enum class SoundType {
	UNDEFINED,
	
	CLICK,


	FIRST_SOUND_TYPE = CLICK,
	LAST_SOUND_TYPE = CLICK,
};

class AudioManager {
private:
	AudioManager();
public:
	static AudioManager* getInstance();


	void playMusic(MusicType musicType);
	void playSound(SoundType soundType);




	bool getIsMusicPaused();
	void setIsMusicPaused(bool _isMusicPaused);

	bool getIsSoundPaused();
	void setIsSoundPaused(bool _isSoundPaused);
private:
	std::string getMusicStrForMusicType(MusicType musicType);
	std::string getSoundStrForSoundType(SoundType soundType);


	bool isMusicPaused;
	bool isSoundPaused;

public:
	void preloadMusic();
	void preloadSound();

	void unloadMusic();
	void unloadSound();
};



#endif //_AUDIO_MANAGER_H_