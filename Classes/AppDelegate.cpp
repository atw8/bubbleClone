#include "AppDelegate.h"


#include "HelloWorldScene.h"
#include "GameModeLayer.h"
#include "GameLayer.h"

#include "Global.h"


#include "ShopLayer.h"

#include "PauseLayer.h"

#include "AudioManager.h"

#include "LevelLayer.h"

USING_NS_CC;

//static cocos2d::Size designResolutionSize = cocos2d::Size(720, 1280);
static cocos2d::Size designResolutionSize = cocos2d::Size(720, 1000);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

EventListenerKeyboard *keyboardListener;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("bubbleClone", Rect(0, 0, designResolutionSize.width*0.7f, designResolutionSize.height*0.78f));
#else
        glview = GLViewImpl::create("bubbleClone");
#endif
        director->setOpenGLView(glview);
    }

	
	srand(time(nullptr));
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60.0f);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
    register_all_packages();


    Global::getInstance()->initLevelTypeFolder();
	

	AudioManager::getInstance()->preloadMusic();
	AudioManager::getInstance()->preloadSound();


	AudioManager::getInstance()->playMusic(MusicType::MAIN_MUSIC);

	/*
	if (!FileUtils::getInstance()->isDirectoryExist(GAME_LEVELS)){
		FileUtils::getInstance()->createDirectory(FileUtils::getInstance()->getDefaultResourceRootPath() + "/" + GAME_LEVELS);
	}
	*/

	keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = &defaultKeyboardListenerOnKeyPressed;
	keyboardListener->onKeyReleased = &defaultKayboardListenerOnKeyReleased;
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(keyboardListener, -1);

	
	//auto scene = PauseLayer::createScene();
	
	
	//auto scene = MainLayer::createDefaultScene();
	
	auto scene = LevelLayer::createScene(LevelState::GAME);
	//auto scene = HelloWorld::createScene(FileUtils::getInstance()->fullPathForDirectory(CUSTOM_LEVELS), "");
	
	//auto scene = ShopLayer::createScene();
	//auto scene = HelloWorld::createScene("", "");
	
	//auto scene = GameModeLayer::createScene(nullptr, GameMode(GameModeType::MOVE, 30, 0), 11);


	
	


    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
