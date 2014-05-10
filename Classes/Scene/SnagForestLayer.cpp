#include "SnagForestLayer.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "PolySprite.h"
#else
#include "Tools/PolySprite.h"
#endif
USING_NS_CC;
USING_NS_CC_EXT;

SnagForestLayer::SnagForestLayer()
: m_randSpeed(1.0)
, m_upBallAngle(-1.0)
, m_upBall(NULL)
, m_isBallGoingUp(true)
, m_removeb(NULL)
, m_devil(NULL)
, m_emitter(NULL)
, m_cellside(0.0f)
{
}

SnagForestLayer::~SnagForestLayer()
{
	_eventDispatcher->removeEventListener(listener);
	m_snagVec.clear();
	m_cellMap.clear();
}

void SnagForestLayer::setPhyWorld(PhysicsWorld* world)
{
	this->m_physicsWorld = world;
}

bool SnagForestLayer::initWithEntryID(int entryId)
{
	m_winSize = Director::getInstance()->getWinSize();
	m_cellside = sqrt((m_winSize.width / 6)*(m_winSize.width / 6) / 2);
	m_winX = m_winSize.width - c_radius * 2;

	/*initialize stuff for this Scene*/
	initMap();
	initSnags();
	initSlots();
	initCell();
	initBallLauncher();
	createParticleFire();
	interactionSubscribe();


	this->scheduleUpdate();


	//add listener for sreen touch
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(SnagForestLayer::TouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(SnagForestLayer::TouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(SnagForestLayer::TouchEnded, this);
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithFixedPriority(listener, -1);

	return true;
}

void SnagForestLayer::update(float dt)
{
	if (m_upBall != NULL && m_upBall->getPositionY() <= (m_upBall->getBallSize().height / 2 + c_ballHeightStop))
	{
		this->removeChild(m_upBall);
		m_upBall = NULL;
		m_isBallGoingUp = true;
		if (!listener->isEnabled())
		{
			listener->setEnabled(true);
		}
		m_emitter->setVisible(false);
	}
	else if (m_upBall != NULL && !m_isBallGoingUp)
	{
		routeDetection();
	}
}

void SnagForestLayer::ballLauncherMoving(float dt)
{
	if (m_upBall != NULL && !listener->isEnabled())
	{
		Point expect;
		if (m_isBallGoingUp)
		{
			expect = Point(BALL_LAUNCH_SPEED*tan(CC_DEGREES_TO_RADIANS(m_upBallAngle)), BALL_LAUNCH_SPEED);
			m_isBallGoingUp = m_upBall->getPositionY() < (m_winSize.height - m_upBall->getBallSize().height / 2) ? true : false;
		}
		else
		{
			expect = Point(BALL_LAUNCH_SPEED*tan(CC_DEGREES_TO_RADIANS(m_upBallAngle)), -BALL_LAUNCH_SPEED);
		}
		m_upBall->setPosition(m_upBall->getPosition() + expect);
		if (!m_isBallGoingUp && m_upBall->getPositionY() < m_winSize.height - c_ballHeightStart)
		{
			createFallBall();
			this->unschedule(schedule_selector(SnagForestLayer::ballLauncherMoving));
		}
	}
}

bool SnagForestLayer::TouchBegan(Touch* touch, Event* event)
{
	schedule(schedule_selector(SnagForestLayer::ballLauncherMoving));
	if (m_upBall != NULL)
	{
		this->removeChild(m_upBall);
		m_upBall = NULL;
	}

	m_upBall = Ball::create();
	m_upBall->bindSprite(CCSprite::create("ball.png"));
	m_upBall->setBallSize(m_upBall->getSprite()->getContentSize());
	auto body = PhysicsBody::createCircle(m_upBall->getBallSize().width / 2);
	body->setDynamic(false);
	m_upBall->setPhysicsBody(body);
	m_upBall->setPosition(Point(m_winSize.width / 2 - CCRANDOM_0_1(), c_ballHeightBegin));

	this->addChild(m_upBall, Z_ORDER_MAX);

	return true;

}
void SnagForestLayer::TouchMoved(Touch* touch, Event* event)
{
	Point start = touch->getStartLocation();

	Point afterStart = touch->getLocation();

	if (afterStart.y - start.y < m_winSize.height && afterStart.x - start.x != 0)
	{
		//m_upBall->setAnchorPoint(Point(0.5,0));
		m_upBallAngle = atanf((afterStart.x - start.x) / abs(afterStart.y - m_upBall->getPositionY()));
		float upBallRotation = (float)CC_RADIANS_TO_DEGREES(m_upBallAngle);
		if (upBallRotation < 0)
		{
			upBallRotation = upBallRotation < -BALL_LAUNCH_ROTATION ? -BALL_LAUNCH_ROTATION : upBallRotation;
		}
		else if (upBallRotation > 0)
		{
			upBallRotation = upBallRotation > BALL_LAUNCH_ROTATION ? BALL_LAUNCH_ROTATION : upBallRotation;
		}
		m_upBallAngle = upBallRotation;
		m_upBall->setRotation(m_upBallAngle);

	}

}
void SnagForestLayer::TouchEnded(Touch* touch, Event* event)
{
	m_upBallAngle = m_upBall->getRotation();
	listener->setEnabled(false);
}


/*private function*/

/* === Initial Scene ===*/
void SnagForestLayer::initMap()
{
	auto edgeBody = PhysicsBody::createEdgeBox(this->m_winSize, PHYSICSBODY_MATERIAL_DEFAULT, 1);
	auto bg_Sprite = Sprite::create("SnagForestScene.jpg");
	bg_Sprite->setPosition(Point(m_winSize.width / 2, m_winSize.height / 2));
	bg_Sprite->setPhysicsBody(edgeBody);
	this->addChild(bg_Sprite, Z_ORDER_ZERO);
}

void SnagForestLayer::initBallLauncher()
{

}

void SnagForestLayer::initSnags()
{
	auto snags = SpriteBatchNode::create("snag.png");
	this->addChild(snags, Z_ORDER_FOUR);
	for (int i = 0; i < 7; ++i)
	{
		for (int j = 0; j < 13; ++j)
		{
			auto snag = Sprite::create("snag.png");
			auto body = PhysicsBody::createCircle(snag->getContentSize().width / 2);
			body->setDynamic(false);
			snag->setPhysicsBody(body);
			if (j % 2 == 1)
			{
				snag->setPosition(Point(m_winX / 6 * i + c_radius, (c_snagHeightStart - (m_winX / 6 / 2)*j)));
			}
			else
			{
				snag->setPosition(Point(m_winX / 6 / 2 + m_winX / 6 * i + c_radius, (c_snagHeightStart - (m_winX / 6 / 2)*j)));
			}
			snags->addChild(snag);
			m_snagVec.pushBack(snag);
		}
	}
}

void SnagForestLayer::initCell()
{
	for (int j = 0; j < 11; ++j)
	{
		//Array* cellArr = Array::create();
		Vector<Ref*> cellVec;
		for (int i = 0; i < 7; ++i)
		{
			auto cell = Sprite::createWithSpriteFrame(SpriteFrame::create("SnagForestScene_bg.jpg", Rect(200, 150, m_cellside, m_cellside)));
			//cell->setScale(0.5f);
			cell->setRotation(45);
			if (j % 2 == 1)
			{
				cell->setPosition(Point(m_winX / 6 * i + c_radius, (c_snagHeightStart - (m_winX / 6 / 2)*(j + 1))));
			}
			else
			{
				cell->setPosition(Point(m_winX / 6 / 2 + m_winX / 6 * i + c_radius, (c_snagHeightStart - (m_winX / 6 / 2)*(j + 1))));
			}
			cell->setVisible(false);
			this->addChild(cell, Z_ORDER_ZERO);
			cellVec.pushBack(cell);
		}
		m_cellMap.insert(std::pair<int, Vector<Ref*>>(j, cellVec));
	}
}

void SnagForestLayer::initSlots()
{
	auto slots = SpriteBatchNode::create("slot.png");
	this->addChild(slots, Z_ORDER_MAX);
	for (int i = 0; i < 8; ++i)
	{
		auto slot = Sprite::create("slot.png");
		auto body = PhysicsBody::createBox(slot->getContentSize());
		body->setDynamic(false);
		slot->setPhysicsBody(body);

		//Node warning : This node has a physics body, the anchor must be in the middle, you cann't change this to other value.
		//slot->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);

		slot->setPosition(Point(m_winX / 9 * (i + 1) + slot->getContentSize().width / 2, slot->getContentSize().height / 2));

		slots->addChild(slot);
	}

}


/* === Ball Action ===*/
void SnagForestLayer::createFallBall()
{
	m_upBall->getPhysicsBody()->setDynamic(true);

	//b2CircleShape shape1;
	//shape1.m_radius = 23.5/PT_RATIO;
	//b2FixtureDef fd1;
	//fd1.shape = &shape1;
	//fd1.density = 100.0f;
	//fd1.friction = 50.0f*CCRANDOM_0_1();
	//float32 restitution = 0.4f*CCRANDOM_0_1();
	//b2BodyDef bd1;
	//bd1.bullet = true;
	//bd1.type = b2_dynamicBody;
	//bd1.position.Set(m_upBall->getPositionX()/PT_RATIO, m_upBall->getPositionY()/PT_RATIO);
	//bd1.userData = m_upBall;

	//b2Body* body = m_box2dWorld->m_world->CreateBody(&bd1);

	//fd1.restitution = restitution;
	//body->CreateFixture(&fd1);
}

void SnagForestLayer::routeDetection()
{
	if (m_upBall->getPositionY() > c_snagHeightStart - (m_winX / 6) && m_upBall->getPositionY() <= c_snagHeightStart)
	{
		showCells(0);
	}
	else if (m_upBall->getPositionY() > c_snagHeightStart - (m_winX / 6 * 1.5) && m_upBall->getPositionY() <= c_snagHeightStart - (m_winX / 6))
	{
		showCells(1);
	}
	else if (m_upBall->getPositionY() > c_snagHeightStart - (m_winX / 6 * 2) && m_upBall->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 1.5))
	{
		showCells(2);
	}
	else if (m_upBall->getPositionY() > c_snagHeightStart - (m_winX / 6 * 2.5) && m_upBall->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 2))
	{
		showCells(3);
	}
	else if (m_upBall->getPositionY() > c_snagHeightStart - (m_winX / 6 * 3) && m_upBall->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 2.5))
	{
		showCells(4);
	}
	else if (m_upBall->getPositionY() > c_snagHeightStart - (m_winX / 6 * 3.5) && m_upBall->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 3))
	{
		showCells(5);
	}
	else if (m_upBall->getPositionY() > c_snagHeightStart - (m_winX / 6 * 4) && m_upBall->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 3.5))
	{
		showCells(6);
	}
	else if (m_upBall->getPositionY() > c_snagHeightStart - (m_winX / 6 * 4.5) && m_upBall->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 4))
	{
		showCells(7);
	}
	else if (m_upBall->getPositionY() > c_snagHeightStart - (m_winX / 6 * 5) && m_upBall->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 4.5))
	{
		showCells(8);
	}
	else if (m_upBall->getPositionY() > c_snagHeightStart - (m_winX / 6 * 5.5) && m_upBall->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 5))
	{
		showCells(9);
	}
	else if (m_upBall->getPositionY() > c_snagHeightStart - (m_winX / 6 * 6) && m_upBall->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 5.5))
	{
		showCells(10);
	}
	//else if (m_upBall->getPositionY() > c_snagHeightStart - (m_winX / 6 * 6.5) && m_upBall->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 6))
	//{
	//	showCells(11);
	//}

}

