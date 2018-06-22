#ifndef __CLIENT__
#define __CLIENT__

#include "cocos2d.h"
#include "network/SocketIO.h"
#pragma comment(lib,"libcurl_imp.lib")

USING_NS_CC;
using namespace std;
using namespace network;

class Client :public cocos2d::network::SocketIO::SIODelegate
{
	
public:
	static SIOClient * _sioClient;
	static Client * client;
	static Client * getInstance();
	static bool isConnected();
	void init();
	static bool flag_connect;

	void Client::on(const std::string& eventName, SIOEvent e);
	void Client::emit(const std::string& eventname, const std::string& args);

	virtual void onConnect(cocos2d::network::SIOClient * client);
	virtual void onMessage(cocos2d::network::SIOClient * client, const std::string& data);
	virtual void onClose(cocos2d::network::SIOClient * client);
	virtual void onError(cocos2d::network::SIOClient * client, const std::string& data);

	void close(cocos2d::network::SIOClient * client, const std::string& data);

	~Client();
private:
};

#endif // client.h
