#include "BrickLayer.h"


const int c_brickNameIndex[3][3] = {
	0, 1, 2,
	3, 4, 5,
	6, 7, 8
};

const char* c_brickName[9] = {
	"TRIANGLE_RED.png", "TRIANGLE_GREEN.png", "TRIANGLE_YELLOW.png",
	"SQUARE_RED.png", "SQUARE_GREEN.png", "SQUARE_YELLOW.png",
	"CIRCLE_RED.png", "CIRCLE_GREEN.png", "CIRCLE_YELLOW.png"
};

BrickLayer::BrickLayer()
	: m_islaunch(true)
	, m_isGameOver(false)
{
}
BrickLayer::~BrickLayer()
{
	m_listener->release();
	m_brickBase->release();
}
bool BrickLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		//CCLOG("getVisibleOrigin ... %f, %f", origin.x,origin.y);
		//CCLOG("getVisibleSize ... %f, %f", size.width,size.height);
		m_winSize = Director::getInstance()->getWinSize();
		Point origin = Director::getInstance()->getVisibleOrigin();
		Size size = Director::getInstance()->getVisibleSize();
		m_brickBase = Layer::create();
		m_brickBase->setContentSize(m_winSize);
		m_brickBase->setVisible(false);
		m_brickBase->setPosition(origin);
		m_brickBase->retain();

		m_listener = EventListenerTouchOneByOne::create();
		m_listener->onTouchBegan = CC_CALLBACK_2(BrickLayer::TouchBegan, this);
		m_listener->setSwallowTouches(true);
		// 必须保留，要不然出了BrickLayer::init()方法，或者绑定m_listener的对象被自动释放以后，listener也会跟着被释放，从而导致无法继续绑定新的精灵
		m_listener->retain();

		NotificationCenter::getInstance()->addObserver(
			this,
			callfuncO_selector(BrickLayer::brickGameStart),
			MsgTypeForObserver::c_BrickStart,
			NULL);

		bRet = true;
	} while (0);

	return bRet;
}


void BrickLayer::brickGameStart(Ref* pData)
{
	initBrickBG();

	auto brick = initGoalBrick();
	brickPutLeft(brick, -1);
	brickPutMid();
	brickPutRight(brick, 1);
	m_brickBase->setVisible(true);
}

bool BrickLayer::TouchBegan(Touch* touch, Event* event)
{
	if (m_islaunch) return false;
	auto brick = static_cast<Brick*>(event->getCurrentTarget());
	Point locationInNode = touch->getLocation();
	Size s = brick->getContentSize();
	Rect rectBrick = Rect(brick->getPositionX() - s.width / 2, brick->getPositionY() - s.height / 2, s.width, s.height);
	if (!rectBrick.containsPoint(locationInNode)) return false;
	m_islaunch = true;

	m_timeLabel->setVisible(false);
	this->unschedule(schedule_selector(BrickLayer::timing));

	if (brick->getPosition() == c_PosLeft)
	{
		this->schedule(schedule_selector(BrickLayer::updateLeft));
	}
	else if (brick->getPosition() == c_PosMid)
	{
		this->schedule(schedule_selector(BrickLayer::updateMidTop));
	}
	else if (brick->getPosition() == c_PosRight)
	{
		this->schedule(schedule_selector(BrickLayer::updateRight));
	}

	return false;
}

void BrickLayer::updateLeft(float dt)
{
	auto brick = static_cast<Brick*>(m_brickBase->getChildByTag(TAG_BRICK_LEFT));
	if (brick->getPositionX() >= c_PosEnd.x)
	{
		this->brickGoalJudge(brick);
		this->brickChangeCurrent(TAG_BRICK_LEFT);
		this->brickPutLeft();
		this->unschedule(schedule_selector(BrickLayer::updateLeft));
		this->brickTimingReset();
		return;
	}
	float x = brick->getPositionX() + c_brickSpeed;
	brick->setPosition(Point(x, left_parabolaY(x)));
	brick->setScale(brick->getScale() - c_brickScaleSpeed);
}


void BrickLayer::updateMidTop(float dt)
{
	auto brick = static_cast<Brick*>(m_brickBase->getChildByTag(TAG_BRICK_MID));
	float y = 0.0f;
	if (brick->getPositionY() < c_PosEnd.y)
	{
		y = brick->getPositionY() + 15.0f;
	}
	else
	{
		y = brick->getPositionY() + 7.5f;
	}

	brick->setPosition(Point(c_PosEnd.x, y));
	brick->setScale(brick->getScale() - c_brickScaleSpeed);

	if (brick->getPositionY() >= c_PosMidTop.y)
	{
		this->unschedule(schedule_selector(BrickLayer::updateMidTop));
		this->schedule(schedule_selector(BrickLayer::updateMidEnd));
		return;
	}
}

