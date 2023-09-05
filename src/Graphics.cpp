#include "Graphics.h"

using namespace Gdiplus;



void HRS::Graphics::Screenshot() const
{
	GdiplusStartupInput gdip;
	ULONG_PTR gdipToken;
	GdiplusStartup(&gdipToken, &gdip, NULL);

	RECT rect;
	GetWindowRect(hwnd, &rect);

	int h = rect.bottom - rect.top;
	int w = rect.right - rect.left;


	HDC     hScreen = GetDC(hwnd);
	HDC     hDc = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
	HGDIOBJ old_obj = SelectObject(hDc, hBitmap);
	BitBlt(hDc, 0, 0, w, h, hScreen, rect.left, rect.top, SRCCOPY);

	Gdiplus::Bitmap bitmap(hBitmap, NULL);
	CLSID clsid;

	GetEncoderClsId(L"image/png", &clsid);

	//GDI+ expects Unicode filenames
	bitmap.Save(L"C:\\Users\\alexj\\OneDrive\\Documents\\poop.png", &clsid);


	GdiplusShutdown(gdipToken);
	SelectObject(hDc, old_obj);
	DeleteDC(hDc);
	ReleaseDC(hwnd, hScreen);
	DeleteObject(hBitmap);
}

int HRS::Graphics::GetEncoderClsId(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0;
	UINT size = 0;

	GetImageEncodersSize(&num, &size);
	
	if (size == 0)
		return -1;

	ImageCodecInfo* pImage = (ImageCodecInfo*)malloc(size);

	if (!pImage)
		return -1;

	GetImageEncoders(num, size, pImage);

	for (UINT j = 0; j < num; j++)
	{
		if (wcscmp(pImage[j].MimeType, format) == 0)
		{
			*pClsid = pImage[j].Clsid;
			free(pImage);
			return j;
		}
	}

	free(pImage);
	return -1;
}
