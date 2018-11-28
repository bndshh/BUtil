
//  Created by Jan Bundschuh.
//  Copyright 2016 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include <vector>
#include <memory>

namespace bUtil
{
	class Bilingual
	{
	public:
		Bilingual(bool A = true);
		virtual void addChild(std::shared_ptr<Bilingual> child);
		virtual const bool isA();
		virtual const bool isB();
		virtual void setA();
		virtual void setB();
		virtual void toggleLanguage();
		virtual const size_t getNumChildren();
		virtual ~Bilingual() = default;

	protected:
		bool mA;
		std::vector<std::shared_ptr<Bilingual>> mChildren;
	};
}

typedef std::shared_ptr<class Bilingual> BilingualRef;