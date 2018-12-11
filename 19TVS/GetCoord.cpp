#include "stdafx.h"
#include "SessionInfo.h"
#include "NDMaking.h"
#include "FuelRodsBelongs.h"
#include "Error.h"
#include "GetParam.h"
#include "Path_Finder.h"
#include "AdditionalCalculatings_1.h"

#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <iostream>


using namespace std;
void ShowGaps();


int gl_c_n = 0;
vector<double> x_coord;
vector<double> y_coord;

const int MAX_CONST_PLANE_VALUE = 796;
const int STEP_VAL = 4;
const int START_VAL = -200;
const int CORNER_COUNT = 25+1;

double gap_size[100];
double gap_size_c[100];
double gs_c_const[50]; // Не удаляется после расчета
bool gap_c[100];
vector<int> gap_name;

int FAga[20][7] =
{
{0,0,0,0,0,0,0},
{ 1,22,16,17,23,29,28 },
{ 2,23,18,19,24,31,30 },
{ 3,11,6,8,12,18,17 },
{ 4,10,7,5,11,16,15, },
{ 5,21,14,15,22,27,26 },
{ 6,33,27,28,34,38,37 },
{ 7,34,29,30,35,40,39 },
{ 8,43,40,41,-1,-1,-1 },
{ 9,35,31,32,-1,-1,41 },
{ 10,24,20,-1,-1,-1,32 },
{ 11,12,19,-1,-1,20,19 },
{ 12,4,-1,-1,-1,9,8 },
{ 13,3,-1,-1,4,6,5 },
{ 14,-1,-1,-1,3,7,2 },
{ 15,-1,-1,2,10,14,13 },
{ 16,-1,-1,13,21,25 },
{ 17,-1,25,26,33,36,-1 },
{ 18,-1,36,37,42,-1,-1 },
{ 19,42,38,39,43,-1,-1 },
};

int FAna[20][7]=
{
{0,0,0,0,0,0},
{1,5,4,3,2,7,6},
{2,1,3,11,10,9,7},
{3,4,13,12,11,2,1},
{4,15,14,13,3,1,5},
{5,16,15,4,1,6,17},
{6,17,5,1,7,19,18},
{7,6,1,2,9,8,19},
{8,19,7,9,-1,-1,-1},
{9,7,2,10,-1,-1,8},
{10,2,11,-1,-1,-1,9},
{11,3,12,-1,-1,10,2},
{12,13,-1,-1,-1,11,3},
{13,14,-1,-1,12,3,4},
{14,-1,-1,-1,13,4,15},
{15,-1,-1,14,4,5,16},
{16,-1,-1,15,5,17,-1},
{17,-1,16,5,6,18,-1},
{18,-1,17,6,19,-1,-1},
{19,18,6,7,8,-1,-1},
};

string FAstCon_P[20][7];
string FAstCon_C[CORNER_COUNT];

// Первый массив возвращает в себе номер углового зазора
// (например: {22,27,28} находятся в массиве под номером 15 -> corner_gap_number=15, а 22, 27, 28 - образующие его плоские зазоры;
// Второй массив принимает в себе номера ТВС и последним числом в строке возвращает номер углового зазора

int cor_from_pln[26][3] =
{
{0,0,0},
{0,0,0},
{4,6,8},
{3,5,7},
{5,6,11},
{2,7,10},
{8,9,12},
{10,14,15},
{11,16,17},
{12,18,19},
{13,14,21},
{15,16,22},
{17,18,23},
{19,20,24},
{21,25,26},
{22,27,28},
{23,29,30},
{24,31,32},
{26,27,33},
{28,29,34},
{30,31,35},
{33,36,37},
{34,38,39},
{35,40,41},
{37,38,42},
{39,40,43},
};

