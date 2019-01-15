#include "stdafx.h"
#include "19TVS.h"
#include "FuelRodsBelongs.h"
#include "Error.h"
#include "Path_Finder.h"
#include "GetParam.h"

#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <ppl.h>
#include <map>

using namespace std;
using namespace Concurrency;

const int FA_COUNT = 19;
const int FR_COUNT = 312;
const int GT_COUNT = 19;

string matrix_path, strMx;
string pDataMx = "bin/data/nt/";
string coordList;
string path_to_arrayCFile;

vector<int> fuelRodNumber;
vector<string> fuelRodType;
vector<int> innerNumeration__;
vector<int> tvs[FA_COUNT + 1];
vector<int> tvsnumMx;
vector<double> xc;
vector<double> yc;
vector<double> axc[FA_COUNT+1];
vector<double> ayc[FA_COUNT + 1];


vector<int> verticalSync;
vector<int> rowSt; 
vector<int> elem_pos_10;
vector<int> elem_pos_16;
vector<int> row_size;


int tvelAccounting[FA_COUNT + 1][FR_COUNT + GT_COUNT + 1];
int GlTvelAccounting[FA_COUNT + 1][FR_COUNT + GT_COUNT + 1];
int GlNumeration[FA_COUNT + 1][FR_COUNT + GT_COUNT + 1];

bool FL = true;






/* START READING */

void FuelRodsNumeration()
{
	matrix_path = Path_Finder("PermakPath") + pDataMx + "one.txt";
	fstream one(matrix_path);
	// Список номеров твэлов слева направо сверху вниз
	if (!one.is_open())
		Error(L"CANT LOAD ONE.TXT", L"FuelRodsNumeration -> FuelRodsBelongs");
	else
	{
		while (!one.eof())
		{
			getline(one, strMx);
			if (!strMx.empty())
				fuelRodNumber.push_back(stoi(strMx));
		}
	}
	one.close();
}

void FuelRodsBelongs()
{
	matrix_path.clear();
	strMx.clear();
	matrix_path = Path_Finder("PermakPath") + pDataMx + "two.txt";
	fstream two(matrix_path);
	// Список типов твэлов слева направо сверху вниз
	if (!two.is_open())
		Error(L"CANT LOAD two.TXT", L"FuelRodsBelongs -> FuelRodsBelongs");
	else
	{
		while (!two.eof())
		{
			getline(two, strMx);
			if (!strMx.empty())
				fuelRodType.push_back(strMx);
		}
	}
	two.close();
}

void FuelRodsInnerNumeration()
{
	matrix_path.clear();
	strMx.clear();
	matrix_path = Path_Finder("PermakPath") + pDataMx + "three.txt";
	fstream three(matrix_path);
	// МЦУ-шная нумерация
	if (!three.is_open())
		Error(L"CANT LOAD two.TXT", L"FuelRodsInnerNumeration -> FuelRodsBelongs");
	else
	{
		while (!three.eof())
		{
			getline(three, strMx);
			innerNumeration__.push_back(stoi(strMx));
		}
	}
	three.close();
}


/* END OF READING */

/* START ASSIGMENT */
void Reassigment()
{
	int inner_num_current = 1;
	int current_number = 0;
	int tmpnum;
	string s;
	for (int tvs_num = 1; tvs_num < FA_COUNT + 1; tvs_num++)
	{

		tmpnum = 1;
		for (unsigned int num = 1; num < fuelRodNumber.size(); num++)
		{

			s = fuelRodType[num];
			if ((s.find("PL") == -1) && (s.find("CR") == -1))
			{
				strMx += s[2];
				strMx += s[3];
			}
			else
			{
				strMx = "0";
			}

			// Определили, к какой ТВС принадлежит элемент
			current_number = stoi(strMx);
			strMx.clear();
			int a1 = 0;
			if (current_number == tvs_num)
			{
				int tmpKK = fuelRodNumber[num];
				if (tvs_num > 1)
				{
					a1 = tvelAccounting[1][tmpnum];
					tvelAccounting[tvs_num][a1] = fuelRodNumber[num];
				}
				else if (tvs_num == 1)
				{
					a1 = fuelRodNumber[num];
					tvelAccounting[tvs_num][a1] = a1;
				}
				GlTvelAccounting[tvs_num][tmpnum] = fuelRodNumber[num];
				GlNumeration[tvs_num][tmpnum] = num;
				tmpnum++;
			}

		}


	}
}