void SnagForestLayer::showCells(unsigned int indexOfCellArr)
{
	Ref* obj = NULL;
	Sprite* cell = NULL;
	auto cellVec = m_cellMap.at(indexOfCellArr);
	for (auto& e : cellVec)
	{
		cell = (Sprite*)e;
		if (isCollidedWithBall(m_upBall, cell))
		{
			if (!cell->isVisible())
			{
				auto cellPos = cell->getPosition();
				cell->setVisible(true);
				this->removeChild(cell);

				Point p[] = {
					Point((cellPos.x - m_winSize.width / 6 / 2) / m_winSize.width, (m_winSize.height - cellPos.y) / m_winSize.height),
					Point(cellPos.x / m_winSize.width, (m_winSize.height - cellPos.y + m_winSize.width / 6 / 2) / m_winSize.height),
					Point((cellPos.x + m_winSize.width / 6 / 2) / m_winSize.width, (m_winSize.height - cellPos.y) / m_winSize.height),
					Point(cellPos.x / m_winSize.width, (m_winSize.height - cellPos.y - m_winSize.width / 6 / 2) / m_winSize.height)
				};

				int index[] = { 0, 1, 2, 0, 3, 2 };
				auto showcell = PolySprite::create("SnagForestScene_bg.jpg", p, 4, index);
				showcell->setPosition(Point(showcell->getAnchorPoint().x*m_winSize.width, showcell->getAnchorPoint().y*m_winSize.height));

				this->addChild(showcell);

				if (m_emitter != NULL)
				{
					m_emitter->setPosition(cellPos);
					m_emitter->setVisible(true);
				}

			}
		}
	}
}

