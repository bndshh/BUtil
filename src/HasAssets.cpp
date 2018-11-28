
//  Created by Jan Bundschuh.
//  Copyright 2016-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#include "HasAssets.h"

TextureRef bUtil::HasAssets::loadTexture(const fs::path &path)
{
	TextureRef texture;

	try
	{
		try
		{
			if (!path.empty())
			{
				auto img = loadImage(path);
				texture = Texture2d::create(img);
			}
		}
		catch (ImageIoExceptionFailedLoad exc)
		{
			CI_LOG_W("'" << path.filename() << "' not found!" << exc.what());
		}
	}
	catch (AssetLoadExc exc)
	{
		CI_LOG_W("'" << path.filename() << "'Asset not found!" << exc.what());
	}

	return texture;
}

TextureRef bUtil::HasAssets::loadTextureFromAssets(const std::string &path)
{
	TextureRef texture;

	try
	{
		auto assetPath = getAssetPath(path);

		try
		{
			try
			{
				if (assetPath.empty())
				{
					auto img = loadImage(path);
					texture = Texture2d::create(img);
				}
				else
				{
					auto img = loadImage(assetPath);
					texture = Texture2d::create(img);
				}

			}
			catch (ImageIoExceptionFailedLoad exc)
			{
				CI_LOG_W("'" << path << "' not found!" << exc.what());
			}
		}
		catch (ImageIoException exc)
		{
			CI_LOG_W("'" << path << "' not found!" << exc.what());
		}
	}
	catch (AssetLoadExc exc)
	{
		CI_LOG_W("'" << path << "'Asset not found!" << exc.what());
	}

	return texture;
}

TextureRef bUtil::HasAssets::loadTextureFromAssetsWithPath(const fs::path &path)
{
	return loadTextureFromAssets(path.string());
}

VoiceRef bUtil::HasAssets::loadAudioFromAssets(const std::string & path)
{	
	VoiceRef voice;

	if (!OptionsSingleton::Instance()->useAudio()) return voice;

	try
	{
		auto assetPath = getAssetPath(path);
		auto extension = assetPath.extension();

		if (extension == ".wav" || extension == ".mp3" || extension == ".ogg")
		{
			try
			{
				if (!assetPath.empty())
				{
					auto sourceFile = audio::load(app::loadAsset(path));
					voice = Voice::create(sourceFile);
				}
			}
			catch (AudioFileExc exc)
			{
				CI_LOG_W("'" << path << "' not found!" << exc.what());
			}
		}
	}
	catch (AssetLoadExc exc)
	{
		CI_LOG_W("'" << path << "'Asset not found!" << exc.what());
	}

	return voice;
}
