#pragma once

template <typename T>
class Singleton
{
public:
	static T* const GetSingleton()
	{
		static T singleton;
		return &singleton;
	}
};