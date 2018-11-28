
//  Created by Jan Bundschuh.
//  Copyright 2016-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "cinder/Timeline.h"
#include "cinder/app/App.h"

#include "Statics.h"

namespace bUtil
{
	class Hideable
	{
	public:
		Hideable(float opacity = 0.0f);
		virtual void show(const float & delay = 0.0f);
		virtual void hide(const float & delay = 0.0f);
		virtual void instantShow();
		virtual void instantHide();

		virtual float const getOpacity() { return mOpacity; };
		virtual bool isVisible() { return mOpacity > 0.0f; };

		virtual void setSpeed(float speed) { mSpeed = speed; };
		virtual const float getSpeed() { return mSpeed; };

		virtual const ci::EaseFn getEasing() { return mEasing; };
		virtual void setEasing(ci::EaseFn easing) { mEasing = easing; };

		virtual void setOpacityMax(const float & opacity) { mOpacityMax = opacity; };
		virtual const float getOpacityMax() { return mOpacityMax; };

		virtual void setDelay(const float & delay) { mDelay = delay; };

		virtual const float getDelay() { return mDelay; };
		virtual ~Hideable() = default;

	protected:
		ci::Anim<float> mOpacity;
		float mOpacityMax;
		float mSpeed;
		float mDelay;
		ci::EaseFn mEasing;
	};
}