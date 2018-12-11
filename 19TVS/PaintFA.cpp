#include "stdafx.h"
#include "19TVS.h"
#include "Error.h"
#include "resource.h"
#include "SessionInfo.h"
#include "KK_KQ_Extracting.h"
#include "AdditionalCalculatings_1.h"
#include "PaintColorFa.h"

#include <Windows.h>
#include <vector>



#include <xstring>
typedef std::basic_string<TCHAR, std::char_traits<TCHAR>,
	std::allocator<TCHAR> > String;
//static String str1,str2;
wstring str1, str2;
string s1, s2;
static int sx, sy;
vector<int> table_cen_x;
vector<int> table_cen_y;




LRESULT CALLBACK WndGraph(HWND hGraph, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static HBRUSH hBrush, hBrush_this, hBrush_this_edge_1,cBr;
	//	HRGN hRgn;
	cBr = CreateSolidBrush(RGB(200, 200, 200));
	hBrush = CreateSolidBrush(RGB(200,200,200));
	hBrush_this = CreateSolidBrush(RGB(252, 125, 160));
	hBrush_this_edge_1 = CreateSolidBrush(RGB(0, 0, 0));
	double z1 = 0;
	double z2 = 0;
	int x = 0;
	int y = 0;

	//message = WM_PAINT;

	int map_size_t = 1;
	double map_size_s = 2.1;
	double map_size_tvs = 1.4;
	double scale = 1.1;
	double cart_pos_y=25;
	int mn = 2;
	double scale_pos = 1.;
	LONG x1 = 12*scale, x2 = 7*scale, y1 = 0, y2 = 10*scale;
	LONG x2t=11, x1t=0, y2t=7, y1t=12;
	POINT pt[6] = { { -x1,y1 },{ -x2 ,-y2 },{ x2 ,-y2 },{ x1 ,y1 },{ x2,y2 },{ -x2,y2 } };
	POINT pt_pos[6] = {
	{x1t*map_size_tvs,y1t*map_size_tvs },{-x2t * map_size_tvs,y2t*map_size_tvs },
	{ -x2t * map_size_tvs,-y2t * map_size_tvs },{ x1t*map_size_tvs,-y1t * map_size_tvs },
	{ x2t*map_size_tvs,-y2t * map_size_tvs },{ x2t*map_size_tvs,y2t*map_size_tvs } };
	POINT new_pt_pos[6];
	new_pt_pos[0].x = x1t * map_size_tvs * mn;
	new_pt_pos[0].y = y1t * map_size_tvs * mn;
	new_pt_pos[1].x = -x2t * map_size_tvs * mn;
	new_pt_pos[1].y = y2t * map_size_tvs * mn;
	new_pt_pos[2].x = -x2t * map_size_tvs * mn;
	new_pt_pos[2].y = -y2t * map_size_tvs * mn;
	new_pt_pos[3].x = x1t * map_size_tvs * mn;
	new_pt_pos[3].y = -y1t * map_size_tvs * mn;
	new_pt_pos[4].x = x2t * map_size_tvs * mn;
	new_pt_pos[4].y = -y2t * map_size_tvs * mn;
	new_pt_pos[5].x = x2t * map_size_tvs * mn;
	new_pt_pos[5].y = y2t * map_size_tvs * mn;
//	const int WIDTH = 400;
//	const int HEIGHT = 600;
	HFONT newFont, oldFont;
	

	switch (message)
	{
	case WM_CREATE:
/*		sx = LOWORD(lParam);
		sy = HIWORD(lParam);*/
		break;
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hGraph, &ps);
		SetMapMode(hdc, MM_ANISOTROPIC);			// режим отображения - разномасштабируемый

	//	int stringNum = GetGraphInfo();
		SetWindowExtEx(hdc, sx, sy, NULL);	// размер окна
		SetViewportExtEx(hdc, sx, sy, NULL);		// размер вывода - непонятная штука
		
		for (int tvs_kart=1;tvs_kart<8;tvs_kart++)
		{
			// НОМЕР КАССЕТЫ
			{
				int fa_num_pos_y = 50;
				newFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
				oldFont = (HFONT)SelectObject(hdc, newFont);
				GetGraphVal(tvs_kart, 2);
				string sss0 = "Fuel Assambly #" + to_string(tvs_kart);
				str1 = wstring(sss0.begin(), sss0.end());
				TextOut(hdc, sx / 2 - 2.5*(str1.size()), 4 * sy / 5 + fa_num_pos_y, str1.data(), str1.size());
				str1.clear();
				SelectObject(hdc, oldFont);
				DeleteObject(newFont);
			}

			// Вывод KEFF MCU
			if (true)
			{
				newFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
				oldFont = (HFONT)SelectObject(hdc, newFont);
				x = ((sx * 7 / 10));
				y = ((sy / 10) - 30);
				wstring descript_1 = L"MCU";
				str1 = GetWstringFromString("Keff = " + to_string(GetKQKeff(false, true, false, false, 1, 0)), 0);
				TextOut(hdc, x, y, str1.data(), str1.size());
				MoveToEx(hdc, x + 80, y + 7, NULL);
				LineTo(hdc, x + 100, y + 7);
				TextOut(hdc, x + 100, y, descript_1.c_str(), descript_1.length());
				SelectObject(hdc, oldFont);
				DeleteObject(newFont);
			}

			// Вывод KEFF
			if (true) 
			{
				newFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
				oldFont = (HFONT)SelectObject(hdc, newFont);
				x = ((sx * 7 / 10));
				y = ((sy / 10) - 10);
				wstring descript_2 = L"PERMAK-A";
				str1 = GetWstringFromString("Keff = " + to_string(GetKQKeff(false, true, true, false, 1, 0)), 0);
				TextOut(hdc, x, y, str1.data(), str1.size());
				MoveToEx(hdc, x + 80, y + 7, NULL);
				LineTo(hdc, x + 100, y + 7);
				TextOut(hdc, x + 100, y, descript_2.c_str(), descript_2.length());
				SelectObject(hdc, oldFont);
				DeleteObject(newFont);
			}

			
			// НУЖЕН COMMON - ТИПА СДВИГ ВСЕХ ЭЛЕМЕНТОВ
			// Рисовка таблицы
			// Столбцы и строки
			if (true)
			{

			int number_of_rows = 2;
			int number_of_columns = 10;

			// Увеличение таблицы по Х и У
			int by = 30;
			int bx = 250;

			// Сдвиг положения таблицы
			int ax = 0;
			int ay = 325;
			

			// Задаем начало отсчета
			int start_x_pos = sx / 2 + ax;
			int start_y_pos = sy / 2 + ay;

			// Задаем верхний левый угол
			int up_left_pos_x = start_x_pos - bx;
			int up_left_pos_y = start_y_pos - by;

			// Задаем нижний правый угол
			int down_right_pos_x = start_x_pos + bx;
			int down_right_pos_y = start_y_pos + by;

			


				Rectangle(hdc, up_left_pos_x, up_left_pos_y, down_right_pos_x, down_right_pos_y);
				int num_l = 1;
				for (x = up_left_pos_x; x < down_right_pos_x; x += 2*bx/number_of_columns+1)
				{
					MoveToEx(hdc, x, up_left_pos_y, NULL);
					LineTo(hdc, x, down_right_pos_y);
					int cur_y = ((up_left_pos_y - down_right_pos_y)/ number_of_rows + 1)+ up_left_pos_y;
					int cur_x = x+ bx / (number_of_columns+1);

					newFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
					oldFont = (HFONT)SelectObject(hdc, newFont);
					TextOut(hdc,(cur_x)-by/2,(cur_y)+1.2*by,ReturnTableValues(true,num_l,tvs_kart).c_str(), ReturnTableValues(true, num_l,tvs_kart).size()); // Table header
					SelectObject(hdc, oldFont);
					DeleteObject(newFont);
					newFont = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
					oldFont = (HFONT)SelectObject(hdc, newFont);
					TextOut(hdc, (cur_x)-by/2, (cur_y)+2.2*by, ReturnTableValues(false, num_l,tvs_kart).c_str(), ReturnTableValues(false, num_l,tvs_kart).size()); // Table Values
					SelectObject(hdc, oldFont);
					DeleteObject(newFont);
					num_l++;
				}
				for (y = up_left_pos_y; y < down_right_pos_y; y += 2*by/(number_of_rows))
				{
					MoveToEx(hdc, up_left_pos_x, y, NULL);
					LineTo(hdc, down_right_pos_x, y);
					int z = 0;
				}
			}

			// Пример описания данных шестиугольника
			if (true)
			{

				int opis_x_pos = 125;
				int opis_y_pos = 220;
				x = (sx / 2)+opis_x_pos;
				y = (sy/2)+opis_y_pos;
				SetViewportOrgEx(hdc, x, y, NULL); // установка центра из которого начинается построение
				BeginPath(hdc);
				Polyline(hdc, new_pt_pos, 6);
				CloseFigure(hdc);
				EndPath(hdc);
				SelectObject(hdc, hBrush);
				SetPolyFillMode(hdc, WINDING);
				FillPath(hdc);
				// Если есть строка 1
				int local_x = -15;
				int local_y = -20;
				if (true)
				{
					SetTextColor(hdc, 0);
					SetBkMode(hdc, TRANSPARENT);
					newFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
					oldFont = (HFONT)SelectObject(hdc, newFont);
					wstring descript_1 = L"Kk (MCU)";
					SetTextColor(hdc, 0);
					SetBkMode(hdc, TRANSPARENT);
					TextOut(hdc, local_x, local_y, L"1.05", 4);
					MoveToEx(hdc, local_x + 30, local_y+7, NULL);
					LineTo(hdc, local_x + 70, local_y+7);
					TextOut(hdc, local_x+70, local_y, descript_1.c_str(), descript_1.length());
					SelectObject(hdc, oldFont);
					DeleteObject(newFont);
				}
				// Если есть строка 2
				if (true)
				{
					SetTextColor(hdc, 0);
					SetBkMode(hdc, TRANSPARENT);
					newFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
					oldFont = (HFONT)SelectObject(hdc, newFont);
					wstring descript_2 = L"Permak deviation";
					SetTextColor(hdc, 0);
					SetBkMode(hdc, TRANSPARENT);
					TextOut(hdc, local_x, local_y+25, L"0.02", 4);
					MoveToEx(hdc, local_x+30, local_y+25+7, NULL);
					LineTo(hdc, local_x+70, local_y + 25+7);
					TextOut(hdc, local_x+70, local_y + 25, descript_2.c_str(), descript_2.length());
					SelectObject(hdc, oldFont);
					DeleteObject(newFont);
				}
				
			}

			if (true)
			{
				if (true)
				{
					int kq_pos_x = -200;
					int kq_pos_y = 110;
					newFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
					oldFont = (HFONT)SelectObject(hdc, newFont);
					x = (sx / 2)+kq_pos_x;
					y = (sy /2)+kq_pos_y;
					SetViewportOrgEx(hdc, x, y, NULL);
					TextOut(hdc, 0, 0, L"KQ", 2);
					SelectObject(hdc, oldFont);
					DeleteObject(newFont);
				}
				
				for (auto tvs_in = 1; tvs_in < 20; tvs_in++)
				{
					SetViewportOrgEx(hdc, sx/2, sy/2, NULL);
					// Вывод рисованных ТВС
					// Рисовка картограммы 19-ти кассетника
					int ntCorePos_x = 0;
					int ntCorePos_y = 75;
					z1 = retRowYT(tvs_in);
					z2 = retColXT(tvs_in);
					x = ((sx / 6) + z2 * 2*map_size_tvs+ntCorePos_x);
					y = ((sy * 6 / 9) - z1 * 2*map_size_tvs + ntCorePos_y);
					
					SetViewportOrgEx(hdc, x, y, NULL); // установка центра из которого начинается построение
					
					/////////////////////////////
					// Вывод KQ

				//	str1 = GetWstringFromString("KQ", 0);
					

					/////////////////////////////////

					SetTextColor(hdc, 0);
					SetBkMode(hdc, TRANSPARENT);
				
					BeginPath(hdc);
					Polyline(hdc, pt_pos, 6);
					CloseFigure(hdc);
					EndPath(hdc);
					if (tvs_kart == tvs_in)
					{
						SelectObject(hdc, hBrush_this);
					}
					else
					{
						SelectObject(hdc, hBrush);
					}
					SetPolyFillMode(hdc, WINDING);
					FillPath(hdc);
					s1 = to_string(tvs_in);
					str1 = wstring(s1.begin(), s1.end());
					newFont = CreateFont(10, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
					oldFont = (HFONT)SelectObject(hdc, newFont);
					TextOut(hdc,-3, (-(y2)-5), str1.data(), str1.size());
					str1.clear();
					str1 = GetWstringFromString(to_string(GetKQKeff(false, false, false, false, tvs_in, 0)),4);
					TextOut(hdc, -7, -5, str1.data(), str1.size());
					str1.clear();
					str1 = GetWstringFromString(to_string(GetKQDelta(tvs_in)), 4);
					TextOut(hdc, -7, 5, str1.data(), str1.size());
					SelectObject(hdc, oldFont);
					DeleteObject(newFont);
				}
				////////////////////////////////
			}

				for (int num = 1; num < 332; num++)
				{
					z1 = retRowY(num);
					z2 = retColX(num);
					x = ((sx / 2) - z2 * map_size_s);
					y = ((cart_pos_y+sy*4 /11) - z1 * map_size_s);
					SetViewportOrgEx(hdc, x, y, NULL); // установка центра из которого начинается построение

					BeginPath(hdc);
					Polyline(hdc, pt, 6);
					CloseFigure(hdc);
					EndPath(hdc);

					s2 = GetGraphInfo(num, 2);
					cBr = CreateSolidBrush(ColorReference("0",s2,100,2));
					
					SelectObject(hdc, cBr);
					SetPolyFillMode(hdc, WINDING);
					FillPath(hdc);
					
					SetTextColor(hdc, 0);
					SetBkMode(hdc, TRANSPARENT);
					newFont = CreateFont(11, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
					oldFont = (HFONT)SelectObject(hdc, newFont);
					s1 = GetGraphInfo(num, 1);
					str1 = wstring(s1.begin(), s1.end());
					TextOut(hdc, -x1+5 , (-(y2)+1) , str1.data(), str1.size());
					//
					//// Вторая штука
					
					str2 = wstring(s2.begin(), s2.end());
					TextOut(hdc, -x1+5 , -y2+9 , str2.data(), str2.size());
					SelectObject(hdc, oldFont);
					DeleteObject(newFont);
					////
					str1.clear();
					str2.clear();
					/////////////////////////////////////////
					DeleteObject(cBr);
				}

				EndPaint(hGraph, &ps);
				string aa1 = ResultPathForMcu("",2)+to_string(tvs_kart)+".bmp";
				wstring graphName = wstring(aa1.begin(), aa1.end());
				StoreClientWindowBitmap(hGraph, graphName.c_str());
				GraphClear();

				RECT rect;
				GetClientRect(hGraph, &rect);
				FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
				
			}
			Writting1209();
			OrderClear();
		break;
	}
	case WM_DESTROY:

		break;
	default:
	{
		return DefWindowProc(hGraph, message, wParam, lParam);
	}
	}
	return 0;
}


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
	DWORD ColorSize, DataSize=0;
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
}


