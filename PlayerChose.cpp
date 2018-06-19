#include "PlayerChose.h"
#include "Start.h"

USING_NS_CC;

extern bool language_flag;   //true->English   false->Chinese
extern char *FontToUTF8(const char* font);

extern int which_player;
//it is define in another .cpp file 
//and it is used to change character

Scene* PlayerChose::createScene()
{
	auto scene = Scene::create();
	auto layer = PlayerChose::create();
	scene->addChild(layer);
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MapChose.cpp\n");
}

// on "init" you need to initialize your instance
bool PlayerChose::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	ScenePrinter();
	NetworkPrinter();
	return true;
}

void PlayerChose::ScenePrinter()
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
	auto *background = Sprite::create("Help_Background.png");
	background->setPosition(Vec2(x, y));
	this->addChild(background);

	///////////////////////////////////
	//a return button which click to back to HelloWorldScene
	auto *return_button = MenuItemImage::create(
		"backtoupper.png",
		"backtoupper_select.png",
		CC_CALLBACK_1(PlayerChose::menuStartScene, this));

	auto *preturn = Menu::create(return_button, NULL);
	x = rect.origin.x + rect.size.width*(10.0f / 11.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 10.0f);
	preturn->setPosition(Vec2(x, y));
	preturn->setScale(1.0f);
	this->addChild(preturn);

	//////////////////////////////////////
	//add two player sceneshot
	auto pre_player1 = Sprite::create("pre_player.png");
	x = rect.origin.x + rect.size.width*(1.0f / 4.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 2.0f);
	pre_player1->setPosition(Vec2(x, y));

	auto gridNodeTarget_1 = NodeGrid::create();
	this->addChild(gridNodeTarget_1);
	gridNodeTarget_1->addChild(pre_player1);

	auto pre_player2 = Sprite::create("pre_player.png");
	x = rect.origin.x + rect.size.width*(3.0f / 4.0f);
	pre_player2->setPosition(Vec2(x, y));

	auto gridNodeTarget_2 = NodeGrid::create();
	this->addChild(gridNodeTarget_2);
	gridNodeTarget_2->addChild(pre_player2);


	/////////////////////////////////////////////
	// Make pre_map1 touchable  
	auto listener1 = EventListenerTouchOneByOne::create();//创建一个触摸监听  
	listener1->setSwallowTouches(true); //设置是否想下传递触摸  

										//通过 lambda 表达式 直接实现触摸事件的回掉方法  
	listener1->onTouchBegan = [=](Touch* touch, Event* event)
	{
		auto target = static_cast<Sprite*>(event->getCurrentTarget());

		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		if (rect.containsPoint(locationInNode))
		{
			CCActionInterval* fadeOutTRTiles = CCFadeOutTRTiles::create(5, CCSize(50, 50));
			gridNodeTarget_2->runAction(fadeOutTRTiles);

			CCActionInterval* shaky3D = CCShaky3D::create(5, CCSize(10, 10), 15, false);
			gridNodeTarget_1->runAction(shaky3D);

			log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
			target->setOpacity(180);
			return true;
		}
		return false;
	};
	listener1->onTouchEnded = [=](Touch* touch, Event* event)
	{
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		target->setOpacity(255);
		which_player = 1;
		log("which_map %d", which_player);
		_sioClient->emit("playerchose", "1");
		_eventDispatcher->removeEventListener(listener1);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, pre_player1);

	/////////////////////////////////////////////
	// Make pre_map2 touchable  
	auto listener2 = EventListenerTouchOneByOne::create();//创建一个触摸监听  
	listener2->setSwallowTouches(true); //设置是否想下传递触摸  

	listener2->onTouchBegan = [=](Touch* touch, Event* event)
	{
		auto target = static_cast<Sprite*>(event->getCurrentTarget());

		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		if (rect.containsPoint(locationInNode))
		{
			CCActionInterval* fadeOutUpTiles = CCFadeOutUpTiles::create(4, CCSize(10, 10));
			gridNodeTarget_1->runAction(fadeOutUpTiles);

			CCActionInterval* liquid = CCLiquid::create(5, CCSize(10, 10), 4, 20);
			gridNodeTarget_2->runAction(liquid);
			log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
			target->setOpacity(180);
			return true;
		}
		return false;
	};
	listener2->onTouchEnded = [=](Touch* touch, Event* event)
	{
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		target->setOpacity(255);
		which_player = 2;
		log("which_map %d", which_player);
		_sioClient->emit("playerchose", "2");
		_eventDispatcher->removeEventListener(listener2);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, pre_player2);
	
}
void PlayerChose::NetworkPrinter()
{
	_sioClient = network::SocketIO::connect("http://120.78.208.162:2333", *this);
}
void PlayerChose::onConnect(SIOClient * client)
{
}
void PlayerChose::onMessage(SIOClient * client, const std::string & data)
{
}
void PlayerChose::onError(SIOClient * client, const std::string & data)
{
}
void PlayerChose::onClose(SIOClient * client)
{
}
void PlayerChose::menuStartScene(Ref* pSender)
{
	auto sc = StartScene::createScene();        //按列分割界面的切换动画
	auto reScene = TransitionSlideInL::create(1.0f, sc);  //从左边推入的动画
	Director::getInstance()->replaceScene(reScene);
}
