#ifndef __EXPPROGRESS_H__
#define __EXPPROGRESS_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Player.h"
#include "ProgressView.h"
USING_NS_CC;

class ExpProgress :public cocos2d::CCLayer
{
public:
	ExpProgress();
	~ExpProgress();
	virtual bool init();
	CREATE_FUNC(ExpProgress);
	void ExpChange(int exp, int limit);
	void LvChange(int level);
	void ButtonAppear(Player* player);
	void ButtonRemove();
	void Choices(Player* player, int i);
private:
	ProgressView * expProgressView;
	Label *lv;
	ui::Button *choice1;
	ui::Button *choice2;
	ui::Button *choice3;
	int c1, c2, c3;
};

#endif