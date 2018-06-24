#ifndef __Player_H__
#define __Player_H__

#include "cocos2d.h"
#include "Entity.h"

using namespace cocos2d;

class Player : public Entity {
public:
	CREATE_FUNC(Player);
	virtual bool init();

	int number;
	bool life = 1;
	int lives = 3;

	//void run1(Player *player, std::map<EventKeyboard::KeyCode, bool>keys, Player *smallplayer);
	void runway1(std::map<EventKeyboard::KeyCode, bool>keys, Player *smallplayer);

	Size visibleSize = CCDirector::getInstance()->getVisibleSize();
	float x_coord = 80.0f;
	float y_coord = 80.0f;

	int level = 1;
	int exp = 0;

	void hpraise(int num);
	bool expraise(int num);
	int explimit();

	bool hurt(float atk);
	Sprite *sprite = 0;
	void animationcreate(int direct);
	void runanimate(std::map<EventKeyboard::KeyCode, bool>keys);

	bool unbeat = 0;

	void attackCD();
	bool attackcd = 0;

	//������
	//�����������
	float speed = 5;      //zz
	float radius = 30;
	float p_hp = 50.0;//��ǰѪ��zz
	int hpLimit = 50;//����Ѫ��zz
	float atkpower = 1.0;//zzһλС��
	float atkrange = 1.0;//zz
	float defpower = 1.0;//zz
	bool magnet = false;
	//�ӵ�����
	int front = 1;//zz
	int leftside = 0;//zz
	int rightside = 0;//zz
	int back = 0;//zz
	//��������1��2�ش�3��4����
	int weapon = 1;
	//��Ѫ����������
	float hpincrease = 1;
	float expincrease = 10;
	//CDʱ��
	float atkCD = 0.8;
	//�ӵ��ܷ�ǽ
	bool ifcan_breakwall = false;//zz

	bool animating = 0;
	void die(int rex, int rey);
	void restart(int rex, int rey);//����
private:
	Point restartp;
};

#endif