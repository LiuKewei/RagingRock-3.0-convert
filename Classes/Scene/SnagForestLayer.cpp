#include "SnagForestLayer.h"
#include "Tools/PolySprite.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define kAccelerometerFrequency 30
#define FRAMES_BETWEEN_PRESSES_FOR_DOUBLE_CLICK 10


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
	//delete m_box2dWorld;
	m_snagArr->release();
	m_cellDic->release();
}

//bool SnagForestLayer::init()
//{
//	bool bRet = false;
//	do 
//	{
//		CC_BREAK_IF(!CCLayer::init());
//		m_winSize = CCDirector::sharedDirector()->getWinSize();
//		m_ball = Ball::create();
//		m_ball->bindSprite(CCSprite::create("CloseNormal.png"));
//		m_ball->setPosition( ccp( m_winSize.width/2, m_winSize.height-20.0));
//		m_ball->m_ballSize = m_ball->getSprite()->getContentSize();
//		m_ball->m_isMoving = true;
//		this->addChild(m_ball);
//
//
//		schedule( schedule_selector(SnagForestLayer::tick) );
//
//		this->scheduleUpdate();
//		m_entryID = 0;
//		m_gameEntry = g_gameEntries + m_entryID;
//		m_box2dWorld = m_gameEntry->createFcn();
//
//		this->setTouchEnabled(true);
//		this->autorelease();
//		bRet = true;
//	} while (0);
//
//	return bRet;
//}

