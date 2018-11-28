
//  Created by Jan Bundschuh on 27/04/15.
//  Copyright 2016-2018 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#include "Button.h"

bUtil::UI::Button::Button(vec2 position, vec2 size, bool enable, AnchorPoint anchor) : UI::Element(position, false, anchor), 
	mSize(size), mInflation(0.0f), mBounds(), mPressed(false), mClicked(false), mReleased(false), mToggable(false), mToggle(false)
{ 
	mDownOffsets = std::vector<vec2>();
	mReleaseOffsets = std::vector<vec2>();
	mDownTouchPoints = std::vector<TouchPoint>();

	init();
}

void bUtil::UI::Button::init()
{
	calculateBounds();

	setOnClicked([] {});
	setOnRelease([] {});
	setOnDown([] {});
	setOnUp([] {});
	setOnToggle([] {});
}

void bUtil::UI::Button::update()
{
	if (isEnabled())
	{
		if (mClicked)
		{
			mClicked = false;
			mToggle = !mToggle;
			if (mToggable) mOnToggle();
			mOnClicked();
		}
		if (mReleased)
		{
			mReleased = false;
			mOnRelease();
		}
	}

	Element::update();
}

void bUtil::UI::Button::calculateBounds()
{
	vec2 size = getSize();
	vec2 position = mPosition;

	vec2 a, b, c, d;

	if (mAnchorPoint == AnchorPoint::Center)
	{
		a = vec2(position.x - size.x / 2, position.y - size.y / 2);
		b = vec2(position.x + size.x / 2, position.y - size.y / 2);
		c = vec2(position.x + size.x / 2, position.y + size.y / 2);
		d = vec2(position.x - size.x / 2, position.y + size.y / 2);
	}
	else if (mAnchorPoint == AnchorPoint::TopLeft)
	{
		a = vec2(position.x, position.y);
		b = vec2(position.x + size.x, position.y);
		c = vec2(position.x + size.x, position.y + size.y);
		d = vec2(position.x, position.y + size.y);
	}

	Path2d path;
	path.moveTo(a);
	path.lineTo(b);
	path.lineTo(c);
	path.lineTo(d);
	path.close();

	mBounds = path;
}

void bUtil::UI::Button::mouseDown(const app::MouseEvent event)
{
	if (isEnabled() && !isPressed())
	{
		mPressed = false;
		mClicked = false;

		mDownOffsets.clear();
		mReleaseOffsets.clear();

		vec2 mousePos = event.getPos();

		auto inflatedBounds = mBounds;
		inflatedBounds.scale(vec2(1.0f + mInflation));

		if (inflatedBounds.contains(mousePos))
		{
			mPressed = true;
			mDownTouchPoints.push_back(TouchPoint(0, mousePos));
			mDownOffsets.push_back(mousePos - vec2(mPosition));

			mOnDown();
		}
	}

	Element::mouseDown(event);
}

void bUtil::UI::Button::mouseDrag(const app::MouseEvent event)
{
	Element::mouseDrag(event);
}

void bUtil::UI::Button::mouseUp(const app::MouseEvent event)
{
	if (isEnabled())
	{
		mOnUp();
		vec2 mousePos = event.getPos();

		if (isPressed())
		{
			auto inflatedBounds = mBounds;
			inflatedBounds.scale(vec2(1.0f + mInflation));

			if (inflatedBounds.contains(mousePos))
			{
				mClicked = true;
			}

			mReleaseOffsets.push_back(mousePos - vec2(mPosition));

			mReleased = true;
			mPressed = false;
		}
	}

	Element::mouseUp(event);
}

void bUtil::UI::Button::touchesBegan(const app::TouchEvent event)
{
	if (isEnabled() && !isPressed())
	{
		mPressed = false;
		mClicked = false;
		//mCurrentTouchIds.clear();
		//mDownPositions.clear();
		//mReleaseOffsets.clear();
		//mDownOffsets.clear();
		
		auto touches = event.getTouches();
		auto inflatedBounds = mBounds;
		inflatedBounds.scale(vec2(1.0f + mInflation));

		for (auto touch : touches)
		{
			auto id = touch.getId();
			auto pos = touch.getPos();

			if (inflatedBounds.contains(pos))
			{
				mPressed = true;

				mDownTouchPoints.push_back(TouchPoint(id, pos));
				mDownOffsets.push_back(pos - vec2(mPosition));

				mOnDown();

				break;
			}
		}
	}

	Element::touchesBegan(event);
}

void bUtil::UI::Button::touchesMoved(const app::TouchEvent event)
{
	Element::touchesMoved(event);
}

void bUtil::UI::Button::touchesEnded(const app::TouchEvent event)
{
	if (isEnabled())
	{
		auto touches = event.getTouches();
		auto inflatedBounds = mBounds;
		inflatedBounds.scale(vec2(1.0f + mInflation));
		
		for (auto touch : touches)
		{
			auto id = touch.getId();
			auto pos = touch.getPos();

			auto it = mDownTouchPoints.begin();
			while (it != mDownTouchPoints.end())
			{
				auto point = *it;
				if (point.id == id)
				{
					mOnUp();

					if (isPressed() && inflatedBounds.contains(pos))
					{
						mClicked = true;
						mReleaseOffsets.push_back(pos - vec2(mPosition));
					}

					if (mPressed) mReleased = true;
					mPressed = false;

					it = mDownTouchPoints.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	}

	Element::touchesEnded(event);
}

const std::vector<vec2> bUtil::UI::Button::getDownOffset()
{
	return mDownOffsets;
}

const std::vector<vec2> bUtil::UI::Button::getReleaseOffset()
{
	return mReleaseOffsets;
}

vec2 const bUtil::UI::Button::getSize()
{
	return vec2(mSize);
}

void bUtil::UI::Button::draw()
{
	if (isVisible())
	{
		ScopedColor color(mEnabled ? (mPressed ? ColorA(Color(1, .2f, .2f), 1.0f) : ColorA(1, 0, 0, 1)) : ColorA(1, 1, 0, 0.25));
		gl::draw(mBounds);
	}
}

void bUtil::UI::Button::setSize(const vec2 &size, const bool &animate)
{ 
	if (animate)
	{
		timeline().apply(&mSize, size, Animation::speed(), Animation::easing()).updateFn([this] { calculateBounds(); });
	}
	else
	{
		mSize = size;
		calculateBounds();
	}
}

void bUtil::UI::Button::setAnchorPoint(const AnchorPoint anchor)
{
	if (mAnchorPoint != anchor)
	{ 
		mAnchorPoint = anchor; 
		calculateBounds();
	}
}
