#include "BalloonLayer.h"

BalloonLayer::BalloonLayer()
: m_balloon(NULL)
{

}
BalloonLayer::~BalloonLayer()
{

}
bool BalloonLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		m_winSize = Director::getInstance()->getWinSize();
		initBalloon();

		m_listener = EventListenerTouchOneByOne::create();
		m_listener->setSwallowTouches(true);
		m_listener->onTouchBegan = CC_CALLBACK_2(BalloonLayer::TouchBegan, this);
		m_listener->onTouchMoved = CC_CALLBACK_2(BalloonLayer::TouchMoved, this);
		m_listener->onTouchEnded = CC_CALLBACK_2(BalloonLayer::TouchEnded, this);
		m_listener->setSwallowTouches(true);
		m_listener->setEnabled(true);
		_eventDispatcher->addEventListenerWithFixedPriority(m_listener, -1);

		bRet = true;
	} while (0);

	return bRet;
}



bool BalloonLayer::TouchBegan(Touch* touch, Event* event)
{
	return true;
}


void BalloonLayer::TouchMoved(Touch* touch, Event* event)
{

}

void BalloonLayer::TouchEnded(Touch* touch, Event* event)
{
	CCLOG("m_balloon->getScale()  %f", m_balloon->getScale());
	m_balloon->setScale(m_balloon->getScale()+0.1f);
}


void BalloonLayer::initBalloon()
{
	m_balloon = Balloon::create();
	m_balloon->bindSprite(Sprite::create("ball.png"));
	m_balloon->setPosition(Point(m_winSize.width/2, m_winSize.height/2));
	this->addChild(m_balloon);
}