bool SnagForestLayer::initWithEntryID(int entryId)
{
	m_winSize = Director::getInstance()->getWinSize();
	CCLOG("m_winSize    %f, %f", m_winSize.width, m_winSize.height);
	m_cellside = sqrt( (m_winSize.width/6)*(m_winSize.width/6)/2 );
	m_winX = m_winSize.width -c_radius*2;
	//m_gameEntry = g_gameEntries + entryId;
	//m_box2dWorld = m_gameEntry->createFcn(m_winSize.width, m_winSize.height);

	initMap();
	initBallLauncher();
	initSnags();
	initCell();
	createParticleFire();
	interactionSubscribe();

	setTouchEnabled( true );
	//schedule( schedule_selector(SnagForestLayer::tick) );

	this->scheduleUpdate();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(SnagForestLayer::TouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(SnagForestLayer::TouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(SnagForestLayer::TouchEnded, this);

	_eventDispatcher->addEventListenerWithFixedPriority(listener, -1);

	return true;
}

void SnagForestLayer::update(float dt)
{
	// base on position of body(box2d) to update position of CCSprite
	//for(b2Body* b = m_box2dWorld->m_world->GetBodyList(); b; b = b->GetNext())
	//{
	//	if ((b->GetType() == b2_dynamicBody) && (b->GetUserData() != NULL))
	//	{
	//		Ball* fallBall = (Ball*)b->GetUserData();
	//		float bX = (float)(b->GetPosition().x);
	//		float bY = (float)(b->GetPosition().y);
	//		if (fallBall->getSprite() != NULL)
	//		{
	//			if (fallBall->getPositionY() <= fallBall->getBallSize().height/2+20)
	//			{
	//				this->removeChild(m_upBall);
	//				m_upBall = NULL;
	//				b->SetUserData(NULL);// otherwise  0xC0000005  error
	//				m_removeb = b;   /* this can generate remove missing, if we have more than one ball at the same time in screen.
	//								 the latest one can cover others, use array can solve it.*/

	//				m_emitter->setVisible(false);
	//				m_emitter->setPosition(ccp(-10,-10));
	//			}
	//			else
	//			{
	//				fallBall->setPosition(ccp(bX*PT_RATIO, bY*PT_RATIO));
	//				fallBall->setRotation(-(float)CC_RADIANS_TO_DEGREES(b->GetAngle()));
	//				if (m_devil != NULL && isCollidedWithBall(fallBall, m_devil))
	//				{
	//					triggerDevil();
	//					removeDevil();
	//					CCLOG("triggerDevil");
	//				}
	//				routeDetection(fallBall);
	//			}
	//		}

	//	}
	//}

	//// need use array to avoid remove missing problem.
	//if(m_removeb != NULL)
	//{
	//	m_box2dWorld->m_world->DestroyBody(m_removeb);
	//	m_removeb = NULL;
	//	m_isBallGoingUp = true;
	//	if(!this->isTouchEnabled())
	//	{
	//		this->setTouchEnabled(true);
	//	}
	//	//createParticleFire();
	//}
}

void SnagForestLayer::tick(float dt)
{
	/*m_box2dWorld->Step(&settings);*/
	//if (m_emitter != NULL && !m_emitter->isActive())
	//{
	//	m_emitter->release();
	//	m_emitter = NULL;
	//	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(2.0f, LoginScene::create()));
	//	//CCTransitionSlideInR
	//	//CCTransitionCrossFade
	//}
}

void SnagForestLayer::ballLauncherMoving(float dt)
{
	if (m_upBall != NULL && !this->isTouchEnabled())
	{
		Point expect;
		if ( m_isBallGoingUp )
		{
			expect = Point(BALL_LAUNCH_SPEED*tan(CC_DEGREES_TO_RADIANS(m_upBallAngle)), BALL_LAUNCH_SPEED);
			m_isBallGoingUp = m_upBall->getPositionY() < (m_winSize.height - m_upBall->getBallSize().height/2) ? true : false;
		}
		else
		{
			expect = Point(BALL_LAUNCH_SPEED*tan(CC_DEGREES_TO_RADIANS(m_upBallAngle)), -BALL_LAUNCH_SPEED);
		}
		m_upBall->setPosition(m_upBall->getPosition() + expect);
		if (!m_isBallGoingUp && m_upBall->getPositionY() < m_winSize.height - 64)
		{
			createFallBall();
			this->unschedule( schedule_selector(SnagForestLayer::ballLauncherMoving) );
		}
	}
}



//void SnagForestLayer::registerWithTouchDispatcher()
//{
//	// higher priority than dragging
//	CCDirector* pDirector = CCDirector::sharedDirector();
//	pDirector->getTouchDispatcher()->addTargetedDelegate(this, -10, true);
//}

bool SnagForestLayer::TouchBegan(Touch* touch, Event* event)
{

	schedule( schedule_selector(SnagForestLayer::ballLauncherMoving) );
	if (m_upBall != NULL)
	{
		this->removeChild(m_upBall);
		m_upBall = NULL;
	}

	m_upBall = Ball::create();
	m_upBall->bindSprite(CCSprite::create("ball.png"));
	m_upBall->setBallSize(m_upBall->getSprite()->getContentSize());
	m_upBall->setPosition(Point(m_winSize.width/2+5 - CCRANDOM_0_1(), 184));

	this->addChild(m_upBall,100);

	return true;

}
void SnagForestLayer::TouchMoved(Touch* touch, Event* event)
{
	Point start = touch->getStartLocation();

	Point afterStart = touch->getLocation();

	if (afterStart.y - start.y < m_winSize.height && afterStart.x - start.x != 0 )
	{
		//m_upBall->setAnchorPoint(Point(0.5,0));
		m_upBallAngle = atanf((afterStart.x - start.x)/abs(afterStart.y - m_upBall->getPositionY()));
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
	this->setTouchEnabled(false);
}


/*private function*/

/* === Initial Scene ===*/
void SnagForestLayer::initMap()
{
	auto bg_Sprite = Sprite::create("SnagForestScene.jpg");
	//bg_Sprite->setScale(0.5f);
	bg_Sprite->setPosition( Point( m_winSize.width/2, m_winSize.height/2));
	this->addChild(bg_Sprite);

	/*CCSprite *fg_Sprite = CCSprite::create("SnagForestScene_fg.jpg");
	fg_Sprite->setScaleX(0.475f);
	fg_Sprite->setScaleY(0.48f);
	fg_Sprite->setPosition( Point( m_winSize.width/2, (m_winSize.height+40)/2));
	this->addChild(fg_Sprite,1);*/
}

void SnagForestLayer::initBallLauncher()
{

}

void SnagForestLayer::initSnags()
{
	m_snagArr = Array::create();
	m_snagArr->retain();
	auto snags = SpriteBatchNode::create("snag.png");
	this->addChild(snags,600);

	for (int i = 0; i < 7; ++i)
	{
		for(int j = 0; j < 13; ++j)
		{
			auto snag = Sprite::create("snag.png");
			//snag->setScale(0.6f);
			if (j%2 == 1)
			{
				snag->setPosition(Point(m_winX/6 * i + c_radius, (c_heightStart-(m_winX/6/2)*j)));
			}
			else
			{
				snag->setPosition(Point(m_winX/6/2 + m_winX/6 * i + c_radius, (c_heightStart-(m_winX/6/2)*j)));
			}
			snags->addChild(snag);
			m_snagArr->addObject(snag);
		}
	}
}

void SnagForestLayer::initCell()
{
	m_cellDic = Dictionary::create();
	m_cellDic->retain();
	for(int j = 0; j < 11; ++j)
	{
		Array* cellArr = Array::create();
		for (int i = 0; i < 7; ++i)
		{
			auto cell = Sprite::createWithSpriteFrame(SpriteFrame::create( "SnagForestScene_bg.jpg", CCRectMake(200, 150, m_cellside, m_cellside ) ));
			//cell->setScale(0.5f);
			cell->setRotation(45);
			if (j%2 == 1)
			{
				cell->setPosition(Point(m_winX/6 * i + c_radius, (c_heightStart-(m_winX/6/2)*(j+1))));
			}
			else
			{
				cell->setPosition(Point(m_winX/6/2 + m_winX/6 * i + c_radius, (c_heightStart-(m_winX/6/2)*(j+1))));
			}
			cell->setVisible(false);
			this->addChild(cell, 3);
			cellArr->addObject(cell);
		}
		m_cellDic->setObject(cellArr,j);
	}
}

void SnagForestLayer::initSlots()
{


}


/* === Ball Action ===*/
void SnagForestLayer::createFallBall()
{
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

void SnagForestLayer::routeDetection(Ball* fallBall)
{
	if (fallBall->getPositionY() > c_heightStart-(m_winX/6) && fallBall->getPositionY() <= c_heightStart)
	{
		showCells(fallBall, 0);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*1.5) && fallBall->getPositionY() <= c_heightStart-(m_winX/6))
	{
		showCells(fallBall, 1);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*2) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*1.5))
	{
		showCells(fallBall, 2);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*2.5) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*2))
	{
		showCells(fallBall, 3);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*3) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*2.5))
	{
		showCells(fallBall, 4);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*3.5) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*3))
	{
		showCells(fallBall, 5);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*4) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*3.5))
	{
		showCells(fallBall, 6);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*4.5) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*4))
	{
		showCells(fallBall, 7);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*5) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*4.5))
	{
		showCells(fallBall, 8);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*5.5) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*5))
	{
		showCells(fallBall, 9);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*6) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*5.5))
	{
		showCells(fallBall, 10);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*6.5) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*6))
	{
		showCells(fallBall, 11);
	}

}

