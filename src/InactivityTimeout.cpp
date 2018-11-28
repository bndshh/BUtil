
#include "InactivityTimeout.h"

bUtil::InactivityTimeout::InactivityTimeout() : mTimeoutSeconds(30.0f), mTimeout(mTimeoutSeconds)
{
	mInactivityCallback = [] {};
	start();
}

void bUtil::InactivityTimeout::setTimeoutSeconds(const float & seconds)
{ 
	mTimeoutSeconds = seconds; 
	mTimeout = seconds;
}

void bUtil::InactivityTimeout::start()
{
	mTimeout.stop();
	timeline().apply(&mTimeout, mTimeoutSeconds, 0.0f, mTimeoutSeconds, EaseNone()).finishFn([this] { trigger(); });
}

const float bUtil::InactivityTimeout::getTimeoutSeconds()
{ 
	return mTimeoutSeconds; 
}

void bUtil::InactivityTimeout::setInactivityCallback(const std::function<void()> & callback)
{ 
	mInactivityCallback = callback; 
}

void bUtil::InactivityTimeout::activity(const std::function<void()> & callback)
{
	start();
	callback();
}

void bUtil::InactivityTimeout::trigger()
{
	if (mInactivityCallback)
	{
		mInactivityCallback();
	}
}