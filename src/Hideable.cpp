
//  Created by Jan Bundschuh.
//  Copyright 2016-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#include "Hideable.h"

bUtil::Hideable::Hideable(float opacity) : mOpacity(opacity), mOpacityMax(1.0f), mDelay(0.0f)
{
	mSpeed	= Animation::speed();
	mEasing	= Animation::easing();
}

void bUtil::Hideable::show(const float & delay)
{
	ci::app::timeline().apply(&mOpacity, mOpacityMax, mSpeed, mEasing).delay(delay + mDelay);
}

void bUtil::Hideable::hide(const float & delay)
{
	ci::app::timeline().apply(&mOpacity, 0.0f, mSpeed, mEasing).delay(delay);
}

void bUtil::Hideable::instantHide()
{
	mOpacity = 0.0f;
}

void bUtil::Hideable::instantShow()
{
	mOpacity = mOpacityMax;
}