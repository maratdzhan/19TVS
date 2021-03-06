#pragma once
#include <string>


using namespace std;


HWND ReturnMainHWnd();
VOID StoreClientWindowBitmap(HWND wnd, LPCTSTR pszFileName);
std::string GetGraphInfo(int tvel, int stringNum);
std::string GetGraphVal(int tvs, int stringNum);
double ReturnOrder(int mainVal, bool max);
void GraphClear();
wstring GetWstringFromString(string str,int lenght);
double GetKQDelta(int tvs_Number);
string ResultPathForMcu(string str_input, int type);
double RetCoordinates(int tvs, bool x, int gstate);



__declspec(dllimport) double retRowY(int num);
__declspec(dllimport) double retColX(int num);
__declspec(dllimport) double retRowYT(int num);
__declspec(dllimport) double retColXT(int num);

// Throw h_tvs, angle, tvs, and x or y;
__declspec(dllimport) double CDefine(float h_TVS, float angle, int tvs, bool x);


 