void BrickLayer::updateMidEnd(float dt)
{
	auto brick = static_cast<Brick*>(m_brickBase->getChildByTag(TAG_BRICK_MID));
	brick->setPosition(Point(c_PosEnd.x, brick->getPositionY() - 6.0f));
	brick->setScale(brick->getScale() - c_brickScaleSpeed);

	if (brick->getPositionY() <= c_PosEnd.y)
	{
		this->brickGoalJudge(brick);
		this->brickChangeCurrent(TAG_BRICK_MID);
		this->brickPutMid();
		this->unschedule(schedule_selector(BrickLayer::updateMidEnd));
		this->brickTimingReset();
		return;
	}
}

void BrickLayer::updateRight(float dt)
{
	auto brick = static_cast<Brick*>(m_brickBase->getChildByTag(TAG_BRICK_RIGHT));
	if (brick->getPositionX() <= c_PosEnd.x)
	{
		this->brickGoalJudge(brick);
		this->brickChangeCurrent(TAG_BRICK_RIGHT);
		this->brickPutRight();
		this->unschedule(schedule_selector(BrickLayer::updateRight));
		this->brickTimingReset();
		return;
	}
	float x = brick->getPositionX() - c_brickSpeed;
	brick->setPosition(Point(x, right_parabolaY(x)));
	brick->setScale(brick->getScale() - c_brickScaleSpeed);
}



void BrickLayer::timing(float dt)
{
	m_timing -= dt;
	if (m_timing < 0)
	{
		this->unschedule(schedule_selector(BrickLayer::timing));
		m_timing = c_brickTiming;
		m_timeLabel->setVisible(false);
		m_islaunch = true;

		if (m_isGameOver)
		{
			m_isGameOver = false;
			NotificationCenter::getInstance()->postNotification(MsgTypeForObserver::c_BrickStop, NULL);
			m_brickBase->removeAllChildren();
			this->removeAllChildren();
			return;
		}
		else
		{
			//计时时间到，自动发射左侧图形
			this->schedule(schedule_selector(BrickLayer::updateLeft));
		}
	}
	sprintf(m_timeLabelstr, "%2.0f", m_timing);
	m_timeLabel->setString(m_timeLabelstr);
}












// ************  private  ************ //
Brick* BrickLayer::brickCreate(Brick* brick, int colorORshape)
{
	int shape = 0;
	int color = 0;
	switch (colorORshape)
	{
	case -1:
		shape = MsgTypeForObserver::getRand(SHAPE_TRIANGLE, SHAPE_CIRCLE);
		color = brick->getColor();
		break;
	case 1:
		shape = brick->getShape();
		color = MsgTypeForObserver::getRand(COLOR_RED, COLOR_YELLOW);
		break;
	default:
		shape = MsgTypeForObserver::getRand(SHAPE_TRIANGLE, SHAPE_CIRCLE);
		color = MsgTypeForObserver::getRand(COLOR_RED, COLOR_YELLOW);
		break;
	}
	auto b = Brick::create();
	b->bindSprite(Sprite::create(c_brickName[c_brickNameIndex[shape][color]]));
	b->setShape(shape);
	b->setColor(color);
	b->setLocalZOrder(Z_ORDER_MAX);
	m_brickBase->addChild(b);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener->clone(), b);
	return b;
}

