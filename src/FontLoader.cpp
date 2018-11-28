
//  Created by Jan Bundschuh.
//  Copyright 2016-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#include "FontLoader.h"

ci::Font bUtil::FontLoader::getFont(const std::string & name, const float & size)
{
	for each (auto font in mFonts)
	{
		auto loadingName = font.second;
		auto fontSize = font.first.getSize();

		if (fontSize == size && loadingName == name) return font.first; // Already loaded
	}

	try
	{
		float contentScale = ci::app::getWindow()->getContentScale();
		std::string pre = "Font/";
		ci::Font font(ci::app::loadAsset(pre + name), size * contentScale);
		mFonts.push_back(std::pair<ci::Font, std::string>(font, name));

		return mFonts.back().first;
	}
	catch (ci::FontInvalidNameExc exc)
	{
		CI_LOG_E(exc.what());
		return ci::Font();
	}
}

ci::gl::SdfText::Font bUtil::FontLoader::getSdfFont(const std::string & name, const float & size)
{
	for each (auto font in mSdfFonts)
	{
		auto loadingName = font.second;
		auto fontSize = font.first.getSize();

		if (fontSize == size && loadingName == name) return font.first; // Already loaded
	}

	try
	{
		float contentScale = ci::app::getWindow()->getContentScale();
		std::string pre = "Font/";
		ci::gl::SdfText::Font font(ci::app::loadAsset(pre + name), size * contentScale);
		mSdfFonts.push_back(std::pair<ci::gl::SdfText::Font, std::string> (font, name));
		
		size_t lastindex = name.find_last_of(".");
		std::string rawname = name.substr(0, lastindex);

		ci::fs::path path = ci::app::getAssetPath("Cache") /= "/cached_" + rawname + ".sdft";
		mSdfTexts.push_back(ci::gl::SdfText::create(path, mSdfFonts.back().first, ci::gl::SdfText::Format(), defaultChars()));
		
		return mSdfFonts.back().first;
	}
	catch (ci::FontInvalidNameExc exc)
	{
		CI_LOG_E(exc.what());
		return ci::gl::SdfText::Font();
	}
}


ci::gl::SdfTextRef bUtil::FontLoader::getSdfText(const std::string & name, const float & size)
{
	for (size_t i = 0; i < mSdfFonts.size(); i++)
	{
		auto font = mSdfFonts.at(i);

		auto loadingName = font.second;
		auto fontSize = font.first.getSize();

		if (fontSize == size && loadingName == name) return mSdfTexts.at(i); // Already loaded
	}

	getSdfFont(name, size);
	return mSdfTexts.back();
}


const std::string bUtil::FontLoader::defaultChars()
{
	return "ABCDEFGHIJKLMNOPQRSTUVWXYZÜÖÄabcdefghijklmnopqrstuvwxyzüöäß1234567890().?!§$,:;'\"„“»«&*=+-–•/\\@#_[]<>%^°llflfiphrids\303\251\303\241\303\250\303\240ffWo";
}

ci::gl::TextureFontRef bUtil::FontLoader::getTextureFont(const std::string & name, const float & size)
{
	ci::gl::TextureFont::Format format;
	format.premultiply(false);
	format.enableMipmapping();

	float contentScale = ci::app::getWindow()->getContentScale();

	for each (auto font in mTextureFonts)
	{
		auto loadingName = font.second;
		auto fontSize = font.first->getFont().getSize();

		if (fontSize == size && loadingName == name) return font.first; // Already loaded
	}

	try
	{
		std::string pre = "Font/";
		ci::Font font(ci::app::loadAsset(pre + name), size);
		mTextureFonts.push_back(std::pair<ci::gl::TextureFontRef, std::string>(ci::gl::TextureFont::create(font, format, defaultChars()), name));
		return mTextureFonts.back().first;
	}
	catch (ci::FontInvalidNameExc exc)
	{
		CI_LOG_E(exc.what());
		return nullptr;
	}
	catch (ci::app::AssetLoadExc exc)
	{
		CI_LOG_E(exc.what());
		return nullptr;
	}
}