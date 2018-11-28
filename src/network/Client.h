
//  Created by Jan Bundschuh.
//  Copyright 2015-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "cinder/app/App.h"
#include "CinderAsio.h"
#include "TcpClient.h"
#include "UdpClient.h"
#include "Options.h"

using namespace std;
using namespace ci;

namespace bUtil
{
	namespace network
	{
		class Client
		{
		public:
			virtual const bool isLogging() { return mLog; };
			virtual void setLogging(const bool &log) { mLog = log; };
			virtual ~Client() = default;

		protected:
			Client(asio::io_service &io, string host, int32_t port, string name) : mHost(host), mPort(port), mName(name), mLog(false) {};

			std::string		mHost;
			uint16_t		mPort;
			std::string		mName;

			bool mLog;
		};
	}
}