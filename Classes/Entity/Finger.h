#ifndef __FINGER_H__
#define __FINGER_H__

#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Finger : public Entity
{
public:
	~Finger();
	virtual bool init();
	CREATE_FUNC(Finger);
	static Finger* createWithMaximumPoint(int limit);
	bool initWithMaximumPoint(int limit);

	void drawFunc(const kmMat4 &transform, bool transformUpdated);
	//÷ÿ‘ÿ∏∏¿‡draw
	void draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated);
	
	std::queue<Point>* getRowedPath();


	CC_SYNTHESIZE_READONLY(unsigned int, m_pointLimit, PointLimit);
	CC_SYNTHESIZE_RETAIN(Texture2D*, m_texture, Texture);
	CC_SYNTHESIZE(float, m_width, Width);
	CC_SYNTHESIZE(float, m_deltaRemainder, DeltaRemainder);
	CC_SYNTHESIZE(bool, m_autoDim, AutoDim);
	CC_SYNTHESIZE(bool, m_reset, Reset);
	CC_SYNTHESIZE(bool, m_finish, Finish);
	CC_SYNTHESIZE(bool, m_willPop, WillPop);

protected:

	//Point m_startPoint;
	//Point m_endPoint;

	std::queue<Point>* m_rowedPath;

	Point* m_vertices;
	Point* m_coordinates;

public:
	void populateVertices();
};
#endif // __FINGER_H__