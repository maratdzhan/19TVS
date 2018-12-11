#include "stdafx.h"
#include "19TVS.h"
#include "Path_Finder.h"
#include "Error.h"

#include <string>
#include <fstream>

using namespace std;
string GL_RES_DIR;
string MCU_RES_DIR;


string Path_Finder(string Word)
{
	string pathP = "D:/Vasiliev/PERMAK-A/Local_new/bin/data/indata.nt";
	ifstream Permak(pathP);
	string arg, path;
	path.clear();


	if (Permak.is_open() != 1)
	{
		Error(L"Not found <indata.nt>", L"Error_PATH_FINDER");
		return Word;
	}

	while (Permak.eof() != 1)
	{
		getline(Permak, arg);
		if (arg.find(Word) != -1)
		{ 
			bool f = false;
			for (unsigned int i = 0; i < arg.length(); i++)
			{
				if (char(arg[i]) != 32)
				{
					if (arg[i] == ';') break;
					if (f)
					{
						if (char(arg[i]) == 92) { arg[i] = '/'; }
						path += arg[i];
					}
					else
					{
						if (arg[i] == '=') f = true;
					}
				}
			}
		}
	}

	if (!path.empty())
		return path;
	else
	{
		wstring stemp = std::wstring(Word.begin(), Word.end());
		LPCWSTR sw = stemp.c_str();
		Error(sw, L"PATH_FINDER");
	}
}


string ResultPathForMcu(string str_input, int type)
{
	if (type == 1)
		return (MCU_RES_DIR);
	if (type == 2)
		return (GL_RES_DIR);
	if (type == 3)
	{
		GL_RES_DIR.clear();
		GL_RES_DIR = str_input;
	}
	if (type == 4)
	{
		MCU_RES_DIR.clear();
		MCU_RES_DIR = str_input;
	}
	return "NAN";
}
