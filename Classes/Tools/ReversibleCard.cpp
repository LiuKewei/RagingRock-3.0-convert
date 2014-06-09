#include "ReversibleCard.h"

ReversibleCard::ReversibleCard()
{
}
ReversibleCard::~ReversibleCard()
{
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
	this->scheduleUpdate();
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

	this->setReversibleCardSize(outCard->getContentSize());

	this->m_duration = duration;
}

void ReversibleCard::openCard()
{
	m_isOpened = false;
	auto openAnimIn = (ActionInterval*)Sequence::create(DelayTime::create(m_duration * .5),
		Show::create(),
		OrbitCamera::create(m_duration * .5, 1, 0, kInAngleZ, kInDeltaZ, 0, 0),
		NULL);

	auto openAnimOut = (ActionInterval *)Sequence::create(OrbitCamera::create(m_duration * .5, 1, 0, kOutAngleZ, kOutDeltaZ, 0, 0),
		Hide::create(),
		DelayTime::create(m_duration * .5),
		NULL);

	Sprite* inCard = (Sprite*)getChildByTag(tag_inCard);
	Sprite* outCard = (Sprite*)getChildByTag(tag_outCard);
	outCard->runAction(openAnimOut);
	inCard->runAction(openAnimIn);
}

void ReversibleCard::openCard(float delay)
{
	m_isOpened = false;
	auto openAnimIn = (ActionInterval*)Sequence::create(DelayTime::create(delay + m_duration * .5),
		Show::create(),
		OrbitCamera::create(m_duration * .5, 1, 0, kInAngleZ, kInDeltaZ, 0, 0),
		CallFuncN::create(CC_CALLBACK_0(ReversibleCard::openCardFinished,this)),
		NULL);
	auto openAnimOut = (ActionInterval *)Sequence::create(DelayTime::create(delay),
		OrbitCamera::create(m_duration * .5, 1, 0, kOutAngleZ, kOutDeltaZ, 0, 0),
		Hide::create(),
		NULL);

	Sprite* inCard = (Sprite*)getChildByTag(tag_inCard);
	Sprite* outCard = (Sprite*)getChildByTag(tag_outCard);
	outCard->runAction(openAnimOut);
	inCard->runAction(openAnimIn);
}

void ReversibleCard::verticalTilt(float deltaY)
{
	auto action = RotateBy::create(0.001f, Vertex3F(deltaY, 0, 0));
	auto tiltAnimIn = Sequence::create(action, NULL);

	Sprite* outCard = (Sprite*)getChildByTag(tag_outCard);
	outCard->runAction(tiltAnimIn);
}

void ReversibleCard::setReversibleCardSize(const Size& size)
{
	this->m_reversibleCardSize = size;
}

const Size& ReversibleCard::getReversibleCardSize()
{
	return this->m_reversibleCardSize;
}

void ReversibleCard::openCardFinished()
{
	m_isOpened = true;
}

bool ReversibleCard::isOpened()
{
	return this->m_isOpened;
}