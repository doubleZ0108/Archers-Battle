#include "Setting.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

USING_NS_CC;
extern bool language_flag;   //true->English   false->Chinese
extern int is_paused;        //关于is_paused的具体解释请见 "HelloWorldScene.h"
extern double window_size;
extern char *FontToUTF8(const char* font);

//it is define in another .cpp file 
//and it is used to change character

Scene* SettingScene::createScene()
{
	return SettingScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool SettingScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	
	ScenePrinter();
	MusicPrinter();
	LanguagePrinter();
	SizePrinter();
	return true;
}



void SettingScene::ScenePrinter()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float x = rect.origin.x + rect.size.width / 2;
	float y = rect.origin.y + rect.size.height / 2;

	///////////////////////////////////////////
	//add Setting_Background
	
	x = rect.origin.x + rect.size.width / 2;
	y = rect.origin.y + rect.size.height / 2;
	auto *background = Sprite::create("Setting_Background.png");
	background->setPosition(Vec2(x, y));
	this->addChild(background);

	///////////////////////////////////
	//a return button which click to back to HelloWorldScene
	auto *return_button = MenuItemImage::create(
		"backtoupper.png",
		"backtoupper_select.png",
		CC_CALLBACK_1(SettingScene::menuHellowWorldScene, this));

	auto *preturn = Menu::create(return_button, NULL);
	x = rect.origin.x + rect.size.width*(10.0f / 11.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 10.0f);
	preturn->setPosition(Vec2(x, y));
	
	preturn->setScale(1.0f);
	this->addChild(preturn);

	/////////////////////////////////////////
	//add  
	auto *musicbutton = Sprite::create("button.png");
	x = rect.origin.x + rect.size.width*(1.0f / 3.0f);
	y = rect.origin.y + rect.size.height*(2.0f / 3.0f);
	musicbutton->setPosition(Vec2(x, y));
	this->addChild(musicbutton);

	Label *musicword;
	if (language_flag)
	{
		musicword = Label::createWithTTF("Music",
			"fonts/Marker Felt.ttf", 40);
	}
	else
	{
		musicword = Label::create(FontToUTF8("音乐"),
			"Arial", 40);
	}
	musicword->setPosition(Vec2(x, y));
	this->addChild(musicword);

	auto *languagebutton = Sprite::create("button.png");
	y = rect.origin.y + rect.size.height*(1.0f / 2.0f);
	languagebutton->setPosition(Vec2(x, y));
	this->addChild(languagebutton);
	Label *languageword;
	if (language_flag)
	{
		languageword = Label::createWithTTF("Language",
			"fonts/Marker Felt.ttf", 40);
	}
	else
	{
		languageword = Label::create(FontToUTF8("语言"),
			"Arial", 40);
	}
	languageword->setPosition(Vec2(x, y));
	this->addChild(languageword);

	auto *sizebutton = Sprite::create("button.png");
	y = rect.origin.y + rect.size.height*(1.0f / 3.0f);
	sizebutton->setPosition(Vec2(x, y));
	this->addChild(sizebutton);
	Label *sizeword;
	if (language_flag)
	{
		sizeword = Label::createWithTTF("Size",
			"fonts/Marker Felt.ttf", 40);
	}
	else
	{
		sizeword = Label::create(FontToUTF8("屏幕尺寸"),
			"Arial", 40);
	}
	
	sizeword->setPosition(Vec2(x, y));
	this->addChild(sizeword);
}
void SettingScene::MusicPrinter()
{
	auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float x, y;
	//play button
	Label *label_play;
	if (language_flag)
	{
		label_play = Label::create("play", "Arial", 40);
	}
	else
	{
		label_play = Label::create(FontToUTF8("播放"),
			"Arial", 40);
	}
	auto* pLabel_play = MenuItemLabel::create(label_play, this, menu_selector(SettingScene::play));
	auto* button_play = Menu::create(pLabel_play, NULL);
	x = rect.origin.x + rect.size.width*(5.0f / 10.0f);
	y = rect.origin.y + rect.size.height*(2.0f / 3.0f);
	button_play->setPosition(Vec2(x, y));
	button_play->setColor(Color3B::BLACK);
	this->addChild(button_play);
	//pause button
	Label *label_pause;
	if (language_flag)
	{
		label_pause = Label::create("pause", "Arial", 40);
	}
	else
	{
		label_pause = Label::create(FontToUTF8("暂停"),
			"Arial", 40);
	}
	auto* pLabel_pause = MenuItemLabel::create(label_pause, this, menu_selector(SettingScene::pause));
	auto* button_pause = Menu::create(pLabel_pause, NULL);
	x = rect.origin.x + rect.size.width*(6.0f / 10.0f);
	button_pause->setPosition(Vec2(x, y));
	button_pause->setColor(Color3B::BLACK);
	this->addChild(button_pause);
	//stop button
	Label *label_stop;
	if (language_flag)
	{
		label_stop = Label::create("stop", "Arial", 40);
	}
	else
	{
		label_stop = Label::create(FontToUTF8("停止"),
			"Arial", 40);
	}
	auto* pLabel_stop = MenuItemLabel::create(label_stop, this, menu_selector(SettingScene::stop));
	auto* button_stop = Menu::create(pLabel_stop, NULL);
	x = rect.origin.x + rect.size.width*(7.0f / 10.0f);
	button_stop->setPosition(Vec2(x, y));
	button_stop->setColor(Color3B::BLACK);
	this->addChild(button_stop);
}
void SettingScene::LanguagePrinter()
{
	auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float x, y;
	//中文按钮
	Label *chinese_word;
	if (language_flag)
	{
		chinese_word = Label::create("Chinese", "Arial", 40);
	}
	else
	{
		chinese_word = Label::create(FontToUTF8("中文"),
			"Arial", 40);
	}
	auto* pchinese_word = MenuItemLabel::create(chinese_word, this,
		menu_selector(SettingScene::language_change_tochinese));
	auto* chinese_button = Menu::create(pchinese_word, NULL);
	x = rect.origin.x + rect.size.width*(21.0f / 40.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 2.0f);
	chinese_button->setPosition(Vec2(x, y));
	chinese_button->setColor(Color3B::BLACK);
	this->addChild(chinese_button);
	//英文按钮
	Label *english_word;
	if (language_flag)
	{
		english_word = Label::create("English", "Arial", 40);
	}
	else
	{
		english_word = Label::create(FontToUTF8("英文"),
			"Arial", 40);
	}
	auto* penglish_word = MenuItemLabel::create(english_word, this,
		menu_selector(SettingScene::language_change_toenglish));
	auto* english_button = Menu::create(penglish_word, NULL);
	x = rect.origin.x + rect.size.width*(27.0f / 40.0f);
	english_button->setPosition(Vec2(x, y));
	english_button->setColor(Color3B::BLACK);
	this->addChild(english_button);

}
void SettingScene::SizePrinter()
{
	auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float x, y;

	/////////////////////////////
	//the optional size
	Label *sizeword_small;
	if (language_flag)
	{
		sizeword_small = Label::create("small", "Arial", 40);
	}
	else
	{
		sizeword_small = Label::create(FontToUTF8("小窗口"), 
			"Arial", 40);
	}
	auto *psizeword_small = MenuItemLabel::create(sizeword_small, this,
		menu_selector(SettingScene::size_change_tosmall));
	auto* size_button_small = Menu::create(psizeword_small, NULL);
	x = rect.origin.x + rect.size.width*(4.5f / 10.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 3.0f);
	size_button_small->setPosition(Vec2(x, y));
	size_button_small->setColor(Color3B::BLACK);
	this->addChild(size_button_small);

	Label *sizeword_middle;
	if (language_flag)
	{
		sizeword_middle = Label::create("middle", "Arial", 40);
	}
	else
	{
		sizeword_middle = Label::create(FontToUTF8("中窗口"),
			"Arial", 40);
	}
	auto *psizeword_middle = MenuItemLabel::create(sizeword_middle, this,
		menu_selector(SettingScene::size_change_tomiddle));
	auto* size_button_middle = Menu::create(psizeword_middle, NULL);
	x = rect.origin.x + rect.size.width*(6.0f / 10.0f);
	size_button_middle->setPosition(Vec2(x, y));
	size_button_middle->setColor(Color3B::BLACK);
	this->addChild(size_button_middle);

	Label *sizeword_large;
	if (language_flag)
	{
		sizeword_large = Label::create("large", "Arial", 40);
	}
	else
	{
		sizeword_large = Label::create(FontToUTF8("大窗口"),
			"Arial", 40);
	}
	auto *psizeword_large = MenuItemLabel::create(sizeword_large, this,
		menu_selector(SettingScene::size_change_tolarge));
	auto* size_button_large = Menu::create(psizeword_large, NULL);
	x = rect.origin.x + rect.size.width*(7.5f / 10.0f);
	size_button_large->setPosition(Vec2(x, y));
	size_button_large->setColor(Color3B::BLACK);
	this->addChild(size_button_large);
}

