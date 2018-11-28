
//  Created by Jan Bundschuh.
//  Copyright 2016-2018 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#include "Help.h"

float bUtil::Help::Numbers::round2(float val)
{
	return floorf(val * 1000) / 1000;
}

const bool bUtil::Help::Numbers::lineIntersectsRect(ci::vec2 a, ci::vec2 b, ci::Rectf r)
{
	return lineIntersectsLine(a, b, ci::vec2(r.x1, r.y1), ci::vec2(r.x1 + r.getWidth(), r.y1)) ||
		lineIntersectsLine(a, b, ci::vec2(r.x1 + r.getWidth(), r.y1), ci::vec2(r.x1 + r.getWidth(), r.y1 + r.getHeight())) ||
		lineIntersectsLine(a, b, ci::vec2(r.x1 + r.getWidth(), r.y1 + r.getHeight()), ci::vec2(r.x1, r.y1 + r.getHeight())) ||
		lineIntersectsLine(a, b, ci::vec2(r.x1, r.y1 + r.getHeight()), ci::vec2(r.x1, r.y1)) ||
		(r.contains(a) && r.contains(b));
}

const bool bUtil::Help::Numbers::lineIntersectsLine(ci::vec2 a1, ci::vec2 a2, ci::vec2 b1, ci::vec2 b2)
{
	float q = (a1.x - b1.y) * (b2.x - b1.x) - (a1.x - b1.x) * (b2.y - b1.y);
	float d = (a2.x - a1.x) * (b2.y - b1.y) - (a2.y - a1.y) * (b2.x - b1.x);

	if (d == 0) return false;

	float r = q / d;

	q = (a1.y - b1.y) * (a2.x - a1.x) - (a1.x - b1.x) * (a2.y - a1.y);
	float s = q / d;

	return (r < 0 || r > 1 || s < 0 || s > 1) ? false : true;
}

std::vector<std::string> bUtil::Help::String::split(const std::string & s, char seperator, bool keepSeperator)
{
	std::vector<std::string> output;

	std::string::size_type prevPos = 0;
	std::string::size_type pos = 0;

	while ((pos = s.find(seperator, pos)) != std::string::npos)
	{
		std::string substring(s.substr(prevPos, pos - prevPos));
		if (!keepSeperator && prevPos > 0)
		{
			substring = substring.substr(1, substring.length());
		}
		output.push_back(substring);
		prevPos = pos++;
	}


	output.push_back(s.substr(prevPos + (keepSeperator ? 0 : 1), pos - prevPos));

	return output;
}

const bool bUtil::Help::String::isImage(const std::string &extension)
{
	return extension == "\".jpg\"" || extension == "\".jpeg\"" || extension == "\".png\"";
}

std::string bUtil::Help::String::toUpperGerman(std::string & str)
{
	std::string fuck[4] = { "ä", "ü", "ö", "ß" };
	std::string to[4] = { "Ä", "Ü", "Ö", "SS" };
	for (size_t i = 0; i < 4; i++)
	{
		std::string from = fuck[i];
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos)
		{
			str = str.replace(start_pos, from.length(), to[i]);
		}
	}

	transform(str.begin(), str.end(), str.begin(), &toupper);
	return str;
}

std::string bUtil::Help::String::intVectorToString(const std::vector<int> & v)
{
	std::string str = "[ ";

	for (size_t i = 0; i < v.size(); i++)
	{
		str += ci::toString(v.at(i));		
		if (i < v.size() - 1) str += ", ";
	}

	return str + " ]";
}

unsigned bUtil::Help::String::getHexFromString(const std::string & str)
{
	try
	{
		return stoi(str, 0, 16);
	}
	catch (std::invalid_argument & exc)
	{
		CI_LOG_W(exc.what() << "With string: '" << str << "'!"); 
		return 0;
	}
}

ci::Color bUtil::Help::String::hexStringToColor(const std::string & hex)
{
	if (hex.size() == 7)
	{
		int number = (int)strtol(&hex[1], NULL, 16);
		int r = number >> 16;
		int g = number >> 8 & 0xFF;
		int b = number & 0xFF;
		return ci::Color(r / 255.0f, g / 255.0f, b / 255.0f);	
	}

	return ci::Color();
}

char * bUtil::Help::String::stringToChar(const std::string & str)
{
	char * result = new char[str.length() + 1];
	strcpy(result, str.c_str());
	return result;
}

std::string bUtil::Help::String::intToString(const int & i)
{
	std::stringstream ss;
	ss << i;
	return ss.str();
}

std::string bUtil::Help::String::floatToString(const float & f)
{
	std::stringstream ss;
	ss << f;
	return ss.str();
}

std::string bUtil::Help::String::replaceAll(const std::string & str, const std::string & toFind, const std::string & replaceWith)
{
	std::string result = "";

	if (toFind.empty() || str.empty())
	{
		return result;
	}
	else {
		result = str;
		size_t startPosition = 0;
		size_t tofindLength = toFind.length();
		while ((startPosition = result.find(toFind, startPosition)) != std::string::npos)
		{
			result.replace(startPosition, tofindLength, replaceWith);
			startPosition += tofindLength;
		}
		return result;
	}
}

