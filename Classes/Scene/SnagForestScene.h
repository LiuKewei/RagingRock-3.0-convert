#ifndef __SNAGFOREST_SCENE_H__
#define __SNAGFOREST_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SnagForestLayer.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "DevilLayer.h"
#include "BalloonLayer.h"
#else
#include "CommLayer/Devil/DevilLayer.h"
#include "CommLayer/Balloon/BalloonLayer.h"
#endif


#define CREATE_FUNC_PHY(__TYPE__) \
static __TYPE__* createWithPhysics() \
{ \
__TYPE__ *pRet = new __TYPE__(); \
if (pRet && pRet->initWithPhysics()) \
{ \
pRet->autorelease(); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = NULL; \
return NULL; \
} \
}

class SnagForestScene: public cocos2d::Scene
{
public:
	SnagForestScene();
	virtual ~SnagForestScene(void);

	virtual bool initWithPhysics();
	CREATE_FUNC_PHY(SnagForestScene);

	CC_SYNTHESIZE(SnagForestLayer*, _snagForestLayer, SnagForestLayer);
	CC_SYNTHESIZE(DevilLayer*, _devilLayer, DevilLayer);
	CC_SYNTHESIZE(BalloonLayer*, _balloonLayer, BalloonLayer);
};

#endif // __SNAGFOREST_SCENE_H__