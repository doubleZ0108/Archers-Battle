#include "GamePlaying.h"
#include "SimpleAudioEngine.h"
#include "Start.h"
#include <cstdlib>
#include <math.h>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>

USING_NS_CC;

std::vector<HP_MESS> GamePlaying::hp_auto_arise;   //���ڴ���������õĻ�Ѫ���ߵ������Ϣ
std::vector<EXP_MESS> GamePlaying::exp_auto_arise;   //���ڴ���������õľ�����ߵ������Ϣ

extern bool language_flag;  //true->English   false->Chinese
extern int is_paused;       //����is_paused�ľ��������� "HelloWorldScene.h"
extern char *FontToUTF8(const char* font);

bool smallmap_switch = true;       //С��ͼ���ƿ���,true->��С��ͼ,false->����С��ͼ
								   //ÿ�δ�С��ͼ��ʱ��С�����λ��Ҫ��m_player����Ӧ�ĵ���
bool music_switch = true;
bool mode_switch = true;

int which_map = 3;
int which_player = 1;

int viewsize = 1;
bool ifbreakwall = false;   //��ǽ����
std::string ID = "guest";
//it is define in another .cpp file 
//and it is used to change character

Scene* GamePlaying::createScene()
{
	auto scene = Scene::create();
	auto layer = GamePlaying::create();
	scene->addChild(layer);
	return scene;
}
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GamePlaying.cpp\n");
}
// on "init" you need to initialize your instance
bool GamePlaying::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	NetworkPrinter();

	/*std::string mess = "owner";
	if (_sioClient)
	{
		_sioClient->emit("owner", mess);
	}*/

	MusicPrinter();
	MapPrinter();
	ScenePrinter();
	SmallmapPrinter();
	ModePrinter();

	//schedule(schedule_selector(GamePlaying::EXP_grow), 0.15f);
	//schedule(schedule_selector(GamePlaying::HP_grow), 2.0f);

	_sioClient->on("HP position", CC_CALLBACK_2(GamePlaying::HP_recieve, this));
	_sioClient->on("EXP position", CC_CALLBACK_2(GamePlaying::EXP_recieve, this));

	_sioClient->on("HP remove", CC_CALLBACK_2(GamePlaying::HP_remove, this));
	_sioClient->on("EXP remove", CC_CALLBACK_2(GamePlaying::EXP_remove, this));
	_sioClient->on("n_player keys", CC_CALLBACK_2(GamePlaying::runEvent_n, this));
	//if (_sioClient)
	//{
	//	if (ID == "guest")
	//	{
	//		
	//	}
	//	//_sioClient->on("owner", CC_CALLBACK_2(GamePlaying::ID_judge, this));
	//	
	//	
	//}
	
	return true;
}
void GamePlaying::ID_judge(SIOClient * client, const std::string & data)
{
	ID = "guest";
}
void GamePlaying::MapPrinter()
{
	size = Director::getInstance()->getVisibleSize();

	//�򿪵�һ����Ƭ��ͼ
	if (1 == which_map)
	{
		tiledmap = TMXTiledMap::create("Player/Tiled Map/ArcherBattle_TiledMap_1.tmx");
	}
	else if (2 == which_map)
	{
		tiledmap = TMXTiledMap::create("Player/Tiled Map/ArcherBattle_TiledMap_2.tmx");
	}
	else if (3 == which_map)
	{
		tiledmap = TMXTiledMap::create("Player/Tiled Map/ArcherBattle_TiledMap_3.tmx");
	}
	
	this->addChild(tiledmap);
	//////////////////////////////////////////
	mapSize = tiledmap->getMapSize();      // ��ȡ��tiles����Ϊ��λ�ĵ�ͼ�ߴ�
	tileSize = tiledmap->getTileSize();    // ��ȡ�����ص�Ϊ��λ��tile�ߴ�����
	//log("tileSize %f %f", tileSize.width, tileSize.height);
	/////////////////////////////////////////
	//��meta����Ϊ���Բ�
	meta = tiledmap->layerNamed("meta");
	meta->setVisible(false);
}
void GamePlaying::ScenePrinter()
{
	float x = rect.origin.x + rect.size.width / 2;
	float y = rect.origin.y + rect.size.height / 2;

	///////////////////////////////////
	//a return button which click to back to HelloWorldScene
	auto *return_button = MenuItemImage::create(
		"Scene/Buttons/closetoupper.png",
		"Scene/Buttons/closetoupper_select.png",
		CC_CALLBACK_1(GamePlaying::menuStartScene, this));

	auto *preturn = Menu::create(return_button, NULL);
	x = rect.origin.x + rect.size.width*(49.0f / 50.0f);
	y = rect.origin.y + rect.size.height*(48.0f / 50.0f);
	preturn->setPosition(Vec2(x, y));

	preturn->setScale(1.0f);
	this->addChild(preturn, 100);   //�ѷ��ذ�ť����100�㣬��ֹ�ڵ�

	///////////////////////////////////////////////
	PlayerPrinter();
	SettingPrinter();
	//////////////////////////////////////////////
	
	////////////////////////////////////////////////////
	//����
	auto weaponMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(GamePlaying::Weapon_change, this),
		MenuItemFont::create("weapon 1"),
		MenuItemFont::create("weapon 2"),
		MenuItemFont::create("weapon 3"),
		MenuItemFont::create("weapon 4"),
		NULL);

	Menu* weaponmn = Menu::create(weaponMenuItem, NULL);
	x = rect.origin.x + rect.size.width*(33.7f / 40.0f);
	y = rect.origin.y + rect.size.height*(9.0f / 20.0f);
	weaponmn->setPosition(Vec2(x, y));
	this->addChild(weaponmn, 1);

	////////////////////////////////////////////////////
	//��ͼ����
	auto tlmapMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(GamePlaying::TLmap_change, this),
		MenuItemFont::create("normal"),
		MenuItemFont::create("explore"),
		MenuItemFont::create("overlook"),
		NULL);

	Menu* tlmapmn = Menu::create(tlmapMenuItem, NULL);
	x = rect.origin.x + rect.size.width*(33.7f / 40.0f);
	y = rect.origin.y + rect.size.height*(8.0f / 20.0f);
	tlmapmn->setPosition(Vec2(x, y));
	this->addChild(tlmapmn, 1);

	////////////////////////////////////////////////////
	//��ͼ����
	auto breakwallMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(GamePlaying::Breakwall_change, this),
		MenuItemFont::create("normal"),
		MenuItemFont::create("breakwall"),
		NULL);

	Menu* breakwallmn = Menu::create(breakwallMenuItem, NULL);
	x = rect.origin.x + rect.size.width*(33.7f / 40.0f);
	y = rect.origin.y + rect.size.height*(7.0f / 20.0f);
	breakwallmn->setPosition(Vec2(x, y));
	this->addChild(breakwallmn, 1);

	/*
	////////////////////////////////////////
	//starting cortoon���뵭��
	auto *actionFade = CCFadeOut::create(3.0f);
	auto *actionFadeBack = actionFade->reverse();
	auto *sequence1 = CCSequence::create(actionFade, actionFadeBack, NULL);
	//starting cortoon ��ɫ�仯
	auto *actionTint = CCTintBy::create(2, -255, -127, -50);
	auto *actionTinback = actionTint->reverse();
	auto *sequence2 = CCSequence::create(actionTint, actionTint, NULL);
	//chose the sequence that you prefer
	cover->runAction(sequence2);*/

}

