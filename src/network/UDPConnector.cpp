
//  Created by Jan Bundschuh.
//  Copyright 2015-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#include "UDPConnector.h"

bUtil::UDPConnector::UDPConnector(asio::io_service &io, string host, int32_t port, string name) : Connector(io, host, port, name)
{
	mClient = UdpClient::create(io);

	mClient->connectConnectEventHandler(&UDPConnector::onConnect, this);
	mClient->connectErrorEventHandler(&UDPConnector::onError, this);
	mClient->connectResolveEventHandler(&UDPConnector::onConnectResolve, this);
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
	mSession->connectReadCompleteEventHandler(&UDPConnector::onConnectReadComplete, this);
	mSession->connectReadEventHandler(&UDPConnector::onRead, this);
	mSession->connectWriteEventHandler(&UDPConnector::onWrite, this);
}

void bUtil::UDPConnector::onRead(ci::BufferRef buffer)
{
	size_t bytes = buffer->getSize();
	string response = UdpSession::bufferToString(buffer);

	if (!response.empty() && response[response.size() - 1] == '\r')
	{
		response = response.erase(response.size() - 1);
	}

	if (mLog)
	{
		CI_LOG_I(mName + " - " + toString(bytes) + " bytes read");
		CI_LOG_I(mName + " - '" + ci::toString(response) + " '");
	}
}