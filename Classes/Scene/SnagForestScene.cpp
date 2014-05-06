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


bool SnagForestScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Scene::init());
		_snagForestLayer = new SnagForestLayer();
		_snagForestLayer->initWithEntryID(0);
		_snagForestLayer->autorelease();
		this->addChild(_snagForestLayer);

		//_devilLayer = DevilLayer::create();
		//this->addChild(_devilLayer,1);

		bRet = true;
	} while (0);

	return bRet;
}