void GamePlaying::NetworkPrinter()//noneed
{
	_sioClient = SIOClient::getInstance();
	_sioClient = SocketIO::connect("http://120.78.208.162:2333", *this);
	
	/*if (!SIOClient::setconnect(_sioClient, 0))
	{
		_sioClient = nullptr;
	}*/

}

void GamePlaying::PlayerPrinter()
{
	m_player->sprite = Sprite::create("Player/Figure/player1.png");
	m_player->bindSprite(m_player->sprite);
	m_player->setScale(2.0f, 2.0f);
	m_player->sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_player->setPosition(Point(m_player->x_coord + 200, m_player->y_coord + 200));
	tiledmap->addChild(m_player, 10);

	n_player->sprite = Sprite::create("Player/Figure/player2.png");
	n_player->bindSprite(n_player->sprite);
	n_player->setScale(1.8, 1.8);
	n_player->setPosition(Point(n_player->x_coord, n_player->y_coord));
	tiledmap->addChild(n_player, 10);

	plsum.push_back(m_player);
	plsum.push_back(n_player);


	////////////////////////////////////
	//Ѫ����ʼ��
	m_pProgressView = new ProgressView;
	m_pProgressView->setPosition(ccp(m_player->x_coord + 200, m_player->y_coord + 50 + 200));
	m_pProgressView->setScale(2);
	m_pProgressView->setBackgroundTexture("Player/Figure/background.png");
	m_pProgressView->setForegroundTexture("Player/Figure/foreground.png");
	m_pProgressView->setTotalProgress(1);
	m_pProgressView->setCurrentProgress(m_player->p_hp / m_player->hpLimit);
	tiledmap->addChild(m_pProgressView, 2);

	n_pProgressView = new ProgressView;
	n_pProgressView->setPosition(ccp(n_player->x_coord, n_player->y_coord + 50));
	n_pProgressView->setScale(2);
	n_pProgressView->setBackgroundTexture("Player/Figure/background.png");
	n_pProgressView->setForegroundTexture("Player/Figure/foreground.png");
	n_pProgressView->setTotalProgress(1);
	n_pProgressView->setCurrentProgress(n_player->p_hp / n_player->hpLimit);
	tiledmap->addChild(n_pProgressView, 2);
	///////////////////////////////////////////////
	this->scheduleUpdate();

	expPro = ExpProgress::create();
	addChild(expPro);
}
void GamePlaying::SettingPrinter()
{
	float x, y;
	////////////////////////////////////////////////
	//add small map
	Label *smallmapword;
	if (language_flag)
	{
		smallmapword = Label::createWithTTF("Global Map",
			"fonts/Marker Felt.ttf", 40);
	}
	else
	{
		smallmapword = Label::create(FontToUTF8("ȫ�ֵ�ͼ"),
			"Arial", 40);
	}
	x = rect.origin.x + rect.size.width*(33.7f / 40.0f);
	y = rect.origin.y + rect.size.height*(18.0f / 20.0f);
	smallmapword->setPosition(Vec2(x, y));
	this->addChild(smallmapword, 1);

	auto smallMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(GamePlaying::Smallmap_Switch, this),
		MenuItemImage::create(
			"Scene/Buttons/checkbox_selected.png",
			"Scene/Buttons/checkbox_selected.png"),
		MenuItemImage::create(
			"Scene/Buttons/checkbox_normal.png",
			"Scene/Buttons/checkbox_normal.png"),
		NULL);
	smallMenuItem->setScale(0.9f);
	Menu* smallmn = Menu::create(smallMenuItem, NULL);
	x = rect.origin.x + rect.size.width*(37.4f / 40.0f);
	smallmn->setPosition(Vec2(x, y));
	this->addChild(smallmn, 1);

	/////////////////////////////////////////////////
	//���ź���ͣ��Ϸ����
	Label *musicword;
	if (language_flag)
	{
		musicword = Label::createWithTTF("Game Music",
			"fonts/Marker Felt.ttf", 40);
	}
	else
	{
		musicword = Label::create(FontToUTF8("��Ϸ����"),
			"Arial", 40);
	}
	x = rect.origin.x + rect.size.width*(33.9f / 40.0f);
	y = rect.origin.y + rect.size.height*(17.0f / 20.0f);
	musicword->setPosition(Vec2(x, y));
	this->addChild(musicword, 1);

	auto musicMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(GamePlaying::Music_Switch, this),
		MenuItemImage::create(
			"Scene/Buttons/checkbox_selected.png",
			"Scene/Buttons/checkbox_selected.png"),
		MenuItemImage::create(
			"Scene/Buttons/checkbox_normal.png",
			"Scene/Buttons/checkbox_normal.png"),
		NULL);
	musicMenuItem->setScale(0.9f);
	Menu* musicmn = Menu::create(musicMenuItem, NULL);
	x = rect.origin.x + rect.size.width*(37.4f / 40.0f);
	musicmn->setPosition(Vec2(x, y));
	this->addChild(musicmn, 1);

	////////////////////////////////////////////////
	//�л���ͬ����Ϸģʽ
	Label *modeword;
	if (language_flag)
	{
		modeword = Label::createWithTTF("Game Mode",
			"fonts/Marker Felt.ttf", 40);
	}
	else
	{
		modeword = Label::create(FontToUTF8("��Ϸģʽ"),
			"Arial", 40);
	}
	x = rect.origin.x + rect.size.width*(33.9f / 40.0f);
	y = rect.origin.y + rect.size.height*(16.0f / 20.0f);
	modeword->setPosition(Vec2(x, y));
	this->addChild(modeword, 1);

	auto modeMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(GamePlaying::Mode_Switch, this),
		MenuItemImage::create(
			"Scene/Buttons/checkbox_selected.png",
			"Scene/Buttons/checkbox_selected.png"),
		MenuItemImage::create(
			"Scene/Buttons/checkbox_normal.png",
			"Scene/Buttons/checkbox_normal.png"),
		NULL);
	modeMenuItem->setScale(0.9f);
	Menu* modemn = Menu::create(modeMenuItem, NULL);
	x = rect.origin.x + rect.size.width*(37.4f / 40.0f);
	modemn->setPosition(Vec2(x, y));
	this->addChild(modemn, 1);
}