const std::string bUtil::Help::JSON::parseString(const ci::JsonTree & parent, const std::string & childName)
{
	return parent.getChild(childName).getValue();
}

const bool bUtil::Help::JSON::parseBool(const ci::JsonTree & parent, const std::string & childName)
{
	std::string str = parseString(parent, childName);
	return (str == "1" || str == "true") ? true : false;
}
const int bUtil::Help::JSON::parseInt(const ci::JsonTree & parent, const std::string & childName)
{
	return atoi(parseString(parent, childName).c_str());
}

std::pair<std::string, std::string> bUtil::Help::JSON::parseTexts(const ci::JsonTree & parent, const std::string & childName)
{
	auto child = parent.getChild(childName);
	auto children = child.getChildren();

	if (children.size() != 2)
	{
		//CI_LOG_W("Error parsing texts: Not 2 childs");
		if (children.size() == 1)
		{
			return std::pair<std::string, std::string>(children.front().getValue().c_str(), "");
		}
		else
		{
			return std::pair<std::string, std::string>();
		}
	}

	auto a = children.front().getValue().c_str();
	auto b = children.back().getValue().c_str();

	return std::pair<std::string, std::string>(a, b);
}

std::vector<std::string> bUtil::Help::JSON::parseStringArray(const ci::JsonTree & parent, const std::string & childName)
{
	std::vector<std::string> strings;

	auto child = parent.getChild(childName);
	auto children = child.getChildren();

	for each (auto c in children)
	{
		strings.push_back(c.getValue<std::string>());
	}

	return strings;
}

const glm::vec2 bUtil::Help::JSON::parseVec2(const ci::JsonTree & parent, const std::string & childName)
{
	auto child = parent.getChild(childName);
	auto children = child.getChildren();

	if (children.size() != 2)
	{
		CI_LOG_W("Error parsing vec2(): Not 2 childs");
		return glm::vec2();
	}

	auto valueX = children.front().getValue().c_str();
	auto valueY = children.back().getValue().c_str();

	return glm::vec2(std::stof(valueX), std::stof(valueY));
}

const unsigned long bUtil::Help::JSON::parseHex(const ci::JsonTree & parent, const std::string & childName)
{
	return std::stoul(parseString(parent, childName), nullptr, 16);
}

const unsigned long long bUtil::Help::Time::currentMillies()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}


float bUtil::Help::Math::cmToInches(const float & cm)
{
	return cm / 2.54f; // * 0.393701
}

float bUtil::Help::Math::inchesToCm(const float & inches)
{
	return inches * 2.54f;
}

ci::vec2 bUtil::Help::Math::diagonalToSize(const float & value, const ci::vec2 & aspectRatio)
{
	float a = aspectRatio.x;
	float b = aspectRatio.y;

	float w = (a / sqrtf(pow(a, 2) + pow(b, 2))) * value;

	return ci::vec2(w, w * (b / a));
}

float bUtil::Help::Math::sizeToDiagonal(const ci::vec2 & sizeCm)
{
	return sqrtf(pow(sizeCm.x, 2) + pow(sizeCm.y, 2));
}

const float bUtil::Help::Math::addToAverage(const float & average, const int & size, const float & value)
{
	return (size * average + value) / (size + 1);
}

const float bUtil::Help::Math::subtractFromAverage(const float &  average, const int & size, const float &  value)
{
	return (size * average - value) / (size - 1);
}

const float bUtil::Help::Math::replaceInAverage(const float & average, const int & size, const float & oldValue, const float & newValue)
{
	return (size * average - oldValue + newValue) / size;
}

const float bUtil::Help::Math::addAveragesTogether(const float & averageA, const int & sizeA, const float & averageB, const int & sizeB)
{
	return (sizeA * averageA + sizeB * averageB) / (sizeA + sizeB);
}

const float bUtil::Help::Math::angleToCenter(const ci::vec2 & center, const bool & positive)
{
	auto angle = ci::toDegrees(atan2(center.y, center.x));
	if (positive && angle < 0.0) angle += 360.0;
	return angle;
}

// Normalizes any number to an arbitrary range 
// by assuming the range wraps around when going below min or above max 
const float bUtil::Help::Math::normalizeDegrees(const float & degrees, const float & min, const float & max)
{
	if (degrees >= min || degrees <= max) return degrees;

	const float width = max - min;   // 
	const float offsetValue = degrees - min;   // value relative to 0

	return (offsetValue - (floor(offsetValue / width) * width)) + min;
	// + start to reset back to start of original range
}

const float bUtil::Help::Math::normalizeDegrees2(const float & degrees)
{
	auto deg = degrees;
	while (deg <   0.0f) deg += 360.0f;
	while (deg > 359.0f) deg -= 360.0f;

	return deg;
}
