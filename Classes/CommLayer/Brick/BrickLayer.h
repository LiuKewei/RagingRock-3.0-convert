#ifndef __BRICK_LAYER_H__
#define __BRICK_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "Brick.h"
#include "MsgTypeForObserver.h"
#else
#include "Entity/Brick.h"
#include "Tools/MsgTypeForObserver.h"
#endif


#define  left_parabolaY(_POS_X_) ( -(_POS_X_)*(_POS_X_)/12.0f + (_POS_X_)*95.0f/2.0f - (18200.0f/3.0f) )
#define right_parabolaY(_POS_X_) ( -(_POS_X_)*(_POS_X_)/12.0f + (_POS_X_)*355.0f/6.0f - 9800.0f )

const Point c_PosLeft = Point(200, 100);
const Point c_PosRight = Point(440, 100);
const Point c_PosEnd = Point(320, 600);


class BrickLayer : public cocos2d::Layer
{
	enum
	{
	};



	enum
	{
		TAG_BRICK = 31250,
	};

public:
	BrickLayer();
	virtual ~BrickLayer();
	virtual bool init();
	CREATE_FUNC(BrickLayer);

	// Touches Callback function
	virtual bool TouchBegan(Touch* touch, Event* event);

	void update(float dt);
private:
	void brickGameStart(Ref* pData);

private:
	Size m_winSize;
	Layer* m_brickBase;
	EventListenerTouchOneByOne* m_listener;


};

#endif // __BRICK_LAYER_H__