void GamePlaying::MusicPrinter()
{
	Label *OnorOff;
	float x, y;
	if (music_switch)
	{
		if (this->getChildByName("MUSIC_OFF"))
		{//�߼�����������,�����һ�����ֽ���MUSIC_OFF�ľ�����
			//��֤���Ѿ�����init�������������
			//�Ǿͼ������Ÿո���ͣ������
			this->getChildByName("MUSIC_OFF")->removeFromParentAndCleanup(true);
			CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		}
		else
		{
			//init�Ż������,��ͷ��������
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Escape.mp3");
		}
		
		if (language_flag)
		{
			OnorOff = Label::createWithTTF("ON",
				"fonts/Marker Felt.ttf", 30);
		}
		else
		{
			OnorOff = Label::create(FontToUTF8("����"),
				"Arial", 30);
		}
		OnorOff->setName("MUSIC_ON");
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		
		if (this->getChildByName("MUSIC_ON"))
		{
			this->getChildByName("MUSIC_ON")->removeFromParentAndCleanup(true);
		}
		if (language_flag)
		{
			OnorOff = Label::createWithTTF("OFF",
				"fonts/Marker Felt.ttf", 30);
		}
		else
		{
			OnorOff = Label::create(FontToUTF8("��ͣ"),
				"Arial", 30);
		}
		OnorOff->setName("MUSIC_OFF");
	}
	x = rect.origin.x + rect.size.width*(39.0f / 40.0f);
	y = rect.origin.y + rect.size.height*(17.0f / 20.0f);
	OnorOff->setPosition(Vec2(x, y));
	this->addChild(OnorOff, 1);
}
void GamePlaying::Music_Switch(Ref * pSender)
{
	_sioClient->emit("mapchose", "1");
	music_switch = (music_switch ? false : true);
	MusicPrinter();
}

void GamePlaying::SmallmapPrinter()
{
	Label *OnorOff;
	float x, y;
	if (smallmap_switch)        //���Ҫ��С��ͼ�������¹�������Ϊÿ�ε�С����λ�ò�ͬ
	{
		////////////////////////////////////////
		//add a smallmap to draw something and besides the smallmap is also a cover
		//cccv�ĵ��ĸ�����ȡֵ0~225��Խ��Խ��͸��
		//m_smallmap = LayerColor::create(ccc4(0, 0, 0, 100), 250, 250);
		switch (which_map)
		{
		case 1:m_smallmap = Sprite::create("smallmap1.png"); break;
		case 2:m_smallmap = Sprite::create("smallmap2.png"); break;
		case 3:m_smallmap = Sprite::create("smallmap3.png"); break;
		default:m_smallmap = nullptr;
		}
		
		m_smallmap->setOpacity(220);     //����С��ͼ��͸����
										 //m_smallmap->setColor(Color3B(0, 0, 205));
		m_smallmap->setAnchorPoint(Vec2(0.0f, 0.0f));
		x = rect.origin.x + rect.size.width*0.0f;
		y = rect.origin.y + rect.size.height*(2.0f / 3.0f - 0.02f);  //��0.02��Ϊ����ȥһ������С��λ��ƫ��
		m_smallmap->setPosition(Vec2(x, y));
		this->addChild(m_smallmap, 1);

		m_smallplayer = Player::create();
		m_smallplayer->bindSprite(Sprite::create("m_smallplayer.png"));
		m_smallplayer->setPosition(
			Vec2(m_player->getPositionX()*RETE, m_player->getPositionY()*RETE));
		m_smallmap->addChild(m_smallplayer);

		n_smallplayer = Player::create();
		n_smallplayer->bindSprite(Sprite::create("n_smallplayer.png"));
		n_smallplayer->setPosition(
			Vec2(n_player->getPositionX()*RETE, n_player->getPositionY()*RETE));
		m_smallmap->addChild(n_smallplayer);

		if (this->getChildByName("SMALLMAP_OFF"))
		{
			this->getChildByName("SMALLMAP_OFF")->removeFromParentAndCleanup(true);
		}
		if (language_flag)
		{
			OnorOff = Label::createWithTTF("ON",
				"fonts/Marker Felt.ttf", 30);
		}
		else
		{
			OnorOff = Label::create(FontToUTF8("����"),
				"Arial", 30);
		}
		OnorOff->setName("SMALLMAP_ON");
	}
	else      //���Ҫ���Ͼ�ֱ�����پ���
	{
		if (m_smallmap)
		{
			m_smallmap->removeFromParentAndCleanup(true);
		}
		
		if (this->getChildByName("SMALLMAP_ON"))
		{
			this->getChildByName("SMALLMAP_ON")->removeFromParentAndCleanup(true);
		}
		if (language_flag)
		{
			OnorOff = Label::createWithTTF("OFF",
				"fonts/Marker Felt.ttf", 30);
		}
		else
		{
			OnorOff = Label::create(FontToUTF8("�ر�"),
				"Arial", 30);
		}
		OnorOff->setName("SMALLMAP_OFF");
	}
	x = rect.origin.x + rect.size.width*(39.0f / 40.0f);
	y = rect.origin.y + rect.size.height*(18.0f / 20.0f);
	OnorOff->setPosition(Vec2(x, y));
	this->addChild(OnorOff, 1);
}
void GamePlaying::Smallmap_Switch(Ref* pSender)
{
	smallmap_switch = (smallmap_switch ? false : true);
	//�����ŵ�С��ͼ���ϣ������ŵ�С��ͼ��
	SmallmapPrinter();
	
}

void GamePlaying::ModePrinter()
{
	Label *OnorOff;
	float x, y;
	if (mode_switch)
	{
		if (this->getChildByName("MODE2"))
		{
			this->getChildByName("MODE2")->removeFromParentAndCleanup(true);
		}

		OnorOff = Label::createWithTTF("1",
			"fonts/Marker Felt.ttf", 30);
		OnorOff->setName("MODE1");
	}
	else
	{
		if (this->getChildByName("MODE1"))
		{
			this->getChildByName("MODE1")->removeFromParentAndCleanup(true);
		}

		OnorOff = Label::createWithTTF("2",
			"fonts/Marker Felt.ttf", 30);
		OnorOff->setName("MODE2");
	}
	x = rect.origin.x + rect.size.width*(39.0f / 40.0f);
	y = rect.origin.y + rect.size.height*(16.0f / 20.0f);
	OnorOff->setPosition(Vec2(x, y));
	this->addChild(OnorOff, 1);
}
void GamePlaying::Mode_Switch(Ref * pSender)
{
	waytorun = (waytorun ? false : true);
	mode_switch = (mode_switch ? false : true);
	keys[k_w] = keys[k_a] = keys[k_s] = keys[k_d] = false;
	touchon = false;
	ModePrinter();
	onEnter();
}


void GamePlaying::Weapon_change(Ref * pSender)
{
	++m_player->weapon;
	if (m_player->weapon == 5)
	{
		m_player->weapon = 1;
	}
}

void GamePlaying::TLmap_change(Ref * pSender)
{
	static int counter = 0;
	++counter;
	switch (counter % 3)
	{
	case 0:
	{
		tiledmap->setScale(1.0f);
		viewsize = 1;
		break;
	}
	case 1:
	{
		tiledmap->setScale(1.35f);
		viewsize = 2;
		break;
	}
	case 2:
	{
		tiledmap->setScale(0.8f);
		viewsize = 0;
		break;
	}
	
	default:break;
	}
}

