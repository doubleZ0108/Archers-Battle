#ifndef _START_H_
#define _START_H_

#include "cocos2d.h"
#include "network\SocketIO.h"

USING_NS_CC;
using namespace cocos2d::network;

class StartScene : public cocos2d::Layer, public cocos2d::network::SocketIO::SIODelegate
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	////////////////////////////////
	//network
	void onConnect(SIOClient* client);
	void onMessage(SIOClient* client, const std::string& data);
	void onError(SIOClient* client, const std::string& data);
	void onClose(SIOClient* client);
	/////////////////////////////

	// a selector callback
	void menuHellowWorldScene(Ref* pSender);
	void menuToPlayButton(Ref* pSender);
	void menuMapChoseButton(Ref* pSender);
	void menuPlayerChoseButton(Ref* pSender);

	void ScenePrinter();

	// implement the "static create()" method manually
	CREATE_FUNC(StartScene);

	
};

#endif //_START_H_