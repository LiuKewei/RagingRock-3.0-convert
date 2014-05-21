#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

inline void PointSet(cocos2d::Point *v, float x, float y){
	v->x = x;
	v->y = y;
}

inline float fangle(Point vect){
	if (vect.x == 0.0 && vect.y == 0.0) {
		return 0;
	}

	if (vect.x == 0.0) {
		return vect.y > 0 ? M_PI / 2 : -M_PI / 2;
	}

	if (vect.y == 0.0 && vect.x < 0) {
		return -(float)M_PI;
	}

	float angle = atan(vect.y / vect.x);

	return vect.x < 0 ? angle + M_PI : angle;
}

inline void f1(Point p1, Point p2, float d, Point *o1, Point *o2){
	float l = p1.getDistance(p2);
	float angle = fangle(p2 - p1);
	*o1 = Point(p1.x + l, p1.y + d).rotateByAngle(p1, angle);
	*o2 = Point(p1.x + l, p1.y - d).rotateByAngle(p1, angle);
}

class Entity : public cocos2d::Node
{
public:
	Entity();
	~Entity();
	Sprite* getSprite();		/*get sprite object*/
	void setSpriteAsNULL();		/*set sprite object*/
	void bindSprite(Sprite* sprite);		/*bind sprite object*/

protected:
	CustomCommand m_customCommand;
private:
	Sprite* m_sprite;
};
#endif // __ENTITY_H__