void GamePlaying::Breakwall_change(Ref * pSender)
{
	ifbreakwall = (ifbreakwall ? false : true);
	if (ifbreakwall)
	{
		auto flash = Sprite::create("flash.png");
		flash->setScale(0.4f);
		flash->setName("player buff");
		m_player->addChild(flash);
	}
	else
	{
		if (m_player->getChildByName("player buff"))
		{
			m_player->getChildByName("player buff")->removeFromParentAndCleanup(true);
		}
	}
}

bool GamePlaying::up(bool flag,int ifxie)  //ifxieĬ�ϲ���Ϊfalse��Ĭ����ֱ����
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	if (y + tileSize.height < MAP_SIZE && (ifbreakwall ||
		(isCanReach(x + DIFF, y)
		&& isCanReach(x - DIFF, y)
		&& isCanReach(x, y) ) ) )   //���ϵ��ж϶�+1������ǽbug
	{	//������������Ǹ��ǵ�ͼ���ϱ߽�
		//֮������һ���С��һ��,����Ϊ�����ê��������,����һ�����±߽�ֻ��Ҫ�ټ�16
		
		if (flag)
		{
			runEvent();
			tofindEat(x, y);

			if (!ifxie)
			{
				if ((y + tiledmap->getPositionY() > size.height / 2 / (viewsize == 2 ? 4.0 : 1.0))
					&& ((MAP_SIZE*TLMAP_SCALE - y) > size.height / 2))
				{
					tiledmap->setPositionY(tiledmap->getPositionY()
						- m_player->speed);//������ͼ,ʹ���ﾡ������
										   //��ͼ�ƶ��ٶ��������ƶ��ٶȱ���һֱ����������Ϸ���飬����˿��
					y_move += m_player->speed;
				}
			}
			else if (1 == ifxie)
			{
				if ((y + tiledmap->getPositionY() > size.height / 2 / (viewsize == 2 ? 4.0 : 1.0))
					&& ((MAP_SIZE*TLMAP_SCALE - y) > size.height / 2))
				{
					tiledmap->setPositionY(tiledmap->getPositionY()
						- m_player->speed*XIE);
					tiledmap->setPositionX(tiledmap->getPositionX()
						- m_player->speed*XIE);
					x_move += m_player->speed*XIE;
					y_move += m_player->speed*XIE;
				}

			}
			else if (2 == ifxie)
			{
				if ((y + tiledmap->getPositionY() > size.height / 2 / (viewsize == 2 ? 4.0 : 1.0))
					&& ((MAP_SIZE*TLMAP_SCALE - y) > size.height / 2))
				{
					tiledmap->setPositionY(tiledmap->getPositionY()
						- m_player->speed*XIE);
					tiledmap->setPositionX(tiledmap->getPositionX()
						+ m_player->speed*XIE);
					x_move -= m_player->speed*XIE;
					y_move += m_player->speed*XIE;
				}
			}
		}

		return true;
	}
	
	return false;
}
bool GamePlaying::right(bool flag, int ifxie)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	if (x + tileSize.width < MAP_SIZE  && (ifbreakwall ||
		(isCanReach(x + 2*DIFF, y - DIFF)
		&& isCanReach(x + 2*DIFF, y - 2*DIFF) ) ) )
	{
		if (flag)
		{
			runEvent();
			tofindEat(x, y);
		}
		if ((x + tiledmap->getPositionX() > size.width / 2 / TLMAP_SCALE) 
			&& ((MAP_SIZE - x) > size.width / 2 / TLMAP_SCALE))
		{
			tiledmap->setPositionX(tiledmap->getPositionX() 
				- m_player->speed);
			x_move += m_player->speed;
		}
		return true;
	}
	return false;
}
bool GamePlaying::left(bool flag, int ifxie)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	if (x>tileSize.width  && (ifbreakwall ||
		( isCanReach(x - 2*DIFF, y - DIFF)
		&& isCanReach(x - 2*DIFF, y - 2*DIFF) ) ))
	{
		if (flag)
		{
			runEvent();
			tofindEat(x, y);
		}
		if ((x + tiledmap->getPositionX() < size.width / 2) 
			&& tiledmap->getPositionX() < 0)
		{
			tiledmap->setPositionX(tiledmap->getPositionX() 
				+ m_player->speed);
			x_move -= m_player->speed;
		}
		return true;
	}
	return false;
}
bool GamePlaying::down(bool flag, int ifxie)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	
	if (y>tileSize.height && (ifbreakwall ||
		(isCanReach(x, y - 4*DIFF)
		&& isCanReach(x-DIFF, y-4*DIFF) ) ) )
	{
		if (flag)
		{
			runEvent();
			tofindEat(x, y);

			if (!ifxie)
			{
				if ((y + tiledmap->getPositionY() < size.height / 2 / TLMAP_SCALE) 
					&& tiledmap->getPositionY() < 0 )
				{
					tiledmap->setPositionY(tiledmap->getPositionY()
						+ m_player->speed);
					y_move -= m_player->speed;
				}
			}
			else if (1 == ifxie)
			{
				if ((y + tiledmap->getPositionY() < size.height / 2 / TLMAP_SCALE) 
					&& tiledmap->getPositionY() < 0)
				{
					tiledmap->setPositionY(tiledmap->getPositionY()
						+ m_player->speed*XIE);
					tiledmap->setPositionX(tiledmap->getPositionX()
						- m_player->speed*XIE);
					x_move += m_player->speed*XIE;
					y_move -= m_player->speed*XIE;
				}
			}
			else if (2 == ifxie)
			{
				if ((y + tiledmap->getPositionY() < size.height / 2 / TLMAP_SCALE)
					&& tiledmap->getPositionY() < 0)
				{
					tiledmap->setPositionY(tiledmap->getPositionY()
						+ m_player->speed*XIE);
					tiledmap->setPositionX(tiledmap->getPositionX()
						+ m_player->speed*XIE);
					x_move -= m_player->speed*XIE;
					y_move -= m_player->speed*XIE;
				}
			}
		}
		return true;
	}
	return false;
}
bool GamePlaying::isCanReach(float x, float y)
{
	double mapX = x/ 32.0;        //��ȥ16�����������ê��������
	double mapY = MAP_HEIGHT - y/32.0;   //49ΪTiled���ͼ���������ֵ
	int tileGid = meta->tileGIDAt(Vec2(mapX, mapY)); //32��һ��Ĵ�С
	
	if (tileGid != GAP_GID)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GamePlaying::HPjudge(const Vec2 &pos)
{
	if (HP_GID == meta->getTileGIDAt(pos))
	{
		//CCLOG("hero is in HP_tiledmap");
		m_player->hpraise(1);
		if (m_player->magnet)   //��������������ܣ��ȵ����߲�����
		{

		}
		meta->setTileGID(NOR_GID, Vec2(static_cast<int>(pos.x), static_cast<int>(pos.y)));
		int judgex = static_cast<int>(pos.x), judgey = static_cast<int>(pos.y);
		//��Χfor��ʹ�ã������ҵ���ǰ�Ӵ��Ļ�Ѫ����
		//����Ӧ�ľ���ɾ������ɾ��vector����Ӧ��Ԫ��
		for (auto &hp_now : hp_auto_arise)
		{
			if (judgex == hp_now.savex && judgey == hp_now.savey)
			{
				if (_sioClient)
				{
					char mess[10];
					sprintf(mess, "%d %d", judgex, judgey);
					std::string HP_pos = mess;
					_sioClient->emit("HP remove", HP_pos);
				}
				
				hp_now.hp_potion->removeFromParentAndCleanup(true);
				auto hp_iter = std::find(hp_auto_arise.begin(), hp_auto_arise.end(), hp_now);
				hp_auto_arise.erase(hp_iter, hp_iter+1);
				
				break;
			}
		}

	}

}
void GamePlaying::HP_grow(float dt)
{
	if (ID == "guest")
	{
		//this->unschedule(schedule_selector(GamePlaying::HP_grow));
		//this->unschedule(schedule_selector(GamePlaying::EXP_grow));
	}
	/*static int time_locker = 0;
	++time_locker;
	if (time_locker > 20)
	{
	this->unschedule(schedule_selector(StartScene::HP_grow));
	}*/
	//�޶����ϻ�Ѫ���ߵ�����������25����ʱ�����ֹͣ����
	if (hp_auto_arise.size() > 25) { return; }
	int metax, metay;
	srand(time(NULL));
	//Ϊ���û�Ѫ���߲����ĸ�ϡ�裨��ʵ��û��ʲô��2333333                 
	metax = ((rand()%MAP_WIDTH)*(rand()%MAP_WIDTH)) % MAP_WIDTH;
	metay = ((rand() % MAP_HEIGHT)*(rand() % MAP_HEIGHT)) % MAP_HEIGHT;
	
	int gid = meta->getTileGIDAt(Vec2(1.0*metax, 1.0*metay));
	if (GAP_GID != gid && HP_GID != gid && EXP_GID != gid)
	{
		meta->setTileGID(HP_GID, Vec2(1.0*metax, 1.0*metay));

		//��Ĺ��캯�������һ����Ѫ����
		hp_auto_arise.push_back(HP_MESS(Sprite::create("Player/Tiled Map/HP_tiledmap.png"), metax, metay));

		int now_vec_maxindex = hp_auto_arise.size() - 1;
		float spritex = metax * tileSize.width, spritey = (mapSize.height - metay)*tileSize.height;
		hp_auto_arise[now_vec_maxindex].hp_potion->ignoreAnchorPointForPosition(false);
		hp_auto_arise[now_vec_maxindex].hp_potion->setAnchorPoint(Vec2(0.0f, 1.0f));

		hp_auto_arise[now_vec_maxindex].hp_potion->setPosition(
			Vec2(spritex, spritey));
		tiledmap->addChild(hp_auto_arise[now_vec_maxindex].hp_potion);

		/*if (ID != "guest")
		{
			char mess[10];
			sprintf(mess, "%d %d", metax, metay);
			std::string HP_pos = mess;
			_sioClient->emit("HP position", HP_pos);
		}*/
		
	}
}
void GamePlaying::EXPjudge(const Vec2 & pos)
{
	if (EXP_GID == meta->getTileGIDAt(pos))
	{
		//CCLOG("hero is in EXP_tiledmap");
		if (m_player->expraise(2))	{levelup();}

		if (m_player->magnet)
		{

		}
		meta->setTileGID(NOR_GID, Vec2(static_cast<int>(pos.x), static_cast<int>(pos.y)));
		int judgex = static_cast<int>(pos.x), judgey = static_cast<int>(pos.y);

		for (auto &exp_now : exp_auto_arise)
		{
			if (judgex == exp_now.savex && judgey == exp_now.savey)
			{
				if (_sioClient)
				{
					char mess[10];
					sprintf(mess, "%d %d", judgex, judgey);
					std::string EXP_pos = mess;
					_sioClient->emit("EXP remove", EXP_pos);
				}

				exp_now.exp_potion->removeFromParentAndCleanup(true);
				auto exp_iter = std::find(exp_auto_arise.begin(), exp_auto_arise.end(), exp_now);
				exp_auto_arise.erase(exp_iter, exp_iter + 1);

				break;
			}
		}

	}
}
void GamePlaying::EXP_grow(float dt)
{
	//if (ID == "guest")
	//{
	//	//this->unschedule(schedule_selector(GamePlaying::HP_grow));
	//	//this->unschedule(schedule_selector(GamePlaying::EXP_grow));
	//}
	/*static int time_locker = 0;
	++time_locker;
	if (time_locker > 20)
	{
	this->unschedule(schedule_selector(StartScene::HP_grow));
	}*/
	//�޶����ϻ�Ѫ���ߵ�����������100����ʱ�����ֹͣ����
	if (exp_auto_arise.size() > 100) { return; }
	int metax, metay;
	srand(time(NULL));               
	metax = rand() % MAP_WIDTH;
	metay = rand() % MAP_HEIGHT;

	int gid = meta->getTileGIDAt(Vec2(1.0*metax, 1.0*metay));
	if (GAP_GID != gid && HP_GID != gid && EXP_GID != gid)
	{
		meta->setTileGID(EXP_GID, Vec2(1.0*metax, 1.0*metay));

		//��Ĺ��캯�������һ����Ѫ����
		exp_auto_arise.push_back(EXP_MESS(Sprite::create("Player/Tiled Map/EXP_tiledmap.png"), metax, metay));

		int now_vec_maxindex = exp_auto_arise.size() - 1;
		float spritex = metax * tileSize.width, spritey = (mapSize.height - metay)*tileSize.height;
		exp_auto_arise[now_vec_maxindex].exp_potion->ignoreAnchorPointForPosition(false);
		exp_auto_arise[now_vec_maxindex].exp_potion->setAnchorPoint(Vec2(0.0f, 1.0f));

		exp_auto_arise[now_vec_maxindex].exp_potion->setPosition(
			Vec2(spritex, spritey));
		tiledmap->addChild(exp_auto_arise[now_vec_maxindex].exp_potion);

		/*if (ID != "guest")
		{
			char mess[10];
			sprintf(mess, "%d %d", metax, metay);
			std::string EXP_pos = mess;
			_sioClient->emit("EXP position", EXP_pos);
		}*/
		
	}
}

void GamePlaying::DeCode_for_Map(const std::string & buf, int & metax, int & metay)
{
	std::string data(buf);
	data.erase(data.begin(), data.begin() + 1);
	data.erase(data.end() - 1, data.end());

	if (data.c_str()[1] != ' ')
	{
		metax = (data.c_str()[0] - '0') * 10 + data.c_str()[1] - '0';
		if (data.size() == 4)
		{
			metay = (data.c_str()[3] - '0');
		}
		else
		{
			metay = (data.c_str()[3] - '0') * 10 + data.c_str()[4] - '0';
		}
	}
	else
	{
		metax = data.c_str()[0] - '0';
		if (data.size() == 3)
		{
			metay = (data.c_str()[2] - '0');
		}
		else
		{
			metay = (data.c_str()[2] - '0') * 10 + data.c_str()[3] - '0';
		}
	}
}

void GamePlaying::HP_recieve(SIOClient * client, const std::string & data)
{
	int metax, metay;
	DeCode_for_Map(data, metax, metay);
	
	int gid = meta->getTileGIDAt(Vec2(1.0*metax, 1.0*metay));
	meta->setTileGID(HP_GID, Vec2(1.0*metax, 1.0*metay));

	//��Ĺ��캯�������һ����Ѫ����
	hp_auto_arise.push_back(HP_MESS(Sprite::create("Player/Tiled Map/HP_tiledmap.png"), metax, metay));

	int now_vec_maxindex = hp_auto_arise.size() - 1;
	float spritex = metax * tileSize.width, spritey = (mapSize.height - metay)*tileSize.height;
	hp_auto_arise[now_vec_maxindex].hp_potion->ignoreAnchorPointForPosition(false);
	hp_auto_arise[now_vec_maxindex].hp_potion->setAnchorPoint(Vec2(0.0f, 1.0f));

	hp_auto_arise[now_vec_maxindex].hp_potion->setPosition(
		Vec2(spritex, spritey));
	tiledmap->addChild(hp_auto_arise[now_vec_maxindex].hp_potion);
}
void GamePlaying::EXP_recieve(SIOClient * client, const std::string & data)
{
	int metax, metay;
	DeCode_for_Map(data, metax, metay);

	int gid = meta->getTileGIDAt(Vec2(1.0*metax, 1.0*metay));
	meta->setTileGID(EXP_GID, Vec2(1.0*metax, 1.0*metay));

	//��Ĺ��캯�������һ����Ѫ����
	exp_auto_arise.push_back(EXP_MESS(Sprite::create("Player/Tiled Map/EXP_tiledmap.png"), metax, metay));

	int now_vec_maxindex = exp_auto_arise.size() - 1;
	float spritex = metax * tileSize.width, spritey = (mapSize.height - metay)*tileSize.height;
	exp_auto_arise[now_vec_maxindex].exp_potion->ignoreAnchorPointForPosition(false);
	exp_auto_arise[now_vec_maxindex].exp_potion->setAnchorPoint(Vec2(0.0f, 1.0f));

	exp_auto_arise[now_vec_maxindex].exp_potion->setPosition(
		Vec2(spritex, spritey));
	tiledmap->addChild(exp_auto_arise[now_vec_maxindex].exp_potion);
}
void GamePlaying::HP_remove(SIOClient * client, const std::string & data)
{
	int judgex, judgey;
	DeCode_for_Map(data, judgex, judgey);

	for (auto &hp_now : hp_auto_arise)
	{
		if (judgex == hp_now.savex && judgey == hp_now.savey)
		{
			hp_now.hp_potion->removeFromParentAndCleanup(true);
			auto hp_iter = std::find(hp_auto_arise.begin(), hp_auto_arise.end(), hp_now);
			hp_auto_arise.erase(hp_iter, hp_iter + 1);

			break;
		}
	}
}
void GamePlaying::EXP_remove(SIOClient * client, const std::string & data)
{
	int judgex, judgey;
	DeCode_for_Map(data, judgex, judgey);

	for (auto &exp_now : exp_auto_arise)
	{
		if (judgex == exp_now.savex && judgey == exp_now.savey)
		{
			exp_now.exp_potion->removeFromParentAndCleanup(true);
			auto exp_iter = std::find(exp_auto_arise.begin(), exp_auto_arise.end(), exp_now);
			exp_auto_arise.erase(exp_iter, exp_iter + 1);

			break;
		}
	}
}
void GamePlaying::tofindEat(const float x, const float y)
{
	if (m_player->magnet)
	{
		for (int i = -32; i <= 32; i += 32)
		{
			for (int j = -32; j <= 32; j += 32)
			{
				HPjudge(Vec2((x + i) / tileSize.width,
					(mapSize.height*tileSize.height - y + j) / tileSize.height));
				EXPjudge(Vec2((x + i) / tileSize.width,
					(mapSize.height*tileSize.height - y + j) / tileSize.height));
			}
		}
	}
	else
	{
		HPjudge(Vec2(x / tileSize.width,
			(mapSize.height*tileSize.height - y) / tileSize.height));
		EXPjudge(Vec2(x / tileSize.width,
			(mapSize.height*tileSize.height - y) / tileSize.height));
	}
}


void GamePlaying::onEnter()
{
	Layer::onEnter();

	auto keylistener = EventListenerKeyboard::create();
	auto mouselistener = EventListenerMouse::create();

	//���̼����������������ƶ�
	keylistener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event *event)
	{
		if (waytorun)
		{
			keys[keyCode] = true;
			keys1[keyCode] = true;
		}
		else
		{
			if (keyCode == (EventKeyboard::KeyCode)59)
				touchon = true;
		}
	};

	keylistener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event *event)
	{
		if (waytorun)
		{
			keys[keyCode] = false;
			keys1[keyCode] = false;
		}
	};

	mouselistener->onMouseMove = [&](Event *event)
	{
		if (!waytorun)
		{
			EventMouse* e = (EventMouse*)event;
			pos.x = e->getCursorX();
			pos.y = e->getCursorY();
			pos.x += this->x_move;
			pos.y += this->y_move;
			pos.x /= MOUCE_SCALE;
			pos.y /= MOUCE_SCALE;
		}
	};

	mouselistener->onMouseDown = [&](Event *event)
	{
		if (waytorun)
		{
			touchon = true;
			EventMouse* e = (EventMouse*)event;
			pos.x = e->getCursorX();
			pos.y = e->getCursorY();
			pos.x += this->x_move;
			pos.y += this->y_move;
			pos.x /= MOUCE_SCALE;
			pos.y /= MOUCE_SCALE;
		}
	};

	EventDispatcher *eventDispatcher1 = Director::getInstance()->getEventDispatcher();
	eventDispatcher1->addEventListenerWithSceneGraphPriority(keylistener, this);

	EventDispatcher *eventDispatcher2 = Director::getInstance()->getEventDispatcher();
	eventDispatcher2->addEventListenerWithSceneGraphPriority(mouselistener, this);

}
void GamePlaying::onConnect(SIOClient * client)
{
}
void GamePlaying::onMessage(SIOClient * client, const std::string & data)
{
}
void GamePlaying::onError(SIOClient * client, const std::string & data)
{
}
void GamePlaying::onClose(SIOClient * client)
{
}

