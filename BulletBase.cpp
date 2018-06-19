#include "Bulletbase.h"

BulletBase::BulletBase()
{
}

BulletBase::~BulletBase()
{
}

bool BulletBase::init()
{
	schedule(schedule_selector(BulletBase::pointChange), 0.02);
	return true;
}

void BulletBase::attacking(Player *player, Point pos)
{
	comefrom = player;
	if (comefrom->weapon == 1)
	{
		atkpower = 5;
		flyspeed = 500;
		range = 400;
		arrow(player, pos);
	}
	else if (comefrom->weapon == 2)
	{
		atkpower = 6;
		range = 300;
		ground(player, pos);
	}
	else if (comefrom->weapon == 3)
	{
		atkpower = 15;
		flyspeed = 300;
		range = 50;
		knife(player, pos);
	}
	else if (comefrom->weapon == 4)
	{
		atkpower = 6;
		flyspeed = 700;
		range = 400;
		arrow(player, pos);
	}
}

void BulletBase::arrow(Player *player, Point pos)
{
	range *= comefrom->atkrange;
	float time = (range) / (flyspeed);//单位为秒
	float length = sqrt((player->x_coord - pos.x)*(player->x_coord - pos.x) + (player->y_coord - pos.y)*(player->y_coord - pos.y));
	float xchange = (pos.x - player->x_coord) / length * range;
	float ychange = (pos.y - player->y_coord) / length * range;
	//在cocos2dx中你甚至能学几何

	auto moveBy = MoveBy::create(time, Point(xchange, ychange));
	point.x = player->x_coord;
	point.y = player->y_coord;
	target.x = point.x + xchange;
	target.y = point.y + ychange;

	//	log("start x = %f  y = %f", point.x, point.y);
	//	log("target x = %f  y = %f", target.x, target.y);

	xcs = xchange / time * 0.02;
	ycs = ychange / time * 0.02;
	rcs = sqrt(xcs*xcs + ycs * ycs);
	flyrange = 0;

	this->runAction(moveBy);
}

void BulletBase::hide()
{
	auto del = CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, this));
	this->runAction(del);
	exist = false;
}

void BulletBase::pointChange(float dt)
{
	if (getSprite() != NULL)
	{
		if (flyrange + rcs <= range)
		{
			point.x += xcs;
			point.y += ycs;
			flyrange += rcs;
		}
		else
		{
			point = target;
			this->hide();
		}
	}
}

bool BulletBase::collidePlayer(Player *player)
{
	if (this->comefrom != player && player->p_hp > 0 && !player->unbeat)
	{
		float dx, dy;
		dx = point.x - player->x_coord;
		dy = point.y - player->y_coord;
		if (dx < 0) dx = -dx;
		if (dy < 0) dy = -dy;
		if (dx < player->radius && dy < player->radius)
		{
			this->hide();
			player->hurt(atkpower*comefrom->atkpower);
			return true;
		}
	}
	return false;
}

void BulletBase::ground(Player* player, Point pos)
{
	xcs = ycs = rcs = 0;
	point = pos;
	MoveBy *moveBy = MoveBy::create(2.0f, ccp(0, 0));
	auto callbackFunc = [&]()
	{
		this->hide();
	};
	CallFunc* callFunc = CallFunc::create(callbackFunc);

	Action* actions = Sequence::create(moveBy, callFunc, NULL);

	this->runAction(actions);
}

void BulletBase::knife(Player *player, Point pos)
{
	range *= comefrom->atkrange;
	float time = (range) / (flyspeed);//单位为秒
	float length = sqrt((player->x_coord - pos.x)*(player->x_coord - pos.x) + (player->y_coord - pos.y)*(player->y_coord - pos.y));
	float xchange = (pos.x - player->x_coord) / length * range;
	float ychange = (pos.y - player->y_coord) / length * range;
	//在cocos2dx中你甚至能学几何

	auto moveBy = MoveBy::create(time, Point(xchange, ychange));
	point.x = player->x_coord;
	point.y = player->y_coord;
	target.x = point.x + xchange;
	target.y = point.y + ychange;

	//	log("start x = %f  y = %f", point.x, point.y);
	//	log("target x = %f  y = %f", target.x, target.y);

	xcs = xchange / time * 0.02;
	ycs = ychange / time * 0.02;
	rcs = sqrt(xcs * xcs + ycs * ycs);
	flyrange = 0;

	this->runAction(moveBy);
}