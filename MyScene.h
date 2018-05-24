#ifndef _MYSCENE_H__
#define _MYSCENE_H__
#include "cocos2d.h"

class MyScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuHellowWorldScene(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(MyScene);
};

#endif // _MYSCENE_H_
