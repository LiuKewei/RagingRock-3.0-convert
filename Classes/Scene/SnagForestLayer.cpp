#include "SnagForestLayer.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "PolySprite.h"
#else
#include "Tools/PolySprite.h"
#endif
USING_NS_CC;
USING_NS_CC_EXT;

SnagForestLayer::SnagForestLayer()
	: m_ballAngle(-1.0)
	, m_ball(NULL)
	, m_arrow(NULL)
	, m_isBallGoingUp(true)
	, m_devil(NULL)
	, m_emitter(NULL)
	, m_listener(NULL)
	, m_resLoadingProgresser(NULL)
	, m_loadingVar(1.0f)
	, m_cellside(0.0f)
	, m_loadBg(NULL)
{
}

SnagForestLayer::~SnagForestLayer()
{
	_eventDispatcher->removeEventListener(m_listener);
	m_cellMap.clear();
}

void SnagForestLayer::setPhyWorld(PhysicsWorld* world)
{
	this->m_physicsWorld = world;
}

bool SnagForestLayer::initWithEntryID(int entryId)
{
	//log("updateProgresser    %d", time(NULL));
	initResourcesWithProgresser();
	interactionSubscribe();
	this->scheduleUpdate();

	//add listener for sreen touch
	m_listener = EventListenerTouchOneByOne::create();
	m_listener->setSwallowTouches(true);
	m_listener->onTouchBegan = CC_CALLBACK_2(SnagForestLayer::TouchBegan, this);
	m_listener->onTouchMoved = CC_CALLBACK_2(SnagForestLayer::TouchMoved, this);
	m_listener->onTouchEnded = CC_CALLBACK_2(SnagForestLayer::TouchEnded, this);
	m_listener->setSwallowTouches(true);
	m_listener->setEnabled(false);
	_eventDispatcher->addEventListenerWithFixedPriority(m_listener, -1);




	return true;
}

void SnagForestLayer::update(float dt)
{
	if (m_ball != NULL && m_ball->getPositionY() <= (m_ball->getBallSize().height / 2 + c_ballHeightStop))
	{
		this->removeChild(m_ball);
		m_ball = NULL;
		m_isBallGoingUp = true;
		if (!m_listener->isEnabled())
		{
			m_listener->setEnabled(true);
		}
		m_emitter->setVisible(false);
	}
	else if (m_ball != NULL && !m_isBallGoingUp)
	{
		routeDetection();
	}
}

void SnagForestLayer::updateProgresser(float dt)
{
	float loadingVar = m_resLoadingProgresser->getPercentage();
	if (m_resLoadingProgresser != NULL && loadingVar < 100.0f)
	{
		m_resLoadingProgresser->setPercentage(loadingVar + 1.0f);
	}
	else if (loadingVar >= 100.0f)
	{
		this->unschedule(schedule_selector(SnagForestLayer::updateProgresser));
		//m_resLoadingProgresser->setPercentage(100.0f);

		this->removeChild(m_resLoadingProgresser);
		this->removeChild(m_loadBg);
		if (m_listener != NULL && !m_listener->isEnabled() )
		{
			m_listener->setEnabled(true);
		}
	}
}

void SnagForestLayer::ballLauncherMoving(float dt)
{
	if (m_ball != NULL && !m_listener->isEnabled())
	{
		Point expect;
		if (m_isBallGoingUp)
		{
			expect = Point(BALL_LAUNCH_SPEED*tan(CC_DEGREES_TO_RADIANS(m_ballAngle)), BALL_LAUNCH_SPEED);
			m_isBallGoingUp = m_ball->getPositionY() < (m_winSize.height - m_ball->getBallSize().height / 2) ? true : false;
		}
		else
		{
			expect = Point(BALL_LAUNCH_SPEED*tan(CC_DEGREES_TO_RADIANS(m_ballAngle)), -BALL_LAUNCH_SPEED);
		}
		m_ball->setPosition(m_ball->getPosition() + expect);
		if (!m_isBallGoingUp && m_ball->getPositionY() < m_winSize.height - c_ballHeightStart)
		{
			createFallBall();
			this->unschedule(schedule_selector(SnagForestLayer::ballLauncherMoving));
		}
	}
}

