
//  Created by Jan Bundschuh.
//  Copyright 2016-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#include "ShaderShop.h"

bUtil::ShaderShop::ShaderShop() {}

ci::gl::GlslProgRef bUtil::ShaderShop::radialGradient()
{
	return ci::gl::GlslProg::create(ci::gl::GlslProg::Format().vertex(basicVertex()).fragment(CI_GLSL(150,

		in vec4		Color;
		in vec4		Position;

		uniform vec2 uCenter;
		uniform float uRadius;
		uniform float uExpand;
		uniform vec4 uColor;

		out vec4	oColor;

		void main(void)
		{
			vec2 p = (uCenter - Position.xy) / uRadius;
			float r = sqrt(dot(p, p));
			oColor = r < 1.0 ? mix(uColor, Color, (r - uExpand) / (1 - uExpand)) : Color;
		}
	)));
}

ci::gl::GlslProgRef bUtil::ShaderShop::radialGradientMask()
{
	return ci::gl::GlslProg::create(ci::gl::GlslProg::Format().vertex(basicVertex()).fragment(CI_GLSL(150,

		in vec4		Color;
		in vec4		Position;

		uniform vec2 uCenter2;
		uniform vec2 uCenter;
		uniform float uRadius;
		uniform float uExpand;
		uniform vec4 uColor;

		out vec4	oColor;

		void main(void)
		{
			vec2 p = (uCenter - Position.xy) / uRadius;
			vec2 p2 = (uCenter2 - Position.xy) / uRadius;
			float r = sqrt(dot(p, p));
			float r2 = sqrt(dot(p2, p2));

			if (r2 > 1.0f - (r2 - r))
			{
				oColor = r < 1.0 ? mix(uColor, Color, (r - uExpand) / (1 - uExpand)) : Color;
			}
			else
			{
				oColor = vec4(uColor.rgb, 0.0f);
			}
		}
	)));
}

ci::gl::GlslProgRef bUtil::ShaderShop::radialGradientTexture()
{
	return ci::gl::GlslProg::create(ci::gl::GlslProg::Format().vertex(textureVertex()).fragment(CI_GLSL(150,

		in vec4		Color;
		in vec4		Position;
		in vec2		TexCoord0;

		uniform vec2 uCenter;
		uniform float uRadius;
		uniform float uExpand;
		uniform vec4 uColor;
		uniform sampler2D uTex0;

		out vec4	oColor;

		void main(void)
		{
			vec2 p = (uCenter - Position.xy) / uRadius;
			float r = sqrt(dot(p, p));
			oColor = texture(uTex0, TexCoord0) * (r < 1.0 ? mix(uColor, Color, (r - uExpand) / (1 - uExpand)) : Color);
	}
	)));
}

ci::gl::GlslProgRef bUtil::ShaderShop::verticalGradient()
{
	return ci::gl::GlslProg::create(ci::gl::GlslProg::Format().vertex(basicVertex()).fragment(CI_GLSL(150,
		
		in vec4		Color;
		in vec4		Position;

		uniform vec2 uCenter;
		uniform float uRadius;
		uniform float uExpand;
		uniform vec4 uColor;

		out vec4	oColor;

		void main(void)
		{
			vec2 p = (uCenter - Position.xy) / uRadius;
			float r = sqrt(dot(p.y, p.y));
			float expand = (r - uExpand) / (1 - uExpand);
			oColor = r < 1.0 ? mix(uColor, Color, (expand < 0.0f ? 0.0f : expand)) : Color;
		}	
	)));
}

ci::gl::GlslProgRef bUtil::ShaderShop::verticalGradientTexture()
{
	return ci::gl::GlslProg::create(ci::gl::GlslProg::Format().vertex(textureVertex()).fragment(CI_GLSL(150,

		in vec4		Color;
		in vec4		Position;
		in vec2		TexCoord0;

		uniform vec2 uCenter;
		uniform float uRadius;
		uniform float uExpand;
		uniform vec4 uColor;
		uniform sampler2D uTex0;

		out vec4	oColor;

		void main(void)
		{
			vec2 p = (uCenter - Position.xy) / uRadius;
			float r = sqrt(dot(p.y, p.y));
			float expand = (r - uExpand) / (1 - uExpand);
			oColor = texture(uTex0, TexCoord0) * (r < 1.0 ? mix(uColor, Color, (expand < 0.0f ? 0.0f : expand)) : Color);
		}
	)));
}

