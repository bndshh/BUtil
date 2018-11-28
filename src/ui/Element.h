
//  Created by Jan Bundschuh.
//  Copyright 2016-2018 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "Drawable.h"
#include "Hideable.h"
#include "HasAssets.h"
#include "Bilingual.h"
#include "cinder/Log.h"

namespace bUtil
{
	namespace UI
	{
		using namespace ci;

		enum : uint8_t
		{
			Right,
			Left,
			Top,
			TopRight,
			TopLeft,
			Bottom,
			BottomRight,
			BottomLeft,
			Center
		} 
		typedef AnchorPoint;

		class Element : public Drawable, public Hideable, public HasAssets, public Bilingual
		{
		public:
			Element(vec2 position = vec2(), bool enable = false, AnchorPoint anchor = AnchorPoint::Center);
			virtual void add(std::shared_ptr<Element> element) { mElements.push_back(element); };
			virtual void draw();
			virtual void resize();
			virtual void update();
			virtual void show(const float &delay = 0.0f) override;
			virtual void hide(const float &delay = 0.0f) override;
			virtual void instantShow() override;
			virtual void instantHide() override;
			
			virtual void mouseDown(const app::MouseEvent event);
			virtual void mouseDrag(const app::MouseEvent event);
			virtual void mouseUp(const app::MouseEvent event);
			virtual void mouseWheel(const app::MouseEvent event);

			virtual void touchesBegan(const app::TouchEvent event);
			virtual void touchesMoved(const app::TouchEvent event);
			virtual void touchesEnded(const app::TouchEvent event);
			
			virtual void enable(const bool &enabled = true);
			virtual void disable(const bool &enabled = true);
			virtual const bool isEnabled();
			virtual void setPosition(const ivec2 &position) { mPosition = position; };
			virtual void setPositionX(const int &x) { mPosition.x = x; };
			virtual void setPositionY(const int &y) { mPosition.y = y; };
			virtual void setRotation(const float &rotation) { mRotation = rotation; };
			virtual void setAnchorPoint(const AnchorPoint anchor) { mAnchorPoint = anchor; };
			virtual const AnchorPoint getAnchorPoint() { return mAnchorPoint; };
			virtual const ivec2 getPosition() { return mPosition; };

			virtual void setA();
			virtual void setB();
			virtual void toggleLanguage() override;

			virtual ~Element() = default;

		protected:
			bool mEnabled;
			ivec2 mPosition;
			float mRotation;
			AnchorPoint mAnchorPoint;
			std::vector<std::shared_ptr<Element>> mElements;
		};

		typedef std::shared_ptr<Element> ElementRef;
	}
}