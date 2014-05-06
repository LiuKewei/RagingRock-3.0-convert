#ifndef __SNAGFOREST_LAYER_H__
#define __SNAGFOREST_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"


#include "Entity/Ball.h"
//#include "MsgTypeForObserver.h"

#define BALL_LAUNCH_ROTATION (20)
#define BALL_LAUNCH_SPEED (7)

const unsigned int c_triSnags = 4;
const float c_radius = 10.0f;
const unsigned int c_heightStart = 840;

struct GameEntry;
class Box2dWorld;
class SnagForestLayer: public cocos2d::Layer 
{
public:
	GameEntry*     m_gameEntry;
	Box2dWorld*    m_box2dWorld;
	int            m_entryID;

public:	
	SnagForestLayer();	
	virtual ~SnagForestLayer();	
	//virtual bool init();
	CREATE_FUNC(SnagForestLayer);

	void update(float dt);
	void tick(float dt);
	void ballLauncherMoving(float dt);
	//void draw();//uses for debug

	//virtual void registerWithTouchDispatcher();

	virtual bool TouchBegan(CCTouch* touch, CCEvent* event);
	virtual void TouchMoved(CCTouch* touch, CCEvent* event);
	virtual void TouchEnded(CCTouch* touch, CCEvent* event);

	bool SnagForestLayer::initWithEntryID(int entryId);

private:
	bool isCollidedWithBall(Ball* fallBall, Node *snag);
	void showCells(Ball* fallBall, unsigned int indexOfCellArr);
	void routeDetection(Ball* fallBall);
	void createFallBall();

	void interactionSubscribe();
	void handleDevil(Object* pData);
	void handleDevilStop(Object* pData);

	void triggerDevil();

	void initMap();
	void initBallLauncher();
	void initSnags();
	void initCell();
	void initSlots();

	bool removeDevil();

	void createParticleFire();

private:
	Ball* m_upBall;
	Size  m_winSize;

	Array* m_snagArr;
	Dictionary* m_cellDic;
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