ci::gl::GlslProgRef bUtil::ShaderShop::horizontalGradientTexture()
{
	return ci::gl::GlslProg::create(ci::gl::GlslProg::Format().vertex(CI_GLSL(150,

		uniform mat4	ciModelViewProjection;

		in vec4			ciPosition;
		in vec2			ciTexCoord0;
		in vec4			ciColor;

		out vec4		Color;
		out vec4		Position;
		out vec2		TexCoord0;

		void main(void)
		{
			gl_Position = ciModelViewProjection * ciPosition;
			Color = ciColor;
			Position = ciPosition;
			TexCoord0 = ciTexCoord0;
		}

	)).fragment(CI_GLSL(150,

		in vec4		Color;
		in vec4		Position;
		in vec2		TexCoord0;

		uniform vec2 uCenter;
		uniform float uRadius;
		uniform float uExpand;
		uniform vec4 uColor;
		uniform sampler2D uTex0;

		out vec4	oColor;

		void main(void)
		{
			vec2 p = (uCenter - Position.xy) / uRadius;
			float r = sqrt(dot(p.x, p.x));
			float expand = (r - uExpand) / (1 - uExpand);
			oColor = texture(uTex0, TexCoord0) * (r < 1.0 ? mix(uColor, Color, (expand < 0.0f ? 0.0f : expand)) : Color);
		}
	)));
}

ci::gl::GlslProgRef bUtil::ShaderShop::textureMaskTexture()
{
	return ci::gl::GlslProg::create(ci::gl::GlslProg::Format().vertex(CI_GLSL(150,

		uniform mat4	ciModelViewProjection;

		in vec4			ciPosition;
		in vec2			ciTexCoord0;

		out vec2		TexCoord0;

		void main(void) 
		{
			gl_Position = ciModelViewProjection * ciPosition;
			TexCoord0 = ciTexCoord0;
		}
	)).fragment(CI_GLSL(150,

		uniform vec4		uColor;
		uniform sampler2D	uTex0;
		uniform sampler2D	uMask0;

		in vec2				TexCoord0;
		out vec4			oColor;

		void main(void) 
		{
			oColor.rgb = texture(uTex0, TexCoord0).rgb * uColor.rgb;
			oColor.a = texture(uMask0, TexCoord0).a;
		}
	)));
}

ci::gl::GlslProgRef bUtil::ShaderShop::texture()
{
	return ci::gl::GlslProg::create(ci::gl::GlslProg::Format().vertex(textureVertex()).fragment(CI_GLSL(150,

		in vec4		Color;
		in vec4		Position;
		in vec2		TexCoord0;

		uniform vec4 uColor;
		uniform sampler2D uTex0;

		out vec4	oColor;

		void main(void)
		{

			oColor = texture(uTex0, TexCoord0) * uColor;
		}
	)));
}

const std::string bUtil::ShaderShop::basicVertex()
{
	return CI_GLSL(150,

		uniform mat4	ciModelViewProjection;

		in vec4			ciPosition;
		in vec4			ciColor;

		out vec4		Color;
		out vec4		Position;

		void main(void)
		{
			gl_Position = ciModelViewProjection * ciPosition;
			Color = ciColor;
			Position = ciPosition;
		}
	);
}

const std::string bUtil::ShaderShop::textureVertex()
{
	return CI_GLSL(150,

		uniform mat4	ciModelViewProjection;

		in vec4			ciPosition;
		in vec2			ciTexCoord0;
		in vec4			ciColor;

		out vec4		Color;
		out vec4		Position;
		out vec2		TexCoord0;

		void main(void)
		{
			gl_Position = ciModelViewProjection * ciPosition;
			Color = ciColor;
			Position = ciPosition;
			TexCoord0 = ciTexCoord0;
		}
	);
}