void GamePlaying::update(float delta)
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	m_player->x_coord = x; m_player->y_coord = y;
	//CCLOG("x=%f , y=%f", x, y);
	/////////////////////////////////////////
	//Ѫ��λ��&�����趨
	m_pProgressView->setCurrentProgress(m_player->p_hp / m_player->hpLimit);
	m_pProgressView->setPosition(ccp(m_player->x_coord, m_player->y_coord + 50));
	n_pProgressView->setCurrentProgress(n_player->p_hp / n_player->hpLimit);
	n_pProgressView->setPosition(ccp(n_player->x_coord, n_player->y_coord + 50));
	m_pProgressView->setScale((float)m_player->hpLimit / 50 * 2, 2);

	if (m_player->level == 11)
		expPro->ExpChange(1, 1);
	else
		expPro->ExpChange(m_player->exp, m_player->explimit());

	//������ײ���
	for (auto pl : plsum)
	{
		if (pl != m_player)
		{
			float distance = (m_player->x_coord - pl->x_coord)*(m_player->x_coord - pl->x_coord)
				+ (m_player->y_coord - pl->y_coord)*(m_player->y_coord - pl->y_coord);
			if (distance > 64 * 64)
				break;
			if (m_player->x_coord < pl->x_coord)
				keys[k_d] = false;
			else
				keys[k_a] = false;
			if (m_player->y_coord < pl->y_coord)
				keys[k_w] = false;
			else
				keys[k_s] = false;
		}
	}

	if (!waytorun)
	{
		keys[k_w] = keys[k_a] = keys[k_s] = keys[k_d] = false;
		keys1[k_w] = keys1[k_a] = keys1[k_s] = keys1[k_d] = false;
		if (pos.x - x > 32.0f)
		{
			keys[k_d] = true;
			keys1[k_d] = true;
		}
		else if (x - pos.x>32.0f)
		{
			keys[k_a] = true;
			keys1[k_a] = true;
		}

		if (pos.y - y > 32.0f)
		{
			keys[k_w] = true;
			keys1[k_w] = true;
		}
		else if (y - pos.y > 32.0f)
		{
			keys[k_s] = true;
			keys1[k_s] = true;
		}
	}

	if (keys[k_w] || keys[k_a] || keys[k_s] || keys[k_d])//�ֱ���wasd���μ�#define
	{
		
		//���ͬʱ����w��d�����ȼ���Ƿ�������ϲ��ҿ�������
		//�������Ƚ��ַ����жϺ����Ĳ�������Ϊfalse
		//����ֻ�����Ƿ������,����ʵ�ʵ�runEvent����
		if (keys[k_w] && keys[k_d])  //���ͬʱ����w��d
		{
			bool flagup = up(false), flagright = right(false);
			if (flagup && flagright)			//������Ϻ����Ҷ�������
			{
				if(_sioClient)
					_sioClient->emit("n_player keys", "wd");
				if ((x + tiledmap->getPositionX() > size.width / 2)
					&& ((MAP_SIZE - x) > size.width / 2))
				{
					up(true, 1);
				}           //��Ȼ���õ���up(true)����ʵ��runEventl()���ߵķ��������ϵ�
					  //������up������ƶ�������������,����Ҫͨ������right(false)���õ�ͼ��ͬ�����ƶ�
				else { up(true); }
			}
			else if (flagup && !flagright)  //���ֻ�����Ͽ����ߣ��Ǳ��ֵ�Ч����������ǽ������
			{
				keys[k_d] = false;         //���ҷ���ļ��̼����ص�������Ϊ�൱��ֻ����w
				if (_sioClient)
					_sioClient->emit("n_player keys", "w");
				up(true);
			}
			else if (!flagup && flagright)
			{
				keys[k_w] = false;
				if (_sioClient)
					_sioClient->emit("n_player keys", "d");
				right(true);
			}

		}
		else if (keys[k_w] && keys[k_a])
		{
			bool flagup = up(false), flagleft = left(false);
			if (flagup && flagleft)
			{
				if (_sioClient)
					_sioClient->emit("n_player keys", "wa");
				if ((x + tiledmap->getPositionX() < size.width / 2)
					&& tiledmap->getPositionX() != 0)
				{
					up(true, 2);
				}
				else { up(true); }
			}
			else if (flagup && !flagleft)
			{
				keys[k_a] = false;
				if (_sioClient)
					_sioClient->emit("n_player keys", "w");
				up(true);
			}
			else if (!flagup && flagleft)
			{
				keys[k_w] = false;
				if (_sioClient)
					_sioClient->emit("n_player keys", "a");
				left(true);
			}
		}
		else if (keys[k_a] && keys[k_s])
		{
			bool flagleft = left(false), flagdown = down(false);
			if (flagleft && flagdown)
			{
				if (_sioClient)
					_sioClient->emit("n_player keys", "as");
				if ((x + tiledmap->getPositionX() < size.width / 2)
					&& tiledmap->getPositionX() != 0)
				{
					down(true, 2);
				}
				else { down(true); }
			}
			else if (flagleft && !flagdown)
			{
				keys[k_s] = false;
				if (_sioClient)
					_sioClient->emit("n_player keys", "a");
				left(true);
			}
			else if (!flagleft && flagdown)
			{
				keys[k_a] = false;
				if (_sioClient)
					_sioClient->emit("n_player keys", "s");
				down(true);
			}
		}
		else if (keys[k_s] && keys[k_d])
		{
			bool flagdown = down(false), flagright = right(false);
			if (flagdown && flagright)
			{
				if (_sioClient)
					_sioClient->emit("n_player keys", "sd");
				if ((x + tiledmap->getPositionX() > size.width / 2)
					&& ((MAP_SIZE - x) > size.width / 2))
				{
					down(true, 1);
				}
				else { down(true); }
			}
			else if (flagdown && !flagright)
			{
				keys[k_d] = false;
				if (_sioClient)
					_sioClient->emit("n_player keys", "s");
				down(true);
			}
			else if (!flagdown && flagright)
			{
				keys[k_s] = false;
				if (_sioClient)
					_sioClient->emit("n_player keys", "d");
				right(true);
			}
		}
		else if (keys[k_w])
		{
			if (_sioClient)
				_sioClient->emit("n_player keys", "w");
			up(true);
		}
		else if (keys[k_a])
		{
			if (_sioClient)
				_sioClient->emit("n_player keys", "a");
			left(true);
		}
		else if (keys[k_s])
		{
			if (_sioClient)
				_sioClient->emit("n_player keys", "s");
			down(true);
		}
		else if (keys[k_d])
		{
			if (_sioClient)
				_sioClient->emit("n_player keys", "d");
			right(true);
		}
	}

	m_player->runanimate(keys1);

	if (touchon && !m_player->attackcd)
	{
		attack();
		touchon = false;
	}
	//������¼���ͨ��ǰ���⼸�в�������ʹ����һ��ʱ������һ�Σ�bug���޸�

	std::vector<BulletBase*>::iterator it;
	for (it = bubsum.begin(); it != bubsum.end();)
	{
		if ((*it)->exist != true)
		{
			it = bubsum.erase(it);
		}
		else it++;
	}
	for (auto &bub : bubsum)
	{
		if (!m_player->ifcan_breakwall)  //û�д�ǽ���ߣ������ӵ���ǽ����ײ
		{
			if (!isCanReach(bub->point.x, bub->point.y))
			{
				bub->hide();
			}
		}
		
		for (auto pl : plsum)
		{
			if (bub->collidePlayer(pl))
			{
				Sprite* star = Sprite::create("attacked.png");
				star->setScale(1.5f, 1.5f);
				srand(time(NULL));
				auto randx = rand() % 20 - 10, randy = rand() % 20 - 10;
				star->setPosition(pl->x_coord + randx - x_move,
					pl->y_coord + randy - y_move);
				this->addChild(star);
				MoveBy* moveBy = MoveBy::create(0.2f, Point(0, 0));
				star->runAction(Sequence::create(moveBy, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, star)), NULL));
			}
		}
	}

}
void GamePlaying::runEvent()
{
	
	//if(waytorun)
		m_player->runway1(keys, m_smallplayer);
	//else m_player->runway2(pos, m_smallplayer);
}
void GamePlaying::runEvent_n(SIOClient * client, const std::string & data)
{
	keys_n[k_w] = keys_n[k_a] = keys_n[k_s] = keys_n[k_d] = false;

	char key1 = data.c_str()[0], key2 = 0;
	if (data.size() > 1) { key2 = data.c_str()[1]; }
	switch (key1)
	{
	case 'w':keys_n[k_w] = true; break;
	case 'a':keys_n[k_a] = true; break;
	case 's':keys_n[k_s] = true; break;
	case 'd':keys_n[k_d] = true; break;
	default:break;
	}
	if (key2)
	{
		switch (key2)
		{
		case 'w':keys_n[k_w] = true; break;
		case 'a':keys_n[k_a] = true; break;
		case 's':keys_n[k_s] = true; break;
		case 'd':keys_n[k_d] = true; break;
		default:break;
		}
	}
	n_player->runway1(keys_n, n_smallplayer);
}

