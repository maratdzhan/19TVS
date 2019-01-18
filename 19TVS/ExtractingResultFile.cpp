#include "stdafx.h"
#include "ExtractingResultFile.h"
#include "Path_Finder.h"
#include "Error.h"
#include "KK_KQ_Extracting.h"
#include "NtCommon.h"

#include <string>
#include <fstream>
#include <vector>



using namespace std;
double keff;



void GetKeff(string keffStr, bool needThis)
{
	LoggerClass Lg_2_1(__FUNCTION__);
	bool localBoolean = false;
	string localKeff;

	
	if (needThis)
	{
		for (int pos = 0; pos < keffStr.size(); pos++)
		{
			if (!localBoolean)
			{
				if (char(keffStr[pos]) != 61)
				{
					localBoolean = false;
					continue;
				}
				else
				{
					localBoolean = true;
				}
			}
			else
			{
				if (((char(keffStr[pos]) >= 47) && (char(keffStr[pos]) <= 57)) || (char(keffStr[pos]) == 46))
				{
					localKeff += keffStr[pos];
				} 
			}
		}
		keff = stod(localKeff);
		TakeKeff(localKeff);
		GetKQKeff(true, true, true, false, 0,keff);
		keff = -777;
	}

}


void ExtractingResultFile(int sostNumber, int gln_l)
{
	LoggerClass lg_2(__FUNCTION__);
	string ResultFilePath = Path_Finder("ResultFilePath");

	ifstream InFile(ResultFilePath);
	string str, str_1;
	vector <string> str_kk;
	int num = 0;

	if (InFile.is_open() != 1)
	{
		Error(L"Not found <ResultFilePath>", L"Error");
	}
	else
	{
		while (!InFile.eof())
		{
			getline(InFile, str);
			if (str.find("KEFF")!=-1 )
			{
				if (num != sostNumber-1)
					GetKeff(str, false);
				else
					GetKeff(str, true);
			}
			if (str.find("Kk NORMALIZED") != -1) num++;
			if (num == sostNumber) break;
		}

		if (num == sostNumber)
		{
		//	GetKeff("0", true);
			while (str.find("1-NUMBER OF ASSEMBLE") == -1)
			{
				getline(InFile, str);
				if (str.find("1-NUMBER OF ASSEMBLE") != -1) break;
				for (unsigned int pos = 0; pos < str.length(); pos++)
				{
					if ((char(str[pos]) != 32) && (char(str[pos] != 0)))
					{
						str_1 += str[pos];
					}
					else
					{
						if ((str_1.empty() != true))
						{
							str_kk.push_back(str_1);
							str_1.clear();
						}
					}
				}
			}
		}


		string OutFilePath = Path_Finder("PermakPath") + "bin/ttmt/ttnt/perm_" + to_string(gln_l) + ".out";
		ofstream OutFile(OutFilePath);

		for (unsigned int num = 0; num < str_kk.size(); num++)
		{
			OutFile << str_kk[num] << endl;
		}

		OutFile.close();
	}

}

