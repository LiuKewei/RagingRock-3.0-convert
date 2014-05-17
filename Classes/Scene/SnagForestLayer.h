#ifndef __SNAGFOREST_LAYER_H__
#define __SNAGFOREST_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

//define which platform
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "Devil.h"
#include "Ball.h"
#include "MsgTypeForObserver.h"

#else
#include "Entity/Devil.h"
#include "Entity/Ball.h"
#include "Tools/MsgTypeForObserver.h"

#endif

#define BALL_LAUNCH_ROTATION (20)
#define BALL_LAUNCH_SPEED (10)

const float c_radius = 10.0f;

const unsigned int c_triSnags = 4;
const unsigned int c_snagHeightStart = 840;

const unsigned int c_ballHeightBegin = 190;
const unsigned int c_ballHeightStart = 64;
const unsigned int c_ballHeightStop = 2;


enum
{
	Z_ORDER_ZERO,
	Z_ORDER_ONE,
	Z_ORDER_TWO,
	Z_ORDER_THREE,
	Z_ORDER_FOUR,

	Z_ORDER_MAX,
};

class SnagForestLayer : public cocos2d::Layer
{
public:
	SnagForestLayer();
	virtual ~SnagForestLayer();
	//Layer init
	bool initWithEntryID(int entryId);
	CREATE_FUNC(SnagForestLayer);

	// default layer scheduler
	void update(float dt);

	//loading resources
	void updateProgresser(float dt);
	// launch ball 
	void ballRising(float dt);

	// Touches Callback function
	virtual bool TouchBegan(Touch* touch, Event* event);
	virtual void TouchMoved(Touch* touch, Event* event);
	virtual void TouchEnded(Touch* touch, Event* event);

	//PhysicsWorld of layer setter function
	void setPhyWorld(PhysicsWorld* world);

private:
	//initialize the resources of this layer
	void initResourcesWithProgresser();
	void initProgresser();
	void initMap();
	void initSnags();
	void initCells();
	void initSlots();
	void initArrow();
	void initParticleFire();
	void initExitButton();
	void menuCloseCallback(Ref* pSender);


	bool isCollidedWithBall(Ball* fallBall, Node *snag);

	void showCells(unsigned int indexOfCellArr);
	void routingDetection();		

	void interactionSubscribe();
	void handleDevil(Ref* pData);
	void handleDevilStop(Ref* pData);
	void triggerDevil();

	void handleBalloonStop(Ref* pData);
	
	bool removeDevil();

	void pause();
	void recover();
private:
	PhysicsWorld* m_physicsWorld;
	EventListenerTouchOneByOne* m_listener;

	// progress of loading resources
	ProgressTimer* m_resLoadingProgresser;
	Sprite* m_loadBg;
	float m_loadingVar;

	// use for routeDetection
	std::map<int, Vector<Ref*>> m_cellMap;

	Devil* m_devil;
	ParticleSystem*    m_emitter;

	Ball* m_ball;
	Sprite* m_arrow;
	Sprite* m_littleGameSlot;
	Size  m_winSize;
	float m_winX;
	float m_cellside;
	float m_ballAngle;
	bool  m_isBallGoingUp;
	bool  m_isLittleGameStart;

};
#endif // __SNAGFOREST_LAYER_H__
