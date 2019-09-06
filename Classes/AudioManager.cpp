#include "AudioManager.h"

#include "DDUserDefault.h"

#include "SimpleAudioEngine.h"


AudioManager::AudioManager() {
	isMusicPaused = DDUserDefault::getInstance()->getIsMusicPaused();
	isSoundPaused = DDUserDefault::getInstance()->getIsSoundPaused();
}

AudioManager* AudioManager::getInstance() {
	static AudioManager gInstance;

	return &gInstance;
}


void AudioManager::playMusic(MusicType musicType) {
	std::string musicStr = getMusicStrForMusicType(musicType);
	
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(musicStr.c_str(), true);

	if (isMusicPaused) {
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}
}
void AudioManager::playSound(SoundType soundType) {
	if (isSoundPaused) {
		return;
	}

	std::string soundStr = getSoundStrForSoundType(soundType);
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundStr.c_str());
}

std::string AudioManager::getMusicStrForMusicType(MusicType musicType) {
	std::string ret;

	switch (musicType) {
	case MusicType::MAIN_MUSIC:
		ret = "game";
		break;
	}

	ret = "music/" + ret + ".mp3";

	return ret;
}
std::string AudioManager::getSoundStrForSoundType(SoundType soundType) {
	std::string ret;

	switch (soundType) {
	case SoundType::CLICK:
		ret = "effect_click";
		break;
	}

	ret = "effects/" + ret + ".mp3";

	return ret;
}


bool AudioManager::getIsMusicPaused() {
	return isMusicPaused;
}
void AudioManager::setIsMusicPaused(bool _isMusicPaused) {
	isMusicPaused = _isMusicPaused;

	DDUserDefault::getInstance()->setIsMusicPaused(isMusicPaused);

	if (isMusicPaused) {
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}
	else {
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
}

bool AudioManager::getIsSoundPaused() {
	return isSoundPaused;
}
void AudioManager::setIsSoundPaused(bool _isSoundPaused) {
	isSoundPaused = _isSoundPaused;

	DDUserDefault::getInstance()->setIsSoundPaused(isSoundPaused);
}


void AudioManager::preloadMusic() {
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(getMusicStrForMusicType(MusicType::MAIN_MUSIC).c_str());
}

void AudioManager::unloadMusic() {
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(getMusicStrForMusicType(MusicType::MAIN_MUSIC).c_str());
}


void AudioManager::preloadSound() {
	return;

	for (int i = (int)SoundType::FIRST_SOUND_TYPE; i <= (int)SoundType::LAST_SOUND_TYPE; i++) {
		SoundType soundType = (SoundType)i;

		CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(getSoundStrForSoundType(soundType).c_str());
	}
}
void AudioManager::unloadSound() {
	return;

	for (int i = (int)SoundType::FIRST_SOUND_TYPE; i <= (int)SoundType::LAST_SOUND_TYPE; i++) {
		SoundType soundType = (SoundType)i;

		CocosDenshion::SimpleAudioEngine::getInstance()->unloadEffect(getSoundStrForSoundType(soundType).c_str());
	}
}