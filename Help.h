#ifndef _HELP_H_
#define _HELP_H_
#include "cocos2d.h"
#include "GUI/CCControlExtension/CCControlSwitch.h" 
#include "ui/CocosGUI.h"  
USING_NS_CC;
using namespace ui;

#define     pWidth      240  
#define     pHeight     160  
class HelpScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuHellowWorldScene(cocos2d::Ref* pSender);

	void ScenePrinter();
	void PagePrinter();
	void Func(cocos2d::Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(HelpScene);


	/////////////////////////////////////////
	/*void initRole();
	static Scene* createS();
	void initS();*/
private:
	PageView * pageView;
	Vec2                    ClickBeganPos;  //µã»÷µÄ×ø±ê  
};

#endif //HELP