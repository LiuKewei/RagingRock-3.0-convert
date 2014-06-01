#include "AppDelegate.h"
#include "Scene/SnagForestScene.h"
USING_NS_CC;

const char* MsgTypeForObserver::c_DevilPosUpdate = "1";
const char* MsgTypeForObserver::c_DevilFightingStart = "2";
const char* MsgTypeForObserver::c_DevilFightingStop = "3";
const char* MsgTypeForObserver::c_DevilPosPush = "4";

const char* MsgTypeForObserver::c_BalloonStart = "5";
const char* MsgTypeForObserver::c_BalloonStop = "6";

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
	timeval psv;
	gettimeofday(&psv, NULL);
	unsigned long int rand_seed = psv.tv_sec * 1000 + psv.tv_usec / 1000;//设置时间随机因子
	srand(rand_seed);

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("Raging Rock");
        director->setOpenGLView(glview);
    }
    //Size frameSize = glview->getFrameSize();
	Size designSize = Size(640, 960);
	Size resourceSize = Size(640, 960);
    director->setContentScaleFactor(resourceSize.height / designSize.height);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	glview->setFrameSize(designSize.width, designSize.height);
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::SHOW_ALL);
#else
	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::SHOW_ALL);
#endif
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
	auto scene = SnagForestScene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Point(0.0f, -1000.0f));
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

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
