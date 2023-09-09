#pragma once

namespace HRS
{
	class Resolution
	{
	public:
		Resolution(int width, int height) : width(width), height(height) {}
		int width;
		int height;
		bool operator==(const Resolution& rhs) 
		{
			return (
				width == rhs.width && height == rhs.height
			);
		}
		bool operator!=(const Resolution& rhs)
		{
			return !(*this == rhs);
		}
	};
}