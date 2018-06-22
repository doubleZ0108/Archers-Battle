#include "client.h"

SIOClient * Client::_sioClient = nullptr;
Client* Client::client = nullptr;
bool Client::flag_connect = false;
bool Client::isConnected()
{
	return flag_connect;
}

void Client::init()
{
	_sioClient = SocketIO::connect("http://120.78.208.162:2333", *this);
	flag_connect = true;
}

Client * Client::getInstance()
{
	return client;
}

Client::~Client()
{
	if (isConnected())
	{
		flag_connect = false;
		_sioClient = nullptr;
	}
}

void Client::on(const std::string& eventName, SIOEvent e)
{
	if(isConnected())	
	_sioClient->on(eventName, e);
}


void Client::emit(const std::string& eventname, const std::string& args)
{
	if(isConnected())
	_sioClient->emit(eventname, args);
}

void Client::close(network::SIOClient*client, const std::string& data)
{
	log("%s", data.c_str());
}

void Client::onConnect(cocos2d::network::SIOClient * client)
{
}

void Client::onMessage(cocos2d::network::SIOClient * client, const std::string & data)
{
}

void Client::onClose(cocos2d::network::SIOClient * client)
{
}

void Client::onError(cocos2d::network::SIOClient * client, const std::string & data)
{
}

