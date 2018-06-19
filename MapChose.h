#ifndef _MAPCHOSE_H_
#define _MAPCHOSE_H_

#include "cocos2d.h"
#include "network\SocketIO.h"

#pragma comment(lib,"libcurl_imp.lib")
using namespace cocos2d::network;

class MapChose : public cocos2d::Layer, public cocos2d::network::SocketIO::SIODelegate
{
public:
	SIOClient * _sioClient;
	static cocos2d::Scene* createScene();

	virtual bool init();
	// a selector callback
	void menuStartScene(cocos2d::Ref* pSender);

	void NetworkPrinter();
	void ScenePrinter();

	//socket连接时调用
	void onConnect(SIOClient* client);
	//收到数据时调用
	void onMessage(SIOClient* client, const std::string& data);
	//连接错误或接收到错误信号时调用
	void onError(SIOClient* client, const std::string& data);
	//socket关闭时调用
	void onClose(SIOClient* client);


	// implement the "static create()" method manually



	CREATE_FUNC(MapChose);
};

#endif //_MAPCHOSE_H_