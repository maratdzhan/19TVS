#include "stdafx.h"
#include "19TVS.h"
#include "GetMCUResult.h"
#include "Path_Finder.h"
#include "Error.h"
#include "GetParam.h"
#include "KK_KQ_Extracting.h"


#include <fstream>
#include <string>
#include <vector>
using namespace std;

const int NUMBER_OF_TVELS = 312;
double mcu_result[110][8][NUMBER_OF_TVELS+1];
int mcu_sotv_list[NUMBER_OF_TVELS + 20]; // (19 -> GT + CT + 1)
string path_gmr, add_path, result, tmpStr, str;

string add_path_gmr = "bin/data/";
string glpath_gmr;
vector<double> mcu_kq_extract;

void SotvLinkMcu()
{
	ifstream SotvMcuFile(Path_Finder("PermakPath") + add_path_gmr + "Nt/mcu_permak_sotv.txt");
	string strSotv;
	int tvel = 0;
	if (!SotvMcuFile.is_open())
		Error(L"ERROR MCU_PERMAK_SOTV.TXT", L"SotvLinkMcu -> GetMCUResult");
	else
	{
		while (!SotvMcuFile.eof())
		{
			getline(SotvMcuFile, strSotv);
			mcu_sotv_list[++tvel] = GetParam(strSotv, 2);
		}
	}

}

int ReturnMcuNumber(int tvel)
{
	return mcu_sotv_list[tvel];
}

void GetMcuInfo(int gln)
{
	// получаем номер расчета
	// и номер твс
	path_gmr = Path_Finder("PermakPath") + add_path_gmr+ "tests/atests/s";
	add_path = to_string(gln);
	add_path += ".kk";
	glpath_gmr = path_gmr+to_string(gln)+".kq";
	tmpStr = path_gmr + add_path;

	string aat1 = ResultPathForMcu("",1);
	wstring thisFile;
	thisFile = wstring(aat1.begin(), aat1.end());
	CreateDirectory(thisFile.c_str(), NULL);


	ifstream inp(tmpStr);
	ifstream inpkq(glpath_gmr);
	ofstream of_mcu_kq(ResultPathForMcu("",1)+"KQ.txt");

	
	if (!inp.is_open())
		Error(L"CANT LOAD MCU RESULT FILE", L"GetMCUResult -> GetMcuInfo");
	else
	{
		tmpStr.clear();
		int line_num = 1;
		while (!inp.eof())
		{
			int inner_num = 1;
			getline(inp, str);
			for (auto i = 0; i < str.size(); i++)
			{
				if (((char(str[i]) >= 48) && (char(str[i]) < 58) || (char(str[i]) == 46)) && (i < str.size() - 1))
				{
					tmpStr += str[i];
				}
				else
				{
					if (!tmpStr.empty())
					{
						mcu_result[gln][inner_num++][line_num] = stod(tmpStr);
						tmpStr.clear();
					}
				}

			}
			tmpStr.clear();
			str.clear();
			line_num++;
		}
	}

	if (!inpkq.is_open())
		Error(L"CANT LOAD MCU KQ FILE", L"GetMCUResult -> GetMcuInfo");
	else
	{
		/////////////////////////////////////////
		//int innerNumerationMcuFA[19] = { 18,13,11,14,2,16,4,7,9,1,8,6,5,15,3,17,12,1,19};
		// OR
		int innerNumerationMcuFA[19] = {8,9,19,10,7,18,2,6,11,1,17,3,5,12,4,16,13,15,14};
		/////////////////////////////////////////
		tmpStr.clear();
		string tt;
		while (!inpkq.eof())
		{
			getline(inpkq,tmpStr);
			tt.clear();
			if (!tmpStr.empty())
			{
				for (int pos = 0; pos < tmpStr.size(); pos++)
				{
					if ((char(tmpStr[pos]) > 47) && (char(tmpStr[pos]) < 58) || (char(tmpStr[pos]) == 46))
					{
						tt += tmpStr[pos];
					}
					else
					{

						if (!tt.empty())
						{
							mcu_kq_extract.push_back(stod(tt));
							tt.clear();
						}
						// Прикол в чем? когда не числа, но и не 
					}
					if (pos == tmpStr.size()-1)
					{
						if (!tt.empty())
						{
							mcu_kq_extract.push_back(stod(tt));
							tt.clear();
							break;
						}
					}
				}
				
			}
		}
		for (int i = 1; i < 20; i++)
		{
			for (int z1 = 0; z1 < 19; z1++)
			{
				if (innerNumerationMcuFA[z1] == i)
				{
					int j = z1;
					of_mcu_kq << mcu_kq_extract[z1] << endl;
					GetKQKeff(true,false,false,false,i,mcu_kq_extract[z1]);
					break;
				}
			}
		}
	}
	inp.close();
	inpkq.close();
	of_mcu_kq.close();
	mcu_kq_extract.clear();
	path_gmr.clear();
	add_path.clear();
}

void GetMcuKeff(int gln)
{
	string path_keff_mcu= path_gmr = Path_Finder("PermakPath") + "bin/data/tests/atests/mcu.keff";
	ifstream MKeff(path_keff_mcu);
	string keffReading;
	if (!MKeff.is_open())
		Error(L"Not Found MCU KEFF FILE", L"GetMcuKeff -> GetMcuResult");
	else
	{
		do
		{
			getline(MKeff, keffReading);
			if (!keffReading.empty())
			{
				if ((GetParam(keffReading, 1)) == gln)
				{
					GetKQKeff(true, true, false, false, 1, GetParamD(keffReading, 2));
				}
			}
		} while (!MKeff.eof());
	}
}

double CalculatingSensCoeff(double firstval, double secondval, int tvs, int tvel)
{
	return 0;
}


void ExtraditionMcuResult(int gln, string path_for_mcu) 
{
	string ofFile;
	for (int i = 1; i < 8; i++)
	{
		
		ofFile = path_for_mcu + to_string(i) + ".txt";
		ofstream out(ofFile);

		for (int j = 1; j < NUMBER_OF_TVELS + 20; j++)
		{
			int res = mcu_sotv_list[j];
			if (res != 0)
			{
				double result = mcu_result[gln][i][res];
				out << result << endl;
			}
			else
			{
				out << 0 << endl;
			}
		}
		out.close();
	}
}




void GetMcuResult(int gln, string ptmcu)
{
	GetMcuInfo(gln);
	ExtraditionMcuResult(gln,ptmcu);
	GetMcuKeff(gln);
}
