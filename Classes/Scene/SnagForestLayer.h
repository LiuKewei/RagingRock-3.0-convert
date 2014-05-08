#ifndef __SNAGFOREST_LAYER_H__
#define __SNAGFOREST_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Entity/Ball.h"
#include "Entity/Entity.h"
//#include "MsgTypeForObserver.h"

#define BALL_LAUNCH_ROTATION (20)
#define BALL_LAUNCH_SPEED (7)

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

class SnagForestLayer: public cocos2d::Layer 
{
public:	
	SnagForestLayer();	
	virtual ~SnagForestLayer();
	CREATE_FUNC(SnagForestLayer);

	void update(float dt);
	void ballLauncherMoving(float dt);

	virtual bool TouchBegan(Touch* touch, Event* event);
	virtual void TouchMoved(Touch* touch, Event* event);
	virtual void TouchEnded(Touch* touch, Event* event);

	bool SnagForestLayer::initWithEntryID(int entryId);


	void setPhyWorld(PhysicsWorld* world);
private:
	bool isCollidedWithBall(Ball* fallBall, Node *snag);
	void showCells(unsigned int indexOfCellArr);
	void routeDetection();
	void createFallBall();

	void interactionSubscribe();
	void handleDevil(Ref* pData);
	void handleDevilStop(Ref* pData);

	void triggerDevil();

	void initMap();
	void initBallLauncher();
	void initSnags();
	void initCell();
	void initSlots();

	bool removeDevil();

	void createParticleFire();

private:
	PhysicsWorld* m_physicsWorld;
	EventListenerTouchOneByOne* listener;

	Ball* m_upBall;
	Size  m_winSize;

	Vector<Ref*> m_snagVec;
	std::map<int, Vector<Ref*>> m_cellMap;

	Node* m_devil;

	float m_randSpeed;
	float m_upBallAngle;
	float m_winX;

	b2Body* m_removeb;

	bool m_isBallGoingUp;

	float m_cellside;
	ParticleSystem*    m_emitter;
};
#endif // __SNAGFOREST_LAYER_H__