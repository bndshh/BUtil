
//  Created by Jan Bundschuh on 28/08/18.
//  Copyright 2015-2018 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#include "RadialSliderResize.h"

bUtil::UI::RadialSliderResize::RadialSliderResize(vec2 center, float radius, float width, float start, float end, float offset) : UI::ButtonDraggable(center),
mCenter(center), mRadius(radius), mWidth(width), mStart(start), mEnd(end), mDownOffsetDegrees(0.0f), mArcLengthMax(180.0f), mOffsetDegrees(offset), mSnap(false), mSnapDegrees(0.0f), mLeft(false)
{
	mArcLength = abs(mEnd - mStart);
	mDegrees = mArcLength / 2;

	auto shaderDef = gl::ShaderDef().color();
	mShader = gl::getStockShader(shaderDef);

	auto geom = geom::Ring().center(mCenter).radius(radius).width(width).angleStart(start).angleEnd(end).subdivisions(5);
	mRing = Batch::create(geom, mShader);
}

void bUtil::UI::RadialSliderResize::draw()
{
	if (!isVisible()) return;
	ScopedModelMatrix matrix;

	ScopedColor color(mEnabled ? (mPressed ? ColorA(1, 0, 0, 1.0f) : ColorA(1, 0, 0, 1)) : ColorA(150, 150, 150, 0.25));
	gl::lineWidth(2);
	gl::draw(mBounds);

	// ScopedColor ringColor(1, 1, 1, mOpacity);
	// mRing->draw();

	{
		ScopedModelMatrix matrix;
		ScopedColor scopedColor2(ColorA(0, 1, 0, mOpacity));
		gl::translate(mCenter);
		gl::rotate(toRadians(mStart + mOffsetDegrees));
		gl::drawSolidCircle(vec2(mRadius, 0), 10);
	}

	{
		ScopedModelMatrix matrix;
		ScopedColor scopedColor2(ColorA(1, 0, 0, mOpacity));
		gl::translate(mCenter);
		gl::rotate(toRadians(mEnd + mOffsetDegrees));
		gl::drawSolidCircle(vec2(mRadius, 0), 10);
	}

	{
		ScopedModelMatrix matrix;
		ScopedColor scopedColor2(ColorA(1, 1, 0, mOpacity));
		gl::translate(mCenter);
		gl::rotate(toRadians(mDegrees));
		gl::drawSolidCircle(vec2(mRadius, 0), 10);
	}
}

void bUtil::UI::RadialSliderResize::update()
{
	UI::ButtonDraggable::update();

	mArcLength = mEnd - mStart;
	mArcLength -= mArcLength > 180.0f ? 360.0f : 0.0f;
	mArcLength += mArcLength < -180.0f ? 360.0f : 0.0f;

	mArcLength += mArcLength < 0.0f ? 360.0f : 0.0f;

	mDegrees = mStart + mArcLength / 2 + mOffsetDegrees;

	if (mArcLength < mSnapDegrees)
	{
		if (mLeft) mEnd = mStart + mSnapDegrees;
		else mStart = mEnd - mSnapDegrees;
		mouseUp(MouseEvent());
	}
	else if (mArcLength > mArcLengthMax)
	{
		if (mLeft) mEnd = mStart + mArcLengthMax;
		else mStart = mEnd - mArcLengthMax;
		mouseUp(MouseEvent());
	}

	calculateBounds();

	if (isPressed()) mDegrees.stop();
}

void bUtil::UI::RadialSliderResize::mouseDown(const MouseEvent event)
{
	if (!mDegrees.isComplete()) return;

	UI::ButtonDraggable::mouseDown(event);

	float downAngle = Help::Math::angleToCenter(vec2(event.getPos()) - mCenter, false);

	mDownOffsetDegreesDiff = downAngle - mDegrees;
	mDownOffsetDegreesDiff -= mDownOffsetDegreesDiff > 180.0f ? 360.0f : 0.0f;
	mDownOffsetDegreesDiff += mDownOffsetDegreesDiff < -180.0f ? 360.0f : 0.0f;

	// CI_LOG_I("deg: " << mDegrees << ", down: " << downAngle << ", down offset: " << mDownOffsetDegreesDiff << ", button: " << mButtonDegrees);

	mLeft = 0.0f <= mDownOffsetDegreesDiff;
	mDownOffsetDegrees = mLeft ? downAngle - mEnd : downAngle - mStart;
}

void bUtil::UI::RadialSliderResize::mouseDrag(const MouseEvent event)
{
	UI::ButtonDraggable::mouseDrag(event);

	if (isEnabled() && isPressed())
	{
		float degrees = Help::Math::angleToCenter(vec2(event.getPos()) - mCenter);

		if (mDownOffsetDegrees < 0.0) mDownOffsetDegrees += 360.0;
		float theta = degrees - mDownOffsetDegrees;
		if (theta < 0.0) theta += 360.0;

		if (mLeft)
		{
			if (mEnd != theta) mEnd = theta;
		}
		else
		{
			if (mStart != theta) mStart = theta;
		}
	}
}

