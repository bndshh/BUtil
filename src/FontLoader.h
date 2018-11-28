
//  Created by Jan Bundschuh.
//  Copyright 2016-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "Resources.h"

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Unicode.h"
#include "cinder/Log.h"

#include "SdfText.h"
#include "Singleton.h"

namespace bUtil
{
	class FontLoader
	{
	public:
		FontLoader() = default;
		static const std::string defaultChars();
		virtual ci::Font getFont(const std::string  & name, const float & size);
		virtual ci::gl::SdfText::Font getSdfFont(const std::string & name, const float & size);
		virtual ci::gl::SdfTextRef bUtil::FontLoader::getSdfText(const std::string & name, const float & size);
		virtual ci::gl::TextureFontRef getTextureFont(const std::string & name, const float & size);
		virtual ci::gl::TextureFont::DrawOptions options() { return mDrawOptions; };
		virtual ~FontLoader() = default;

	protected:
		std::vector<std::pair<ci::Font, std::string>> mFonts;
		std::vector<std::pair<ci::gl::SdfText::Font, std::string>> mSdfFonts;
		std::vector<ci::gl::SdfTextRef> mSdfTexts;
		std::vector<std::pair<ci::gl::TextureFontRef, std::string>> mTextureFonts;
		ci::gl::TextureFont::DrawOptions mDrawOptions;
	};

	typedef Singleton<FontLoader> FontLoaderSingleton;
}
