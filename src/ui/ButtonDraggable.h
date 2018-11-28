
//  Created by Jan Bundschuh.
//  Copyright 2016 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "Button.h"

namespace bUtil
{
	namespace UI
	{
		class ButtonDraggable : public bUtil::UI::Button
		{
		public:
			ButtonDraggable(vec2 position = vec2(), vec2 size = vec2(50), bool enable = false, AnchorPoint anchor = AnchorPoint::Center);
			virtual void mouseUp(const app::MouseEvent event) override;
			virtual void mouseDown(const app::MouseEvent event) override;
			virtual void mouseDrag(const app::MouseEvent event) override;
			virtual void touchesBegan(const app::TouchEvent event) override;
			virtual void touchesMoved(const app::TouchEvent event) override;
			virtual void touchesEnded(const app::TouchEvent event) override;
			virtual void calculateBounds() override;
			virtual void setDragDirection(const DragDirection &direction) { mDragDirection = direction; };
			virtual void setBorder(const Rectf &border, bool active = true);
			virtual void setPosition(const ivec2 &position) override;
			virtual const float getPercent();
			virtual const ivec2 getPosition() { return ivec2(vec2(mUsedPosition)); };
			virtual const bool isDragged() { return mDragged; };
			virtual const std::vector<Path2d> getDraggLines();
			virtual void setPercent(const float &percent, const bool &animate = true);
			virtual ~ButtonDraggable() = default;

		protected:
			DragDirection mDragDirection;
			Rectf mBorder;
			bool mHasBorder;
			bool mDragged;
			vec2 mStartPosition;
			std::vector<TouchPoint> mDragTouchPoints;
			Anim<vec2> mUsedPosition;
		};

		typedef std::shared_ptr<ButtonDraggable> ButtonDraggableRef;
	}
}