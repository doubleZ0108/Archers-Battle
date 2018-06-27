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
	Size visibleSize = CCDirector::getInstance()->getVisibleSize();

	int number;				 //����ID
	Sprite *sprite = 0;		 //�󶨵ľ���
	bool life = true;		 //�����ж�
	int lives = 3;			 //ʣ�ิ�����
	int level = 1;			 //�ȼ�
	int exp = 0;			 //��ǰ����ֵ
	bool unbeat = false;	 //�޵�װ��
	bool attackcd = false;   //�Ƿ��ڹ���CD��
	bool animating = false;	 //�Ƿ��ڲ��Ŷ���
	float x_coord = 80.0f;
	float y_coord = 80.0f;

	void runway1(std::map<EventKeyboard::KeyCode, bool>keys, Player *smallplayer);
	void hpraise(int num);		//�ȵ�����ʱѪ��������
	bool expraise(int num);		//����ֵ������
	int explimit();				//��ǰ�ȼ������

	bool hurt(float atk);
	void animationcreate(int direct);	//���ɶ����Ķ���
	void runanimate(std::map<EventKeyboard::KeyCode, bool>keys); //�����Ĳ���
	void attackCD();					//��������ȴʱ��
	void die(int rex, int rey);			//��������
	void restart(int rex, int rey);		//����
	

	//������
	float speed = 5;      //�ٶ�
	float radius = 30;		//�ж���С
	float p_hp = 50.0;//��ǰѪ��
	int hpLimit = 50;//Ѫ������
	float atkpower = 1.0;//������
	float atkrange = 1.0;//��������
	float defpower = 1.0;//������
	bool magnet = false;  //��������

	//�ӵ�����
	int front = 1;		//��ǰ�����ӵ�����
	int leftside = 0;	//�������ӵ�����
	int rightside = 0;	//���ҷ����ӵ�����
	int back = 0;		//���ҷ����ӵ�����

	//��������1��2�ش�3��4����
	int weapon = 1;
	//��Ѫ����������
	float hpincrease = 1;
	float expincrease = 10;
	//CDʱ��
	float atkCD = 0.8;
	//��ǽ����
	bool ifcan_breakwall = false;	//�����ܷ�ǽ
	bool ifbreakwall = false;		//�ӵ��ܷ�ǽ

	
	
private:
	Point restartp;
};

#endif