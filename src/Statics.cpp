
//  Created by Jan Bundschuh.
//  Copyright 2016-2018 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#include "Statics.h"

ci::vec2 bUtil::Sizes::resolution()
{
	return ci::vec2(1080, 1920);
}

float bUtil::Animation::speed()
{
	return 0.3f;
}

ci::EaseFn bUtil::Animation::easing()
{
	return ci::EaseOutCubic();
}