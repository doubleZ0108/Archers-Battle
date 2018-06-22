#include "Start.h"
#include "HelloWorldScene.h"
#include "GamePlaying.h"
#include "MapChose.h"
#include "PlayerChose.h"

USING_NS_CC;

extern bool language_flag;  //true->English   false->Chinese
extern int is_paused;       //����is_paused�ľ��������� "HelloWorldScene.h"
extern char *FontToUTF8(const char* font);
//it is define in another .cpp file 
//and it is used to change character

Scene* StartScene::createScene()
{
	is_paused = reply_music;   //������ʽ��Ϸ��ɳ�ʼ������Ϊreply_music
	//��ʽ������Ϸ����л����µ���Ϸ���֣����ѻ�ӭ�����������Ϊstop
	//�ٴη��ص���ӭ�����ʱ��ش�ͷ��������
	//////////////////////////////////
	auto scene = Scene::create();
	auto layer = StartScene::create();
	scene->addChild(layer);
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in StartScene.cpp\n");
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	ScenePrinter();

	return true;
}

void StartScene::ScenePrinter()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float x = rect.origin.x + rect.size.width / 2;
	float y = rect.origin.y + rect.size.height / 2;
	
	///////////////////////////////////
	//a return button which click to back to HelloWorldScene
	auto *return_button = MenuItemImage::create(
		"backtoupper.png",
		"backtoupper_select.png",
		CC_CALLBACK_1(StartScene::menuHellowWorldScene, this));

	auto *preturn = Menu::create(return_button, NULL);
	x = rect.origin.x + rect.size.width*(10.0f / 11.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 10.0f);
	preturn->setPosition(Vec2(x, y));

	preturn->setScale(1.0f);
	this->addChild(preturn,100);   //�ѷ��ذ�ť����100�㣬��ֹ�ڵ�
	///////////////////////////////////////////////
	//add blue_background

	x = rect.origin.x + rect.size.width / 2;
	y = rect.origin.y + rect.size.height / 2;
	auto *background = Sprite::create("blue_background.png");
	background->setPosition(Vec2(x, y));
	this->addChild(background);
	/////////////////////////////////////
	//add start background
	auto toplaybutton = MenuItemImage::create(
		"ToPlay_normal.png",
		"ToPlay_select.png",
		CC_CALLBACK_1(StartScene::menuToPlayButton, this));
	
	auto *toplay = Menu::create(toplaybutton, NULL);
	y = rect.origin.y + rect.size.height*(1.0f / 2.0f);
	toplay->setPosition(Vec2(x, y));
	toplay->setAnchorPoint(Vec2(0.0f, 0.0f));
	toplay->setScale(2.0f);
	this->addChild(toplay);
	///////////////////////////////////
	//add some botton to let player to chose maps and players
	auto mapchosebutton = MenuItemImage::create(
		"button.png",
		"button2.png",
		CC_CALLBACK_1(StartScene::menuMapChoseButton, this));
	mapchosebutton->setScale(1.5f);
	Label *mapchoseword;
	if (language_flag)
	{
		mapchoseword = Label::create("Chose Map", "Arial", 40);
	}
	else
	{
		mapchoseword = Label::create(FontToUTF8("��ͼѡ��"),
			"Arial", 40);
	}
	auto *pmapchoseword = MenuItemLabel::create(mapchoseword,
		CC_CALLBACK_1(StartScene::menuMapChoseButton, this));
	auto *mapchose = Menu::create(mapchosebutton, pmapchoseword, NULL);
	x = rect.origin.x + rect.size.width*(1.0f / 4.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 3.0f);
	mapchose->setPosition(Vec2(x, y));
	mapchose->setAnchorPoint(Vec2(0.0f, 0.0f));
	this->addChild(mapchose);

	auto playerchosebutton = MenuItemImage::create(
		"button.png",
		"button2.png",
		CC_CALLBACK_1(StartScene::menuPlayerChoseButton, this));
	playerchosebutton->setScale(1.5f);
	Label *playerchoseword;
	if (language_flag)
	{
		playerchoseword = Label::create("Player Chose", "Arial", 40);
	}
	else
	{
		playerchoseword = Label::create(FontToUTF8("����ѡ��"),
			"Arial", 40);
	}
	auto *pplayerchoseword = MenuItemLabel::create(playerchoseword,
		CC_CALLBACK_1(StartScene::menuPlayerChoseButton, this));
	auto *playerchose = Menu::create(playerchosebutton, pplayerchoseword, NULL);
	x = rect.origin.x + rect.size.width*(3.0f / 4.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 3.0f);
	playerchose->setPosition(Vec2(x, y));
	playerchose->setAnchorPoint(Vec2(0.0f, 0.0f));
	this->addChild(playerchose);

}

void StartScene::menuHellowWorldScene(Ref* pSender)
{
	auto sc = HelloWorld::createScene();        //���Ž�����л�����
	auto reScene = TransitionShrinkGrow::create(1.0f, sc);
	Director::getInstance()->replaceScene(reScene);
}

void StartScene::menuToPlayButton(Ref * pSender)
{
	auto sc = GamePlaying::createScene();        //���Ž�����л�����
	auto reScene = TransitionFade::create(2.0f, sc);
	Director::getInstance()->replaceScene(reScene);
}

void StartScene::menuMapChoseButton(Ref * pSender)
{
	auto sc = MapChose::createScene();
	auto reScene = TransitionSlideInL::create(1.0f, sc);  //���������Ķ���
	Director::getInstance()->replaceScene(reScene);
}

void StartScene::menuPlayerChoseButton(Ref * pSender)
{
	auto sc = PlayerChose::createScene();        //���Ž�����л�����
	auto reScene = TransitionSlideInR::create(1.0f, sc);  //���ұ�����Ķ���
	Director::getInstance()->replaceScene(reScene);
}
