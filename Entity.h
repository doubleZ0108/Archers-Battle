#ifndef __Entity_H__
#define __Entity_H__

#include "cocos2d.h"
USING_NS_CC;

class Entity : public Node {
public:
	Entity();
	~Entity();
	Sprite* getSprite();	/* 获取精灵对象 */
	void bindSprite(Sprite* sprite);	/* 绑定精灵对象 */
private:
	Sprite * m_sprite;
};
#endif