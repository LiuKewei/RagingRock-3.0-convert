#ifndef __REVERSIBLE_CARD_H__
#define __REVERSIBLE_CARD_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

#define kInAngleZ (270) //���濨�Ƶ���ʼZ��Ƕ�
#define kInDeltaZ (90) //���濨����ת��Z��ǶȲ�

#define kOutAngleZ (0) //���濨�Ƶ���ʼZ��Ƕ�
#define kOutDeltaZ (90) //���濨����ת��Z��ǶȲ�

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

	void update(float dt);

	// Touches Callback function
	virtual bool TouchBegan(Touch* touch, Event* event);
	virtual void TouchMoved(Touch* touch, Event* event);
	virtual void TouchEnded(Touch* touch, Event* event);

private:
	void openCard();

private:
	bool m_isOpened;
    ActionInterval* m_openAnimIn;
    ActionInterval* m_openAnimOut;

	void initData(const char* inCardImageName, const char* outCardImageName, float duration);
	
	EventListenerTouchOneByOne* m_listener;
};

#endif // __REVERSIBLE_CARD_H__