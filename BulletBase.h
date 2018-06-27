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

	Player *comefrom;					//�ӵ���Դ
	void attacking(Player *player, Point pos);//�ӵ����
	float collidePlayer(Player *player);//�ж���ײ
	bool exist = false;					//�ӵ�����
	bool real = true;					//�ӵ���Դ�����ҷ����ǵз�
	void hide();						//ɾ���ӵ�
	Point point;						//�ӵ���ǰ����
private:
	Point target;						//Ԥ����������
	float xcs, ycs, rcs;				//x y r��λʱ��ı���
	float flyrange;						//�ѷ��о���

	float atkpower;
	int flyspeed;
	float range;

	void pointChange(float dt);
	void arrow(Player *player, Point pos);
	void ground(Player* player, Point pos);
	void knife(Player* player, Point pos);
};

#endif