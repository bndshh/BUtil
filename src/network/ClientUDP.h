
//  Created by Jan Bundschuh.
//  Copyright 2015-2018 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "network/Client.h"

using namespace std;
using namespace ci;
using namespace bUtil;

namespace bUtil
{
	namespace network
	{
		class ClientUDP : public Client
		{
		public:
			virtual void connect();
			virtual void send(string message, string delimiter = "\r\n");
			virtual void update();
			virtual const bool isConnected();
			virtual const size_t getNumRequests() { return mRequests.size(); };
			virtual void setOnConnect(const std::function<void()> &callback) { mOnConnect = callback; };
			virtual ~ClientUDP();

		protected:
			ClientUDP(asio::io_service &io, string host, int32_t port, string name);
			
			UdpClientRef	mClient;
			UdpSessionRef	mSession;
			vector<string>	mResponses;
			vector<string>  mRequests;
			string			mResponsesConcat;

			bool mNeedsUpdate;
			virtual void handleResponses();

			virtual void onClientResolve();
			virtual void onClientConnect(UdpSessionRef session);
			virtual void onClientError(std::string err, size_t bytesTransferred);

			virtual void write();
			virtual void onSessionClose();
			virtual void onSessionError(std::string err, size_t bytesTransferred);
			virtual void onSessionReadComplete();
			virtual void onSessionRead(ci::BufferRef buffer);
			virtual void onSessionWrite(size_t bytesTransferred);

			std::function<void()> mOnConnect;
		};

		typedef shared_ptr<ClientUDP> ClientUDPRef;
	}
}