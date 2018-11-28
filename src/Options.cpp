
//  Created by Jan Bundschuh.
//  Copyright 2016-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#include "Options.h"

bUtil::Options::Options() : mDrawFPS(true), mPosition(ci::vec2(0)), mKioskMode(false), mUseTimeout(false), mTimeout(0), mUseAudio(false)
{
	try
	{
		ci::JsonTree json = ci::JsonTree(ci::app::loadAsset("options.json"));

		try
		{
			mOptions = json.getChild("Options");

			mDrawFPS = Help::JSON::parseBool(mOptions, "drawFPS");
			mPosition = Help::JSON::parseVec2(mOptions, "position");
			mKioskMode = Help::JSON::parseBool(mOptions, "kioskMode");
			mUseTouch = Help::JSON::parseBool(mOptions, "useTouch");
			mUseTimeout = Help::JSON::parseBool(mOptions, "useTimeout");
			mTimeout = Help::JSON::parseInt(mOptions, "timeout");
			mUseAudio = Help::JSON::parseBool(mOptions, "useAudio");
		}
		catch (ci::JsonTree::ExcChildNotFound & exc)
		{
			CI_LOG_W(exc.what());
		}
	}
	catch (ci::app::AssetLoadExc & exc)
	{
		CI_LOG_W(exc.what());
	}
}