bool SnagForestLayer::TouchBegan(Touch* touch, Event* event)
{
	schedule(schedule_selector(SnagForestLayer::ballLauncherMoving));
	if (m_ball != NULL)
	{
		this->removeChild(m_ball);
		m_ball = NULL;
	}

	m_ball = Ball::create();
	m_ball->bindSprite(Sprite::createWithSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("ball.png")
		));
	m_ball->setBallSize(m_ball->getSprite()->getContentSize());
	auto body = PhysicsBody::createCircle(m_ball->getBallSize().width / 2);
	body->setDynamic(false);
	m_ball->setPhysicsBody(body);
	CC_ASSERT(m_arrow != NULL);
	m_ball->setPosition(m_arrow->getPosition());

	this->addChild(m_ball, Z_ORDER_MAX);
	m_arrow->setVisible(true);
	return true;

}
void SnagForestLayer::TouchMoved(Touch* touch, Event* event)
{
	Point start = touch->getStartLocation();

	Point afterStart = touch->getLocation();

	if (afterStart.y - start.y < m_winSize.height && afterStart.x - start.x != 0)
	{
		m_ballAngle = atanf((afterStart.x - start.x) / abs(afterStart.y - m_ball->getPositionY()));
		float upBallRotation = (float)CC_RADIANS_TO_DEGREES(m_ballAngle);
		if (upBallRotation < 0)
		{
			upBallRotation = upBallRotation < -BALL_LAUNCH_ROTATION ? -BALL_LAUNCH_ROTATION : upBallRotation;
		}
		else if (upBallRotation > 0)
		{
			upBallRotation = upBallRotation > BALL_LAUNCH_ROTATION ? BALL_LAUNCH_ROTATION : upBallRotation;
		}
		m_ballAngle = upBallRotation;
		m_ball->setRotation(m_ballAngle);
		m_arrow->setRotation(m_ballAngle);
	}

}
void SnagForestLayer::TouchEnded(Touch* touch, Event* event)
{
	m_ballAngle = m_ball->getRotation();
	m_listener->setEnabled(false);
}


/*private function*/

/* === Initial Scene ===*/
void SnagForestLayer::initResourcesWithProgresser()
{
	m_winSize = Director::getInstance()->getWinSize();
	m_cellside = sqrt((m_winSize.width / 6)*(m_winSize.width / 6) / 2);
	m_winX = m_winSize.width - c_radius * 2;
	initProgresser();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SnagForestScene.plist", "SnagForestScene.png");


	/*initialize stuff for this Scene*/

	initMap();
	initSnags();
	initSlots();
	initCell();
	initBallLauncher();
	initArrow();
	createParticleFire();
}

void SnagForestLayer::initProgresser()
{
	Point p = Point(m_winSize.width / 2, m_winSize.height / 4 + 50);
	m_loadBg = Sprite::create("slider_bar.png");//进程条的底图  
	//m_loadBg->setContentSize(Size(m_winSize.width, m_winSize.height));
	m_loadBg->setScale(300);
	m_loadBg->setPosition(p);
	this->addChild(m_loadBg, Z_ORDER_MAX + 1);

	m_resLoadingProgresser = ProgressTimer::create(Sprite::create("silder_progressBar.png"));
	CC_ASSERT(m_resLoadingProgresser != NULL);
	m_resLoadingProgresser->setType(ProgressTimer::Type::BAR);
	m_resLoadingProgresser->setPosition(p);
	m_resLoadingProgresser->setMidpoint(Point(0, 0));
	m_resLoadingProgresser->setBarChangeRate(Point(1, 0));
	m_resLoadingProgresser->setPercentage(0);
	this->addChild(m_resLoadingProgresser, Z_ORDER_MAX+1);
	this->schedule(schedule_selector(SnagForestLayer::updateProgresser));
}

void SnagForestLayer::initMap()
{
	auto edgeBody = PhysicsBody::createEdgeBox(this->m_winSize, PHYSICSBODY_MATERIAL_DEFAULT, 1);
	auto bg_Sprite = Sprite::createWithSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("SnagForestScene.jpg")
		);
	bg_Sprite->setPosition(Point(m_winSize.width / 2, m_winSize.height / 2));
	bg_Sprite->setPhysicsBody(edgeBody);
	this->addChild(bg_Sprite, Z_ORDER_ZERO);
}

void SnagForestLayer::initBallLauncher()
{

}

void SnagForestLayer::initSnags()
{
	auto snags = SpriteBatchNode::createWithTexture(SpriteFrameCache::getInstance()->getSpriteFrameByName("snag.png")->getTexture());
	this->addChild(snags, Z_ORDER_FOUR);
	for (int i = 0; i < 7; ++i)
	{
		for (int j = 0; j < 13; ++j)
		{
			auto snag = Sprite::createWithSpriteFrame(
				SpriteFrameCache::getInstance()->getSpriteFrameByName("snag.png")
				);
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
		}
	}
}

