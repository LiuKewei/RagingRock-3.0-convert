#include "BrickLayer.h"


const char* c_brickName[6] = {
	"TRIANGLE_RED.png", "TRIANGLE_GREEN.png", "TRIANGLE_YELLOW.png",
	"SQUARE_RED.png", "SQUARE_GREEN.png", "SQUARE_YELLOW.png"
};

BrickLayer::BrickLayer()
	: m_islaunch(false)
{
}
BrickLayer::~BrickLayer()
{
	m_listener->release();
}
bool BrickLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		Point origin = Director::getInstance()->getVisibleOrigin();
		Size size = Director::getInstance()->getVisibleSize();
		CCLOG("getVisibleOrigin ... %f, %f", origin.x,origin.y);
		CCLOG("getVisibleSize ... %f, %f", size.width,size.height);
		m_winSize = Director::getInstance()->getWinSize();
		m_brickBase = Layer::create();
		m_brickBase->setContentSize(m_winSize);
		//m_brickBase->setVisible(false);
		m_brickBase->setPosition(origin);
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


		initGoalBrick();
		brickPutLeft();
		brickPutMid();
		brickPutRight();

		bRet = true;
	} while (0);

	return bRet;
}


void BrickLayer::brickGameStart(Ref* pData)
{

}

bool BrickLayer::TouchBegan(Touch* touch, Event* event)
{
	if (m_islaunch) return false;
	auto brick = static_cast<Brick*>(event->getCurrentTarget());
	Point locationInNode = touch->getLocation();
	Size s = brick->getContentSize();
	Rect rect = Rect(brick->getPositionX() - s.width / 2, brick->getPositionY() - s.height / 2, s.width, s.height);
	if (!rect.containsPoint(locationInNode)) return false;
	m_islaunch = true;

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
		m_islaunch = false;
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
		m_islaunch = false;
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
		m_islaunch = false;
		return;
	}
	float x = brick->getPositionX() - c_brickSpeed;
	brick->setPosition(Point(x, right_parabolaY(x)));
	brick->setScale(brick->getScale() - c_brickScaleSpeed);
}
















// ************  private  ************ //
Brick* BrickLayer::brickCreate()
{
	int shape = MsgTypeForObserver::getRand(SHAPE_TRIANGLE, SHAPE_SQUARE);
	int color = MsgTypeForObserver::getRand(COLOR_RED, COLOR_YELLOW);
	auto brick = Brick::create();
	brick->bindSprite(Sprite::create(c_brickName[c_brickNameIndex[shape][color]]));
	brick->setShape(shape);
	brick->setColor(color);
	brick->setLocalZOrder(Z_ORDER_MAX);
	m_brickBase->addChild(brick);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener->clone(), brick);
	return brick;
}

void BrickLayer::initGoalBrick()
{
	int shape = MsgTypeForObserver::getRand(SHAPE_TRIANGLE, SHAPE_SQUARE);
	int color = MsgTypeForObserver::getRand(COLOR_RED, COLOR_YELLOW);
	auto brick = Brick::create();
	brick->bindSprite(Sprite::create(c_brickName[c_brickNameIndex[shape][color]]));
	brick->setShape(shape);
	brick->setColor(color);
	brick->setLocalZOrder(Z_ORDER_ZERO);
	brick->setPosition(c_PosEnd);
	brick->setTag(TAG_BRICK_CURRENT);
	m_brickBase->addChild(brick);
}

void BrickLayer::brickPutLeft()
{
	auto brick = brickCreate();
	brick->setPosition(c_PosLeft);
	brick->setTag(TAG_BRICK_LEFT);
}
void BrickLayer::brickPutMid()
{
	auto brick = brickCreate();
	brick->setPosition(c_PosMid);
	brick->setTag(TAG_BRICK_MID);
}
void BrickLayer::brickPutRight()
{
	auto brick = brickCreate();
	brick->setPosition(c_PosRight);
	brick->setTag(TAG_BRICK_RIGHT);
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
		CCLOG("SCORE ++++++++++");
	}
	else
	{
		//不加分，结束游戏
		CCLOG("GAME OVER!!!!!!");
	}
	return false;
}