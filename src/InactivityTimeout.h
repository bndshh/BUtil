
//  Created by Jan Bundschuh.
//  Copyright 2016-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "Singleton.h"
#include "cinder/app/App.h"
#include "cinder/Timeline.h"
#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;

using namespace bUtil;

namespace bUtil
{
	class InactivityTimeout
	{
	public:
		InactivityTimeout();
		virtual void setTimeoutSeconds(const float & seconds);
		virtual const float getTimeoutSeconds();
		virtual void setInactivityCallback(const std::function<void()> & callback);
		virtual void activity(const std::function<void()> &callback = [] {});
		virtual void start();
		virtual ~InactivityTimeout() = default;

	protected:
		float mTimeoutSeconds;
		std::function<void()> mInactivityCallback;
		void trigger();
		Anim<float> mTimeout;
	};

	typedef Singleton<InactivityTimeout> InactivityTimeouter;
	typedef std::shared_ptr<InactivityTimeout> InactivityTimeoutRef;
};