
//  Created by Jan Bundschuh on 28/08/18.
//  Copyright 2015-2018 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#include "RadialSlider.h"

bUtil::UI::RadialSlider::RadialSlider(vec2 center, float radius, float width, float start, float arcLength) : UI::ButtonDraggable(center),
mCenter(center), mRadius(radius), mWidth(width), mStart(start), mArcLength(arcLength), mDownOffsetDegrees(0.0f), mMinDegrees(0.0f), mMaxDegrees(360.0f), mOffsetDegrees(0.0f), mSnap(false), mSnapDegrees(0.0f)
{
	mDegrees = mStart + mArcLength / 2 + mOffsetDegrees;
}

void bUtil::UI::RadialSlider::draw()
{
	if (!isVisible()) return;
	ScopedModelMatrix matrix;

	ScopedColor color(mEnabled ? (mPressed ? ColorA(1, 0, 0, 1.0f) : ColorA(1, 0, 0, 1)) : ColorA(150, 150, 150, 0.25));
	gl::lineWidth(5);
	gl::draw(mBounds);

	{
		ScopedModelMatrix matrix;
		ScopedColor scopedColor2(ColorA(1, 1, 0, mOpacity));
		gl::translate(mCenter);
		gl::rotate(toRadians(mDegrees));
		gl::drawSolidCircle(vec2(mRadius, 0), 10);
	}
}

void bUtil::UI::RadialSlider::update()
{
	UI::ButtonDraggable::update();

	calculateBounds();

	if (isPressed())
	{
		mDegrees.stop();
	}
}

void bUtil::UI::RadialSlider::mouseDown(const MouseEvent event)
{
	if (!mDegrees.isComplete()) return;

	UI::ButtonDraggable::mouseDown(event);

	mDownOffsetDegrees = Help::Math::angleToCenter(vec2(event.getPos()) - mCenter, false);
	mDownOffsetDegreesDiff = mDownOffsetDegrees - mDegrees;
}

void bUtil::UI::RadialSlider::mouseDrag(const MouseEvent event)
{
	UI::ButtonDraggable::mouseDrag(event);

	if (isEnabled() && isPressed())
	{
		float degrees = Help::Math::angleToCenter(vec2(event.getPos()) - mCenter);

		if (mDownOffsetDegrees < 0.0) mDownOffsetDegrees += 360.0;

		float theta = degrees - mDownOffsetDegreesDiff;
		mDegrees = theta;
	}
}

void bUtil::UI::RadialSlider::mouseUp(const MouseEvent event)
{
	if (mDragged && mSnap)
	{
		float snap = round(mDegrees / mSnapDegrees) * mSnapDegrees;
		timeline().apply(&mDegrees, snap + mArcLength / 2, 0.4f, EaseOutBack());
	}

	mDegrees = mStart + mArcLength / 2 + mOffsetDegrees;

	UI::ButtonDraggable::mouseUp(event);
}

void bUtil::UI::RadialSlider::calculateBounds()
{
	float innerRadius = mRadius - 0.5f * mWidth;
	float outerRadius = mRadius + 0.5f * mWidth;

	float t = toRadians(mDegrees - mArcLength / 2 + mOffsetDegrees);
	vec2 unit(math<float>::cos(t), math<float>::sin(t));
	auto b = mCenter + unit * outerRadius;

	auto start = toRadians(mDegrees - mArcLength / 2 + mOffsetDegrees);
	auto end = toRadians(mDegrees + mArcLength / 2 + mOffsetDegrees);

	mBounds = Path2d();
	mBounds.moveTo(b);
	mBounds.arc(mCenter, outerRadius, start, end, true);
	mBounds.arc(mCenter, innerRadius, end, start, false);
	mBounds.close();
}

void bUtil::UI::RadialSlider::setSnap(const float & degrees)
{
	if (degrees < 180.0f && degrees > 0.000001f)
	{
		mSnapDegrees = degrees;
		mSnap = true;
	}
	else
	{
		CI_LOG_W("Not going to snap that (" << degrees << "°)");
	}
}

void bUtil::UI::RadialSlider::noSnap()
{
	mSnap = false;
	mSnapDegrees = 0.0f;
}
