
//  Created by Jan Bundschuh.
//  Copyright 2015-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#include "Connector.h"

bUtil::Connector::Connector(asio::io_service &io, string host, int32_t port, string name) : mHost(host), mPort(port), mName(name), mRetry(false), mLog(false)
{
}

void bUtil::Connector::onConnectResolve()
{
	if (mLog)
	{
		CI_LOG_I(mName + " - Endpoint resolved");
	}
}

void bUtil::Connector::onError(std::string err, size_t bytesTransferred)
{
	string text = mName + " - Error";

	if (mLog)
	{
		if (!err.empty())
		{
			text += ": " + err;
		}

		CI_LOG_I(text);
	}

	if (mRetry && !isConnected() && (!mRetryCue || mRetryCue->isComplete()))
	{
		mRetryCue = app::timeline().add([&] {
			connect();
		}, app::timeline().getCurrentTime() + 5.0f);
	}
}

void bUtil::Connector::onConnectReadComplete()
{
	if (mLog)
	{
		CI_LOG_I(mName + " - Read complete");
	}
}

void bUtil::Connector::onWrite(size_t bytesTransferred)
{
	if (mLog)
	{
		CI_LOG_I(mName + " - " + toString(bytesTransferred) + " bytes written");
	}

	receive();
}