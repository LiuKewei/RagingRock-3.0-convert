#ifndef __DEVIL_LAYER_H__
#define __DEVIL_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "Devil.h"
#include "Finger.h"
#include "MsgTypeForObserver.h"
#else
#include "Entity/Devil.h"
#include "Entity/Finger.h"
#include "Tools/MsgTypeForObserver.h"
#endif


const unsigned int c_fingers_capability = 3;
const unsigned int c_finger_point_limit = 30;


class DevilLayer: public cocos2d::Layer 
{
public:	
	DevilLayer();	
	virtual ~DevilLayer();	
	virtual bool init();
	CREATE_FUNC(DevilLayer);

	void update(float dt);
	void updateDevil(float dt);
	void updateFightingBar(float dt);
	
	// Touches Callback function
	virtual bool TouchBegan(Touch* touch, Event* event);
	virtual void TouchMoved(Touch* touch, Event* event);
	virtual void TouchEnded(Touch* touch, Event* event);


private:
	void initDevil();
	void initFingers();

	void devilFighting(Ref* pData);
	void devilPosHandle(Ref* pData);
	void destoryDevilLayer(Ref* pData);

private:
	Size  m_winSize;
	Devil* m_devil;
	EventListenerTouchOneByOne* m_listener;
	ProgressTimer* m_progress;
	ParticleSystemQuad* m_fingerSparkle;
	Vector<Finger*>* m_fingers;
	Finger* m_finger;


	float m_fightingVal;
	float m_fightingMoved;
	
	bool m_isdevilpos;
};
#endif // __DEVIL_LAYER_H__