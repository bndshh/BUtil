
//  Created by Jan Bundschuh.
//  Copyright 2016-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "cinder/ImageIo.h"
#include "cinder/Timeline.h"
#include "cinder/app/App.h"
#include "cinder/Log.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Fbo.h"

#include "ShaderShop.h"

#include "Statics.h"

namespace bUtil
{
	class Drawable
	{
	public:
		virtual void draw() = 0;
	};
}
