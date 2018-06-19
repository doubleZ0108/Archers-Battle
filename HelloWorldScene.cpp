#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Setting.h"
#include "Start.h"
#include "Help.h"
USING_NS_CC;

bool language_flag = true;     //true->English   false->Chinese
int is_paused = reply_music;   //关于is_paused的具体解释请见 "HelloWorldScene.h"
extern char *FontToUTF8(const char* font);
//it is define in another .cpp file 
//and it is used to change character

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	//MusicPrinter();   //draw the background music
	ScenePrinter();   //draw all the thing that player can see

    return true;
}
void HelloWorld::ScenePrinter()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	///////////////////////////////////////
	//add HelloScene_Background
	auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float x = rect.origin.x + rect.size.width / 2;
	float y = rect.origin.y + rect.size.height / 2;
	auto *background = Sprite::create("HelloScene_Background.png");
	background->setPosition(Vec2(x, y));
	this->addChild(background);
	///////////////////////////////////////
	//add title "Archer Battle"
	Label *title;
	if (language_flag)
	{
		title = Label::createWithTTF("Archer Battle",
			"fonts/Marker Felt.ttf", 100);
	}
	else
	{
		title = Label::create(FontToUTF8("弓箭手大作战"),
			"Arial", 100);
	}
	y = rect.origin.y + rect.size.height*0.618f;
	title->setPosition(Vec2(x, y));
	title->setAnchorPoint(Vec2(0.5f, 0.0f));
	title->enableShadow(Color4B::YELLOW, Size(5, 5));
	title->enableOutline(Color4B::RED, 3);
	this->addChild(title);

	/////////////////////////////////////////
	//add 3 buttons
	auto startbutton = MenuItemImage::create(
		"StartNormal.png",
		"StartSelected.png",
		CC_CALLBACK_1(HelloWorld::menuStartButton, this));
	auto *start = Menu::create(startbutton, NULL);
	y = rect.origin.y + rect.size.height*(1.0f / 3.0f);
	start->setPosition(Vec2(x, y));
	start->setAnchorPoint(Vec2(0.0f, 0.0f));
	start->setScale(0.5f);
	this->addChild(start);

	auto settingbutton = MenuItemImage::create(
		"button.png",
		"button2.png",
		CC_CALLBACK_1(HelloWorld::menuSettingButton, this));
	Label *settingword;
	if (language_flag)
	{
		settingword = Label::create("Setting", "Arial", 40);
	}
	else
	{
		settingword = Label::create(FontToUTF8("设置"),
			"Arial", 40);
	}
	auto *psettingword = MenuItemLabel::create(settingword,
		CC_CALLBACK_1(HelloWorld::menuSettingButton, this));
	auto *setting = Menu::create(settingbutton, psettingword, NULL);
	x = rect.origin.x + rect.size.width*(1.0f / 4.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 3.0f);
	setting->setPosition(Vec2(x, y));
	setting->setAnchorPoint(Vec2(0.0f, 0.0f));
	this->addChild(setting);

	auto helpbutton = MenuItemImage::create(
		"button.png",
		"button2.png",
		CC_CALLBACK_1(HelloWorld::menuHelpButton, this));
	Label *helpword;
	if (language_flag)
	{
		helpword = Label::create("Help", "Arial", 40);
	}
	else
	{
		helpword = Label::create(FontToUTF8("帮助"),
			"Arial", 40);
	}
	auto *phelpword = MenuItemLabel::create(helpword,
		CC_CALLBACK_1(HelloWorld::menuHelpButton, this));
	auto *help = Menu::create(helpbutton, phelpword, NULL);
	x = rect.origin.x + rect.size.width*(3.0f / 4.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 3.0f);
	help->setPosition(Vec2(x, y));
	help->setAnchorPoint(Vec2(0.0f, 0.0f));
	this->addChild(help);
}
void HelloWorld::MusicPrinter()
{
	if (is_paused == stop_music)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	}
	else if (continue_music == is_paused)
	{
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Funky_Stars.mp3");
		is_paused = true;
	}
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
void HelloWorld::menuStartButton(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	auto sc = StartScene::createScene();        //随机瓦片的切换动画
	auto reScene = TransitionSplitCols::create(1.0f, sc);
	Director::getInstance()->replaceScene(reScene);
}
void HelloWorld::menuSettingButton(Ref* pSender)
{
	auto sc = SettingScene::createScene();
	auto reScene = TransitionSlideInL::create(1.0f, sc);  //从左边推入的动画
	Director::getInstance()->replaceScene(reScene);
}
void HelloWorld::menuHelpButton(Ref* pSender)
{
	auto sc = HelpScene::createScene();        //网格过渡的切换动画
	auto reScene = TransitionFadeTR::create(1.0f, sc);
	Director::getInstance()->replaceScene(reScene);
}