void bUtil::UI::RadialSliderResize::mouseUp(const MouseEvent event)
{
	if (mDragged && mSnap)
	{
		//float snap = round(mDegrees / mStepDegrees) * mStepDegrees;
		//setDegrees(snap - mButtonDegrees / 2, false);

		if (mLeft)
		{
			float snapEnd = round(mEnd / mSnapDegrees) * mSnapDegrees;
			timeline().apply(&mEnd, snapEnd, 0.4f, EaseOutBack());
		}
		else
		{
			float snapStart = round(mStart / mSnapDegrees) * mSnapDegrees;
			timeline().apply(&mStart, snapStart, 0.4f, EaseOutBack());
		}
	}

	UI::ButtonDraggable::mouseUp(event);
}

void bUtil::UI::RadialSliderResize::calculateBounds()
{
	float innerRadius = mRadius - 0.5f * mWidth;
	float outerRadius = mRadius + 0.5f * mWidth;

	auto s = Help::Math::normalizeDegrees2(mStart + mOffsetDegrees);
	auto e = Help::Math::normalizeDegrees2(mEnd + mOffsetDegrees);

	auto start = toRadians(s);
	auto end = toRadians(e);

	vec2 unit(math<float>::cos(start), math<float>::sin(start));
	auto b = mCenter + unit * outerRadius;

	mBounds = Path2d();
	mBounds.moveTo(b);
	mBounds.arc(mCenter, outerRadius, start, end, true);
	mBounds.arc(mCenter, innerRadius, end, start, false);
	mBounds.close();

	if (e == 0.0f && s == 180.0f)
	{
		// auto temp = start;
		//start = end;
		// end = temp;
	}

	auto geom = geom::Ring().center(mCenter).radius(mRadius).width(mWidth).angleStart(toDegrees(start)).angleEnd(toDegrees(end));
	mRing = Batch::create(geom, mShader);
}

void bUtil::UI::RadialSliderResize::setSnap(const float & degrees)
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

void bUtil::UI::RadialSliderResize::noSnap()
{
	mSnap = false;
	mSnapDegrees = 0.0f;
}

const float bUtil::UI::RadialSliderResize::getDegrees()
{
	return mDegrees;
}

const float bUtil::UI::RadialSliderResize::getArcLength()
{
	return mArcLength;
}

void bUtil::UI::RadialSliderResize::setArcLengthMax(const float & degrees)
{
	mArcLengthMax = degrees;
}

void bUtil::UI::RadialSliderResize::setStartDegrees(const float & degrees)
{
	mStart = degrees;
}

const float bUtil::UI::RadialSliderResize::getStartDegrees()
{
	return mStart;
}

void bUtil::UI::RadialSliderResize::setEndDegrees(const float & degrees)
{
	mEnd = degrees;
}

const float bUtil::UI::RadialSliderResize::getEndDegrees()
{
	return mEnd;
}

const bool bUtil::UI::RadialSliderResize::contains(std::shared_ptr<RadialSliderResize> slider)
{
	auto d = slider->getDegrees();
	auto l = slider->getArcLength();
	auto s = d - l / 2.0f;
	auto e = d + l / 2.0f;
	
	auto s_ = mDegrees - mArcLength / 2.0f;
	auto e_ = mDegrees + mArcLength / 2.0f;

	return s_ <= s && e_ >= e;
}

const bool bUtil::UI::RadialSliderResize::borders(std::shared_ptr<RadialSliderResize> slider)
{
	return mStart == slider->getEndDegrees() || mEnd == slider->getStartDegrees();
}

const bool bUtil::UI::RadialSliderResize::intersects(std::shared_ptr<RadialSliderResize> slider)
{
	float d = slider->getDegrees();
	float l = slider->getArcLength();

	float s = d - l / 2.0f;
	float e = d + l / 2.0f;

	float s_ = mDegrees - mArcLength / 2.0f;
	float e_ = mDegrees + mArcLength / 2.0f;

	return s_ < s || e_ > e;
}

const bool bUtil::UI::RadialSliderResize::merge(std::shared_ptr<RadialSliderResize> slider)
{
	auto d = slider->getDegrees();
	auto l = slider->getArcLength();

	auto s = d - l / 2.0f;
	if (s < 360.0f) s += 360.0f;
	if (s >= 360.0f) s -= 360.0f;
	auto e = d + l / 2.0f;
	if (e < 360.0f) e += 360.0f;
	if (e >= 360.0f) e -= 360.0f;

	auto s_ = mDegrees - mArcLength / 2.0f;
	if (s_ < 360.0f) s_ += 360.0f;
	if (s_ >= 360.0f) s_ -= 360.0f;
	auto e_ = mDegrees + mArcLength / 2.0f;
	if (e_ < 360.0f) e_ += 360.0f;
	if (e_ >= 360.0f) e_ -= 360.0f;

	setStartDegrees(s_ < s ? mStart : slider->getStartDegrees());
	setEndDegrees(e_ > e ? mEnd : slider->getEndDegrees());

	return false;
}

const float bUtil::UI::RadialSliderResize::getArcLengthMax()
{
	return mArcLengthMax;
}

void bUtil::UI::RadialSliderResize::setOffsetDegrees(const float & degrees)
{
	mOffsetDegrees = degrees;
}

const float bUtil::UI::RadialSliderResize::getOffsetDegrees()
{
	return mOffsetDegrees;
}
