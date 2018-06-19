#ifndef _BulletBase_H_
#define _BulletBase_H_

#include "Entity.h"
#include "Player.h"

class BulletBase :public Entity
{
public:
	BulletBase();
	~BulletBase();

	CREATE_FUNC(BulletBase);
	virtual bool init();

	Player *comefrom;//子弹来源

	void attacking(Player *player, Point pos);
	bool collidePlayer(Player *player);//判断碰撞

	bool exist = false;//子弹存在

	void hide();	Point point;//子弹当前坐标
private:

	Point target;//预定攻击坐标
	float xcs, ycs, rcs;//x y r单位时间改变量
	float flyrange;//已飞行距离

	int atkpower;
	int flyspeed;
	float range;

	void pointChange(float dt);
	void arrow(Player *player, Point pos);
	void ground(Player* player, Point pos);
	void knife(Player* player, Point pos);
};

#endif