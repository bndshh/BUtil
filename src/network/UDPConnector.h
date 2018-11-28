
//  Created by Jan Bundschuh.
//  Copyright 2015-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "Connector.h"

using namespace std;
using namespace ci;
using namespace bUtil;

namespace bUtil
{
	class UDPConnector : public Connector
	{
	public:	
		virtual void connect(const bool &retry = true) override;
		virtual void receive() override;
		virtual void send(string msg);
		virtual const bool isConnected() override;
		virtual ~UDPConnector() = default;

	protected:
		UDPConnector(asio::io_service &io, string host, int32_t port, string name);
		UdpClientRef	mClient;
		UdpSessionRef	mSession;

		virtual void onRead(ci::BufferRef buffer) override;
		virtual void onConnect(UdpSessionRef session);
	};

	typedef shared_ptr<UDPConnector> UDPConnectorRef;
}