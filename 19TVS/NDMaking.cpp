#include "stdafx.h"
#include "NDMaking.h"
#include "NtCommon.h"
#include "Error.h"
#include "GetParam.h"
#include "Path_Finder.h"
#include "SessionInfo.h"

#include <string>
#include <fstream>
using namespace std;

string fileName = Path_Finder("PermakPath");
string ndTestsPath = fileName + "bin/data/tests/nds/newdata_";
const int MAX_ARRAY_SIZE = 500;
const int INLINE_SIZE = 5;
int sl_l[MAX_ARRAY_SIZE+1][INLINE_SIZE+1];

void slRead()
{
	// «апускаетс€ один раз при запуске приложени€
	string path_si = ReturnMainPath("slink");
	string str; 
	ifstream ifs(path_si);

	if (ifs.is_open() != 1)
	{
		Error(L"Error opening sl.ink", L"slRead -> NDMaking");
	}
	else
	{
		for (int i = 1; i < MAX_ARRAY_SIZE + 1; i++)
		{
			getline(ifs, str);
			for (int j = 1; j < INLINE_SIZE + 1; j++)
			{
				sl_l[i][j] = GetParam(str, j);

			}
		}
	}
	path_si.clear();
	ifs.close();
}

int ndMaking(int inputNumber, int retParam)
{
	// ¬озвращает параметр retparam
	// из списка sl.ink
	// считанного в slread()
	if ((inputNumber <= MAX_ARRAY_SIZE) && (retParam <= INLINE_SIZE))
	{
		// “ут кос€к - если номер сост больше 500 - происходит ретурн -123

		for (int i = 0; i != MAX_ARRAY_SIZE; i++)
		{
			if (inputNumber==sl_l[i][1]) 
				return  sl_l[i][retParam];
		}
		// /* это дерьмо-код нужен дл€ того, чтобы записи типа 10,15,6,1 не игнорировались
		for (int i = 0; i != MAX_ARRAY_SIZE; i++)
		{
			if (inputNumber == sl_l[i][2])
				return  sl_l[i][retParam];
		}
	}
		
	
	return -123;
}

string RetWithOutLastSym(string str, char(c))
{
	// »щет в строке символ C и возвращает строку длины L
	// причем L = lastPos (C) (char == 47)
	string q;
	int lastPos = 0;
	q.clear();
	for (int i = 0; i < str.size(); i++)
	{
		if (char(str[i]) == c)
		{
			lastPos = i;
		}
	}

	for (int i = 0; i != lastPos + 1; i++)
	{
		q += str[i];
	}
	return q;
}

void ndWritting(int inpNumber)
{
	// «апись в корень нужного файла NEWDATA
	string path_si;
	string tmpStr;
	path_si += ndTestsPath;
	path_si += to_string(ndMaking(inpNumber,3));

	ifstream inp(path_si);

	tmpStr.clear();
	
	//path_si += fileName;
	tmpStr += RetWithOutLastSym(Path_Finder("ResultFilePath"), 47);
	tmpStr += "newdata";

	ofstream outp(tmpStr);
	

	if (!inp.is_open())
		Error(L" Error opening NEWDATA ", L" NDMaking -> ndWritting ");
	else
	{
		while (!inp.eof())
		{
			getline(inp, tmpStr);
			outp << tmpStr << endl;
			tmpStr.clear();
		}
	}
	tmpStr.clear();
	path_si.clear();
	inp.close();
	outp.close();
}

