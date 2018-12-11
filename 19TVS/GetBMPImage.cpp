#include "stdafx.h"
#include <Windows.h>
/*
HDC bmp_hdc;
HWND bmp_hwnd;



// Сохранение BMP в файл

BOOL StoreBitmapFile(LPCTSTR lpszFileName, HBITMAP HBM)
{

	BITMAP BM;
	BITMAPFILEHEADER BFH;
	LPBITMAPINFO BIP;
	HDC DC;
	LPBYTE Buf;
	DWORD ColorSize, DataSize;
	WORD BitCount;
	HANDLE FP;
	DWORD dwTemp;


	GetObject(HBM, sizeof(BITMAP), (LPSTR)&BM);


	BitCount = (WORD)BM.bmPlanes * BM.bmBitsPixel;
	switch (BitCount)
	{
	case 1:
	case 4:
	case 8:
	case 32:
		ColorSize = sizeof(RGBQUAD) * (1 << BitCount);
		break;
	case 16:
	case 24:
		ColorSize = 0;
		break;
	default: 
		ColorSize = sizeof(RGBQUAD) * (1 << BitCount);
		break;
	}


	DataSize = ((BM.bmWidth*BitCount + 31) & ~31) / 8 * BM.bmHeight;

	BIP = (LPBITMAPINFO)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BITMAPINFOHEADER) + ColorSize);

	BIP->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BIP->bmiHeader.biWidth = BM.bmWidth;
	BIP->bmiHeader.biHeight = BM.bmHeight;
	BIP->bmiHeader.biPlanes = 1;
	BIP->bmiHeader.biBitCount = BitCount;
	BIP->bmiHeader.biCompression = 0;
	BIP->bmiHeader.biSizeImage = DataSize;
	BIP->bmiHeader.biXPelsPerMeter = 0;
	BIP->bmiHeader.biYPelsPerMeter = 0;
	if (BitCount < 16)
		BIP->bmiHeader.biClrUsed = (1 << BitCount);
	BIP->bmiHeader.biClrImportant = 0;



	BFH.bfType = 0x4d42;
	BFH.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + BIP->bmiHeader.biClrUsed * sizeof(RGBQUAD);
	BFH.bfReserved1 = 0;
	BFH.bfReserved2 = 0;
	BFH.bfSize = BFH.bfOffBits + DataSize;


	Buf = (LPBYTE)GlobalAlloc(GMEM_FIXED, DataSize);


	DC = GetDC(0);
	GetDIBits(DC, HBM, 0, (WORD)BM.bmHeight, Buf, BIP, DIB_RGB_COLORS);
	ReleaseDC(0, DC);

	FP = CreateFile(lpszFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(FP, &BFH, sizeof(BITMAPFILEHEADER), &dwTemp, NULL);
	WriteFile(FP, BIP, sizeof(BITMAPINFOHEADER) + BIP->bmiHeader.biClrUsed * sizeof(RGBQUAD), &dwTemp, NULL);
	WriteFile(FP, Buf, DataSize, &dwTemp, NULL);

	CloseHandle(FP);
	GlobalFree((HGLOBAL)Buf);
	HeapFree(GetProcessHeap(), 0, (LPVOID)BIP);

	return(true);
}

// создание BMP из DC
HBITMAP CreateBitmap(HDC DC, int w, int h)
{
	HDC memDC;
	HBITMAP bm, oldBM;


	memDC = CreateCompatibleDC(DC);
	bm = CreateCompatibleBitmap(DC, w, h);
	oldBM = (HBITMAP)SelectObject(memDC, bm);
	BitBlt(memDC, 0, 0, w, h, DC, 0, 0, SRCCOPY);
	SelectObject(memDC, oldBM);
	DeleteDC(memDC);
	DeleteObject(oldBM);
	return bm;
}

// создание BMP клиентской области окна
HBITMAP CreateClientWindwowBitmap(HWND wnd)
{
	RECT r;
	GetClientRect(wnd, &r);
	return(CreateBitmap(GetDC(wnd), r.right, r.bottom));
}

// создание BMP окна
HBITMAP CreateWindwowBitmap(HWND wnd)
{
	RECT r;
	POINT pt;
	GetWindowRect(wnd, &r);
	pt.x = r.right;
	pt.y = r.bottom;
	ScreenToClient(wnd, &pt);
	return(CreateBitmap(GetWindowDC(wnd), pt.x, pt.y));
}

// создание BMP рабочего стола
HBITMAP CreateDesktopBitmap(void)
{
	RECT r;

	SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0);

	return(CreateBitmap(GetDC(GetDesktopWindow()), r.right, r.bottom));
}

// создание BMP экрана
HBITMAP CreateScreenBitmap(void)
{

	DWORD w, h;
	HDC DC;

	w = GetSystemMetrics(SM_CXSCREEN);
	h = GetSystemMetrics(SM_CYSCREEN);

	DC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	return(CreateBitmap(DC, w, h));
	DeleteDC(DC);
}

// сохранение в BMP-файл клиентской области окна
VOID StoreClientWindowBitmap(HWND wnd, LPCTSTR pszFileName)
{

	HBITMAP bm;

	bm = CreateClientWindwowBitmap(wnd);
	StoreBitmapFile(pszFileName, bm);
	DeleteObject(bm);
}

// сохранение в BMP-файл окна
VOID StoreWindowBitmap(HWND wnd, LPCTSTR pszFileName)
{

	HBITMAP bm;

	bm = CreateWindwowBitmap(wnd);
	StoreBitmapFile(pszFileName, bm);
	DeleteObject(bm);
}

// сохранение в BMP-файл рабочего стола
VOID StoreDesktopBitmap(LPCTSTR pszFileName)
{

	HBITMAP bm;

	bm = CreateDesktopBitmap();
	StoreBitmapFile(pszFileName, bm);
	DeleteObject(bm);
}

// сохранение в BMP-файл экрана
VOID StoreScreenBitmap(LPCTSTR pszFileName)
{

	HBITMAP bm;

	bm = CreateScreenBitmap();
	StoreBitmapFile(pszFileName, bm);
	DeleteObject(bm);
}

// примеры использования
void StoreAll(void)
{
	StoreClientWindowBitmap(bmp_hwnd, L"c:/client.bmp");  // клиентская область окна    
	StoreWindowBitmap(bmp_hwnd, L"c:/window.bmp");        // окно
	StoreDesktopBitmap(L"c:/desktop.bmp");           // десктоп
	StoreScreenBitmap(L"c:/screen.bmp");             // экран
}*/