void GamePlaying::attack()
{
	attackweapon(m_player->front);

	Point pos1, pos2 = pos;
	pos1.y = m_player->y_coord + pos.x - m_player->x_coord;
	pos1.x = m_player->x_coord - pos.y + m_player->y_coord;
	pos = pos1;
	attackweapon(m_player->leftside);
	pos = pos2;

	pos1.y = m_player->y_coord - pos.x + m_player->x_coord;
	pos1.x = m_player->x_coord + pos.y - m_player->y_coord;
	pos = pos1;
	attackweapon(m_player->rightside);
	pos = pos2;

	pos1.y = m_player->y_coord - pos.y + m_player->y_coord;
	pos1.x = m_player->x_coord - pos.x + m_player->x_coord;
	pos = pos1;
	attackweapon(m_player->back);
	pos = pos2;

	m_player->attackCD();
}

void GamePlaying::attackweapon(float num)
{
	if (num == 0)
		return;

	Point pos1 = pos;
	Point pos2 = pos;
	float xd = (pos1.y - m_player->y_coord) / 20;
	float yd = (pos1.x - m_player->x_coord) / 20;
	pos.x -= (num / 2 - 0.5) * xd;
	pos.y += (num / 2 - 0.5) * yd;
	for (int i = 0; i < num; i++)
	{
		if (m_player->weapon == 1)
		{
			auto Abullet = BulletBase::create();
			Abullet->bindSprite(Sprite::create("arrow.png"));
			Abullet->setPosition(Point(m_player->x_coord, m_player->y_coord));
			tiledmap->addChild(Abullet);

			float angle;
			float dx, dy;
			dx = pos.x - m_player->x_coord;
			dy = pos.y - m_player->y_coord;

			if (dy == 0)
			{
				if (dx >= 0)
					angle = 90;
				else angle = 270;
			}
			else
			{
				angle = atan(dx / dy) / 3.1416 * 180;
				if (dy < 0)
					angle += 180;
			}

			auto * rotateto = RotateTo::create(0, 45 + angle);
			Abullet->runAction(rotateto);

			Abullet->exist = true;
			bubsum.push_back(Abullet);

			Abullet->attacking(m_player, pos);
		}
		else if (m_player->weapon == 2)
		{
			int num = 300 * m_player->atkrange / 32;
			float length = sqrt((m_player->x_coord - pos.x)*(m_player->x_coord - pos.x) + (m_player->y_coord - pos.y)*(m_player->y_coord - pos.y));
			float xchange = (pos.x - m_player->x_coord) / length * 32;
			float ychange = (pos.y - m_player->y_coord) / length * 32;

			for (int i = 1; i <= num && m_player->x_coord + i * xchange >= 0 && m_player->x_coord + i * xchange <= 1600 && m_player->y_coord + i * ychange >= 0 && m_player->y_coord + i * ychange <= 1600; i++)
			{
				auto Abullet = BulletBase::create();
				Abullet->bindSprite(Sprite::create("ground.png"));
				Abullet->setPosition(Point(m_player->x_coord + i * xchange, m_player->y_coord + i * ychange));
				tiledmap->addChild(Abullet);
				Abullet->exist = true;
				bubsum.push_back(Abullet);
				Abullet->attacking(m_player, Point(m_player->x_coord + i * xchange, m_player->y_coord + i * ychange));
			}
		}
		else if (m_player->weapon == 3)
		{
			auto Abullet = BulletBase::create();
			Abullet->bindSprite(Sprite::create("knife.png"));
			Abullet->setPosition(Point(m_player->x_coord, m_player->y_coord));
			tiledmap->addChild(Abullet);

			float angle;
			float dx, dy;
			dx = pos.x - m_player->x_coord;
			dy = pos.y - m_player->y_coord;

			if (dy == 0)
			{
				if (dx >= 0)
					angle = 90;
				else angle = 270;
			}
			else
			{
				angle = atan(dx / dy) / 3.1416 * 180;
				if (dy < 0)
					angle += 180;
			}

			auto * rotateto = RotateTo::create(0, angle);
			Abullet->runAction(rotateto);

			Abullet->exist = true;
			bubsum.push_back(Abullet);
			Abullet->attacking(m_player, pos);
		}
		else if (m_player->weapon == 4)
		{
			auto Abullet = BulletBase::create();
			Abullet->bindSprite(Sprite::create("darts.png"));
			Abullet->setPosition(Point(m_player->x_coord, m_player->y_coord));
			tiledmap->addChild(Abullet);

			auto * rotateto = RotateTo::create(2.0f, 90);
			Abullet->runAction(rotateto);

			Abullet->exist = true;
			bubsum.push_back(Abullet);
			Abullet->attacking(m_player, pos);
		}
		pos.x += xd;
		pos.y -= yd;
	}
	pos = pos2;
}


void GamePlaying::menuStartScene(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

	//���ذ�ť֮ǰ��һ�лص����״̬
	smallmap_switch = true; 
	music_switch = true;
	mode_switch = true;

	auto sc = StartScene::createScene();        //���Ž�����л�����
	auto reScene = TransitionShrinkGrow::create(1.0f, sc);
	Director::getInstance()->replaceScene(reScene);
}

void GamePlaying::levelup()
{
	log("level up");
	

	/*auto flash_sprite = Sprite::create("flash.png");
	auto level_sprite = Sprite::create("level_sprite.png");
	float x = rect.origin.x + rect.size.width / 2;
	float y = rect.origin.y + rect.size.height / 2;
	flash_sprite->setPosition(Vec2(x, y));
	level_sprite->setPosition(Vec2(x, y));
	flash_sprite->setName("flash");
	level_sprite->setName("level up");
	this->addChild(flash_sprite);
	this->addChild(level_sprite);

	if (this->getChildByName("flash"))
	{
		this->getChildByName("flash")->removeFromParentAndCleanup(true);
		this->getChildByName("level up")->removeFromParentAndCleanup(true);
	}*/
	

	expPro->LvChange(m_player->level);
	expPro->player = m_player;
}
