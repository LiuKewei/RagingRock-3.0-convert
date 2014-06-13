#ifndef __EXIT_LAYER_H__
#define __EXIT_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "MsgTypeForObserver.h"
#else
#include "Tools/MsgTypeForObserver.h"
#endif



class ExitLayer : public cocos2d::Layer
{
public:
	ExitLayer();
	virtual ~ExitLayer();
	virtual bool init();
	CREATE_FUNC(ExitLayer);


private:

	void initExitButton();
	void menuCloseCallback(Ref* pSender);

private:

};
#endif // __EXIT_LAYER_H__