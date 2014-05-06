#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

class Entity: public cocos2d::Node
{
public:	
	Entity();	
	~Entity();
	Sprite* getSprite();		/*get sprite object*/
	void setSpriteAsNULL();		/*set sprite object*/
	void bindSprite(Sprite* sprite);		/*bind sprite object*/
private:
	Sprite* m_sprite;
};
#endif // __ENTITY_H__