int cor_from_tvs[26][4]
{
{13,10,17,1},
{ 3,13,12,2 },
{ 4,14,13,3 },
{ 3,4,13,4 },
{ 4,15,14,5 },
{ 3,12,11,6 },
{ 4,5,15,7 },
{ 1,4,3,8 },
{2,3,11,9},
{ 5,16,15,10 },
{ 1,5,4,11 },
{ 1,3,2,12 },
{2,11,10,13},
{ 5,17,16,14 },
{ 1,6,5,15 },
{ 1,2,7,16 },
{2,10,9,17},
{ 5,6,17,18 },
{ 1,7,6,19 },
{2,9,7,20},
{ 6,18,17,21 },
{ 6,7,19,22 },
{ 7,9,8,23 },
{ 6,19,18,24 },
{ 7,8,19,25 },

};

///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
// _ TEMP _ : _ dKinf aver_calc

vector<double> Kinf;

///////////////////////////////////
///////////////////////////////////
///////////////////////////////////


void GetCoord(int gsn)
{
	int maxNumber = (int)RetCoordinates(0, true, gsn);

	int currentNumber = ndMaking(gl_c_n,3);
	for (int i = 1; i < maxNumber-1; i++)
	{
		x_coord.push_back(RetCoordinates((x_coord.size()), true, gsn));
		y_coord.push_back(RetCoordinates((y_coord.size()), false, gsn));

		int z = 0;
	}

}

void GetGapSize()
{
	try
	{
	for (int num = 1; num < 20; num++)
	{
		for (int edge = 1; edge < 7; edge++)
		{
			// б
			// FAga[num][edge] - номер зазора
			// г
			if (FAga[num][edge] > 0)
			{
				if (gap_c[FAga[num][edge]] == false)
				{
					gap_c[FAga[num][edge]] = true;
					// а
					gap_size[FAga[num][edge]] =
						sqrt(
						(x_coord[num] - x_coord[FAna[num][edge]])*(x_coord[num] - x_coord[FAna[num][edge]]) 
							+ 
						(y_coord[num] - y_coord[FAna[num][edge]])*(y_coord[num] - y_coord[FAna[num][edge]])
						) - 234.0
						-2.0;

				}
			}
		}
	}

	}
	catch (const std::exception& excp)
	{
		Error(L"Error in SQRT(x^2+y^2). Check coord list file - 19+1 string must be present.", L"GetGapSize()->GetCoord");
	}

}

void WriteDescGap(int _gln)
{
	ofstream wgs(Path_Finder("DESC") + to_string(_gln) + "_Gaps.txt");
	for (int num = 1; num < 20; num++)
	{
		for (int edge = 1; edge < 7; edge++)
		{
			if (FAga[num][edge] > 0)
			{
				wgs << gap_size[FAga[num][edge]]<<",";
			}
		}
		wgs <<endl;
	}
	wgs.close();
}

