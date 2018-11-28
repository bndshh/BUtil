
//  Created by Jan Bundschuh.
//  Copyright 2016-2018 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include <vector>
#include <math.h>
#include <iostream>
#include <locale>
#include <algorithm>

#include "cinder/Json.h"
#include "cinder/CinderMath.h"
#include "cinder/Vector.h"
#include "cinder/Rect.h"
#include "cinder/Log.h"
#include "cinder/Color.h"

namespace bUtil
{
	namespace Help
	{
		namespace Numbers
		{
			float round2(float val);
			const bool lineIntersectsRect(ci::vec2 a, ci::vec2 b, ci::Rectf r);
			const bool lineIntersectsLine(ci::vec2 a1, ci::vec2 a2, ci::vec2 b1, ci::vec2 b2);
		};

		namespace String
		{
			std::vector<std::string> split(const std::string & s, char seperator, bool keepSeperator = true);
			const bool isImage(const std::string & extension);
			std::string toUpperGerman(std::string & str);
			unsigned getHexFromString(const std::string & str);
			std::string intVectorToString(const std::vector<int> & v);
			ci::Color hexStringToColor(const std::string & hex);
			char * stringToChar(const std::string & str);
			std::string intToString(const int & i);
			std::string floatToString(const float & f);
			std::string replaceAll(const std::string & str, const std::string & toFind, const std::string & replaceWith);
		};

		namespace JSON
		{
			const std::string parseString(const ci::JsonTree & parent, const std::string & childName);
			const bool parseBool(const ci::JsonTree & parent, const std::string & childName);
			const int parseInt(const ci::JsonTree & parent, const std::string & childName);
			std::pair<std::string, std::string> parseTexts(const ci::JsonTree & parent, const std::string & childName);
			std::vector<std::string> parseStringArray(const ci::JsonTree & parent, const std::string & childName);
			const glm::vec2 parseVec2(const ci::JsonTree & parent, const std::string & childName);
			const unsigned long parseHex(const ci::JsonTree & parent, const std::string & childName);
		};

		namespace Time
		{
			const unsigned long long currentMillies();
		};

		namespace Math
		{
			float cmToInches(const float & cm);
			float inchesToCm(const float & inches);

			ci::vec2 diagonalToSize(const float & value, const ci::vec2 & aspectRatio);
			float sizeToDiagonal(const ci::vec2 & sizeCm);

			const float addToAverage(const float & average, const int & size, const float & value);
			const float subtractFromAverage(const float & average, const int & size, const float & value);
			const float replaceInAverage(const float & average, const int & size, const float & oldValue, const float & newValue);
			const float addAveragesTogether(const float & averageA, const int & sizeA, const float & averageB, const int & sizeB);

			const float angleToCenter(const ci::vec2 & center, const bool & positive = true);
			const float normalizeDegrees(const float & degrees, const float & min = 0.0f, const float & max = 360.0f);
			const float normalizeDegrees2(const float & degrees);
		}
	};
};