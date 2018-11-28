
//  Created by Jan Bundschuh.
//  Copyright 2016-2018 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include "cinder/Timeline.h"

#include "Options.h"

namespace bUtil
{
	namespace Sizes
	{
		ci::vec2 resolution();
	}

	namespace Animation
	{
		float speed();
		ci::EaseFn easing();
	}
}