#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Ball: public Entity
{
public:	
	Ball();	
	~Ball();
	virtual bool init();
	CREATE_FUNC(Ball);

	Rect getBoundingBox(); //obtain collide range


	bool isMoving();
	void setMoving(bool isMoving);
	bool getMovingDirection();
	void setMovingDirection(bool movingDirection);
	float getMovingSpeed();
	void setMovingSpeed(float movingSpeed);
	Size getBallSize();
	void setBallSize(Size ballSize);

protected:
	bool m_isMoving;
	bool m_movingDirection;    /*true means left, false means right*/
	float m_movingSpeed;
	Size m_ballSize;
private:
	//CCSprite* m_preBeCollided;
};
#endif // __BALL_H__