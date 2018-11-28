
//  Created by Jan Bundschuh.
//  Copyright 2016-2018 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#include "Element.h"

bUtil::UI::Element::Element(vec2 position, bool enable, AnchorPoint anchor) : Hideable(), HasAssets(), Bilingual(), 
mPosition(position), mEnabled(enable), mRotation(0), mAnchorPoint(anchor) 
{
	mElements = std::vector<std::shared_ptr<Element>>();
}

void bUtil::UI::Element::draw()
{
	if (isVisible())
	{
		for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
		{
			auto element = *iter;
			if (element) element->draw();
		}
	}
}

void bUtil::UI::Element::resize()
{
	for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
	{
		auto element = *iter;
		if (element) element->resize();
	}
}

void bUtil::UI::Element::update()
{
	if (isEnabled())
	{
		for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
		{
			auto element = *iter;
			if (element) element->update();
		}
	}
}

void bUtil::UI::Element::show(const float & delay)
{
	enable();
	Hideable::show(delay);
	
	for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
	{
		auto element = *iter;
		if (element) element->show(delay);
	}
}

void bUtil::UI::Element::hide(const float & delay)
{
	disable();
	Hideable::hide(delay);
	
	for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
	{
		auto element = *iter;
		if (element) element->hide(delay);
	}
}

void bUtil::UI::Element::instantShow()
{
	enable();
	Hideable::instantShow();
	
	for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
	{
		auto element = *iter;
		if (element) element->instantShow();
	}
}

void bUtil::UI::Element::instantHide()
{
	disable();
	Hideable::instantHide();
	
	for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
	{
		auto element = *iter;
		if (element) element->instantHide();
	}
}

void bUtil::UI::Element::mouseDown(const app::MouseEvent event)
{
	if (isEnabled())
	{
		for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
		{
			auto element = *iter;
			if (element) element->mouseDown(event);
		}
	}
}

void bUtil::UI::Element::mouseDrag(const app::MouseEvent event)
{
	if (isEnabled())
	{
		for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
		{
			auto element = *iter;
			if (element) element->mouseDrag(event);
		}
	}
}

void bUtil::UI::Element::mouseUp(const app::MouseEvent event)
{
	if (isEnabled())
	{
		for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
		{
			auto element = *iter;
			element->mouseUp(event);
		}
	}
}

void bUtil::UI::Element::mouseWheel(const app::MouseEvent event)
{
	if (isEnabled())
	{
		for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
		{
			auto element = *iter;
			if (element) element->mouseWheel(event);
		}
	}
}

void bUtil::UI::Element::touchesBegan(const app::TouchEvent event)
{
	if (isEnabled())
	{
		for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
		{
			auto element = *iter;
			if (element) element->touchesBegan(event);
		}
	}
}

void bUtil::UI::Element::touchesMoved(const app::TouchEvent event)
{
	if (isEnabled())
	{
		for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
		{
			auto element = *iter;
			if (element) element->touchesMoved(event);
		}
	}
}

void bUtil::UI::Element::touchesEnded(const app::TouchEvent event)
{
	if (isEnabled())
	{
		for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
		{
			auto element = *iter;
			if (element) element->touchesEnded(event);
		}
	}
}

void bUtil::UI::Element::enable(const bool & enabled)
{
	if (enabled != mEnabled)
	{
		mEnabled = enabled;

		for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
		{
			auto element = *iter;
			if (element) element->enable(mEnabled);
		}
	}
}

void bUtil::UI::Element::disable(const bool & enabled)
{
	enable(!enabled);
}

const bool bUtil::UI::Element::isEnabled()
{
	return mEnabled;
}

void bUtil::UI::Element::setA()
{
	Bilingual::setA();

	for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
	{
		auto element = *iter;
		if (element) element->setA();
	}
}

void bUtil::UI::Element::setB()
{
	Bilingual::setB();

	for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
	{
		auto element = *iter;
		if (element) element->setB();
	}
}

void bUtil::UI::Element::toggleLanguage()
{
	Bilingual::toggleLanguage();

	for (auto iter = mElements.begin(); iter != mElements.end(); ++iter)
	{
		auto element = *iter;
		if (element) element->toggleLanguage();
	}
}
