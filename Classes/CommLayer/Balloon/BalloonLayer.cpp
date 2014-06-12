#include "BalloonLayer.h"

BalloonLayer::BalloonLayer()
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
		initLabels();
		m_listener = EventListenerTouchOneByOne::create();
		m_listener->setSwallowTouches(true);
		m_listener->onTouchBegan = CC_CALLBACK_2(BalloonLayer::TouchBegan, this);
		//m_listener->onTouchMoved = CC_CALLBACK_2(BalloonLayer::TouchMoved, this);
		//m_listener->onTouchEnded = CC_CALLBACK_2(BalloonLayer::TouchEnded, this);
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
	auto balloon = (Balloon*)this->getChildByTag(TAG_BALLOON);
	auto balloonStatus = (Balloon*)this->getChildByTag(TAG_BALLOON_STAT);
	balloon->setCounter(0);
	balloon->setMaxCnt(MsgTypeForObserver::getRand(1, 5));
	char tmp[10];
	sprintf(tmp, "Status%d", balloon->getMaxCnt());
	m_bomb->setVisible(false);
	balloon->setVisible(true);
	balloonStatus->setVisible(true);
	m_listener->setEnabled(true);
	balloonStatus->getArmature()->getAnimation()->play(tmp);
	this->unschedule(schedule_selector(BalloonLayer::bombedreset));
}

void BalloonLayer::unbombedreset(float dt)
{
	auto balloon = (Balloon*)this->getChildByTag(TAG_BALLOON);
	if (!balloon->isbombed())
	{
		auto balloonSucc = (Armature*)this->getChildByTag(TAG_BALLOON_SUCC);
		balloonSucc->setVisible(true);
		balloonSucc->getAnimation()->play("A1");
		balloon->setSuccessCnt(balloon->getSuccessCnt() + 1);

		char tmp[10];
		sprintf(tmp, " %d", balloon->getSuccessCnt());
		m_succCntLabel->setString(tmp);

		if (balloon->getSuccessCnt() == 10)
		{
			this->schedule(schedule_selector(BalloonLayer::playSucc));
		}
		else
		{
			balloon->setCounter(0);
			balloon->setMaxCnt(MsgTypeForObserver::getRand(1, 5));
			sprintf(tmp, "Status%d", balloon->getMaxCnt());
			balloon->getArmature()->getAnimation()->play("Animation0");
			auto balloonStatus = (Balloon*)this->getChildByTag(TAG_BALLOON_STAT);
			balloonStatus->getArmature()->getAnimation()->play(tmp);
		}
		this->unschedule(schedule_selector(BalloonLayer::unbombedreset));
	}
}

void BalloonLayer::playSucc(float dt)
{
	auto balloonSucc = (Armature*)this->getChildByTag(TAG_BALLOON_SUCC);
	if (balloonSucc->getAnimation()->isComplete())
	{
		auto balloon = (Balloon*)this->getChildByTag(TAG_BALLOON);
		NotificationCenter::getInstance()->postNotification(MsgTypeForObserver::c_BalloonStop, NULL);
		m_balloonLayout->setVisible(false);
		balloon->setVisible(false);
		this->getChildByTag(TAG_BALLOON_STAT)->setVisible(false);
		m_balloonLabel->setVisible(false);
		m_succCntLabel->setVisible(false);
		m_listener->setEnabled(false);
		this->unschedule(schedule_selector(BalloonLayer::playSucc));
	}
}

