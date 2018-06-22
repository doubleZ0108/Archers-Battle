#ifndef _GAMEPLAYING_H_
#define _GAMEPLAYING_H_

#include "cocos2d.h"
#include "Player.h"
#include "BulletBase.h"
#include "ProgressView.h"
#include "ExpProgress.h"
#include <windows.h>  
#include <vector>
#include "network\SocketIO.h"

#pragma comment(lib,"libcurl_imp.lib")

#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)  
#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)  

#define k_w (EventKeyboard::KeyCode)146
#define k_a (EventKeyboard::KeyCode)124
#define k_s (EventKeyboard::KeyCode)142
#define k_d (EventKeyboard::KeyCode)127

#define MAP_SIZE 1598
#define MAP_WIDTH 49
#define MAP_HEIGHT 49

#define DIFF 16   //�����ǽ�ڼ�ľ����ֵ���Ƚ���ѧ�Ĳ��ԣ���Ҫ���ڼ��ĳһ�������Ƿ��в������ߵĵط�

#define GAP1_GID 145
#define NOR1_GID 138
#define HP1_GID 137
#define EXP1_GID 142

#define GAP2_GID 129
#define NOR2_GID 122
#define HP2_GID 123
#define EXP2_GID 128

#define GAP3_GID 405
#define NOR3_GID 398
#define HP3_GID 399
#define EXP3_GID 404

#define GAP_GID (1==which_map?GAP1_GID : (2==which_map? GAP2_GID : GAP3_GID))
#define NOR_GID (1==which_map?NOR1_GID : (2==which_map? NOR2_GID : NOR3_GID))
#define HP_GID (1==which_map?HP1_GID : (2==which_map? HP2_GID : HP3_GID))
#define EXP_GID (1==which_map?EXP1_GID : (2==which_map? EXP2_GID : EXP3_GID))

#define SM_MAP_SIZE 245
#define RETE (260.0/1600)  //smallplayer��player�ƶ�����ı�
#define XIE 0.707
#define TLMAP_SCALE (viewsize==1? 1.0 :(viewsize==1.75? 2.0: 0.8))//��ͼ����ʱ�ı�������
#define MOUCE_SCALE (viewsize==1? 1.0 :(viewsize==2? 1.35: 0.8))//��ͼ����ʱ���ı�������
USING_NS_CC;
using namespace cocos2d::network;
struct HP_MESS
{
	Sprite* hp_potion;   //����ҩˮͼƬ�ľ���
	int savex, savey;  //�����µ��ߵ�λ�����ڼ���
	HP_MESS(Sprite* buf, int randx, int randy)
		:hp_potion(buf), savex(randx), savey(randy) {}
	//����==�����������Ļ�����std::find()����ʱ���bug
	//std::find()���ڲ�ʵ��Ӧ���ǽ�����==�����
	bool operator==(const HP_MESS &thv)
	{
		if(this->savex==thv.savex && this->savey==thv.savey)
		{
			return true;
		}
		else { return false; }
	}
};
struct EXP_MESS
{
	Sprite* exp_potion;   //����ҩˮͼƬ�ľ���
	int savex, savey;  //�����µ��ߵ�λ�����ڼ���
	EXP_MESS(Sprite* buf, int randx, int randy)
		:exp_potion(buf), savex(randx), savey(randy) {}
	bool operator==(const EXP_MESS &thv)
	{
		if (this->savex == thv.savex && this->savey == thv.savey)
		{
			return true;
		}
		else { return false; }
	}
};
class GamePlaying : public cocos2d::Layer, public cocos2d::network::SocketIO::SIODelegate
{
private:

	/////////////////////////////////////////////
	//��������ļ���
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Rect rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	/////////////////////////////////////////////

private:
	Size size;
	Sprite* sprite;

	Size mapSize;
	Size tileSize;
	float x_move = 0.0;   //���ڵ�ͼ����֮���������λ������Ӧ����ֵת��
	float y_move = 0.0;
	TMXTiledMap* tiledmap;
	TMXLayer* meta;
	TMXLayer* HP_objects;

