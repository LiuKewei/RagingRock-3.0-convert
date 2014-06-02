#ifndef __MACRO_DEFINITION_H__
#define __MACRO_DEFINITION_H__

//SnagForestLayer.cpp
#define BALL_LAUNCH_ROTATION (20)
#define BALL_LAUNCH_SPEED (10)


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

#endif // __MACRO_DEFINITION_H__