bool SnagForestLayer::isCollidedWithBall(Ball* ball, Node *node)
{
	if (ball->getSprite() != NULL)
	{
		auto entityRrct = ball->getBoundingBox();
		auto nodePos = node->getPosition();
		return entityRrct.containsPoint(nodePos);
	}

	return false;
}


/* === Devil Action ===*/
void SnagForestLayer::handleDevil(Ref* pData)
{
	m_devil = (Node*)pData;
	m_devil->retain();
}

void SnagForestLayer::handleDevilStop(Ref* pData)
{
	//removeDevil();
	//this->schedule( schedule_selector(SnagForestLayer::tick) );
	//this->scheduleUpdate();
}

void SnagForestLayer::interactionSubscribe()
{
	//CCNotificationCenter::sharedNotificationCenter()->addObserver(
	//	this,
	//	callfuncO_selector(SnagForestLayer::handleDevil),
	//	MsgTypeForObserver::c_DevilPosUpdate,
	//	NULL);

	//CCNotificationCenter::sharedNotificationCenter()->addObserver(
	//	this,
	//	callfuncO_selector(SnagForestLayer::handleDevilStop),
	//	MsgTypeForObserver::c_DevilFightingStop,
	//	NULL);
}

void SnagForestLayer::triggerDevil()
{
	//CCNotificationCenter::sharedNotificationCenter()->postNotification(MsgTypeForObserver::c_DevilFightingStart, NULL);
	//this->setTouchEnabled( false );
	////this->setVisible(false);
	//CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	//this->unschedule( schedule_selector(SnagForestLayer::tick) );
	//this->unscheduleUpdate();
}


bool SnagForestLayer::removeDevil()
{
	if (m_devil != NULL)
	{
		m_devil->release();
		m_devil = NULL;
		return true;
	}
	return false;
}

// Particle
void SnagForestLayer::createParticleFire()
{
	m_emitter = ParticleFire::create();
	m_emitter->retain();
	this->addChild(m_emitter, Z_ORDER_MAX);
	m_emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("fire.png"));
	m_emitter->setEmissionRate(800);
	m_emitter->setSpeed(120);
	m_emitter->setSpeedVar(0);
	m_emitter->setContentSize(m_winSize);
	m_emitter->setLifeVar(0.3f);
	m_emitter->setLife(0.2f);
	m_emitter->setDuration(-1);
	m_emitter->setStartSize(20.0f);
	m_emitter->setStartSizeVar(50.0f);
	m_emitter->setEndSize(ParticleSystem::START_SIZE_EQUAL_TO_END_SIZE);
	//m_emitter->setPosition( ccp(m_winSize.width/2, 0) );
	m_emitter->setBlendAdditive(true);
	m_emitter->setVisible(false);
}