bool BalloonLayer::TouchBegan(Touch* touch, Event* event)
{
	auto balloon = (Balloon*)this->getChildByTag(TAG_BALLOON);
	if (balloon->getCounter() < balloon->getMaxCnt())
	{
		char tmp[11];
		sprintf(tmp, "Animation%d", balloon->getCounter() + 1);
		balloon->getArmature()->getAnimation()->play(tmp);
	}
	balloon->setCounter(balloon->getCounter() + 1);

	if (balloon->getCounter() > balloon->getMaxCnt())
	{
		//爆炸,等待1秒,重置气球MaxCnt，counter
		m_listener->setEnabled(false);
		balloon->setbombed(true);
		balloon->setVisible(false);
		m_bomb->setVisible(true);
		balloon->getArmature()->getAnimation()->play("Animation0");
		this->unschedule(schedule_selector(BalloonLayer::unbombedreset));
		this->schedule(schedule_selector(BalloonLayer::bombedreset), 1.0);
		return false;
	}
	else if (balloon->getCounter() == balloon->getMaxCnt())
	{
		// 等待1秒，重置气球MaxCnt,counter
		balloon->setbombed(false);
		this->schedule(schedule_selector(BalloonLayer::unbombedreset), 1.0);
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
	Point p = Point(m_winSize.width / 2, m_winSize.height / 2);

	m_balloonLayout = GUIReader::getInstance()->widgetFromJsonFile("Balloon_1.ExportJson");
	m_balloonLayout->setVisible(false);
	this->addChild(m_balloonLayout);

	ArmatureDataManager::getInstance()->addArmatureFileInfo("BalloonAnimation.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("BalloonSuccAnimation.ExportJson");

	auto balloon = Balloon::create();
	balloon->bindArmature(Armature::create("BalloonAnimation"));
	balloon->setPosition(Point(243, 614));
	balloon->setVisible(false);
	balloon->setMaxCnt(MsgTypeForObserver::getRand(1, 5));
	balloon->setCounter(0);
	balloon->setbombed(false);
	balloon->setSuccessCnt(0);
	balloon->setTag(TAG_BALLOON);
	this->addChild(balloon);


	auto balloonStatus = Balloon::create();
	balloonStatus->bindArmature(Armature::create("BalloonAnimation"));
	balloonStatus->setPosition(Point(243, 614));
	balloonStatus->setVisible(false);
	balloonStatus->setMaxCnt(balloon->getMaxCnt());
	char tmp[10];
	sprintf(tmp, "Status%d", balloon->getMaxCnt());
	balloonStatus->getArmature()->getAnimation()->play(tmp);
	balloonStatus->setCounter(0);
	balloonStatus->setbombed(false);
	balloonStatus->setSuccessCnt(0);
	balloonStatus->setTag(TAG_BALLOON_STAT);
	this->addChild(balloonStatus);

	auto balloonSucc = Armature::create("BalloonSuccAnimation");
	balloonSucc->setPosition(Point(480, 480));
	balloonSucc->setVisible(false);
	balloonSucc->setTag(TAG_BALLOON_SUCC);
	this->addChild(balloonSucc);
}

void BalloonLayer::initLabels()
{
	TTFConfig config2("Marker Felt.ttf", 30, GlyphCollection::DYNAMIC, nullptr, true);
	m_balloonLabel = Label::createWithTTF(config2, "Balloon Success Count : ", TextHAlignment::LEFT);//创建显示 气球: 的label
	m_balloonLabel->setPosition(Point(m_winSize.width / 2 - 30, m_winSize.height / 2 + 400));
	m_balloonLabel->setVisible(false);
	this->addChild(m_balloonLabel, 1);


	m_succCntLabel = Label::createWithTTF(config2, "", TextHAlignment::LEFT);//创建显示 气球次数 的label
	m_succCntLabel->setPosition(Point(m_winSize.width / 2 + 100, m_winSize.height / 2 + 400));
	m_succCntLabel->setVisible(false);
	this->addChild(m_succCntLabel, 1);


	m_bomb = Label::createWithTTF(config2, "Bomb !!", TextHAlignment::LEFT);//创建显示 爆炸 的label
	m_bomb->setPosition(Point(m_winSize.width / 2, m_winSize.height / 2));
	m_bomb->setVisible(false);
	this->addChild(m_bomb, 1);
}

void BalloonLayer::balloonGameStart(Ref* pData)
{
	auto balloon = (Balloon*)this->getChildByTag(TAG_BALLOON);
	balloon->setVisible(true);
	balloon->setSuccessCnt(0);
	balloon->setCounter(0);
	balloon->setMaxCnt(MsgTypeForObserver::getRand(1, 5));
	char tmp[10];
	sprintf(tmp, "Status%d", balloon->getMaxCnt());
	balloon->getArmature()->getAnimation()->play("Animation0");

	auto balloonStatus = (Balloon*)this->getChildByTag(TAG_BALLOON_STAT);
	balloonStatus->getArmature()->getAnimation()->play(tmp);
	balloonStatus->setVisible(true);

	this->getChildByTag(TAG_BALLOON_SUCC)->setVisible(true);

	m_listener->setEnabled(true);
	m_balloonLayout->setVisible(true);
	m_balloonLabel->setVisible(true);
	m_succCntLabel->setString(" 0");
	m_succCntLabel->setVisible(true);
}