#include "DevilLayer.h"


DevilLayer::DevilLayer()
	: m_devil(NULL)
	, m_fightingVal(0.0f)
	, m_fightingMoved(0.0f)
	, m_winSize(0,0)
	, m_progress(NULL)
	, m_isdevilpos(false)
{
}

DevilLayer::~DevilLayer()
{
	if (m_devil)
	{
		//delete m_devil;
		m_devil = NULL;
	}
	if (m_progress)
	{
		//delete m_progress;
		m_progress = NULL;
	}

	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool DevilLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());
		m_winSize = Director::getInstance()->getWinSize();
		initDevil();

		m_listener = EventListenerTouchOneByOne::create();
		m_listener->setSwallowTouches(true);
		m_listener->onTouchBegan = CC_CALLBACK_2(DevilLayer::TouchBegan, this);
		m_listener->onTouchMoved = CC_CALLBACK_2(DevilLayer::TouchMoved, this);
		m_listener->onTouchEnded = CC_CALLBACK_2(DevilLayer::TouchEnded, this);
		m_listener->setSwallowTouches(true);
		m_listener->setEnabled(true);
		_eventDispatcher->addEventListenerWithFixedPriority(m_listener, -1);

		

		NotificationCenter::getInstance()->addObserver(
			this,
			callfuncO_selector(DevilLayer::devilFighting),
			MsgTypeForObserver::c_DevilFightingStart,
			NULL);

		NotificationCenter::getInstance()->addObserver(
			this,
			callfuncO_selector(DevilLayer::devilPosHandle),
			MsgTypeForObserver::c_DevilPosPush,
			NULL);

		

		bRet = true;
	} while (0);

	return bRet;
}

void DevilLayer::updateDevil(float dt)
{
	unsigned int maxIdx = m_devil->getDevilMaxIndexInCurrent();
	//CCLOG("maxIdx %d", maxIdx);
	//CCLOG("m_devil->getDevilPosVec()->size() %d", m_devil->getDevilPosVec()->size());
	CC_ASSERT(maxIdx <= m_devil->getDevilPosVec()->size());
	int idx = (int)(CCRANDOM_0_1()*(maxIdx + 1));
	--idx;
	m_devil->setPosition(m_devil->getDevilPosVec()->at(idx == -1 ? 0 : idx));
}


bool DevilLayer::TouchBegan(Touch* touch, Event* event)
{
	if (m_progress == NULL)
	{
		return false;
	}
	return true;
}

void DevilLayer::TouchMoved(Touch* touch, Event* event)
{
	Point start = touch->getStartLocation();

	Point afterStart = touch->getLocation();
	if ( abs(afterStart.x - m_fightingMoved) > 10.0f)
	{
		m_fightingVal = 0.1f;
	}
	else
	{
		m_fightingVal = (m_progress !=NULL && (m_progress->getPercentage() <= 80.0f)) ? -0.08f : -0.18f;
		CCLOG("m_fightingVal  %f", m_fightingVal);
	}
	m_fightingMoved = afterStart.x;
}

void DevilLayer::TouchEnded(Touch* touch, Event* event)
{
	m_fightingVal = -0.1f;
}

/*private function*/
void DevilLayer::initDevil()
{
	m_devil = Devil::create();
	m_devil->bindSprite(Sprite::create("devil.png"));
	m_devil->setDevilMaxIndexInCurrent(0);
	m_devil->setDevilPosCnt(0);
	NotificationCenter::getInstance()->postNotification(MsgTypeForObserver::c_DevilPosUpdate, m_devil);
}


void DevilLayer::devilFighting(Ref* pData)
{
	m_devil->setVisible(false);
	auto fightingProgressBg = Sprite::create("slider_bar.png");
	fightingProgressBg->setPosition(Point(150,300));
	this->addChild(fightingProgressBg);

	auto* fightingProgress = Sprite::create("silder_progressBar.png");
	m_progress = ProgressTimer::create(fightingProgress);
	m_progress->setType(ProgressTimer::Type::BAR);
	m_progress->setPosition(Point(150,300));
	m_progress->setMidpoint(Point(0,0));
	m_progress->setBarChangeRate(Point(1,0));
	m_progress->setPercentage(50);
	this->addChild(m_progress);
	this->schedule( schedule_selector(DevilLayer::updateFightingBar) );
}


void DevilLayer::devilPosHandle(Ref* pData)
{
	this->schedule( schedule_selector(DevilLayer::updateDevil), 2.0 );
	this->addChild(m_devil);
	NotificationCenter::getInstance()->removeObserver(this,MsgTypeForObserver::c_DevilPosPush);
}

void DevilLayer::destoryDevilLayer(Ref* pData)
{
	this->removeAllChildren();
}

void DevilLayer::updateFightingBar(float dt)
{
	float proPercet = m_progress->getPercentage();
	if (proPercet == 0.0f)
	{
		this->removeAllChildren();
		m_progress = NULL;
		m_devil = NULL;
		m_listener->setEnabled(false);
		this->unschedule( schedule_selector(DevilLayer::updateDevil) );
		this->unschedule( schedule_selector(DevilLayer::updateFightingBar) );
		NotificationCenter::getInstance()->postNotification(MsgTypeForObserver::c_DevilFightingStop, NULL);
	}
	else
	{
		m_progress->setPercentage(proPercet+m_fightingVal);
	}
}

