#ifndef __Player_H__
#define __Player_H__

#include "cocos2d.h"
#include "Entity.h"

using namespace cocos2d;

class Player : public Entity {
public:
	CREATE_FUNC(Player);
	virtual bool init();

	//void run1(Player *player, std::map<EventKeyboard::KeyCode, bool>keys, Player *smallplayer);
	void runway1(std::map<EventKeyboard::KeyCode, bool>keys, Player *smallplayer);
	void runway2(Point point, Player *smallplayer);

	Size visibleSize = CCDirector::getInstance()->getVisibleSize();
	float x_coord = 80.0f;
	float y_coord = 80.0f;

	int level = 1;
	int exp = 0;

	void hpraise(int num);
	bool expraise(int num);
	int explimit();

	void hurt(int atk);
	Sprite *sprite;
	void animationcreate(int direct);

	bool unbeat = 0;

	//技能区
	//人物基本属性
	int speed = 3;
	float radius = 30;
	float p_hp = 50.0;//当前血量
	int hpLimit = 50;//上限血量
	float atkpower = 1.0;
	float atkrange = 1.0;
	float defpower = 1.0;
	//子弹数量
	int front = 1;
	int leftside = 0;
	int rightside = 0;
	int back = 0;
	//武器种类
	int weapon = 1;//1箭2地刺3刀4飞镖
				   //回血、经验增加
	bool hpincrease = 0;
	bool expincrease = 0;

	bool ifcan_breakwall = false;//子弹能否穿墙
	
private:
	bool animating = 0;
	void die();
	void restart();//复活
};

#endif