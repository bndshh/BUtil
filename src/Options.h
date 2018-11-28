
//  Created by Jan Bundschuh.
//  Copyright 2016-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "Singleton.h"
#include "Help.h"

#include "cinder/Json.h"
#include "cinder/Log.h"
#include "cinder/app/App.h"

namespace bUtil
{
	class Options
	{
	public:
		Options();

		const bool drawFPS() { return mDrawFPS; };
		const ci::vec2 position() { return mPosition; };
		const bool kioskMode() { return mKioskMode; };
		const bool useTouch() { return mUseTouch; };
		const bool useTimeout() { return mUseTimeout; };
		const unsigned int timeout() { return mTimeout; };
		const bool useAudio() { return mUseAudio; };

		~Options() = default;

	protected:
		ci::JsonTree mOptions;

		bool mDrawFPS;
		ci::vec2 mPosition;
		bool mKioskMode;
		bool mUseTouch;
		bool mUseTimeout;
		unsigned int mTimeout;
		bool mUseAudio;
	};

	typedef Singleton<Options> OptionsSingleton;
}