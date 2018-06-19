#ifndef _PLAYERCHOSE_H_
#define _PLAYERCHOSE_H_

#include "cocos2d.h"
#include "network\SocketIO.h"

using namespace cocos2d::network;
class PlayerChose: public cocos2d::Layer, public cocos2d::network::SocketIO::SIODelegate
{
public:
	SIOClient * _sioClient;
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuStartScene(cocos2d::Ref* pSender);

	void ScenePrinter();
	void NetworkPrinter();

	void onConnect(SIOClient* client);
	void onMessage(SIOClient* client, const std::string& data);
	void onError(SIOClient* client, const std::string& data);
	void onClose(SIOClient* client);

	// implement the "static create()" method manually
	CREATE_FUNC(PlayerChose);
};

#endif //_PLAYERCHOSE_H_