#include "BrickLayer.h"

BrickLayer::BrickLayer()
{
}
BrickLayer::~BrickLayer()
{
}
bool BrickLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		m_winSize = Director::getInstance()->getWinSize();
		m_brickBase = Layer::create();
		m_brickBase->setContentSize(Size(640, 960));
		//m_brickBase->setVisible(false);
		this->addChild(m_brickBase);

		m_listener = EventListenerTouchOneByOne::create();
		m_listener->onTouchBegan = CC_CALLBACK_2(BrickLayer::TouchBegan, this);
		m_listener->setSwallowTouches(true);
		m_listener->retain();// 必须保留，要不然出了BrickLayer::init()方法，m_listener就会被释放然后变成不可访问的地址。

		NotificationCenter::getInstance()->addObserver(
			this,
			callfuncO_selector(BrickLayer::brickGameStart),
			MsgTypeForObserver::c_BrickStart,
			NULL);

		auto brick = Brick::create();
		brick->bindSprite(Sprite::create("ball.png"));
		brick->setPosition(c_PosLeft);
		brick->setTag(TAG_BRICK);
		m_brickBase->addChild(brick);

		_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener->clone(), brick);

		bRet = true;
	} while (0);

	return bRet;
}


void BrickLayer::brickGameStart(Ref* pData)
{

}

bool BrickLayer::TouchBegan(Touch* touch, Event* event)
{
	auto brick = static_cast<Brick*>(event->getCurrentTarget());
	if (brick->getPosition() == c_PosLeft)
	{
		this->scheduleUpdate();
	}
	
	return false;
}

void BrickLayer::update(float dt)
{
	auto brick = static_cast<Brick*>(m_brickBase->getChildByTag(TAG_BRICK));
	if (brick->getPositionX() >= c_PosEnd.x)
	{
		CCLOG("void BrickLayer::update(float dt)           END");
		this->unscheduleUpdate();
	}
	float x = brick->getPositionX() + 2.0f;
	brick->setPosition(Point(x, left_parabolaY(x)));
	CCLOG("%f , %f", brick->getPositionX(), brick->getPositionY());
}