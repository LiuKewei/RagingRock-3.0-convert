#ifndef __SNAGFOREST_SCENE_H__
#define __SNAGFOREST_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SnagForestLayer.h"
//#include "DevilLayer.h"

class SnagForestScene: public cocos2d::Scene
{
public:
	SnagForestScene();
	virtual ~SnagForestScene(void);

	virtual bool init();
	CREATE_FUNC(SnagForestScene);

	CC_SYNTHESIZE(SnagForestLayer*, _snagForestLayer, SnagForestLayer);
	//CC_SYNTHESIZE(DevilLayer*, _devilLayer, DevilLayer);

};

#endif // __SNAGFOREST_SCENE_H__