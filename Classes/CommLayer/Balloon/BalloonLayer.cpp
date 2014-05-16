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
		m_listener->setSwallowTouches(false);
		m_listener->setEnabled(false);
		_eventDispatcher->addEventListenerWithFixedPriority(m_listener, -1);



		NotificationCenter::getInstance()->addObserver(
			this,
			callfuncO_selector(BalloonLayer::balloonGameStart),
			MsgTypeForObserver::c_BalloonStart,
			NULL);


		



		bRet = true;
	} while (0);

	return bRet;
}

void BalloonLayer::bombedreset(float dt)
{
	m_balloon->setCounter(0);
	m_balloon->setMaxCnt(MsgTypeForObserver::getRand(1, 5));
	CCLOG("m_balloon->getMaxCnt()    %d", m_balloon->getMaxCnt() );
	m_listener->setEnabled(true);
	m_balloon->setVisible(true);
	this->unschedule(schedule_selector(BalloonLayer::bombedreset));
}

void BalloonLayer::unbombedreset(float dt)
{
	if (!m_balloon->isbombed())
	{
		m_balloon->setSuccessCnt(m_balloon->getSuccessCnt() + 1);
		m_balloon->setCounter(0);
		m_balloon->setMaxCnt(MsgTypeForObserver::getRand(1, 5));
	CCLOG("m_balloon->getMaxCnt()    %d", m_balloon->getMaxCnt() );
		m_balloon->setScale(1.0f);
		this->unschedule(schedule_selector(BalloonLayer::unbombedreset));
	}
}

bool BalloonLayer::TouchBegan(Touch* touch, Event* event)
{
	if (m_balloon->getCounter() <= m_balloon->getMaxCnt())
	{
		m_balloon->setScale(m_balloon->getScale()+0.4f);
		m_balloon->setCounter(m_balloon->getCounter() + 1);
	}

	CCLOG("m_balloon->getCounter()    %d", m_balloon->getCounter() );

	if (m_balloon->getCounter() > m_balloon->getMaxCnt())
	{
		//±¬Õ¨,µÈ´ı1Ãë,ÖØÖÃÆøÇòMaxCnt£¬counter
		m_listener->setEnabled(false);
		m_balloon->setbombed(true);
		m_balloon->setVisible(false);
		m_balloon->setScale(1.0f);
		this->schedule( schedule_selector(BalloonLayer::bombedreset), 1.0 );
		return false;
	}
	else if (m_balloon->getCounter() == m_balloon->getMaxCnt())
	{
		// µÈ´ı1Ãë£¬ÖØÖÃÆøÇòMaxCnt,counter
		m_balloon->setbombed(false);
		this->schedule( schedule_selector(BalloonLayer::unbombedreset), 1.0 );
		return false;
	}
	return true;
}


void BalloonLayer::TouchMoved(Touch* touch, Event* event)
{

}

void BalloonLayer::TouchEnded(Touch* touch, Event* event)
{

}


void BalloonLayer::initBalloon()
{
	Point p = Point(m_winSize.width / 2, m_winSize.height / 4 + 50);
	m_loadBg = Sprite::create("slider_bar.png");//base map of progress bar
	m_loadBg->setScale(300);
	m_loadBg->setPosition(p);
	m_loadBg->setVisible(false);
	this->addChild(m_loadBg);

	m_balloon = Balloon::create();
	m_balloon->bindSprite(Sprite::create("ball.png"));
	m_balloon->setPosition(Point(m_winSize.width/2, m_winSize.height/2));
	m_balloon->setVisible(false);
	m_balloon->setMaxCnt(MsgTypeForObserver::getRand(1, 5));
	m_balloon->setCounter(0);
	m_balloon->setbombed(false);
	m_balloon->setSuccessCnt(0);
	this->addChild(m_balloon);
}



void BalloonLayer::balloonGameStart(Ref* pData)
{
	m_loadBg->setVisible(true);
	m_balloon->setVisible(true);

	m_listener->setEnabled(true);
}