/* END OFASSIGMENT */


void GrabCoordinates()
{
	string grab_coord_str=Path_Finder("PermakPath")+ pDataMx + "Coordinates.dat";
	ifstream grFile(grab_coord_str);
	string grab_coords_ws; // WS - work string
	grab_coord_str.clear();

	int current_number = 0;
	bool path_now = false;
	int Set_Mode = -999;

	if (!grFile.is_open())
	{
		Error(L"COORDINATES CANT BE DEFINED! NOT FOUND <COORDITATES.DAT>", L"GrabCoordinates -> FuelRodsBelings");
	}
	else
	{
		getline(grFile, grab_coords_ws);
		Set_Mode = GetParam(grab_coords_ws, 1);
		if (Set_Mode >0)
		{
			
			for (int pos = 0; pos < grab_coords_ws.size(); pos++)
			{
				if (grab_coords_ws[pos] == ',')
				{
					path_now = true;
					continue;
				}
				if (path_now)
					grab_coord_str += grab_coords_ws[pos];
				//Сейчас NTCL
			}
			grab_coords_ws.clear();

			grab_coords_ws = Path_Finder(grab_coord_str);
			path_to_arrayCFile = grab_coords_ws;
			grab_coords_ws.clear();
		}
		if (Set_Mode==0)
		{
			grab_coords_ws.clear();
			do
			{
				getline(grFile, grab_coords_ws);
				yc.push_back(GetParamD(grab_coords_ws, 1));
				xc.push_back(GetParamD(grab_coords_ws, 2));
			} while (!grFile.eof());
		}
		
		
		if (Set_Mode < 0)
			Error(L"Check coordinates in <Coordinates.dat>", L"GrabCoordinates()->FuelRodsBelongs");
	}
	grFile.close();

	int ar_c_elem = 1;
	for (int state = 1; state < Set_Mode+1; state++)
	{
		string debugstr = path_to_arrayCFile + "_" + to_string(state);
		ifstream caf(debugstr);
		if (!caf.is_open())
		{
			wstring text_1 = GetWstringFromString(path_to_arrayCFile, path_to_arrayCFile.length());
			Error(text_1.c_str(), L"Error coordinates array list");;
		}
		else
		{
			
			do
			{
				getline(caf, grab_coords_ws);
				axc[ar_c_elem].push_back(-1*GetParamD(grab_coords_ws, 2));
				ayc[ar_c_elem].push_back(GetParamD(grab_coords_ws, 1));

				grab_coords_ws.clear();
			} while (!caf.eof());
		}
		caf.close();
		ar_c_elem++;
	}
}

void FirstLaunchNumeration()
{
	LoggerClass lg_2(__FUNCTION__);
	for (auto i = 0; i < FA_COUNT; i++)
	{

		axc[i].push_back(0);
		ayc[i].push_back(0); // если тут не комментить, то почему-то ayc[] на 1 элемент вверх прыгает


		tvs[i].push_back(0);
		for (auto j = 1; j<FR_COUNT + GT_COUNT + 1; j++)
		{
			tvelAccounting[i][j] = 0;
		}
	}
	tvsnumMx.push_back(0);
	fuelRodNumber.push_back(0);
	fuelRodType.push_back("0");
	innerNumeration__.push_back(0);
	xc.push_back(0);
	yc.push_back(0);
	

	GrabCoordinates();
	FuelRodsNumeration();
	FuelRodsBelongs();
	FuelRodsInnerNumeration();
	Reassigment();
}

