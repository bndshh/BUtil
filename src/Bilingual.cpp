
//  Created by Jan Bundschuh.
//  Copyright 2016 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#include "Bilingual.h"

bUtil::Bilingual::Bilingual(bool A) : mA(A) 
{
	mChildren = std::vector<std::shared_ptr<Bilingual>>();
}

void bUtil::Bilingual::addChild(std::shared_ptr<Bilingual> child)
{
	mChildren.push_back(child);
}

const bool bUtil::Bilingual::isA()
{ 
	return mA; 
}

const bool bUtil::Bilingual::isB()
{ 
	return !mA; 
}

void bUtil::Bilingual::setA()
{ 
	mA = true; 

	for (auto iter = mChildren.begin(); iter != mChildren.end(); iter++)
	{
		auto child = *iter;
		child->setA();
	}

}

void bUtil::Bilingual::setB()
{ 
	mA = false; 

	for (auto iter = mChildren.begin(); iter != mChildren.end(); iter++)
	{
		auto child = *iter;
		child->setB();
	}
}

void bUtil::Bilingual::toggleLanguage()
{
	mA = !mA; 

	for (auto iter = mChildren.begin(); iter != mChildren.end(); iter++)
	{
		auto child = *iter;
		child->toggleLanguage();
	}
}

const size_t bUtil::Bilingual::getNumChildren()
{
	return mChildren.size();
}