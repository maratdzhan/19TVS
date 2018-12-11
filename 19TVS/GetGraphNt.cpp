#include "stdafx.h"
#include "19TVS.h"
#include "Error.h"
#include "Path_Finder.h"
#include "NDMaking.h"
#include "KK_KQ_Extracting.h"
#include "AdditionalCalculatings_1.h"

#include <string>
#include <fstream>
#include <vector>
using namespace std;

string global_str1;
string global_str2;
string tmp_str_graph;

unsigned int ACC =5; // ƒÎËÌ‡ ÒÚÓÍË ‚ Ù‡ÈÎÂ

vector<string> value1;
vector<string> value2;
vector<double> KQPvalue;
vector<double> KQMvalue;
vector<double> KQDelta;
double PKeffValue;
double MKeffValue;

double MaxElementVal1=0;
double MaxElementVal2=0;
double MinElementVal1=0;
double MinElementVal2=0;

/////////////////////////////////
// “ŒÀ‹ Œ —≈…◊¿—
//string st1 = "kr/mcu_";
string st2 = "kr/delta_";
// GetDeltaKK AdditionalCalculatings_1
string st1 = "mcu/";
//string st2 = "kk_N_";

/////////////////////////////////


void GetOrder(int tvs) noexcept
{
	////////////////// temporary
	double centerMaxVal = -101;
	double cornerMaxVal = -101;
	string description1, description2;
	/////////////////////////////
	for (unsigned int i = 0; i < value1.size(); i++)
	{
		if (stod(value1[i]) > MaxElementVal1) MaxElementVal1 = stod(value1[i]);
		if (stod(value1[i]) < MinElementVal1 && (abs(stod(value1[i]))>0.01)) MinElementVal1 = stod(value1[i]);
		if (stod(value2[i]) > MaxElementVal2) MaxElementVal2 = stod(value2[i]);
		if (stod(value2[i]) < MinElementVal2) MinElementVal2 = stod(value2[i]);

		if (((i == 277)) || ((i == 287)) || ((i == 297)) || ((i == 307)) || ((i == 317)) || ((i == 327)))
		{
			double t1 = (abs(stod(value2[i])));
			if ((centerMaxVal) < t1)
			{
				centerMaxVal = t1;
				description1 = to_string(i) + "->" + to_string(tvs);
			}
		}
		if ((i == 272) || (i ==282) || (i == 292) || (i ==302) || (i == 312) || (i ==322))
		{
			double t1 = (abs(stod(value2[i])));
			if ((cornerMaxVal)<t1)
			{
				cornerMaxVal = t1;
				description2 = to_string(i) + "->" + to_string(tvs);
			}
		}

	}
	/////////////////////////////////////////
	//// TEMPORARY
	Extraction1209(GetKQDelta(tvs),(to_string(tvs)+" "),0,1);
	Extraction1209(centerMaxVal, description1, 1, 0);
	Extraction1209(cornerMaxVal, description2,0,0);
	/////////////////////////////////////////

}

string GetGraphVal(int tvs, int stringNum)
{

		tmp_str_graph = ResultPathForMcu("",2);
		global_str1 = ResultPathForMcu("",2) + st1;
		global_str2 = tmp_str_graph + st2;
		value1.push_back("0");
		value2.push_back("0");
		if (stringNum > 0)
		{
			global_str1 += to_string(tvs);
			global_str1 += ".txt";
			ifstream inVal1(global_str1);

			if (!inVal1.is_open())
			{
				Error(L"ERROR OPENING SOURCE_1", L"GetGraphInfo");
			}
			else
			{
				while (!inVal1.eof())
				{
					getline(inVal1, tmp_str_graph);
					if (!tmp_str_graph.empty())
					{
						value1.push_back(tmp_str_graph);
						tmp_str_graph.clear();
					}
				}
			}
			inVal1.close();
		}

		if (stringNum > 1)
		{
			global_str2 += to_string(tvs);
			global_str2 += ".txt";
			ifstream inVal2(global_str2);
			if (!inVal2.is_open())
				Error(L"ERROR OPENING SOURCE_2", L"GetGraphInfo");
			else
			{

				while (!inVal2.eof())
				{
					getline(inVal2, tmp_str_graph);
					if (!tmp_str_graph.empty())
					{
						value2.push_back(tmp_str_graph);
						tmp_str_graph.clear();
					}
				}
				inVal2.close();
			}
		}
		global_str1.clear();
		global_str2.clear();
		GetOrder(tvs);


	return "NaN";
	
}

std::string GetGraphInfo(int tvel, int stringNum)
{
	string retVal;
	string tmp_str_to_ret_1;
	
	if (stringNum == 1)
	{
		tmp_str_to_ret_1 = value1[tvel];
		int size_1 = ACC;
		if (tmp_str_to_ret_1.size() < ACC) size_1 = tmp_str_to_ret_1.size();
		for (int i = 0; i < size_1; i++)
		{
			retVal += tmp_str_to_ret_1[i];

		}
	}
	if (stringNum == 2)
	{
		tmp_str_to_ret_1 = value2[tvel];
		int size_1 = ACC;
		if (tmp_str_to_ret_1.size() < ACC) size_1 = tmp_str_to_ret_1.size();
		for (int i = 0; i < size_1; i++)
		{
			retVal += tmp_str_to_ret_1[i];
		}
	}
	return retVal;
}

void GraphClear()
{
	value1.clear();
	value2.clear();
}
// GetKqKeff(true,true,true,false,
double GetKQKeff(bool write, bool getKeff, bool permak, bool clear, int returnAssamblyNumber, double acceptedValue)
{
	if (clear)
	{
		KQPvalue.clear();
		KQMvalue.clear();
		return 0;
	}

	if (write)
	{
		if (getKeff)
		{
			if (permak)
				PKeffValue = acceptedValue;
			else
				MKeffValue = acceptedValue;
		}
		else if (permak)
			KQPvalue.push_back(acceptedValue);
		else
			KQMvalue.push_back(acceptedValue);
	}
	else
	{
		if (getKeff)
		{
			if (permak)
				return PKeffValue;
			else
				return MKeffValue;
		}
		else
		{
			if (permak)
				return (KQPvalue[returnAssamblyNumber-1]);
			else
				return (KQMvalue[returnAssamblyNumber-1]);
		}
	}

}

wstring GetWstringFromString(string str,int lenght)
{
	wstring val;
	if (!lenght)
	val = wstring(str.begin(), str.end());
	else
	{
		string preRes;
		for (int pos = 0; pos < lenght + 1; pos++)
		{
			preRes += str[pos];
		}
		val = wstring(preRes.begin(), preRes.end());
	}

	return val;
}

double GetKQDelta(int tvs_Number)//(double valM, double valS) // val_Master, val_Slave
{
	double resultDelta = 100 * (KQPvalue[tvs_Number - 1] - KQMvalue[tvs_Number - 1]) / KQMvalue[tvs_Number - 1];
	return resultDelta;
}



double ReturnOrder(int mainVal, bool max)
{
	if (max)
	{
		if (mainVal == 1)
		{
			return MaxElementVal1;
		}
		else
		{
			return MaxElementVal2;
		}
	}

	if (!max)
	{
		if (mainVal == 1)
		{
			return MinElementVal1;
		}
		else
		{
			return MinElementVal2;
		}
	}
	Error(L"Do not know, what i must return", L"GetGraphNt -> ReturnOrder");
	return -987;
}

void OrderClear()
{
	MinElementVal1=0;
	MinElementVal2=0;
	MaxElementVal1=0;
	MaxElementVal2=0;
}


 