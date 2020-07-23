#include "Error.h"
#include "stdafx.h"
#include <fstream>
#include <Windows.h>
#include <chrono>
#include <string>
#include <ctime>
#include <stdio.h>
#include <iomanip>
#include <map>



void Error(const wchar_t mText[200], const wchar_t hText[50])
{
	MessageBox(NULL, mText, hText, MB_ICONERROR | MB_OK);
}


 