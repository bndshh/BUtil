
//  Created by Jan Bundschuh on 28/08/18.
//  Copyright 2015-2018 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "ButtonDraggable.h"
#include "Help.h"

namespace bUtil
{
	namespace UI
	{

		class RadialSlider : public ButtonDraggable
		{
		public:
			RadialSlider(vec2 center, float radius, float width, float start, float arcLength);
			virtual void draw() override;
			virtual void update() override;
			virtual void mouseDown(const MouseEvent event) override;
			virtual void mouseDrag(const MouseEvent event) override;
			virtual void mouseUp(const MouseEvent event) override;
			virtual void calculateBounds() override;
			virtual void setSnap(const float & degrees);
			virtual void noSnap();

			virtual ~RadialSlider() = default;

		protected:
			vec2 mCenter;
			float mRadius;
			float mWidth;
			float mStart;

			Anim<float> mDegrees;

			float mDownOffsetDegrees;
			float mDownOffsetDegreesDiff;

			float mOffsetDegrees;

			float mMinDegrees;
			float mMaxDegrees;

			float mArcLength;

			bool mSnap;
			float mSnapDegrees;
		};

		typedef std::shared_ptr<RadialSlider> RadialSliderRef;
	}
}