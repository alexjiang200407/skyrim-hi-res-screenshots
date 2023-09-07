#pragma once

namespace HRS
{
	class Resolution
	{
	public:
		Resolution(int width, int height) : width(width), height(height) {}
		int width;
		int height;
	};
}