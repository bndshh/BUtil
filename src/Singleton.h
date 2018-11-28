
//  Created by Jan Bundschuh.
//  Copyright 2016-2017 TRIAD Berlin Projektgesellschaft mbH. All rights reserved.

#pragma once

#include <stddef.h>
#include <assert.h>

namespace bUtil
{
	template <class T>

	class Singleton
	{
	public:
		static T * Instance()
		{
			if (!m_pInstance)
			{
				m_pInstance = new T;
			}

			assert(m_pInstance != NULL);
			return m_pInstance;
		}
		Singleton(Singleton const &) = delete;
		Singleton& operator=(Singleton const &) = delete;
	protected:
		Singleton() {};
		virtual ~Singleton() = default;

	private:
		static T * m_pInstance;
	};

	template <class T> T * Singleton<T>::m_pInstance = NULL;
}