///////////////////////////////////////////////////////////////////////
//////////////////___ TVEL #210/988-18 from 12.02.2018
//////////////////___ p.4.2
double GetCorrectionFactor(double cb, double gam, double ro5, double gs, bool plane)
{
	double modifier = 0;

	double _cfBor, _cfGam ,_cfRo5, cfGs_1, cfGs_2, cfGs_3, cfBor_1, cfBor_2, cfBor_3,
	cfGam_1, cfGam_2, cfGam_3, cfRo5_1, cfRo5_2, cfRo5_3, cfB_1, cfB_2, cfB_3;

	if (plane)
	{
		_cfBor = 6.54;	//const
		_cfGam = 0.725;	//const
		_cfRo5 = 3.4;	//const

						// Поправка до 0
		cfGs_1 = 0.0122;
		cfGs_2 = -0.0475;
		cfGs_3 = 0.1463;

		cfBor_1 = 0.0037;
		cfBor_2 = -0.0258;
		cfBor_3 = 0.0509;

		cfGam_1 = 0;// 0.0037;
		cfGam_2 = 0;// = -0.1750;
		cfGam_3 = 0;// = 2.3613;

		cfRo5_1 = -0.0059;
		cfRo5_2 = 0.0850;
		cfRo5_3 = -0.3151;

		cfB_1 = 0.0059;
		cfB_2 = -0.0025;
		cfB_3 = 0.9756;
	}
	else
	{
		_cfBor = 6.54;
		_cfGam = 0.725;
		_cfRo5 = 3.4;

		cfGs_1 = 0.0066;
		cfGs_2 = -0.0545;
		cfGs_3 = 0.2968;

		cfBor_1 = -0.002;
		cfBor_2 = 0.0107;
		cfBor_3 = 0.0144;

		cfGam_1 = -0.2074;
		cfGam_2 = 1.4073;
		cfGam_3 = 6.4448;

		cfRo5_1 = 0.0039;
		cfRo5_2 = -0.2945;
		cfRo5_3 = -1.1275;

		cfB_1 = -0.007;
		cfB_2 = 0.0108;
		cfB_3 = 1.2;// 0.6568;
	}
	double m1, m2, m3, m4, m5,m6;
	double timeFunction = Kinf[0];
	
	//
	double mn1 = 1.105;//1.105;
	double mn2 = (-1*abs(ro5 - _cfRo5)/10);// -0.004;
	if (plane)
	{
		m1 = (cfGs_1*gs*gs + cfGs_2 * gs + cfGs_3)*gs;						// ++
		m2 = (cfBor_1*gs*gs + cfBor_2 * gs + cfBor_3)*(cb - _cfBor);			// ++
		m3 = (cfGam_1*gs*gs + cfGam_2 * gs + cfGam_3)*(gam - _cfGam);			// ++
		m4 = (cfRo5_1*gs*gs + cfRo5_2 * gs + cfRo5_3)*(ro5 - _cfRo5);		// ++
		m5 = (cfB_1*gs*gs + cfB_2 * gs + cfB_3);
	}
	else
	{
		m1 = 0.66*gs;
		m2 = 0.039*(cb - _cfBor);
		m3 = 21.2*(gam - _cfGam);
		m4 = -4.04*(ro5 - _cfRo5); // -4.04*(ro5 - _cfRo5);
		m5 = 1.11;
	}
	//
	modifier = m1 + m2 + m3 + m4 + m5;
	////////////
	string path_for_modifiers = (Path_Finder("TMP") + "modifiers.txt");
	ofstream modifiers(path_for_modifiers, ios_base::app);
	modifiers << setprecision(5) << fixed;
	modifiers << "State: <"<<gl_c_n<<">. for gap="<<gs<< " GS = " << m1 << "; for cb="<<cb<<" BOR = " << m2 << "; Gam = " << m3 << "; Ro5 = " << m4 << "; Const = " << m5<<"\n";
	modifiers.close();
	////////////
	modifier = modifier + gs;

	return modifier;
}

void GetModifierCoefficient(bool plane)
{
	if (plane)
	{
		for (int num = 1; num < 20; num++)
		{
			for (int edge = 1; edge < 7; edge++)
			{
					//// 
					wstring tmwcb1 = ReturnTableValues(0, 3, 1);
					wstring tmwgam2 = ReturnTableValues(0, 4, 1);
					double tmp_cb_1 = (double)_wtof(tmwcb1.c_str());
					double tmp_gam_2 = (double)_wtof(tmwgam2.c_str());

					short neigNum = (FAna[num][edge] > 0 ? FAna[num][edge] : 0);
					double tmp_ro5_3 = ReturnKinfValue(num, neigNum, 0);



					if ((FAga[num][edge] > 0) && (gap_c[FAga[num][edge]] == true))
					{
						gap_size[FAga[num][edge]] = GetCorrectionFactor(tmp_cb_1, tmp_gam_2, tmp_ro5_3, gap_size[FAga[num][edge]], true);

						if (gap_size[FAga[num][edge]] < START_VAL)
							gap_size[FAga[num][edge]] = START_VAL / 100.;
						if (gap_size[FAga[num][edge]] * 100 > MAX_CONST_PLANE_VALUE)
							gap_size[FAga[num][edge]] = (MAX_CONST_PLANE_VALUE) / 100.;
						gap_c[FAga[num][edge]] = false;
					}
			}
		}

		for (int num = 1; num < 20; num++)
		{
			for (int edge = 1; edge < 7; edge++)
			{
				gap_c[FAga[num][edge]] = true;
			}
		}
	}
	else
	{
		for (short cornerNum = 1; cornerNum < CORNER_COUNT; cornerNum++)
		{
				wstring tmwcb1 = ReturnTableValues(0, 3, 1);
				wstring tmwgam2 = ReturnTableValues(0, 4, 1);
				double tmp_cb_1 = (double)_wtof(tmwcb1.c_str());
				double tmp_gam_2 = (double)_wtof(tmwgam2.c_str());
				short master = cor_from_tvs[cornerNum-1][0];
				short slave_1 = cor_from_tvs[cornerNum - 1][1];
				short slave_2 = cor_from_tvs[cornerNum - 1][2];
				double tmp_ro5_3 = ReturnKinfValue(master, slave_1, slave_2);
				gap_size_c[cornerNum] = GetCorrectionFactor(tmp_cb_1, tmp_gam_2, tmp_ro5_3, gap_size_c[cornerNum], false);
		}
	}

}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void GetRoundPlane()
{
	for (int num = 1; num < 20; num++)
	{
		for (int edge = 1; edge < 7; edge++)
		{
			if (gap_c[FAga[num][edge]] == true)
			{
				if (FAga[num][edge] > 0)
				{
					for (int j = START_VAL; j <= MAX_CONST_PLANE_VALUE; j += STEP_VAL)
					{
						if ((100 * gap_size[FAga[num][edge]]) <= j)
						{
							gap_size[FAga[num][edge]] = (j) / 100.;
							if (((j)) != 0)
							{
								FAstCon_P[num][edge] = "P" + to_string((j));
							}
							else
							{
								FAstCon_P[num][edge] = "PLNZZN";
							}
						//	gap_c[FAga[num][edge]] = false;

							break;
						}
					}
				}
			}
		}
	}
}