void SettingScene::menuHellowWorldScene(Ref* pSender)
{
	is_paused = continue_music;

	auto* setting_default = UserDefault::getInstance();
	setting_default->setIntegerForKey("language", language_flag);
	setting_default->setIntegerForKey("music", is_paused);
	setting_default->setDoubleForKey("size", window_size);
	setting_default->flush();

	auto sc = HelloWorld::createScene();
	auto reScene = TransitionSlideInR::create(1.0f, sc);  //从右边推入的动画
	Director::getInstance()->replaceScene(reScene);
}
void SettingScene::play(cocos2d::Object* pSender)
{
	if (stop_music == is_paused)
	{
		//解决按了stop后马上按play却是接着播放的bug
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(); 
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Funky_Stars.mp3");
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
}
void SettingScene::stop(cocos2d::Object* pSender)
{
	is_paused = stop_music;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}
void SettingScene::pause(cocos2d::Object* pSender)
{
	is_paused = continue_music;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}


void SettingScene::language_change_tochinese(cocos2d::Object * pSender)
{
	language_flag = false;
	is_paused = continue_music;      //防止切换语言的时候音乐重新重头播放
	Director::getInstance()->replaceScene(SettingScene::create());
}
void SettingScene::language_change_toenglish(cocos2d::Object * pSender)
{
	language_flag = true;
	is_paused = continue_music;
	Director::getInstance()->replaceScene(SettingScene::create());
}


void SettingScene::size_change_tosmall(cocos2d::Object * pSender)
{
	window_size = 0.6f;
	size_change();
}
void SettingScene::size_change_tomiddle(cocos2d::Object * pSender)
{
	window_size = 1.0f;
	size_change();
}
void SettingScene::size_change_tolarge(cocos2d::Object * pSender)
{
	window_size = 1.35f;
	size_change();
}

void SettingScene::size_change()
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	glview->setFrameZoomFactor(window_size);
}
