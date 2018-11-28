
//  Created by Jan Bundschuh. (#f083ad)
//  Copyright 2016-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#include "ButtonDraggable.h"

bUtil::UI::ButtonDraggable::ButtonDraggable(vec2 position, vec2 size, bool enable, AnchorPoint anchor) : Button(position, size, enable, anchor),
mDragDirection(DragDirection::FREE), mUsedPosition(position), mStartPosition(position), mHasBorder(false), mDragged(false) 
{
	mBorder = Rectf();
	mDragTouchPoints = std::vector<TouchPoint>();
}

void bUtil::UI::ButtonDraggable::mouseDown(const app::MouseEvent event)
{
	mDragged = false;
	if (!mUsedPosition.isComplete()) return;
	Button::mouseDown(event);
}

void bUtil::UI::ButtonDraggable::mouseDrag(const app::MouseEvent event)
{
	if (isEnabled() && isPressed())
	{
		vec2 touchPosition = vec2(event.getPos()) - (mDownOffsets.size() > 0 ? mDownOffsets.at(0) : vec2());
		vec2 pos = mPosition;

		if (mDragDirection == DragDirection::FREE)
		{
			pos = touchPosition;
		}
		else if (mDragDirection == DragDirection::X)
		{
			pos = vec2(touchPosition.x, pos.y);
		}
		else if (mDragDirection == DragDirection::Y)
		{
			pos = vec2(pos.x, touchPosition.y);
		}

		if (mHasBorder && !mBorder.contains(pos))
		{
			pos = mBorder.closestPoint(pos);
		}

		mDragged = true;
		mUsedPosition = pos;
	}
}

void bUtil::UI::ButtonDraggable::mouseUp(const app::MouseEvent event)
{
	mDragged = false;
	bUtil::UI::Button::mouseUp(event);
}

void bUtil::UI::ButtonDraggable::touchesBegan(const app::TouchEvent event)
{
	mDragged = false;
	if (!mUsedPosition.isComplete()) return;
	Button::touchesBegan(event);
}

void bUtil::UI::ButtonDraggable::touchesMoved(const app::TouchEvent event)
{
	if (!isEnabled()) return;

	if (isPressed()) {
		auto touches = event.getTouches();

		mDragTouchPoints.clear();
		
		for (auto touch : touches)
		{
			auto id = touch.getId();
			auto p = touch.getPos();
			mDragTouchPoints.push_back(TouchPoint(id, p));

			for (auto dtp : mDownTouchPoints)
			{
				if (id == dtp.id)
				{
					vec2 touchPosition = vec2(touch.getPos()) - (mDownOffsets.size() > 0 ? mDownOffsets.at(0) : vec2());
					vec2 pos = mPosition;

					if (mDragDirection == DragDirection::FREE)
					{
						pos = touchPosition;
					}
					else if (mDragDirection == DragDirection::X)
					{
						pos = vec2(touchPosition.x, pos.y);
					}
					else if (mDragDirection == DragDirection::Y)
					{
						pos = vec2(pos.x, touchPosition.y);
					}

					if (mHasBorder && !mBorder.contains(pos))
					{
						pos = mBorder.closestPoint(pos);
					}

					mDragged = true;
					mUsedPosition = pos;
				}
			}
		}
	}
}

void bUtil::UI::ButtonDraggable::touchesEnded(const app::TouchEvent event)
{
	mDragged = false;
	mDragTouchPoints.clear();
	bUtil::UI::Button::touchesEnded(event);
}

void bUtil::UI::ButtonDraggable::setBorder(const Rectf &border, bool active)
{
	mBorder = border + mUsedPosition;

	vec2 ul = mBorder.getUpperLeft();
	vec2 lr = mBorder.getLowerRight();
	vec2 size = mSize;
	mBorder = Rectf(ul + vec2(size.x / 2, 0), lr - vec2(size.x / 2, 0));

	mUsedPosition = vec2(active ? mBorder.x2 : mBorder.x1, mBorder.getCenter().y);
	update();

	mHasBorder = true;
}

void bUtil::UI::ButtonDraggable::calculateBounds()
{
	vec2 size = mSize;
	vec2 position = mUsedPosition;

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

	ci::Path2d path;
	path.moveTo(a);
	path.lineTo(b);
	path.lineTo(c);
	path.lineTo(d);
	path.close();

	mBounds = path;
}

const float bUtil::UI::ButtonDraggable::getPercent()
{
	vec2 pos = mUsedPosition;
	
	if (mDragDirection == DragDirection::X)
	{
		return (pos.x - mBorder.x1) / mBorder.getWidth();
	}
	else if (mDragDirection == DragDirection::Y)
	{
		return (pos.y - mBorder.y1) / mBorder.getHeight();
	}
	else
	{
		return 0.0f;
	}
}

const std::vector<Path2d> bUtil::UI::ButtonDraggable::getDraggLines()
{
	auto paths = std::vector<Path2d>();

	auto itA = mDownTouchPoints.begin();
	while (itA != mDownTouchPoints.end())
	{	
		auto a = *itA;
		auto itB = mDragTouchPoints.begin();
		while (itB != mDragTouchPoints.end())
		{
			auto b = *itB;
			if (a.id == b.id)
			{
				Path2d path;
				path.moveTo(a.pos);
				path.lineTo(b.pos);
				paths.push_back(path);
			}
			itB++;
		}
		itA++;
	}
	return paths;
}

void bUtil::UI::ButtonDraggable::setPercent(const float &percent, const bool &animate)
{
	if (percent > 1.0f || percent < 0.0f) return;

	vec2 pos = mUsedPosition;
	vec2 destination = pos;

	if (mDragDirection == DragDirection::X)
	{
		float x = (percent * mBorder.getWidth()) + mBorder.x1;
		destination = vec2(x, pos.y);
	}
	else if (mDragDirection == DragDirection::Y)
	{
		float y = (percent * mBorder.getHeight()) + mBorder.y1;
		destination = vec2(pos.x, y);
	}

	if (animate)
	{
		timeline().apply(&mUsedPosition, destination, 0.5f);
	}
	else
	{
		mUsedPosition = destination;
	}
}

void bUtil::UI::ButtonDraggable::setPosition(const ivec2 &position)
{
	mPosition = position;
	mUsedPosition = position;
	calculateBounds();
}