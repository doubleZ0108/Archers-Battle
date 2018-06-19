#include "Start.h"
#include "HelloWorldScene.h"
#include "GamePlaying.h"
#include "MapChose.h"
#include "PlayerChose.h"

USING_NS_CC;

extern bool language_flag;  //true->English   false->Chinese
extern int is_paused;       //关于is_paused的具体解释请见 "HelloWorldScene.h"
extern char *FontToUTF8(const char* font);
//it is define in another .cpp file 
//and it is used to change character

Scene* StartScene::createScene()
{
	is_paused = reply_music;   //进入正式游戏后吧初始音乐设为reply_music
	//正式进入游戏后会切换到新的游戏音乐，并把欢迎界面的音乐设为stop
	//再次返回到欢迎界面的时候回从头播放音乐
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
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
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
void StartScene::onConnect(SIOClient * client)
{
}
void StartScene::onMessage(SIOClient * client, const std::string & data)
{
}
void StartScene::onError(SIOClient * client, const std::string & data)
{
}
void StartScene::onClose(SIOClient * client)
{
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
	this->addChild(preturn,100);   //把返回按钮置于100层，防止遮挡
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
		mapchoseword = Label::create(FontToUTF8("地图选择"),
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
		playerchoseword = Label::create(FontToUTF8("人物选择"),
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
	auto sc = HelloWorld::createScene();        //缩放交替的切换动画
	auto reScene = TransitionShrinkGrow::create(1.0f, sc);
	Director::getInstance()->replaceScene(reScene);
}

void StartScene::menuToPlayButton(Ref * pSender)
{
	auto sc = GamePlaying::createScene();        //缩放交替的切换动画
	auto reScene = TransitionFade::create(2.0f, sc);
	Director::getInstance()->replaceScene(reScene);
}

void StartScene::menuMapChoseButton(Ref * pSender)
{
	auto sc = MapChose::createScene();
	auto reScene = TransitionSlideInL::create(1.0f, sc);  //从左边推入的动画
	Director::getInstance()->replaceScene(reScene);
}

void StartScene::menuPlayerChoseButton(Ref * pSender)
{
	auto sc = PlayerChose::createScene();        //缩放交替的切换动画
	auto reScene = TransitionSlideInR::create(1.0f, sc);  //从右边推入的动画
	Director::getInstance()->replaceScene(reScene);
}