int FRNumber(int tvs, int tvel, int param)
{
	if (param == 0)
		// возврат в локальной нумерации
		return (tvelAccounting[tvs][tvel]);
	if (param == 1)
		// возврат в глобальной нумерации от центра единой твс
		return (GlTvelAccounting[tvs][tvel]);
	if (param == 2)
		return (GlNumeration[tvs][tvel]);
	if (param == 3)
		return (GlTvelAccounting[1][tvel]);
}

void newMcuMatrixNumeration()
{

	int tmp_mx[FR_COUNT + GT_COUNT + 1];
	strMx.clear();
	strMx = Path_Finder("PermakPath") + pDataMx + "test_mx.txt.bak";
	ifstream inFile(strMx);
	strMx.clear();
	string add_path_gmr_1 = "bin/data/";
	ofstream ofFile(Path_Finder("PermakPath") + add_path_gmr_1 + "Nt/mcu_permak_sotv.txt");

	if (!inFile.is_open())
	{
		Error(L"Error opening mcu_mx.txt",L"newMcuMatrixNumeration");
	}
	else
	{
		int itter_1 = 1;
		while (!inFile.eof())
		{
			
			getline(inFile, strMx);
			if (!strMx.empty())
			{
				int num_tmp = stoi(strMx);
			///	if ()
				tmp_mx[GlTvelAccounting[1][itter_1]] = stoi(strMx);
				itter_1++;
			}
		}
		for (auto i = 1; i < FR_COUNT + GT_COUNT + 1; i++)
		{
			ofFile << i << "," << tmp_mx[i] << "," << endl;
		}
	}
	inFile.close();
	ofFile.close();

}

double RetCoordinates(int tvs, bool x, int gstate)
{
	double result = -567;

	if (x)
		result = axc[gstate][tvs];
	else
		result=ayc[gstate][tvs];
	// ЕСЛИ ТВС == 0 - ВОЗВРАЩАЕТ РАЗМЕР МАССИВА
	if (tvs == 0)
		result = axc[gstate].size();


	return result;
}

int ElemSumma(int i)
{
	if (i >= 1)
		return (i+(i-1));
	else return 0;
}

void FakeTvelDef(bool clearing)
{
	LoggerClass lg_2(__FUNCTION__);
	if (!clearing)
	{
		verticalSync.push_back(0);
		rowSt.push_back(0);
		row_size.push_back(0);
		for (short row = 1; row <= 41; row++)
		{
			unsigned int st_val = verticalSync.size();
			rowSt.push_back(st_val);
			if (row <= 11)
			{
				row_size.push_back(row);
				for (short i = st_val; i < (st_val + row); i++)
					verticalSync.push_back(row);
			}
			if ((row >= 12) && (row < 32))
			{
				int add_n = 0;
				if (row % 2 == 0)
					add_n = 10;
				else
					add_n = 11;
				row_size.push_back(add_n);
				for (short i = st_val; i < st_val + add_n; i++)
					verticalSync.push_back(row);
			}
			if (row >= 32)
			{
				int row_size_c = 42 - row;
				row_size.push_back(row_size_c);
				for (unsigned short i = st_val; i < st_val + row_size_c; i++)
					verticalSync.push_back(row);
			}
		}
		////////////////////////////////////////////////////////////////////////
		//////////// Это для 10-й ТВС номера, которые есть в ней.
		int currentPos = 1;
		int last_size = 1;
		elem_pos_10.push_back(0);
		elem_pos_16.push_back(0);
		for (unsigned short pos = 1; pos < verticalSync.size(); pos++)
		{
			int row_lenght = 0;
			if ((row_size[verticalSync[pos]] % 2) == 0)
				row_lenght = (row_size[verticalSync[pos]]) / 2;
			else
				row_lenght = (row_size[verticalSync[pos]]+1) / 2;
			


			if (currentPos <= row_lenght)
			{
				int resOut = FRNumber(1, pos, 3);
				int aa1= row_size[verticalSync[pos]];
				elem_pos_10.push_back(FRNumber(1, pos, 3));
			}
			if (currentPos < row_size[verticalSync[pos]])
				currentPos++;
			else
				currentPos = 1;
			

		}
		currentPos = 1;


		for (unsigned short pos = 1; pos < verticalSync.size(); pos++)
		{
			int row_lenght = 0;
			row_lenght = 1 + (row_size[verticalSync[pos]]) / 2;


			if (currentPos >= row_lenght)
			{
				elem_pos_16.push_back(FRNumber(1, pos, 3));
			}
			if (currentPos < row_size[verticalSync[pos]])
				currentPos++;
			else
				currentPos = 1;

		}

	}
	else
	{
		rowSt.clear();
		verticalSync.clear();
		elem_pos_10.clear();
		elem_pos_16.clear();
	}
	int z111 = 90;
}

