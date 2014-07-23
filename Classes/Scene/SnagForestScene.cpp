#include "SnagForestScene.h"

USING_NS_CC;
USING_NS_CC_EXT;
//using namespace ui;

SnagForestScene::SnagForestScene()
{
}

SnagForestScene::~SnagForestScene()
{

}


bool SnagForestScene::initWithPhysics()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Scene::initWithPhysics());

		_snagForestLayer = new SnagForestLayer();
		_snagForestLayer->autorelease();
		_snagForestLayer->setPhyWorld(this->getPhysicsWorld());
		_snagForestLayer->initWithEntryID(0);
		this->addChild(_snagForestLayer);

		_devilLayer = DevilLayer::create();
		this->addChild(_devilLayer,1);

		_balloonLayer = BalloonLayer::create();
		this->addChild(_balloonLayer, 2);

		_battleLayer = BattleLayer::create();
		this->addChild(_battleLayer, 3);

		_brickLayer = BrickLayer::create();
		this->addChild(_brickLayer, 4);


		_exitLayer = ExitLayer::create();
		this->addChild(_exitLayer, Z_ORDER_MAX);

		bRet = true;
	} while (0);

	return bRet;
}