void BrickLayer::initBrickBG()
{
	this->addChild(m_brickBase);

	m_brickLayout = GUIReader::getInstance()->widgetFromJsonFile("BrickUI_1.ExportJson");
	m_brickBase->addChild(m_brickLayout);

	Sprite* role = Sprite::create("BrickRole.png");
	role->setPosition(Point(325,425));
	role->setScale(0.8f);
	role->setTag(TAG_BRICK);
	m_brickBase->addChild(role);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch* touch, Event* event){
		auto target = static_cast<Label*>(event->getCurrentTarget());
		Point locationInNode = touch->getLocation();
		Size s = target->getContentSize();
		log("Label size %f %f", s.width, s.height);
		log("Label locationInNode %f %f", locationInNode.x, locationInNode.y);
		log("Label position %f %f", target->getPositionX(), target->getPositionY());
		Rect rect = Rect(target->getPositionX() - 250, target->getPositionY() - 30, 500, 60);
		if (!rect.containsPoint(locationInNode)) return false;
		this->brickTimingReset();

		auto scoreLable = static_cast<Label*>(m_brickBase->getChildByTag(TAG_BRICK_SCORE));
		scoreLable->setString("0000");
		_eventDispatcher->removeEventListener(listener);
		return false;
	};
	listener->setSwallowTouches(true);

	auto scoreLabel = Label::createWithBMFont("fonts/futura-48.fnt", "Press To Start");
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, scoreLabel);
	m_brickBase->addChild(scoreLabel);
	scoreLabel->setScale(1.8f);
	scoreLabel->setTag(TAG_BRICK_SCORE);
	scoreLabel->setPosition(c_PosBrickStartBtn);
	m_brickScore = 0;

	TTFConfig config2("Marker Felt.ttf", 30, GlyphCollection::DYNAMIC, nullptr, true);
	m_timeLabel = Label::createWithTTF(config2, "", TextHAlignment::LEFT);//创建显示 倒计时 的label
	m_timeLabel->setPosition(Point(m_winSize.width / 2 + 100, m_winSize.height / 2 + 200));
	m_brickBase->addChild(m_timeLabel, 1);
}

Brick* BrickLayer::initGoalBrick()
{
	int shape = MsgTypeForObserver::getRand(SHAPE_TRIANGLE, SHAPE_CIRCLE);
	int color = MsgTypeForObserver::getRand(COLOR_RED, COLOR_YELLOW);
	auto brick = Brick::create();
	brick->bindSprite(Sprite::create(c_brickName[c_brickNameIndex[shape][color]]));
	brick->setShape(shape);
	brick->setColor(color);
	brick->setLocalZOrder(Z_ORDER_ZERO);
	brick->setPosition(c_PosEnd);
	brick->setTag(TAG_BRICK_CURRENT);
	m_brickBase->addChild(brick);

	return brick;
}

void BrickLayer::brickPutLeft(Brick* brick, int colorORshape)
{
	auto b = brickCreate(brick, colorORshape);
	b->setPosition(c_PosLeft);
	b->setTag(TAG_BRICK_LEFT);
}
void BrickLayer::brickPutMid(Brick* brick, int colorORshape)
{
	auto b = brickCreate(brick, colorORshape);
	b->setPosition(c_PosMid);
	b->setTag(TAG_BRICK_MID);
}
void BrickLayer::brickPutRight(Brick* brick, int colorORshape)
{
	auto b = brickCreate(brick, colorORshape);
	b->setPosition(c_PosRight);
	b->setTag(TAG_BRICK_RIGHT);
}


void BrickLayer::brickChangeCurrent(brickTagEnum eTag)
{
	m_brickBase->removeChildByTag(TAG_BRICK_CURRENT);
	m_brickBase->getChildByTag(eTag)->setTag(TAG_BRICK_CURRENT);
	m_brickBase->getChildByTag(TAG_BRICK_CURRENT)->setLocalZOrder(Z_ORDER_ZERO);
}


bool BrickLayer::brickGoalJudge(Brick* brick)
{
	auto curBrick = static_cast<Brick*>(m_brickBase->getChildByTag(TAG_BRICK_CURRENT));
	if (brick->getShape() == curBrick->getShape() || brick->getColor() == curBrick->getColor())
	{
		//加分
		//CCLOG("SCORE ++++++++++");
		m_brickScore += 50;
		char tmp[5];
		sprintf(tmp, "%04d", m_brickScore);
		auto scoreLable = static_cast<Label*>(m_brickBase->getChildByTag(TAG_BRICK_SCORE));
		scoreLable->setString(tmp);
	}
	else
	{
		//不加分，结束游戏
		//CCLOG("GAME OVER!!!!!!");
		auto scoreLable = static_cast<Label*>(m_brickBase->getChildByTag(TAG_BRICK_SCORE));
		scoreLable->setString("GAME OVER!");

		m_isGameOver = true;

		m_islaunch = true;
		this->unschedule(schedule_selector(BrickLayer::timing));
		m_timeLabel->setVisible(false);
	}
	return false;
}


void BrickLayer::brickTimingReset()
{
	m_timing = c_brickTiming;
	if (m_isGameOver)
	{
		m_islaunch = true;
	}
	else
	{
		m_islaunch = false;
	}
	m_timeLabel->setVisible(true);
	sprintf(m_timeLabelstr, "%2.0f", m_timing);
	m_timeLabel->setString(m_timeLabelstr);
	this->schedule(schedule_selector(BrickLayer::timing), 1.0f);
}