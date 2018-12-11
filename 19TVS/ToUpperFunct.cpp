#include "stdafx.h"
#include "ToUpperFunct.h"

#include <string>

using namespace std;

string ToUpperFunc(string ThisLine)
{
	string result;
	for (unsigned int pos = 0; pos < ThisLine.size(); pos++)
	{
		if ((ThisLine[pos] != '\n') && (ThisLine[pos] != '\0'))
		{
			result += toupper(ThisLine[pos]);
		}
	}
	return result;
}