void GetRoundCorner()
{

	for (int num = 1; num < CORNER_COUNT; num++)
	{
		if (gap_size_c[num] >= MAX_CONST_PLANE_VALUE/100.)
		{
			gap_size_c[num] = MAX_CONST_PLANE_VALUE / 100.;
			FAstCon_C[num] = "C" + to_string((MAX_CONST_PLANE_VALUE));
		}
		else
		{
			for (int j = START_VAL; j <= MAX_CONST_PLANE_VALUE; j += STEP_VAL)
			{
				if ((100 * gap_size_c[num]) <= j)
				{
					gap_size_c[num] = (j) / 100.;
					gs_c_const[num] = gap_size_c[num];
					if (((j)) != 0)
					{
						FAstCon_C[num] = "C" + to_string((j));
					}
					else
					{
						FAstCon_C[num] = "CORZZN";
					}
					//	gap_c[FAga[num][edge]] = false;

					break;
				}

			}
		}
	}
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


void GetCornerSize()
{
	try
	{
		for (int num = 1; num < CORNER_COUNT; num++)
		{
			gap_size_c[num] =
				(gap_size[cor_from_pln[num][0]] + gap_size[cor_from_pln[num][1]] + gap_size[cor_from_pln[num][2]])/3;
		}
	}
	catch (const std::exception& excp)
	{
		Error(L"Error in corner gap calculating. Check coord list file - 19+1 string must be present.", L"GetCornerSize()->GetCoord");
	}
}

void GetWritePlane()
{
	string path_to_tmp_plane_container = Path_Finder("PermakPath") + "bin/ttmt/ttnt/tmp_gap.tmp";
	ofstream ofStream(path_to_tmp_plane_container);

	for (int num = 1; num < 20; num++)
	{
		for (int edge = 1; edge < 7; edge++)
		{
			if (FAga[num][edge] > 0)
			{
				if (gap_c[FAga[num][edge]] == true)
				{
					if (FAga[num][edge] < 10)
						path_to_tmp_plane_container = "0" + to_string(FAga[num][edge]);
					else
						path_to_tmp_plane_container = to_string(FAga[num][edge]);
					ofStream << "PL" << path_to_tmp_plane_container << "," << FAstCon_P[num][edge] << endl;
				}
				gap_c[FAga[num][edge]] = false;
			}
			
		}
	}
	for (int num = 1; num < CORNER_COUNT; num++)
	{
		if (!FAstCon_C[num].empty())
		{
			string numTypeString = (num > 9 ? to_string(num) : "0" + to_string(num));
			ofStream << "CR" << numTypeString << "," << FAstCon_C[num] << endl;
		}
	}

	ofStream.close();
}

void GetGapSize(int gln,int gsn)
{
	x_coord.push_back(0);
	y_coord.push_back(0);
	//gap_size.push_back(0);
	gl_c_n = gln;
	GetCoord(gsn);
	GetGapSize(); // Внутри переход к dX
	GetCornerSize();
	bool use_eG = true;
	// Расчет с dX

	if (use_eG)
		GetModifierCoefficient(true);

	// Ищем константы через dX (dx=0 -> plnzzn)
	GetRoundPlane();
	WriteDescGap(gln);

	
	if (use_eG)
		GetModifierCoefficient(false);

	GetRoundCorner();
	GetWritePlane();

	ShowGaps();
	x_coord.clear();
	y_coord.clear();
	//gap_size.clear();
}

void DefineKinf(short cycleNumber, short timeNumber)
{
	// Тут считываем Kinf из файла KinfCycles. Он имеет вид AssemblyKinf,CyclyNum
	// Таким образом мы имеем набор параметров, где каждый элемент соответствует номеру ТВС.
	ifstream KinfFile(Path_Finder("PermakPath") + "bin/data/tests/KinfCycles.txt"); 
	string str1;
	if (!KinfFile.is_open())
		Error(L"ERROR OPENING KINF FILE", L"Define Kinf -> GetCoord");
	else
	{
		Kinf.clear();
		short time = (cycleNumber == 1 ? (0 + timeNumber * 10) : (400 + timeNumber * 10));
		Kinf.push_back(time);
		while (!KinfFile.eof())
		{
			getline(KinfFile,str1);
			if (!str1.empty())
			{
				if (GetParam(str1, 2) == cycleNumber)
					Kinf.push_back(GetParamD(str1, 1));
			}
		}
	}

	KinfFile.close();
}

double ReturnCornerGapSize(short tvs1, short tvs2, short tvs3)
{
	// Передавай данные по часовой стрелке!!!!!
	double value = -9990;
	for (short t1 = 0; t1 < CORNER_COUNT + 1; t1++)
	{
		for (short t2 = 0; t2 < 4; t2++)
		{
			if (tvs1 == cor_from_tvs[t1][t2])
			{
				if (tvs2 == cor_from_tvs[t1][t2+1])
				{
					if (tvs3 == cor_from_tvs[t1][t2+2])
					{
						value = gs_c_const[cor_from_tvs[t1][3]];
						break;
					}
				}
			}
		}
	}
	return value;
}

double ReturnKinfValue(short tvs1, short tvs2, short tvs3)
{
	// Передавай данные по часовой стрелке!!!!!
	double value = -9990;
	short del;
	del=(tvs3 == 0 ? 2 : 3);
	// Как тут бороться с 0?
	double kinf1 = (tvs1 != 0 ? Kinf[tvs1] : 0);
	double kinf2 = (tvs2 != 0 ? Kinf[tvs2] : 0);
	double kinf3 = (tvs3 != 0 ? Kinf[tvs3] : 0);

	value = (kinf1 + kinf2 + kinf3) / del;
	return value;
}

void ClearGaps()
{
	for (short i = 0; i < 100; i++)
	{
		gap_size[i] = 0;
		gap_size_c[i]=0;
		gap_c[i] = 0;
		gs_c_const[i / 2] = 0;
	}
}

void ShowGaps()
{
	ofstream ofile("D:/Vasiliev/PERMAK-A/Local_new/bin/ttmt/ttnt/gaps_for_check.txt", ios_base::app);
	ofile << gap_size_c[3] << " " << FAstCon_C[3] << " making from: " << gap_size[7] << "+" << gap_size[5] << "+" << gap_size[3] << "\n";
	ofile << gap_size_c[4] << " " << FAstCon_C[4] << " making from: " << gap_size[5] << "+" << gap_size[6] << "+" << gap_size[11] << "\n";
	ofile << gap_size_c[5] << " " << FAstCon_C[5] << " making from: " << gap_size[2] << "+" << gap_size[7] << "+" << gap_size[10] << "\n";
	ofile.close();
}