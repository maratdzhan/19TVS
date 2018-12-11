#include "Error.h"
#include "stdafx.h"
#include <string>
#include <fstream>
#include <Windows.h>
using namespace std;


void Error(const wchar_t mText[200], const wchar_t hText[50])
{
	MessageBox(NULL, mText, hText, MB_ICONERROR | MB_OK);
}

