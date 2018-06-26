#include "GamePlaying.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
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
bool network_switch = false;

int which_map = 1;
int which_player = 1;

int viewsize = 1;

std::vector<int> numbers(0);

bool isowner = true;  //����������Ѫ�������ж�
bool isconnect = false;

std::string hero_nature;

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

	MusicPrinter();
	MapPrinter();
	ScenePrinter();

	SmallmapPrinter();
	n_smallplayer->setVisible(false);

	ModePrinter();

	/*if (!isconnect)
	{
	schedule(schedule_selector(GamePlaying::EXP_grow), 0.15f);
	schedule(schedule_selector(GamePlaying::HP_grow), 2.0f);
	}*/
	if (isowner)
	{
		schedule(schedule_selector(GamePlaying::EXP_grow), 0.15f);
		schedule(schedule_selector(GamePlaying::HP_grow), 2.0f);
	}
	return true;
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

	PlayerPrinter();
	SettingPrinter();
	//////////////////////////////////////////////

	////////////////////////////////////////////////////
	//����
	//auto weaponMenuItem = MenuItemToggle::createWithCallback(
	//	CC_CALLBACK_1(GamePlaying::Weapon_change, this),
	//	MenuItemFont::create("weapon"),
	//	MenuItemFont::create("weapon"),
	//	MenuItemFont::create("weapon"),
	//	MenuItemFont::create("weapon"),
	//	NULL);

	//Menu* weaponmn = Menu::create(weaponMenuItem, NULL);
	//x = rect.origin.x + rect.size.width*(33.7f / 40.0f);
	//y = rect.origin.y + rect.size.height*(9.0f / 20.0f);
	//weaponmn->setPosition(Vec2(x, y));
	//this->addChild(weaponmn, 1);

	//////////////////////////////////////////////////////
	////��ͼ����
	/*auto tlmapMenuItem = MenuItemToggle::createWithCallback(
	CC_CALLBACK_1(GamePlaying::TLmap_change, this),
	MenuItemFont::create("normal"),
	MenuItemFont::create("explore"),
	MenuItemFont::create("overlook"),
	NULL);
	Menu* tlmapmn = Menu::create(tlmapMenuItem, NULL);
	x = rect.origin.x + rect.size.width*(33.7f / 40.0f);
	y = rect.origin.y + rect.size.height*(8.0f / 20.0f);
	tlmapmn->setPosition(Vec2(x, y));
	this->addChild(tlmapmn, 1);*/

	//////////////////////////////////////////////////////
	////��ǽ
	//auto breakwallMenuItem = MenuItemToggle::createWithCallback(
	//	CC_CALLBACK_1(GamePlaying::Breakwall_change, this),
	//	MenuItemFont::create("normal"),
	//	MenuItemFont::create("breakwall"),
	//	NULL);

	//Menu* breakwallmn = Menu::create(breakwallMenuItem, NULL);
	//x = rect.origin.x + rect.size.width*(33.7f / 40.0f);
	//y = rect.origin.y + rect.size.height*(7.0f / 20.0f);
	//breakwallmn->setPosition(Vec2(x, y));
	//this->addChild(breakwallmn, 1);

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

	//�����������
	++this->live;

	if (!SIOClient::setconnect(_sioClient, 1))
	{
		_sioClient = nullptr;
	}
	if (_sioClient)
	{
		_sioClient->on("IDJudge", CC_CALLBACK_2(GamePlaying::ID_judge, this));
		_sioClient->on("enemy", CC_CALLBACK_2(GamePlaying::addEnemy, this));
	}
	if (_sioClient)
	{
		_sioClient->on("HP position", CC_CALLBACK_2(GamePlaying::HP_recieve, this));
		_sioClient->on("EXP position", CC_CALLBACK_2(GamePlaying::EXP_recieve, this));
		_sioClient->on("HP remove", CC_CALLBACK_2(GamePlaying::HP_remove, this));
		_sioClient->on("EXP remove", CC_CALLBACK_2(GamePlaying::EXP_remove, this));
		_sioClient->on("n_player keys", CC_CALLBACK_2(GamePlaying::runEvent_n, this));
		_sioClient->on("n_player run", CC_CALLBACK_2(GamePlaying::playRun_n, this));
		_sioClient->on("attack", CC_CALLBACK_2(GamePlaying::attack_n, this));
		_sioClient->on("hero nature", CC_CALLBACK_2(GamePlaying::heronature_n, this));
		_sioClient->on("hurt", CC_CALLBACK_2(GamePlaying::hurt_n, this));
		_sioClient->on("restart point", CC_CALLBACK_2(GamePlaying::restart_n, this));
		_sioClient->on("red hp", CC_CALLBACK_2(GamePlaying::redhp_n, this));
	}

}
void GamePlaying::ID_judge(SIOClient * client, const std::string & data)
{
	int num = data.c_str()[0] - '0';
	if (num == 0)
	{
		isowner = true;
	}
	else if (num == 1)
	{
		isowner = false;
		this->unschedule(schedule_selector(GamePlaying::HP_grow));
		this->unschedule(schedule_selector(GamePlaying::EXP_grow));
	}
	hp_auto_arise.clear();
	exp_auto_arise.clear();
}
void GamePlaying::Network_Switch(Ref * Spender)
{
	network_switch = (network_switch ? false : true);
	if (_sioClient)
	{
		if (network_switch)
		{
			isconnect = true;

			char mapmess[5];
			sprintf(mapmess, "%d", which_map);
			_sioClient->emit("mapchose", mapmess);

			char enemy_mess[100];
			sprintf(enemy_mess, "%d %d %d %d %d %d", which_player,
				static_cast<int>(m_player->x_coord), static_cast<int>(m_player->y_coord),
				static_cast<int>(m_player->p_hp), m_player->hpLimit, m_player->weapon);
			//log("player %s", enemy_mess);
			_sioClient->emit("playerchose", enemy_mess);

			HPEXP_clear();
		}
		else
		{
			_sioClient = nullptr;
			isconnect = false;
			this->unschedule(schedule_selector(GamePlaying::HP_grow));
			this->unschedule(schedule_selector(GamePlaying::EXP_grow));
		}
	}
}

