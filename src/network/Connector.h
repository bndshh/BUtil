
//  Created by Jan Bundschuh.
//  Copyright 2015-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "CinderAsio.h"
#include "TcpClient.h"
#include "UdpClient.h"
#include "Options.h"

#include "cinder/app/App.h"
#include "cinder/Timeline.h"

using namespace std;
using namespace ci;


//  Created by Jan Bundschuh.
//  Copyright 2015-2018 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

namespace bUtil
{
	class Connector
	{
	public:
		virtual void connect(const bool &retry = true) = 0;
		virtual void receive() = 0;
		virtual const bool isConnected() = 0;
		virtual const bool isLogging() { return mLog; };
		virtual void setLogging(const bool &log) { mLog = log; };

		virtual ~Connector() = default;

	protected:
		Connector(asio::io_service &io, string host, int32_t port, string name);

		std::string		mHost;
		uint16_t		mPort;
		std::string		mName;

		bool mRetry;
		CueRef mRetryCue;

		bool mLog;

		virtual void onRead(ci::BufferRef buffer) = 0;

		virtual void onConnectResolve();
		virtual void onError(std::string err, size_t bytesTransferred);
		virtual void onConnectReadComplete();
		virtual void onWrite(size_t bytesTransferred);
	};
}