int ActuallyTvel(int tvs, int tvel, bool finding)
{
	int ActualTvel = -927;
	if (!finding)
	{
		if (tvs == 10)
		{
			for (unsigned short i=0;i< elem_pos_10.size(); i++)
			{
				if (elem_pos_10[i] == tvel)
					return tvel;
			}
			return -62;
		}
		else if (tvs==16)
		{
			ActualTvel = FRNumber(1, tvel,false);
			for (unsigned short i = 0; i< elem_pos_16.size(); i++)
			{
				if (elem_pos_16[i] == tvel)
					return tvel;
			}
			return -63;
		}
	}
	else
	{
		if (tvs == 10)
		{
			for (unsigned short i = 0; i< elem_pos_10.size(); i++)
			{
				if (elem_pos_10[i] == tvel)
					return i;
			}
			return -62;
		}
		else if (tvs == 16)
		{
			ActualTvel = FRNumber(1, tvel, false);
			for (unsigned short i = 0; i< elem_pos_16.size(); i++)
			{
				if (elem_pos_16[i] == tvel)
					return i;
			}
			return -63;
		}
	}

	return ActualTvel;
}

int FakeTvelMethod(int tvsNum, int tvelNum, bool verticalSym)
{
	if (verticalSym)
	{
		int start_pos = rowSt[verticalSync[tvelNum]];
		int lenght_pos = row_size[verticalSync[tvelNum]];
		int current_pos = tvelNum;
		int val5 = start_pos + (lenght_pos - (current_pos - start_pos + 1));
		return val5;
	}
	else
	{
		return (332 - tvelNum);
	}

}

double FakeTvel(int tvsNum, int tvelNum)
{
	int retVal = -997;
	if ((tvsNum == 8)|| (tvsNum == 14)|| (tvsNum == 18)|| (tvsNum == 12))
	{
			retVal = (FakeTvelMethod(tvsNum, tvelNum, false));
	}
	if ((tvsNum == 10)|| (tvsNum == 16))
	{
		int grTvel = FRNumber(1, tvelNum, 3);
		int inFact = ActuallyTvel(tvsNum, grTvel,false);

		if (inFact ==grTvel)
			return tvelNum;

		retVal = (FakeTvelMethod(tvsNum, tvelNum, true));
	}
	if (retVal != -997)
		return retVal;
	else
		Error(L"FAKE_TVEL ERROR", L"ERROR CONSISTING 8-19 TVS");
}

void ClearingInitial()
{

	
	fuelRodNumber.clear();
	fuelRodType.clear();
	innerNumeration__.clear();
	verticalSync.clear();
	rowSt.clear();
	elem_pos_10.clear();
	elem_pos_16.clear();
	row_size.clear();
	tvsnumMx.clear();


}

void Clearing_Temporary()
{
	for (short i = 0; i < FA_COUNT + 1; i++)
	{

		tvs[i].clear();

	}
	xc.clear();
	yc.clear();
}









