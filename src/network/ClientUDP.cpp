
//  Created by Jan Bundschuh.
//  Copyright 2015-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#include "ClientUDP.h"

bUtil::network::ClientUDP::ClientUDP(asio::io_service & io, string host, int32_t port, string name) : Client(io, host, port, name), mNeedsUpdate(false), mRequests()
{
	mClient = UdpClient::create(io);
	mClient->connectResolveEventHandler(&ClientUDP::onClientResolve, this);
	mClient->connectConnectEventHandler(&ClientUDP::onClientConnect, this);
	mClient->connectErrorEventHandler(&ClientUDP::onClientError, this);
}

void bUtil::network::ClientUDP::connect()
{
	mClient->connect(mHost, mPort);

	if (mLog) CI_LOG_I(mName + " - try to connect to " + mHost + ":" + toString(mPort) + ".");
}

void bUtil::network::ClientUDP::send(string message, string delimiter)
{
	if (isConnected())
	{
		mRequests.push_back(message + delimiter);
		mNeedsUpdate = true;
		return;
	}

	if (mLog) CI_LOG_W(mName + " - connect before send.");
}

void bUtil::network::ClientUDP::update()
{
	if (mNeedsUpdate)
	{
		write();
		mNeedsUpdate = false;
	}

	if (!mResponses.empty())
	{
		handleResponses();
	}
}

void bUtil::network::ClientUDP::handleResponses()
{
}

const bool bUtil::network::ClientUDP::isConnected()
{
	return mSession && mSession->getSocket()->is_open();
}

void bUtil::network::ClientUDP::onClientResolve()
{
	if (mLog) CI_LOG_I(mName + " - Endpoint resolved.");
}

void bUtil::network::ClientUDP::onClientConnect(UdpSessionRef session)
{
	CI_LOG_I(mName + " - Connected to " + mHost + ":" + toString(mPort) + ".");

	if (mSession) mSession.reset();

	mSession = session;
	mSession->connectErrorEventHandler(&ClientUDP::onSessionError, this);
	mSession->connectReadCompleteEventHandler(&ClientUDP::onSessionReadComplete, this);
	mSession->connectReadEventHandler(&ClientUDP::onSessionRead, this);
	mSession->connectWriteEventHandler(&ClientUDP::onSessionWrite, this);

	mResponsesConcat = "";
	if (mOnConnect) mOnConnect();
}

void bUtil::network::ClientUDP::onClientError(std::string err, size_t bytesTransferred)
{
	string text = mName + " - Client Error";
	if (!err.empty()) text += ": " + err;
	if (mLog) CI_LOG_E(text);
}

void bUtil::network::ClientUDP::write()
{
	if (isConnected() && !mRequests.empty())
	{
		try
		{
			vector<string>::iterator it = mRequests.begin();
			while (it != mRequests.end())
			{
				string str = *it;
				mSession->write(TcpSession::stringToBuffer(str));
				it = mRequests.erase(it);
			}

			mSession->read();
		}
		catch (Exception &e)
		{
			CI_LOG_E(e.what());
		}
		return;
	}

	if (mLog) CI_LOG_I(mName + " - connect before write.");
}

void bUtil::network::ClientUDP::onSessionClose()
{
	if (mLog) CI_LOG_I(mName + " - Disconnected.");
}

void bUtil::network::ClientUDP::onSessionError(std::string err, size_t bytesTransferred)
{
	string text = mName + " - Session Error";
	if (!err.empty()) text += ": " + err;
	if (mLog) CI_LOG_E(text);
}

void bUtil::network::ClientUDP::onSessionReadComplete()
{
	if (mLog) CI_LOG_I(mName + " - Read complete.");
}

void bUtil::network::ClientUDP::onSessionRead(ci::BufferRef buffer)
{
	size_t bytes = buffer->getSize();

	// if (mLog) CI_LOG_I(mName + " - " + toString(bytes) + " bytes read.");

	string response = mSession->bufferToString(buffer);

	// if (mLog) CI_LOG_I(mName + " - response '" + response + "'");

	//if (bytes > 35 && response.at(0) != 's')
	{
		//CI_LOG_I(mName + " - response '" + response + "'");
	}

	mResponsesConcat += response;
	while (mResponsesConcat.size() > 1000)
	{
		mResponsesConcat.erase(0, 1);
	}

	mResponses.push_back(response);
}

void bUtil::network::ClientUDP::onSessionWrite(size_t bytesTransferred)
{
	if (mLog) CI_LOG_I(mName + " - " + toString(bytesTransferred) + " bytes written.");
}

bUtil::network::ClientUDP::~ClientUDP()
{
	if (mLog) CI_LOG_I(mName + " - ClientTCP destroyed.");
}

/*
#include "UDPConnector.h"

bUtil::UDPConnector::UDPConnector(asio::io_service &io, string host, int32_t port, string name) : Connector(io, host, port, name)
{
	mClient = UdpClient::create(io);

	mClient->connectConnectEventHandler(&UDPConnector::onConnect, this);
	mClient->connectErrorEventHandler(&UDPConnector::onError, this);
	mClient->connectResolveEventHandler(&UDPConnector::onResolve, this);
}

void bUtil::UDPConnector::connect(const bool &retry)
{
	mRetry = retry;

	if (!isConnected())
	{
		if (mLog)
		{
			CI_LOG_I(mName + " - Connecting to: " + mHost + ":" + toString(mPort));
		}

		mClient->connect(mHost, mPort);
	}
	else
	{
		if (mLog)
		{
			CI_LOG_I(mName + " - Already connected.");
		}
	}
}


void bUtil::UDPConnector::receive()
{
	if (isConnected())
	{
		if (mLog)
		{
			//	CI_LOG_I(mName + " - read..");
		}

		mSession->read();
	}
	else
	{
		if (mLog)
		{
			//	CI_LOG_W(mName + " - Not connected.");
		}
	}
}


void bUtil::UDPConnector::send(string msg)
{
	if (isConnected())
	{
		if (mLog)
		{
			CI_LOG_I(mName + " - Send: " + msg);
		}
		mSession->write(UdpSession::stringToBuffer(msg));
	}
	else
	{
		if (mLog)
		{
			CI_LOG_W("Please connect before sending messages.");
		}
	}
}

const bool bUtil::UDPConnector::isConnected()
{
	return mSession && mSession->getSocket()->is_open();
}

void bUtil::UDPConnector::onConnect(UdpSessionRef session)
{
	CI_LOG_I(mName + " - Connected");

	mSession = session;
	
	mSession->connectErrorEventHandler(&UDPConnector::onError, this);
	mSession->connectReadCompleteEventHandler(&UDPConnector::onReadComplete, this);
	mSession->connectReadEventHandler(&UDPConnector::onRead, this);
	mSession->connectWriteEventHandler(&UDPConnector::onWrite, this);
}

void bUtil::UDPConnector::onRead(ci::BufferRef buffer)
{
	size_t bytes = buffer->getSize();
	string response = UdpSession::bufferToString(buffer);

	try
	{
		if (!response.empty() && response.at(response.size() - 1) == '\r')
		{
			response = response.erase(response.size() - 1);
		}

		if (mLog)
		{
			CI_LOG_I(mName + " - " + toString(bytes) + " bytes read");
			CI_LOG_I(mName + " - '" + ci::toString(response) + " '");
		}
	}
	catch (const std::out_of_range& oor)
	{
		CI_LOG_E("Out of Range error: " << oor.what());
	}
}

*/


