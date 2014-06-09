#ifndef __REVERSIBLE_CARD_H__
#define __REVERSIBLE_CARD_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

#define kInAngleZ (270) //里面卡牌的起始Z轴角度
#define kInDeltaZ (90) //里面卡牌旋转的Z轴角度差

#define kOutAngleZ (0) //封面卡牌的起始Z轴角度
#define kOutDeltaZ (90) //封面卡牌旋转的Z轴角度差

enum {
	tag_inCard = 1,
	tag_outCard
};


class ReversibleCard: public cocos2d::Sprite
{
public:
	ReversibleCard();
	virtual ~ReversibleCard();

	static ReversibleCard* create(const char* inCardImageName, const char* outCardImageName, float duration);
	virtual bool init(const char* inCardImageName, const char* outCardImageName, float duration);

	void openCard();
	void openCard(float delay);
	void verticalTilt(float deltaY);

	//void waitForOpened(float dt);

	void setReversibleCardSize(const Size& size);
	const Size& getReversibleCardSize();
	bool isOpened();

private:
	void openCardFinished();

private:
	bool m_isOpened;
	float m_duration;

	//ActionInterval* m_openAnimIn;
	//ActionInterval* m_openAnimOut;
	Size m_reversibleCardSize;

	void initData(const char* inCardImageName, const char* outCardImageName, float duration);
};

#endif // __REVERSIBLE_CARD_H__