void SnagForestLayer::showCells(Ball* fallBall, unsigned int indexOfCellArr)
{
	Object* obj = NULL;
	Sprite* cell = NULL;
	Array* cellArr = (Array*)m_cellDic->objectForKey(indexOfCellArr);
	CCARRAY_FOREACH(cellArr, obj)
	{
		cell = (Sprite *)obj;
		if (isCollidedWithBall(fallBall, cell))
		{
			if (!cell->isVisible())
			{
				auto cellPos = cell->getPosition();
				cell->setVisible(true);
				this->removeChild(cell);

				//Pointoint p[] = {Point(0.2, 0.5), Point(0.6, 0.6), Point(0.7, 0.5), Point(0.6, 0.4)};//纹理坐标百分比
				//int index[] = {0, 1, 2, 0, 3, 2};
				//PolySprite* showcell = PolySprite::create("SnagForestScene_bg.jpg",p,4,index);
				//CCLOG("showcell->getContentSize()    %f, %f", showcell->getContentSize().width, showcell->getContentSize().height);
				//CCLOG("showcell->getAnchorPoint()    %f, %f", showcell->getAnchorPoint().x, showcell->getAnchorPoint().y);
				////showcell->setAnchorPoint(Point(0.2,0.5));
				//showcell->setPosition(Point(0.2*m_winSize.width,0.45*m_winSize.height));
				//
				////showcell->setPosition(Point(p[0].x*m_winSize.width,p[0].y*m_winSize.height));




				Point p[] = { 
					Point((cellPos.x-m_winSize.width/6/2)/m_winSize.width, (m_winSize.height-cellPos.y)/m_winSize.height), 
					Point(cellPos.x/m_winSize.width, (m_winSize.height-cellPos.y+m_winSize.width/6/2)/m_winSize.height),
					Point((cellPos.x+m_winSize.width/6/2)/m_winSize.width, (m_winSize.height-cellPos.y)/m_winSize.height),
					Point(cellPos.x/m_winSize.width, (m_winSize.height-cellPos.y-m_winSize.width/6/2)/m_winSize.height)
				};

				CCLOG("p[0]   %f    %f", p[0].x, p[0].y);
				CCLOG("p[1]   %f    %f", p[1].x, p[1].y);
				CCLOG("p[2]   %f    %f", p[2].x, p[2].y);
				CCLOG("p[3]   %f    %f", p[3].x, p[3].y);

				int index[] = {0, 1, 2, 0, 3, 2};
				auto showcell = PolySprite::create("SnagForestScene_bg.jpg",p,4,index);
				CCLOG("showcell->getAnchorPoint()    %f, %f", showcell->getAnchorPoint().x, showcell->getAnchorPoint().y);

				showcell->setPosition(Point(showcell->getAnchorPoint().x*m_winSize.width, showcell->getAnchorPoint().y*m_winSize.height));


				if (m_emitter != NULL)
				{
					m_emitter->setPosition( cellPos );
					m_emitter->setVisible(true);
				}

				this->addChild(showcell,10);
			}
		}
	}
}