void SnagForestLayer::initCell()
{
	for (int j = 0; j < 11; ++j)
	{
		Vector<Ref*> cellVec;
		for (int i = 0; i < 7; ++i)
		{
			auto cell = Sprite::createWithSpriteFrame(
				SpriteFrame::createWithTexture(
				SpriteFrameCache::getInstance()->getSpriteFrameByName("SnagForestScene.jpg")->getTexture(), 
				Rect(200, 150, m_cellside, m_cellside)
				)
				);
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
	auto slots = SpriteBatchNode::createWithTexture(SpriteFrameCache::getInstance()->getSpriteFrameByName("slot.png")->getTexture());
	this->addChild(slots, Z_ORDER_MAX);
	for (int i = 0; i < 8; ++i)
	{
		auto slot = Sprite::createWithSpriteFrame(
			SpriteFrameCache::getInstance()->getSpriteFrameByName("slot.png")
			);
		auto body = PhysicsBody::createBox(slot->getContentSize());
		body->setDynamic(false);
		slot->setPhysicsBody(body);

		//Node warning : This node has a physics body, the anchor must be in the middle, you cann't change this to other value.
		//slot->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);

		slot->setPosition(Point(m_winX / 9 * (i + 1) + slot->getContentSize().width / 2, slot->getContentSize().height / 2));

		slots->addChild(slot);
	}

}

void SnagForestLayer::initArrow()
{
	m_arrow = Sprite::createWithSpriteFrame(
		SpriteFrameCache::getInstance()->getSpriteFrameByName("slot.png")
		);
	m_arrow->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	m_arrow->setPosition(Point(m_winSize.width / 2 - CCRANDOM_0_1(), c_ballHeightBegin));
	m_arrow->setVisible(false);
	this->addChild(m_arrow, Z_ORDER_MAX-1);
}


/* === Ball Action ===*/
void SnagForestLayer::createFallBall()
{
	m_ball->getPhysicsBody()->setDynamic(true);

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
	//bd1.position.Set(m_ball->getPositionX()/PT_RATIO, m_ball->getPositionY()/PT_RATIO);
	//bd1.userData = m_ball;

	//b2Body* body = m_box2dWorld->m_world->CreateBody(&bd1);

	//fd1.restitution = restitution;
	//body->CreateFixture(&fd1);
}

void SnagForestLayer::routeDetection()
{
	if (m_ball->getPositionY() > c_snagHeightStart - (m_winX / 6) && m_ball->getPositionY() <= c_snagHeightStart)
	{
		showCells(0);
	}
	else if (m_ball->getPositionY() > c_snagHeightStart - (m_winX / 6 * 1.5) && m_ball->getPositionY() <= c_snagHeightStart - (m_winX / 6))
	{
		showCells(1);
	}
	else if (m_ball->getPositionY() > c_snagHeightStart - (m_winX / 6 * 2) && m_ball->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 1.5))
	{
		showCells(2);
	}
	else if (m_ball->getPositionY() > c_snagHeightStart - (m_winX / 6 * 2.5) && m_ball->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 2))
	{
		showCells(3);
	}
	else if (m_ball->getPositionY() > c_snagHeightStart - (m_winX / 6 * 3) && m_ball->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 2.5))
	{
		showCells(4);
	}
	else if (m_ball->getPositionY() > c_snagHeightStart - (m_winX / 6 * 3.5) && m_ball->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 3))
	{
		showCells(5);
	}
	else if (m_ball->getPositionY() > c_snagHeightStart - (m_winX / 6 * 4) && m_ball->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 3.5))
	{
		showCells(6);
	}
	else if (m_ball->getPositionY() > c_snagHeightStart - (m_winX / 6 * 4.5) && m_ball->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 4))
	{
		showCells(7);
	}
	else if (m_ball->getPositionY() > c_snagHeightStart - (m_winX / 6 * 5) && m_ball->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 4.5))
	{
		showCells(8);
	}
	else if (m_ball->getPositionY() > c_snagHeightStart - (m_winX / 6 * 5.5) && m_ball->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 5))
	{
		showCells(9);
	}
	else if (m_ball->getPositionY() > c_snagHeightStart - (m_winX / 6 * 6) && m_ball->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 5.5))
	{
		showCells(10);
	}
	//else if (m_ball->getPositionY() > c_snagHeightStart - (m_winX / 6 * 6.5) && m_ball->getPositionY() <= c_snagHeightStart - (m_winX / 6 * 6))
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
		if (isCollidedWithBall(m_ball, cell))
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