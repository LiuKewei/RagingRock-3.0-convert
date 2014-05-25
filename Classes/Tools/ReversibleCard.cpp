#include "ReversibleCard.h"

ReversibleCard::ReversibleCard()
{
}
ReversibleCard::~ReversibleCard()
{
	m_openAnimIn->release();
	m_openAnimOut->release();
}

ReversibleCard* ReversibleCard::create(const char* inCardImageName, const char* outCardImageName, float duration)
{
	ReversibleCard *card = new ReversibleCard();
	if (card && card->init(inCardImageName, outCardImageName, duration))
	{
		card->autorelease();
		return card;
	}
	CC_SAFE_DELETE(card);
	return NULL;
}

bool ReversibleCard::init(const char* inCardImageName, const char* outCardImageName, float duration)
{
	if (!Sprite::init())
	{
		return false;
	}
	initData(inCardImageName, outCardImageName, duration);
	return true;
}

void ReversibleCard::initData(const char* inCardImageName, const char* outCardImageName, float duration)
{
	m_isOpened = false;

	Sprite* inCard = Sprite::create(inCardImageName);
	inCard->setPosition(Point::ZERO);
	inCard->setVisible(false);
	inCard->setTag(tag_inCard);
	addChild(inCard);

	Sprite* outCard = Sprite::create(outCardImageName);
	outCard->setPosition(Point::ZERO);
	outCard->setTag(tag_outCard);
	addChild(outCard);

	m_openAnimIn = (ActionInterval*)Sequence::create(DelayTime::create(duration * .5),
		Show::create(),
		OrbitCamera::create(duration * .5, 1, 0, kInAngleZ, kInDeltaZ, 0, 0),
		NULL);
	m_openAnimIn->retain();

	m_openAnimOut = (ActionInterval *)Sequence::create(OrbitCamera::create(duration * .5, 1, 0, kOutAngleZ, kOutDeltaZ, 0, 0),
		Hide::create(),
		DelayTime::create(duration * .5),
		NULL);
	m_openAnimOut->retain();
}

void ReversibleCard::openCard()
{
    Sprite* inCard = (Sprite*)getChildByTag(tag_inCard);
    Sprite* outCard = (Sprite*)getChildByTag(tag_outCard);
    inCard->runAction(m_openAnimIn);
    outCard->runAction(m_openAnimOut);
}