	static std::vector<HP_MESS> hp_auto_arise;
	static std::vector<EXP_MESS> exp_auto_arise;
public:
	SIOClient * _sioClient;
	static cocos2d::Scene* createScene();

	virtual bool init();

	//////////////////////////////////
	//���ֽ���Ļ���
	void NetworkPrinter();
	void PlayerPrinter();
	void SettingPrinter();
	void MapPrinter();
	void ScenePrinter();
	void MusicPrinter();
	void SmallmapPrinter();
	void ModePrinter();
	////////////////////////////////////

	void Weapon_change(Ref* pSender);
	void TLmap_change(Ref* pSender);
	void Breakwall_change(Ref* pSender);

	///////////////////////////////////
	//���ֿ��صĻص�����
	void menuStartScene(Ref* pSender);
	void Network_Switch(Ref* Spender);
	void Smallmap_Switch(Ref* pSender);
	void Mode_Switch(Ref* pSender);
	void Music_Switch(Ref* pSender);
	////////////////////////////////////

	////////////////////////////////////
	//����������жϺ�ǽ�ڼ��
	bool up(bool flag, int ifxie=0); //true��������Ҫ����runEvent����ʵʵ���ڵ��ƶ�
	bool right(bool flag, int ifxie=0);//false������ֻ�����ж���������ܲ����ߣ���ʵ�����ƶ�
	bool left(bool flag, int ifxie=0);
	bool down(bool flag, int ifxie=0);
	bool isCanReach(float x, float y);
	//////////////////////////////////

	/////////////////////////////////////////
	//���ߵ�������ֺ��� �� �жϲ��Ե����ߵĺ���
	void HPjudge(const Vec2 &pos);
	void HP_grow(float dt);
	void EXPjudge(const Vec2 &pos);
	void EXP_grow(float dt);


	void DeCode_for_Map(const std::string &buf, int &metax, int &metay);
	void HP_recieve(SIOClient* client, const std::string& data);
	void EXP_recieve(SIOClient* client, const std::string& data);
	void HP_remove(SIOClient* client, const std::string& data);
	void EXP_remove(SIOClient* client, const std::string& data);

	void tofindEat(const float x, const float y);
	/////////////////////////////////////////

	// implement the "static create()" method manually
	CREATE_FUNC(GamePlaying);

	//socket����ʱ����
	void onConnect(SIOClient* client);
	//�յ�����ʱ����
	void onMessage(SIOClient* client, const std::string& data);
	//���Ӵ������յ������ź�ʱ����
	void onError(SIOClient* client, const std::string& data);
	//socket�ر�ʱ����
	void onClose(SIOClient* client);

	virtual void update(float delta);
	void runEvent();
	void runEvent_n(SIOClient* client, const std::string& data);
	void onEnter();
	void attack();
	void attackweapon(float num);
	void levelup();
	/////////////////////////
private:
	Player * m_player = Player::create();		//����1
	Player* n_player = Player::create();       //����2����Ϊ�����У��еĻ�Ҳ��
	std::map<EventKeyboard::KeyCode, bool>keys;//��¼����״̬
	std::map<EventKeyboard::KeyCode, bool>keys1;
	std::map<EventKeyboard::KeyCode, bool>keys_n;
	std::map<EventKeyboard::KeyCode, bool>keys1_n;
	bool touchon = false;//�Ƿ񵥻�
	Point pos;//�������꣬���ڹ���
	std::vector<Player*>plsum;
	std::vector<BulletBase*>bubsum;

	bool waytorun = true;  //true->�����ƶ���깥��

	Sprite *m_smallmap;
	//LayerColor *m_smallmap;
	Player *m_smallplayer;
	Player *n_smallplayer;

	///////////////////////////////
	//Ѫ��
	ProgressView *m_pProgressView;
	ProgressView *n_pProgressView;

	/////////////////////////
	ExpProgress *expPro;
	Label *lv;
};

#endif //_GAMEPLAYING_H_