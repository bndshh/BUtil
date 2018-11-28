
//  Created by Jan Bundschuh on 27/04/15.
//  Copyright 2016-2018 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "Element.h"

namespace bUtil
{
	namespace UI
	{
		struct TouchPoint 
		{
			TouchPoint() {}
			TouchPoint(const uint32_t &id, const vec2 &pos) : id(id), pos(pos) {}
			void draw() const
			{
				ScopedModelMatrix scopedModelMatrix;
				gl::drawStrokedCircle(pos, 25);
				//gl::drawString(toString(id), pos + vec2(30, 0), Color(1, 0, 1));
			}

			uint32_t id;
			vec2 pos;
		};

		enum : uint8_t
		{
			X,
			Y,
			FREE
		} typedef DragDirection;

		class Button : public UI::Element
		{
		public:
			Button(vec2 position = vec2(), vec2 size = vec2(50), bool enable = false, AnchorPoint anchor = AnchorPoint::Center);
			virtual void draw() override;
			virtual void update() override;

			virtual void mouseDown(const app::MouseEvent event) override;
			virtual void mouseDrag(const app::MouseEvent event) override;
			virtual void mouseUp(const app::MouseEvent event) override;
			
			virtual void touchesBegan(const app::TouchEvent event) override;
			virtual void touchesMoved(const app::TouchEvent event) override;
			virtual void touchesEnded(const app::TouchEvent event) override;
			
			virtual void setOnClicked(std::function<void()> func) { mOnClicked = func; };
			virtual void triggerOnClick() { mOnClicked(); };
			virtual void setOnRelease(std::function<void()> func) { mOnRelease = func; };
			virtual void setOnDown(std::function<void()> func) { mOnDown = func; };
			virtual void setOnUp(std::function<void()> func) { mOnUp = func; };
			virtual void setOnToggle(std::function<void()> func) { mOnToggle = func; };
			virtual const std::vector<TouchPoint> getDownTouchPoints() { return mDownTouchPoints; };
			virtual const std::vector<vec2> getDownOffset();
			virtual const std::vector<vec2> getReleaseOffset();
			virtual const vec2 getSize();
			virtual bool const isPressed() { return mPressed; };
			virtual void calculateBounds();
			virtual const Path2d getBounds() { return mBounds; };
			virtual void setToggable(const bool &toggable = true) { mToggable = toggable; };
			virtual void setInflation(const vec2 &inflation) { mInflation = inflation; };
			virtual void setPosition(const ivec2 &position) override { mPosition = position; calculateBounds(); };
			virtual void setSize(const vec2 &size, const bool &animate = false);
			virtual void setAnchorPoint(const AnchorPoint anchor) override;
			virtual void callOnClicked() { mOnClicked(); };
			virtual void setToggle(const bool &toggle = false) { mToggle = toggle; };
			virtual const bool getToggle() { return mToggle; };
			virtual ~Button() = default;

		protected:
			void init();

			Anim<vec2> mSize;
			vec2 mInflation;

			std::vector<vec2> mDownOffsets;
			std::vector<vec2> mReleaseOffsets;
			std::vector<TouchPoint> mDownTouchPoints;

			Path2d mBounds;

			bool mPressed;
			bool mClicked;
			bool mReleased;
			bool mToggable;
			bool mToggle;

			std::function<void()> mOnClicked;
			std::function<void()> mOnRelease;
			std::function<void()> mOnDown;
			std::function<void()> mOnUp;
			std::function<void()> mOnToggle;
		};

		typedef std::shared_ptr<Button> ButtonRef;
	}
}