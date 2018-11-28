
//  Created by Jan Bundschuh on 28/08/18.
//  Copyright 2015-2018 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "ButtonDraggable.h"
#include "Help.h"

namespace bUtil
{
	namespace UI
	{

		class RadialSliderResize : public ButtonDraggable
		{
		public:
			RadialSliderResize(vec2 center, float radius, float width, float start, float end, float offset = 0.0f);
			virtual void draw() override;
			virtual void update() override;
			virtual void mouseDown(const MouseEvent event) override;
			virtual void mouseDrag(const MouseEvent event) override;
			virtual void mouseUp(const MouseEvent event) override;
			virtual void calculateBounds() override;
			virtual void setSnap(const float & degrees);
			virtual void noSnap();

			virtual const float getDegrees();
			virtual const float getArcLength();

			virtual void setArcLengthMax(const float & degrees);
			virtual const float getArcLengthMax();

			virtual void setOffsetDegrees(const float & degrees);
			virtual const float getOffsetDegrees();

			virtual void setStartDegrees(const float & degrees);
			virtual const float getStartDegrees();

			virtual void setEndDegrees(const float & degrees);
			virtual const float getEndDegrees();

			virtual const bool contains(std::shared_ptr<RadialSliderResize> slider);
			virtual const bool borders(std::shared_ptr<RadialSliderResize> slider);
			virtual const bool intersects(std::shared_ptr<RadialSliderResize> slider);
			virtual const bool merge(std::shared_ptr<RadialSliderResize> slider);

			virtual ~RadialSliderResize() = default;

		protected:
			vec2 mCenter;
			float mRadius;
			float mWidth;

			Anim<float> mDegrees;
			Anim<float> mStart;
			Anim<float> mEnd;

			float mDownOffsetDegrees;
			float mDownOffsetDegreesDiff;

			float mOffsetDegrees;

			float mArcLength;
			float mArcLengthMax;

			bool mLeft;

			bool mSnap;
			float mSnapDegrees;

			GlslProgRef mShader;
			BatchRef mRing;
		};

		typedef std::shared_ptr<RadialSliderResize> RadialSliderResizeRef;
	}
}