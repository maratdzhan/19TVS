#include "stdafx.h"
#include "GetParam.h"
#include <string>
#include <algorithm>



int GetParam(std::string s, int num)
{
	std::string resStr;
	int locNum = 1;

	for (unsigned int pos = 0; pos < s.size(); pos++)
	{
		if (s[pos] != ',')
		{
			resStr += s[pos];
		}
		else
		{
			if (num == locNum)
			{
				locNum = stoi(resStr);
				return locNum;
			}
			else
			{
				locNum++;
				resStr.clear();
			}
		}
	}
	if (!resStr.empty())
	{
		locNum = stoi(resStr);
	}
	return locNum;
}

double GetParamD(std::string s, int num)
{
	std::string resStr;
	int counter = 1;
	double locNum = -899;

	for (unsigned int pos = 0; pos < s.size(); pos++)
	{
		if (s[pos] != ',')
		{
			resStr += s[pos];
		}
		else
		{
			if (num == counter)
			{
				locNum = stod(resStr);
				return locNum;
			}
			else
			{
				counter++;
				resStr.clear();
			}
		}
	}
	if (!resStr.empty())
	{
		locNum = stod(resStr);
	}
	return locNum;
}

std::string GetParamS(std::string s, int num)
{
	std::string resStr;
	int counter = 1;

	for (unsigned int pos = 0; pos < s.size(); pos++)
	{
		if ((s[pos] != ',')&&(s[pos] != ' '))
		{
			resStr += s[pos];
		}
		else
		{
			if (num == counter)
			{

				return resStr;
			}
			else
			{
				counter++;
				resStr.clear();
			}
		}
	}
	if (!resStr.empty())
	{
		resStr;
	}
	return resStr;
}

std::string ReturnNumbers(std::string str)
{
	std::string result;
	if (!str.empty())
	{
		for (short i = 0; i < str.size(); i++)
		{
			if (((char(str[i]) > 47) && (char(str[i]) < 58)) || (char(str[i] == 44)) || (char(str[i] == 46)) || (char(str[i]) == 45))
			{
				if ((char(str[i] == 44)))
					result += ".";
				else
					result += str[i];
			}
		}
	}
	try
	{
		(std::stod(result));
	}
	catch (const std::invalid_argument& EX)
	{
		result = "0";
	}
	return result;
}

std::string ToUpperFunct(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);

	return str;
}