void GamePlaying::PlayerPrinter()
{
	srand(time(NULL));
	int randx, randy, flag_start;
	while (1)
	{
		flag_start = true;
		randx = (rand() % MAP_SIZE) / 2 + 80.0f;
		randy = (rand() % MAP_SIZE) / 2 + 80.0f;
		for (int xi = -32; xi <= 32; xi += 32)
		{
			for (int yj = -32; yj <= 32; yj += 32)
			{
				if (!isCanReach(randx + xi, randy + yj))
				{
					flag_start = false;
					break;
				}
			}
			if (!flag_start) { break; }
		}
		if (flag_start) { break; }
	}
	m_player->x_coord = randx;
	m_player->y_coord = randy;

	++this->live;
	m_player->number = which_player;
	char figure_choose[30];
	sprintf(figure_choose, "Player/Figure/player%d.png", m_player->number);
	m_player->sprite = Sprite::create(figure_choose);
	m_player->weapon = m_player->number;

	m_player->bindSprite(m_player->sprite);
	m_player->setScale(2.0f, 2.0f);
	m_player->sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_player->setPosition(Point(m_player->x_coord, m_player->y_coord));
	tiledmap->addChild(m_player, 10);

	//////////////////////////////////
	tiledmap->setPositionX(tiledmap->getPositionX() - m_player->x_coord / MAP_SIZE * 310);
	x_move += m_player->x_coord / MAP_SIZE * 310;
	tiledmap->setPositionY(tiledmap->getPositionY() - m_player->y_coord / MAP_SIZE * 800);
	y_move += m_player->y_coord / MAP_SIZE * 800;
	////////////////////////////////

	n_player->sprite = Sprite::create();
	n_player->bindSprite(n_player->sprite);
	n_player->x_coord = 0;
	n_player->y_coord = 0;
	n_player->setPosition(Vec2(n_player->x_coord, n_player->y_coord));
	n_player->number = 2;

	tiledmap->addChild(n_player, 10);

	plsum.push_back(m_player);
	plsum.push_back(n_player);

	////////////////////////////////////
	//Ѫ����ʼ��
	m_pProgressView = new ProgressView;
	m_pProgressView->setPosition(Vec2(m_player->x_coord, m_player->y_coord + 50));
	m_pProgressView->setScale(2);
	m_pProgressView->setBackgroundTexture("Player/Figure/background.png");
	m_pProgressView->setForegroundTexture("Player/Figure/foreground.png");
	m_pProgressView->setTotalProgress(1);
	m_pProgressView->setCurrentProgress(m_player->p_hp / m_player->hpLimit);
	tiledmap->addChild(m_pProgressView, 2);

	n_pProgressView = new ProgressView;

	///////////////////////////////////////////////
	this->scheduleUpdate();

	expPro = ExpProgress::create();
	expPro->player = m_player;
	addChild(expPro);
	expPro_n = ExpProgress::create();
	expPro_n->player = n_player;
}
void GamePlaying::SettingPrinter()
{
	float x, y;
	///////////////////////////////////
	//a return button which click to back to StartScene
	auto *return_button = MenuItemImage::create(
		"Scene/Buttons/closetoupper.png",
		"Scene/Buttons/closetoupper_select.png",
		CC_CALLBACK_1(GamePlaying::menuHelloWorldScene, this));

	auto *preturn = Menu::create(return_button, NULL);
	x = rect.origin.x + rect.size.width*(49.0f / 50.0f);
	y = rect.origin.y + rect.size.height*(48.0f / 50.0f);
	preturn->setPosition(Vec2(x, y));

	preturn->setScale(1.0f);
	this->addChild(preturn, 100);   //�ѷ��ذ�ť����100�㣬��ֹ�ڵ�

									///////////////////////////////////////////////
									//add network choice
	auto networkMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(GamePlaying::Network_Switch, this),
		MenuItemImage::create(
			"Scene/Buttons/network_normal.png",
			"Scene/Buttons/network_normal.png"),
		MenuItemImage::create(
			"Scene/Buttons/network_selected.png",
			"Scene/Buttons/network_selected.png"),
		NULL);
	networkMenuItem->setScale(0.5f);
	Menu* networkmn = Menu::create(networkMenuItem, NULL);
	x = rect.origin.x + rect.size.width*(46.0f / 50.0f);

	networkmn->setPosition(Vec2(x, y));
	this->addChild(networkmn, 1);

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
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/Escape.mp3");
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
		case 1:m_smallmap = Sprite::create("Player/Tiled Map/smallmap1.png"); break;
		case 2:m_smallmap = Sprite::create("Player/Tiled Map/smallmap2.png"); break;
		case 3:m_smallmap = Sprite::create("Player/Tiled Map/smallmap3.png"); break;
		default:m_smallmap = nullptr;
		}
		m_smallmap->setName("smallmap");

		m_smallmap->setOpacity(220);     //����С��ͼ��͸����
										 //m_smallmap->setColor(Color3B(0, 0, 205));
		m_smallmap->setAnchorPoint(Vec2(0.0f, 0.0f));
		x = rect.origin.x + rect.size.width*0.0f;
		y = rect.origin.y + rect.size.height*(2.0f / 3.0f - 0.02f);  //��0.02��Ϊ����ȥһ������С��λ��ƫ��
		m_smallmap->setPosition(Vec2(x, y));
		this->addChild(m_smallmap, 1);

		m_smallplayer = Player::create();
		m_smallplayer->bindSprite(Sprite::create("Player/Tiled Map/m_smallplayer.png"));
		m_smallplayer->setPosition(
			Vec2(m_player->x_coord*RETE, m_player->y_coord*RETE));
		m_smallmap->addChild(m_smallplayer);

		if (n_player->sprite)
		{
			n_smallplayer = Player::create();
			n_smallplayer->bindSprite(Sprite::create("Player/Tiled Map/n_smallplayer.png"));
			n_smallplayer->setPosition(
				Vec2(n_player->x_coord*RETE, n_player->y_coord*RETE));
			m_smallmap->addChild(n_smallplayer);
		}


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
	m_player->ifbreakwall = (m_player->ifbreakwall ? false : true);
	if (m_player->ifbreakwall)
	{
		auto flash = Sprite::create("Player/Figure/flash.png");
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

bool GamePlaying::up(bool flag, int ifxie)  //ifxieĬ�ϲ���Ϊfalse��Ĭ����ֱ����
{
	float x = m_player->getPositionX(), y = m_player->getPositionY();
	if (x < 0 || y < 0) { return false; }

	if (y + tileSize.height < MAP_SIZE && (m_player->ifbreakwall ||
		(isCanReach(x + DIFF, y)
			&& isCanReach(x - DIFF, y)
			&& isCanReach(x, y))))   //���ϵ��ж϶�+1������ǽbug
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
	if (x < 0 || y < 0) { return false; }

	if (x + tileSize.width < MAP_SIZE && (m_player->ifbreakwall ||
		(isCanReach(x + 2 * DIFF, y - DIFF)
			&& isCanReach(x + 2 * DIFF, y - 2 * DIFF))))
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
	if (x < 0 || y < 0) { return false; }

	if (x>tileSize.width && (m_player->ifbreakwall ||
		(isCanReach(x - 2 * DIFF, y - DIFF)
			&& isCanReach(x - 2 * DIFF, y - 2 * DIFF))))
	{
		if (flag)
		{
			runEvent();
			tofindEat(x, y);
		}
		if ((x + tiledmap->getPositionX() < size.width / 2 / TLMAP_SCALE)
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
	if (x < 0 || y < 0) { return false; }
	if (y>tileSize.height && (m_player->ifbreakwall ||
		(isCanReach(x, y - 4 * DIFF)
			&& isCanReach(x - DIFF, y - 4 * DIFF))))
	{
		if (flag)
		{
			runEvent();
			tofindEat(x, y);

			if (!ifxie)
			{
				if ((y + tiledmap->getPositionY() < size.height / 2 / TLMAP_SCALE)
					&& tiledmap->getPositionY() < 0)
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
	double mapX = x / 32.0;        //��ȥ16�����������ê��������
	double mapY = MAP_HEIGHT - y / 32.0;   //49ΪTiled���ͼ���������ֵ
	int tileGid = meta->tileGIDAt(Vec2(mapX, mapY)); //32��һ��Ĵ�С

													 //log("gid %d", tileGid);
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

		if (_sioClient && isconnect)
		{
			//red hp
			_sioClient->emit("red hp", "red hp");
		}

		if (m_player->magnet)   //��������������ܣ��ȵ����߲�����
		{
			auto mag = Sprite::create("Player/Figure/magnet_hp.png");
			mag->setScale(0.5f);
			mag->setName("mag");
			m_player->addChild(mag);
			MoveBy *moveBy = MoveBy::create(0.3f, ccp(0, 0));
			auto callbackFunc = [&]()
			{
				m_player->getChildByName("mag")->removeFromParentAndCleanup(true);
			};
			CallFunc* callFunc = CallFunc::create(callbackFunc);
			Action* actions = Sequence::create(moveBy, callFunc, NULL);
			mag->runAction(actions);
		}

		meta->setTileGID(NOR_GID, Vec2(static_cast<int>(pos.x), static_cast<int>(pos.y)));
		int judgex = static_cast<int>(pos.x), judgey = static_cast<int>(pos.y);
		//��Χfor��ʹ�ã������ҵ���ǰ�Ӵ��Ļ�Ѫ����
		//����Ӧ�ľ���ɾ������ɾ��vector����Ӧ��Ԫ��
		for (auto &hp_now : hp_auto_arise)
		{
			if (judgex == hp_now.savex && judgey == hp_now.savey)
			{
				if (_sioClient && isconnect)
				{
					char mess[10];
					sprintf(mess, "%d %d ", judgex, judgey);
					std::string HP_pos = mess;
					_sioClient->emit("HP remove", HP_pos);
				}

				hp_now.hp_potion->removeFromParentAndCleanup(true);
				auto hp_iter = std::find(hp_auto_arise.begin(), hp_auto_arise.end(), hp_now);
				hp_auto_arise.erase(hp_iter, hp_iter + 1);

				break;
			}
		}

	}

}
void GamePlaying::HP_grow(float dt)
{
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
	metax = ((rand() % MAP_WIDTH)*(rand() % MAP_WIDTH)) % MAP_WIDTH;
	metay = ((rand() % MAP_HEIGHT)*(rand() % MAP_HEIGHT)) % MAP_HEIGHT;

	int gid = meta->getTileGIDAt(Vec2(1.0*metax, 1.0*metay));
	if (GAP_GID != gid && HP_GID != gid && EXP_GID != gid)
	{
		meta->setTileGID(HP_GID, Vec2(1.0*metax, 1.0*metay));

		//��Ĺ��캯�������һ����Ѫ����
		hp_auto_arise.push_back(HP_MESS(Sprite::create("Player/Tiled Map/HP_tiledmap.png"), metax, metay));

		int now_vec_maxindex = hp_auto_arise.size() - 1;
		float spritex = metax * tileSize.width, spritey = (mapSize.height - metay)*tileSize.height;

		hp_auto_arise[now_vec_maxindex].hp_potion->setAnchorPoint(Vec2(0.0f, 1.0f));

		hp_auto_arise[now_vec_maxindex].hp_potion->setPosition(
			Vec2(spritex, spritey));
		tiledmap->addChild(hp_auto_arise[now_vec_maxindex].hp_potion);

		if (_sioClient && isowner && isconnect)
		{
			char mess[10];
			sprintf(mess, "%d %d ", metax, metay);
			std::string HP_pos = mess;
			_sioClient->emit("HP position", HP_pos);
		}

	}
}
void GamePlaying::EXPjudge(const Vec2 & pos)
{
	if (EXP_GID == meta->getTileGIDAt(pos))
	{
		//CCLOG("hero is in EXP_tiledmap");
		if (m_player->expraise(2)) { levelup(); }

		if (m_player->magnet)   //��������������ܣ��ȵ����߲�����
		{
			auto mag2 = Sprite::create("Player/Figure/magnet_exp.png");
			mag2->setScale(0.5f);
			mag2->setName("mag2");
			m_player->addChild(mag2);

			MoveBy *moveBy = MoveBy::create(0.3f, ccp(0, 0));
			auto callbackFunc = [&]()
			{
				m_player->getChildByName("mag2")->removeFromParentAndCleanup(true);
			};
			CallFunc* callFunc = CallFunc::create(callbackFunc);
			Action* actions = Sequence::create(moveBy, callFunc, NULL);
			mag2->runAction(actions);
		}
		meta->setTileGID(NOR_GID, Vec2(static_cast<int>(pos.x), static_cast<int>(pos.y)));
		int judgex = static_cast<int>(pos.x), judgey = static_cast<int>(pos.y);

		for (auto &exp_now : exp_auto_arise)
		{
			if (judgex == exp_now.savex && judgey == exp_now.savey)
			{
				if (_sioClient && isconnect)
				{
					char mess[10];
					sprintf(mess, "%d %d ", judgex, judgey);
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
		//exp_auto_arise[now_vec_maxindex].exp_potion->ignoreAnchorPointForPosition(false);
		exp_auto_arise[now_vec_maxindex].exp_potion->setAnchorPoint(Vec2(0.0f, 1.0f));

		exp_auto_arise[now_vec_maxindex].exp_potion->setPosition(
			Vec2(spritex, spritey));
		tiledmap->addChild(exp_auto_arise[now_vec_maxindex].exp_potion);

		if (_sioClient && isowner && isconnect)
		{
			char mess[10];
			sprintf(mess, "%d %d ", metax, metay);
			std::string EXP_pos = mess;
			_sioClient->emit("EXP position", EXP_pos);
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
void GamePlaying::HPEXP_clear()
{
	for (auto& hpnow : hp_auto_arise)
	{
		meta->setTileGID(NOR_GID, Vec2(hpnow.savex, hpnow.savey));
		hpnow.hp_potion->removeFromParentAndCleanup(true);
	}
	for (auto& expnow : exp_auto_arise)
	{
		meta->setTileGID(NOR_GID, Vec2(expnow.savex, expnow.savey));
		expnow.exp_potion->removeFromParentAndCleanup(true);
	}
	hp_auto_arise.clear();
	exp_auto_arise.clear();
}


void GamePlaying::DeCode_for_Map(const std::string & buf, int & metax, int & metay)
{
	std::string data(buf);
	data.erase(data.begin(), data.begin() + 1);
	data.erase(data.end() - 1, data.end());

	metax = metay = 0;
	int i;
	for (i = 0; data[i] != ' '; ++i)
	{
		metax *= 10;
		metax += data[i] - '0';
	}
	data.erase(data.begin(), data.begin() + i + 1);
	for (i = 0; data[i] != ' '; ++i)
	{
		metay *= 10;
		metay += data[i] - '0';
	}
}
void GamePlaying::DeCode_for_Run(const std::string & buf)
{
	std::string data(buf);
	data.erase(data.begin(), data.begin() + 1);
	data.erase(data.end() - 1, data.end());

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
}
void GamePlaying::DeCode_for_Playrun(const std::string & buf)
{
	std::string data(buf);
	data.erase(data.begin(), data.begin() + 1);
	data.erase(data.end() - 1, data.end());
	char key = data.c_str()[0];

	switch (key)
	{
	case 'w':keys1_n[k_w] = true; break;
	case 'a':keys1_n[k_a] = true; break;
	case 's':keys1_n[k_s] = true; break;
	case 'd':keys1_n[k_d] = true; break;
	default:break;
	}
}
void GamePlaying::DeCode_for_Enemy(const std::string & buf,
	int & v_player, float & v_posx, float & v_posy,
	float & v_hp, int & v_hplimit, int & v_weapon)
{
	int i;
	std::string data(buf);
	data.erase(data.begin(), data.begin() + 1);
	data.erase(data.end() - 1, data.end());
	//����which n_player
	v_player = data[0] - '0';
	data.erase(data.begin(), data.begin() + 2);
	//����n_player��x�����y����
	v_posx = v_posy = 0;
	for (i = 0; data[i] != ' '; ++i)
	{
		v_posx *= 10;
		v_posx += data[i] - '0';
	}
	data.erase(data.begin(), data.begin() + i + 1);
	for (i = 0; data[i] != ' '; ++i)
	{
		v_posy *= 10;
		v_posy += data[i] - '0';
	}
	data.erase(data.begin(), data.begin() + i + 1);
	//����n_player�ĵ�ǰhp��hp����
	v_hp = v_hplimit = 0;
	for (i = 0; data[i] != ' '; ++i)
	{
		v_hp *= 10;
		v_hp += data[i] - '0';
	}
	data.erase(data.begin(), data.begin() + i + 1);
	for (i = 0; data[i] != ' '; ++i)
	{
		v_hplimit *= 10;
		v_hplimit += data[i] - '0';
	}
	data.erase(data.begin(), data.begin() + i + 1);
	//����n_player ����
	v_weapon = data[0] - '0';
}
void GamePlaying::DeCode_for_attack(const std::string & buf, int & v_weapon,
	float &v_posx, float &v_posy)
{
	std::string data(buf);
	data.erase(data.begin(), data.begin() + 1);
	data.erase(data.end() - 1, data.end());

	v_weapon = data[0] - '0';
	data.erase(data.begin(), data.begin() + 2);

	int i;
	for (i = 0; data[i] != ' '; ++i)
	{
		v_posx *= 10;
		v_posx += data[i] - '0';
	}
	data.erase(data.begin(), data.begin() + i + 1);
	for (i = 0; data[i] != ' '; ++i)
	{
		v_posy *= 10;
		v_posy += data[i] - '0';
	}
}

void GamePlaying::DeCode_for_heronature(const std::string & buf,
	float & v_speed, float & v_p_hp, int & v_hpLimit,
	float & v_atkpower, float & v_atkrange, float & v_defpower,
	int & v_front, int & v_leftside, int & v_rightside, int & v_back, bool & v_ifcanbreakwall)
{
	std::string data(buf);
	data.erase(data.begin(), data.begin() + 1);
	data.erase(data.end() - 1, data.end());
	int i;
	//�ٶ�f
	v_speed = 0.0;
	for (i = 0; data[i] != ' '; ++i)
	{
		v_speed *= 10;
		v_speed += data[i] - '0';
	}
	v_speed /= 10.0;
	data.erase(data.begin(), data.begin() + i + 1);
	//��ǰѪ��f
	v_p_hp = 0.0;
	for (i = 0; data[i] != ' '; ++i)
	{
		v_p_hp *= 10;
		v_p_hp += data[i] - '0';
	}
	v_p_hp /= 10.0;
	data.erase(data.begin(), data.begin() + i + 1);
	//Ѫ������
	v_hpLimit = 0;
	for (i = 0; data[i] != ' '; ++i)
	{
		v_hpLimit *= 10;
		v_hpLimit += data[i] - '0';
	}
	data.erase(data.begin(), data.begin() + i + 1);
	//������f
	v_atkpower = 0.0;
	for (i = 0; data[i] != ' '; ++i)
	{
		v_atkpower *= 10;
		v_atkpower += data[i] - '0';
	}
	v_atkpower /= 10.0;
	data.erase(data.begin(), data.begin() + i + 1);
	//������Χf
	v_atkrange = 0.0;
	for (i = 0; data[i] != ' '; ++i)
	{
		v_atkrange *= 10;
		v_atkrange += data[i] - '0';
	}
	v_atkrange /= 10.0;
	data.erase(data.begin(), data.begin() + i + 1);
	//������f
	v_defpower = 0.0;
	for (i = 0; data[i] != ' '; ++i)
	{
		v_defpower *= 10;
		v_defpower += data[i] - '0';
	}
	v_defpower /= 10.0;
	data.erase(data.begin(), data.begin() + i + 1);
	//ǰ
	v_front = 0;
	for (i = 0; data[i] != ' '; ++i)
	{
		v_front *= 10;
		v_front += data[i] - '0';
	}
	data.erase(data.begin(), data.begin() + i + 1);
	//��
	v_leftside = 0;
	for (i = 0; data[i] != ' '; ++i)
	{
		v_leftside *= 10;
		v_leftside += data[i] - '0';
	}
	data.erase(data.begin(), data.begin() + i + 1);
	//��
	v_rightside = 0;
	for (i = 0; data[i] != ' '; ++i)
	{
		v_rightside *= 10;
		v_rightside += data[i] - '0';
	}
	data.erase(data.begin(), data.begin() + i + 1);
	//��
	v_back = 0;
	for (i = 0; data[i] != ' '; ++i)
	{
		v_back *= 10;
		v_back += data[i] - '0';
	}
	data.erase(data.begin(), data.begin() + i + 1);
	//��ǽ�ж�bool
	v_ifcanbreakwall = false;
	for (i = 0; data[i] != ' '; ++i)
	{
		v_ifcanbreakwall *= 10;
		v_ifcanbreakwall += data[i] - '0';
	}
	if (v_ifcanbreakwall == 1) { v_ifcanbreakwall = true; }
	else { v_ifcanbreakwall = false; }
}
void GamePlaying::DeCode_for_hurt(const std::string &buf, int &v_bloodmin)
{
	std::string data(buf);
	data.erase(data.begin(), data.begin() + 1);
	data.erase(data.end() - 1, data.end());

	for (int i = 0; data[i] != ' '; ++i)
	{
		v_bloodmin *= 10;
		v_bloodmin += data[i] - '0';
	}
}
void GamePlaying::DeCode_for_restart(const std::string & buf, float & v_posx, float & v_posy)
{
	std::string data(buf);
	data.erase(data.begin(), data.begin() + 1);
	data.erase(data.end() - 1, data.end());

	v_posx = v_posy = 0.0f;
	int i;
	for (i = 0; data[i] != ' '; ++i)
	{
		v_posx *= 10;
		v_posx += data[i] - '0';
	}
	data.erase(data.begin(), data.begin() + i + 1);
	for (i = 0; data[i] != ' '; ++i)
	{
		v_posy *= 10;
		v_posy += data[i] - '0';
	}
}

void GamePlaying::runEvent_n(SIOClient * client, const std::string & data)
{
	keys_n[k_w] = keys_n[k_a] = keys_n[k_s] = keys_n[k_d] = false;

	DeCode_for_Run(data);

	n_player->runway1(keys_n, n_smallplayer);
}
void GamePlaying::playRun_n(SIOClient * client, const std::string & data)
{
	keys1_n[k_w] = keys1_n[k_a] = keys1_n[k_s] = keys1_n[k_d] = false;

	DeCode_for_Playrun(data);

	n_player->runanimate(keys1_n);
}
void GamePlaying::attack_n(SIOClient * client, const std::string & data)
{
	Point point_n;

	DeCode_for_attack(data, n_player->weapon, point_n.x, point_n.y);

	attack_nn(n_player, point_n);
}
void GamePlaying::heronature_n(SIOClient * client, const std::string & data)
{
	DeCode_for_heronature(data, n_player->speed, n_player->p_hp, n_player->hpLimit,
		n_player->atkpower, n_player->atkrange, n_player->defpower,
		n_player->front, n_player->leftside, n_player->rightside, n_player->back,
		n_player->ifcan_breakwall);
}
void GamePlaying::hurt_n(SIOClient * client, const std::string & data)
{
	int bloodmin = 0;

	DeCode_for_hurt(data, bloodmin);

	m_player->hurt(bloodmin);
}
void GamePlaying::restart_n(SIOClient * client, const std::string & data)
{
	float restartx, restarty;

	DeCode_for_restart(data, restartx, restarty);

	m_player->die(restartx, restarty);

	if (this->getChildByName("smallmap"))
	{
		this->getChildByName("smallmap")->removeFromParentAndCleanup(true);
		SmallmapPrinter();
	}
}
void GamePlaying::redhp_n(SIOClient * client, const std::string & data)
{
	n_player->hpraise(1);
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
void GamePlaying::addEnemy(SIOClient * client, const std::string & data)
{
	DeCode_for_Enemy(data, n_player->number, n_player->x_coord, n_player->y_coord,
		n_player->p_hp, n_player->hpLimit, n_player->weapon);

	char figure_choose[30];
	sprintf(figure_choose, "Player/Figure/player%d.png", n_player->number);
	n_player->sprite = Sprite::create(figure_choose);

	n_player->setScale(2.0, 2.0);
	n_player->bindSprite(n_player->sprite);
	n_player->setPosition(Point(n_player->x_coord, n_player->y_coord));

	n_pProgressView->setPosition(Vec2(n_player->x_coord, n_player->y_coord + 50));
	n_pProgressView->setScale(2);
	n_pProgressView->setBackgroundTexture("Player/Figure/background.png");
	n_pProgressView->setForegroundTexture("Player/Figure/foreground.png");
	n_pProgressView->setTotalProgress(1);
	n_pProgressView->setCurrentProgress(n_player->p_hp / n_player->hpLimit);
	tiledmap->addChild(n_pProgressView, 2);

	if (this->getChildByName("smallmap"))
		this->getChildByName("smallmap")->removeFromParentAndCleanup(true);

	SmallmapPrinter();
	n_smallplayer->setVisible(true);
}


void GamePlaying::onEnter()
{
	Layer::onEnter();

	keylistener = EventListenerKeyboard::create();
	mouselistener = EventListenerMouse::create();

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

	eventDispatcher1 = Director::getInstance()->getEventDispatcher();
	eventDispatcher1->addEventListenerWithSceneGraphPriority(keylistener, this);

	eventDispatcher2 = Director::getInstance()->getEventDispatcher();
	eventDispatcher2->addEventListenerWithSceneGraphPriority(mouselistener, this);

}
void GamePlaying::update(float delta)
{
	if (expPro->ifchose)
	{
		if (_sioClient && isconnect)
		{
			_sioClient->emit("hero nature", hero_nature);
		}
		expPro->ifchose = 0;
	}

	float x = m_player->getPositionX(), y = m_player->getPositionY();
	m_player->x_coord = x; m_player->y_coord = y;
	//CCLOG("x=%f , y=%f", x, y);
	/////////////////////////////////////////
	//Ѫ��λ��&�����趨
	m_pProgressView->setCurrentProgress(m_player->p_hp / m_player->hpLimit);
	m_pProgressView->setPosition(Vec2(m_player->x_coord, m_player->y_coord + 50));
	n_pProgressView->setCurrentProgress(n_player->p_hp / n_player->hpLimit);
	n_pProgressView->setPosition(Vec2(n_player->x_coord, n_player->y_coord + 50));
	m_pProgressView->setScale((float)m_player->hpLimit / 50 * 2, 2);
	n_pProgressView->setScale((float)n_player->hpLimit / 50 * 2, 2);

	if (m_player->level == 11)
		expPro->ExpChange(1, 1);
	else
		expPro->ExpChange(m_player->exp, m_player->explimit());

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


	//������
	int releft = 0;
	Player* repl;
	for (auto pl : plsum)
	{
		if (pl->life)
		{
			releft++;
			repl = pl;
		}
		if (pl != m_player && pl->life)
		{
			float dx = m_player->x_coord - pl->x_coord;
			float dy = m_player->y_coord - pl->y_coord;
			if (dx < 0)dx = -dx;
			if (dy < 0)dy = -dy;
			if (dx > 65 || dy > 65)
				continue;
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
	if (live > 1 && releft == 1)
	{
		if (repl == m_player)
			expPro->youwin();
		else expPro->youlose();
		live = 0;
		eventDispatcher1->removeEventListener(keylistener);
		eventDispatcher2->removeEventListener(mouselistener);
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
				if (_sioClient && isconnect)
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
				if (_sioClient && isconnect)
					_sioClient->emit("n_player keys", "w");
				up(true);
			}
			else if (!flagup && flagright)
			{
				keys[k_w] = false;
				if (_sioClient && isconnect)
					_sioClient->emit("n_player keys", "d");
				right(true);
			}

		}
		else if (keys[k_w] && keys[k_a])
		{
			bool flagup = up(false), flagleft = left(false);
			if (flagup && flagleft)
			{
				if (_sioClient && isconnect)
					_sioClient->emit("n_player keys", "wa");
				if ((x + tiledmap->getPositionX() < size.width / 2)
					&& tiledmap->getPositionX() < 0)
				{
					up(true, 2);
				}
				else { up(true); }
			}
			else if (flagup && !flagleft)
			{
				keys[k_a] = false;
				if (_sioClient && isconnect)
					_sioClient->emit("n_player keys", "w");
				up(true);
			}
			else if (!flagup && flagleft)
			{
				keys[k_w] = false;
				if (_sioClient && isconnect)
					_sioClient->emit("n_player keys", "a");
				left(true);
			}
		}
		else if (keys[k_a] && keys[k_s])
		{
			bool flagleft = left(false), flagdown = down(false);
			if (flagleft && flagdown)
			{
				if (_sioClient && isconnect)
					_sioClient->emit("n_player keys", "as");
				if ((x + tiledmap->getPositionX() < size.width / 2)
					&& tiledmap->getPositionX() < 0)
				{
					down(true, 2);
				}
				else { down(true); }
			}
			else if (flagleft && !flagdown)
			{
				keys[k_s] = false;
				if (_sioClient && isconnect)
					_sioClient->emit("n_player keys", "a");
				left(true);
			}
			else if (!flagleft && flagdown)
			{
				keys[k_a] = false;
				if (_sioClient && isconnect)
					_sioClient->emit("n_player keys", "s");
				down(true);
			}
		}
		else if (keys[k_s] && keys[k_d])
		{
			bool flagdown = down(false), flagright = right(false);
			if (flagdown && flagright)
			{
				if (_sioClient && isconnect)
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
				if (_sioClient && isconnect)
					_sioClient->emit("n_player keys", "s");
				down(true);
			}
			else if (!flagdown && flagright)
			{
				keys[k_s] = false;
				if (_sioClient && isconnect)
					_sioClient->emit("n_player keys", "d");
				right(true);
			}
		}
		else if (keys[k_w])
		{
			bool flagup = up(false);
			if (flagup)
			{
				if (_sioClient && isconnect)
					_sioClient->emit("n_player keys", "w");
				up(true);
			}

		}
		else if (keys[k_a])
		{
			bool flagleft = left(false);
			if (flagleft)
			{
				if (_sioClient && isconnect)
					_sioClient->emit("n_player keys", "a");
				left(true);
			}

		}
		else if (keys[k_s])
		{
			bool flagdown = down(false);
			if (flagdown)
			{
				if (_sioClient && isconnect)
					_sioClient->emit("n_player keys", "s");
				down(true);
			}

		}
		else if (keys[k_d])
		{
			bool flagright = right(false);
			if (flagright)
			{
				if (_sioClient && isconnect)
					_sioClient->emit("n_player keys", "d");
				right(true);
			}

		}
	}

	m_player->runanimate(keys1);

	if (keys1[k_w])
	{
		if (_sioClient && isconnect)
			_sioClient->emit("n_player run", "w");
	}
	else if (keys1[k_s])
	{
		if (_sioClient && isconnect)
			_sioClient->emit("n_player run", "s");
	}
	else if (keys1[k_d])
	{
		if (_sioClient && isconnect)
			_sioClient->emit("n_player run", "d");
	}
	else if (keys1[k_a])
	{
		if (_sioClient && isconnect)
			_sioClient->emit("n_player run", "a");
	}

	if (touchon && !m_player->attackcd)
	{
		attack(m_player, pos);

		if (_sioClient && isconnect)
		{
			char mess[20];
			sprintf(mess, "%d %d %d ", m_player->weapon, static_cast<int>(pos.x),
				static_cast<int>(pos.y));
			_sioClient->emit("attack", mess);
		}

		touchon = false;
	}
	else touchon = false;
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
	////////////////////////////////////
	for (auto bub : bubsum)
	{
		if (!bub->comefrom->ifcan_breakwall)  //û�д�ǽ���ߣ������ӵ���ǽ����ײ
		{
			if (!isCanReach(bub->point.x, bub->point.y))
			{
				bub->hide();
			}
		}
	}
	for (auto bub : bubsum)
	{
		for (auto pl : plsum)
		{
			int a = bub->collidePlayer(pl);

			if (a && bub->real)
			{
				bub->real = false;

				if (_sioClient && isconnect)
				{
					char hurtmess[5];
					sprintf(hurtmess, "%d ", a);
					_sioClient->emit("hurt", hurtmess);
				}

				if (pl->hurt(a))
				{
					srand(time(NULL));
					int randx, randy, flag_start;
					while (1)
					{
						flag_start = true;
						randx = (rand() % MAP_SIZE) / 2 + 80.0f;
						randy = (rand() % MAP_SIZE) / 2 + 80.0f;
						for (int xi = -32; xi <= 32; xi += 32)
						{
							for (int yj = -32; yj <= 32; yj += 32)
							{
								if (!isCanReach(randx + xi, randy + yj))
								{
									flag_start = false;
									break;
								}
							}
							if (!flag_start) { break; }
						}
						if (flag_start) { break; }
					}
					pl->die(randx, randy);

					if (_sioClient && isconnect)
					{
						char restart_mess[10];
						sprintf(restart_mess, "%d %d ", randx, randy);
						_sioClient->emit("restart point", restart_mess);
					}

					//tiledmap->setPositionX(tiledmap->getPositionX() - m_player->x_coord / MAP_SIZE * 310);
					//x_move += m_player->x_coord / MAP_SIZE * 310;
					//tiledmap->setPositionY(tiledmap->getPositionY() - m_player->y_coord / MAP_SIZE * 800);
					//y_move += m_player->y_coord / MAP_SIZE * 800;

					if (this->getChildByName("smallmap"))
					{
						this->getChildByName("smallmap")->removeFromParentAndCleanup(true);
						SmallmapPrinter();
					}
					

					if (bub->comefrom->expraise(pl->level * 5))
						levelup();
				}
			}
		}
	}


}
void GamePlaying::runEvent()
{
	m_player->runway1(keys, m_smallplayer);
}
void GamePlaying::attack(Player* player, Point point)
{
	Point p;
	p.x = player->x_coord;
	p.y = player->y_coord;

	attackweapon(player, player->front, point);

	Point pos1;
	pos1.y = player->y_coord + point.x - player->x_coord;
	pos1.x = player->x_coord - point.y + player->y_coord;
	attackweapon(player, player->leftside, pos1);

	pos1.y = player->y_coord - point.x + player->x_coord;
	pos1.x = player->x_coord + point.y - player->y_coord;
	attackweapon(player, player->rightside, pos1);

	pos1.y = player->y_coord - point.y + player->y_coord;
	pos1.x = player->x_coord - point.x + player->x_coord;
	attackweapon(player, player->back, pos1);

	player->attackCD();
}
void GamePlaying::attackweapon(Player* player, float num, Point point)
{
	if (num == 0)
		return;

	Point atkpoint = point;//�����ӵ�������

	float xd = (point.y - player->y_coord) / 20;
	float yd = (point.x - player->x_coord) / 20;

	atkpoint.x -= (num / 2 - 0.5) * xd;
	atkpoint.y += (num / 2 - 0.5) * yd;
	for (int i = 0; i < num; i++)
	{
		if (player->weapon == 1)
		{
			auto Abullet = BulletBase::create();
			Abullet->bindSprite(Sprite::create("Player/Weapon/arrow.png"));
			Abullet->setPosition(Point(player->x_coord, player->y_coord));
			tiledmap->addChild(Abullet);

			float angle;
			float dx, dy;
			dx = atkpoint.x - player->x_coord;
			dy = atkpoint.y - player->y_coord;

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

			Abullet->attacking(player, atkpoint);
		}
		else if (player->weapon == 2)
		{
			int num = 300 * player->atkrange / 32;
			float length = sqrt((player->x_coord - atkpoint.x)*(player->x_coord - atkpoint.x)
				+ (player->y_coord - atkpoint.y)*(player->y_coord - atkpoint.y));
			float xchange = (atkpoint.x - player->x_coord) / length * 32;
			float ychange = (atkpoint.y - player->y_coord) / length * 32;

			for (int i = 1; i <= num && player->x_coord + i * xchange >= 0 && player->x_coord + i * xchange
				<= 1600 && player->y_coord + i * ychange >= 0 && player->y_coord + i * ychange <= 1600; i++)
			{
				auto Abullet = BulletBase::create();
				Abullet->bindSprite(Sprite::create("Player/Weapon/ground.png"));
				Abullet->setPosition(Point(player->x_coord + i * xchange, player->y_coord + i * ychange));
				tiledmap->addChild(Abullet);
				Abullet->exist = true;
				bubsum.push_back(Abullet);
				Abullet->attacking(player, Point(player->x_coord + i * xchange, player->y_coord + i * ychange));
			}
		}
		else if (player->weapon == 3)
		{
			auto Abullet = BulletBase::create();
			Abullet->bindSprite(Sprite::create("Player/Weapon/knife.png"));
			Abullet->setPosition(Point(player->x_coord, player->y_coord));
			tiledmap->addChild(Abullet);

			float angle;
			float dx, dy;
			dx = atkpoint.x - player->x_coord;
			dy = atkpoint.y - player->y_coord;

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
			Abullet->attacking(player, atkpoint);
		}
		else if (player->weapon == 4)
		{
			auto Abullet = BulletBase::create();
			Abullet->bindSprite(Sprite::create("Player/Weapon/darts.png"));
			Abullet->setPosition(Point(player->x_coord, player->y_coord));
			tiledmap->addChild(Abullet);

			auto * rotateto = RotateTo::create(0.5f, 180);
			RepeatForever* Rotate = RepeatForever::create(rotateto);
			Abullet->runAction(Rotate);

			Abullet->exist = true;
			bubsum.push_back(Abullet);
			Abullet->attacking(player, atkpoint);
		}
		atkpoint.x += xd;
		atkpoint.y -= yd;
	}
}
void GamePlaying::attack_nn(Player* player, Point point)
{
	Point p;
	p.x = player->x_coord;
	p.y = player->y_coord;

	attackweapon_nn(player, player->front, point);

	Point pos1;
	pos1.y = player->y_coord + point.x - player->x_coord;
	pos1.x = player->x_coord - point.y + player->y_coord;
	attackweapon_nn(player, player->leftside, pos1);

	pos1.y = player->y_coord - point.x + player->x_coord;
	pos1.x = player->x_coord + point.y - player->y_coord;
	attackweapon_nn(player, player->rightside, pos1);

	pos1.y = player->y_coord - point.y + player->y_coord;
	pos1.x = player->x_coord - point.x + player->x_coord;
	attackweapon_nn(player, player->back, pos1);
}
void GamePlaying::attackweapon_nn(Player* player, float num, Point point)
{
	if (num == 0)
		return;

	Point atkpoint = point;//�����ӵ�������

	float xd = (point.y - player->y_coord) / 20;
	float yd = (point.x - player->x_coord) / 20;

	atkpoint.x -= (num / 2 - 0.5) * xd;
	atkpoint.y += (num / 2 - 0.5) * yd;
	for (int i = 0; i < num; i++)
	{
		if (player->weapon == 1)
		{
			auto Abullet = BulletBase::create();
			Abullet->real = false;
			Abullet->bindSprite(Sprite::create("Player/Weapon/arrow.png"));
			Abullet->setPosition(Point(player->x_coord, player->y_coord));
			tiledmap->addChild(Abullet);

			float angle;
			float dx, dy;
			dx = atkpoint.x - player->x_coord;
			dy = atkpoint.y - player->y_coord;

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

			Abullet->attacking(player, atkpoint);
		}
		else if (player->weapon == 2)
		{
			int num = 300 * player->atkrange / 32;
			float length = sqrt((player->x_coord - atkpoint.x)*(player->x_coord - atkpoint.x)
				+ (player->y_coord - atkpoint.y)*(player->y_coord - atkpoint.y));
			float xchange = (atkpoint.x - player->x_coord) / length * 32;
			float ychange = (atkpoint.y - player->y_coord) / length * 32;

			for (int i = 1; i <= num && player->x_coord + i * xchange >= 0 && player->x_coord + i * xchange
				<= 1600 && player->y_coord + i * ychange >= 0 && player->y_coord + i * ychange <= 1600; i++)
			{
				auto Abullet = BulletBase::create();
				Abullet->real = false;
				Abullet->bindSprite(Sprite::create("Player/Weapon/ground.png"));
				Abullet->setPosition(Point(player->x_coord + i * xchange, player->y_coord + i * ychange));
				tiledmap->addChild(Abullet);
				Abullet->exist = true;
				bubsum.push_back(Abullet);
				Abullet->attacking(player, Point(player->x_coord + i * xchange, player->y_coord + i * ychange));
			}
		}
		else if (player->weapon == 3)
		{
			auto Abullet = BulletBase::create();
			Abullet->real = false;
			Abullet->bindSprite(Sprite::create("Player/Weapon/knife.png"));
			Abullet->setPosition(Point(player->x_coord, player->y_coord));
			tiledmap->addChild(Abullet);

			float angle;
			float dx, dy;
			dx = atkpoint.x - player->x_coord;
			dy = atkpoint.y - player->y_coord;

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
			Abullet->attacking(player, atkpoint);
		}
		else if (player->weapon == 4)
		{
			auto Abullet = BulletBase::create();
			Abullet->real = false;
			Abullet->bindSprite(Sprite::create("Player/Weapon/darts.png"));
			Abullet->setPosition(Point(player->x_coord, player->y_coord));
			tiledmap->addChild(Abullet);

			auto * rotateto = RotateTo::create(0.5f, 180);
			RepeatForever* Rotate = RepeatForever::create(rotateto);
			Abullet->runAction(Rotate);

			Abullet->exist = true;
			bubsum.push_back(Abullet);
			Abullet->attacking(player, atkpoint);
		}
		atkpoint.x += xd;
		atkpoint.y -= yd;
	}
}

void GamePlaying::levelup()
{
	m_player->speed -= 0.1;

	if (_sioClient && isconnect)
	{
		char hero_mess[40];
		sprintf(hero_mess, "%d %d %d %d %d %d %d %d %d %d %d ",
			static_cast<int>(m_player->speed * 10), static_cast<int>(m_player->p_hp * 10), m_player->hpLimit,
			static_cast<int>(m_player->atkpower * 10), static_cast<int>(m_player->atkrange * 10),
			static_cast<int>(m_player->defpower * 10),
			m_player->front, m_player->leftside, m_player->rightside, m_player->back,
			(m_player->ifcan_breakwall ? 1 : 0));

		hero_nature = hero_mess;
		_sioClient->emit("hero nature", hero_nature);
	}


	expPro->LvChange(m_player->level);
}

void GamePlaying::menuHelloWorldScene(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

	if (_sioClient)
	{
		_sioClient->emit("quit", "quit");
		//CC_SAFE_DELETE(_sioClient);
		_sioClient = nullptr;
	}


	//���ذ�ť֮ǰ��һ�лص����״̬
	smallmap_switch = true;
	music_switch = true;
	mode_switch = true;
	isowner = true;
	isconnect = false;

	auto sc = HelloWorld::createScene();        //���Ž�����л�����
	auto reScene = TransitionShrinkGrow::create(1.0f, sc);
	Director::getInstance()->replaceScene(reScene);
}