
//  Created by Jan Bundschuh.
//  Copyright 2016-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "Singleton.h"

#include "cinder/gl/GlslProg.h"

namespace bUtil
{
	class ShaderShop
	{
	public:
		ShaderShop();
		static ci::gl::GlslProgRef radialGradient();
		static ci::gl::GlslProgRef radialGradientMask();
		static ci::gl::GlslProgRef radialGradientTexture();
		static ci::gl::GlslProgRef verticalGradient();
		static ci::gl::GlslProgRef verticalGradientTexture();
		static ci::gl::GlslProgRef horizontalGradientTexture();
		static ci::gl::GlslProgRef texture();
		static ci::gl::GlslProgRef textureMaskTexture();
		static const std::string basicVertex();
		static const std::string textureVertex();
		virtual ~ShaderShop() = default;
	};

	typedef Singleton<ShaderShop> ShaderShopping;
};