bool SnagForestLayer::isCollidedWithBall(Ball* fallBall, Node *node)
{
	if (fallBall->getSprite() != NULL)
	{
		auto entityRrct = fallBall->getBoundingBox();
		auto nodePos = node->getPosition();
		return entityRrct.containsPoint(nodePos);
	}

	return false;
}


/* === Devil Action ===*/
void SnagForestLayer::handleDevil(Object* pData)
{
	m_devil = (Node*)pData;
	m_devil->retain();
}

void SnagForestLayer::handleDevilStop(Object* pData)
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
	this->addChild(m_emitter,100);
	m_emitter->setTexture( TextureCache::sharedTextureCache()->addImage("fire.png") );
	m_emitter->setEmissionRate(800);
	m_emitter->setSpeed(120);
	m_emitter->setSpeedVar(0);
	m_emitter->setContentSize(m_winSize);
	m_emitter->setLifeVar(0.3);
	m_emitter->setLife(0.2);
	m_emitter->setDuration(-1);
	m_emitter->setStartSize(20.0f);
	m_emitter->setStartSizeVar(50.0f);
	m_emitter->setEndSize(kCCParticleStartSizeEqualToEndSize);
	//m_emitter->setPosition( ccp(m_winSize.width/2, 0) );
	m_emitter->setBlendAdditive(true);
	m_emitter->setVisible(false);
}