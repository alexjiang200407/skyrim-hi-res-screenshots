#pragma once
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>

#pragma comment (lib,"Gdiplus.lib")

namespace HRS
{
	class Graphics
	{
	public:
		Graphics(const HWND& hwnd) : hwnd(hwnd) {};
		void Screenshot() const;
	private:
		static int GetEncoderClsId(const WCHAR *format, CLSID* pClsid);
		HWND hwnd;
	};
}
