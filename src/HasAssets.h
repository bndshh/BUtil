
//  Created by Jan Bundschuh.
//  Copyright 2016-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/app/App.h"
#include "cinder/Log.h"
#include "cinder/audio/audio.h"

#include "Options.h"

using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace ci::audio;

namespace bUtil
{
	class HasAssets
	{
	public:
		HasAssets() {};
		virtual TextureRef loadTexture(const fs::path &path);
		virtual TextureRef loadTextureFromAssets(const std::string &path);
		virtual TextureRef loadTextureFromAssetsWithPath(const fs::path &path);
		virtual VoiceRef loadAudioFromAssets(const std::string &path);
		virtual ~HasAssets() = default;
	};
}