#include "stdafx.h"
#include "KK_KQ_Extracting.h"
#include "Path_Finder.h"
//#include "ToUpperFunct.h"
#include "19TVS.h"
#include "Error.h"
#include "FuelRodsBelongs.h";
#include "NDMaking.h"
#include "NtCommon.h"

#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <thread>
#include <direct.h>
#include <map>
#include <iomanip>
using namespace std;


const int MAX_SIZE_TVEL = 331;
const int NUMBER_OF_FA = 19;
const int HALF_PART_TVS = 165;

class TVSnt
{
	// external frnum contains in frnum
	// inner frnum contains in [i]
public:
	int frnum[MAX_SIZE_TVEL + 1];
	double kk[MAX_SIZE_TVEL + 1];
	double kq;
	double kk_in[MAX_SIZE_TVEL + 1];
	double summa_w_dev = 0;
	double summa_wo_dev = 0;
	double mcu_kk[332];
	double delta_mcu_kk[332];

	double maxUnderestimation=1000;
	double maxRevaluation=-1000;
	double maxDevPeriphery=-1000;
	double maxDevSecondRow=-1000;

};

double tmp = 0;
short gln_Local = 0;

vector<int> number, thnumber;
extern vector<int> number;

unsigned int neuro = 0;
unsigned int value1 = 5000;


vector<int> numtype;
vector<string> celltype;
vector<double> kk;
vector<int> tvsnum;
int frnum[MAX_SIZE_TVEL + 1];
bool inputTypeSQL = false;
///////////////////////////////
int summary[NUMBER_OF_FA + 1][MAX_SIZE_TVEL + 1];
///////////////////////////////
TVSnt ntCore[NUMBER_OF_FA + 1];
//TVSnt nts[NUMBER_OF_FA + 1];

// string kres		= "";
// string tmp_path	= "";
// string name1		= "";
// string name2		= "";
 string pData		= "bin/data/nt/";
 string pRes		= "bin/res/ntc/result/";
 string pTtmt		= "bin/ttmt/ttnt/";
 string pTmppath	= "bin/res/ntc/tmp/";
 string for_mcu;
 string resultDir;


void Sort()
{
	LoggerClass Lg_2_1(__FUNCTION__);
	string PP = Path_Finder("PermakPath");
	string a = PP + pData + "tmp-1.TXT";
	ifstream theirMX(a);
	a = PP + pData + "tmp.TXT";
	ifstream ourMX(a);
	a = PP + pData + "test.txt";
	ofstream test(a);
	int iter = 0, i = 0;
	string s;
	while (theirMX.eof() != 1)
	{
		getline(theirMX, s);
		thnumber.push_back(stoi(s));
	}

	while (ourMX.eof() != 1)
	{
		getline(ourMX, s);
		i = stoi(s);
		if (i < 332)
		{
			if (thnumber[iter] != 0)
			{
				number.push_back(i);
				test << i << endl;
			}
			s.clear();
			iter++;
		}
	}
	iter = 0;
}

int checkPath(int err)
{
	LoggerClass Lg_2_1(__FUNCTION__);
	string PP = Path_Finder("PermakPath");
	string path = Path_Finder("PermakPath") + pTtmt +"perm_"+ to_string(gln_Local) + ".out";

	ifstream one(PP + pData + "one.txt");
	ifstream two(PP + pData + "two.txt");
	ifstream three(PP + pData + "three.txt");
	ifstream four(path);

	if (one.is_open() != 1) {
		printf("Error in one\n"); err++;
	}
	if (two.is_open() != 1) {
		printf("Error in two\n"); err++;
	}
	if (three.is_open() != 1) {
		printf("Error in three\n"); err++;
	}
	if (four.is_open() != 1) {
		printf("Error in four\n"); err++;
	}
	

	string aa = PP + pRes + to_string(gln_Local);
	/*
	// Для _mkdir
	const char * aaw = aa.c_str();
	//_mkdir(aaw);

	*/
	for_mcu.clear();
	wstring dr = wstring(aa.begin(), aa.end());
	resultDir = aa+"/";
	ResultPathForMcu(resultDir, 3);
	for_mcu = aa+"/mcu/";
	ResultPathForMcu(for_mcu, 4);
	CreateDirectory(dr.c_str(), NULL);

	return err;
}

void DefineKK()
{
	LoggerClass Lg_2_1(__FUNCTION__);
	string path = Path_Finder("PermakPath") + pTtmt + "perm_" + to_string(gln_Local) + ".out";
	ifstream four(path);
	string s;


	while (!four.eof())
	{
		getline(four, s);
		if (!s.empty())
		{
			double z = stod(s);
			kk.push_back(z / 1000);
		}
	}
}

void Assigment()
{
	LoggerClass Lg_2_1(__FUNCTION__);
	int l = 1;
	if (inputTypeSQL)
	{

	}
	else
	{
		for (int i = 1; i < NUMBER_OF_FA + 1; i++)
		{
			for (unsigned int j = 1; j < numtype.size(); j++)
			{
				if (tvsnum[j] == i)
				{
					ntCore[i].frnum[l] = numtype[j];
					l++;
				}
			}
			l = 1;
		}
	}
}

void KkAssigment()
{
	LoggerClass Lg_2_1(__FUNCTION__);
	map<int, int> tempor;
	if (inputTypeSQL)
	{

	}
	else
	{
		for (int tvs = 1; tvs < NUMBER_OF_FA + 1; tvs++)
		{
			for (int tvel = 1; tvel < MAX_SIZE_TVEL + 1; tvel++)
			{
				/* 
				int z1 = FRNumber(tvs, tvel, 0);
				tmp = FRNumber(tvs, tvel, 1);
				ntCore[tvs].kk[FRNumber(tvs, tvel, 3)] = kk[FRNumber(tvs, tvel, 2)];

				// OLD METHOD */


				if ((tvs != 8) && (tvs != 10) && (tvs != 12) && (tvs != 14) && (tvs != 16) && (tvs != 18))
					ntCore[tvs].kk[FRNumber(1, tvel,3)] = kk[FRNumber(tvs, tvel,2)];
				else
				{
						
					if ((tvs != 10) && (tvs != 16))
					{
						if (FRNumber(tvs,tvel,2)!=0)
						ntCore[tvs].kk[FRNumber(1, tvel, 3)] = kk[FRNumber(tvs,tvel,2)];
						else
						ntCore[tvs].kk[FRNumber(1, tvel, 3)] = kk[FRNumber(tvs,FakeTvel(tvs,tvel),2)];
					}
					else
					{
						int v = FakeTvel(tvs, tvel);
						int vreg = v;
						// ниже заходит номер твэла в нумерации полной кассеты.
						v=ActuallyTvel(tvs, FRNumber(tvs,v,3), true);
						v = FRNumber(tvs, v, 2);
						if (kk[v] == 0)
							tempor[tvel] = vreg;
						ntCore[tvs].kk[FRNumber(1, tvel, 3)] = kk[v];
					}
				}
				// NEW METHOD */
			}
		}
	}
}


double Calculating(int tvs)
{
	LoggerClass Lg_2_1(__FUNCTION__);
	double summa = 0;
	int type = 0;
	bool err = false;
	double half_correct = 0;
	int max_num = MAX_SIZE_TVEL + 1;
	wstring dr;

	multimap<int, int> emptyElements;


	string tmp_path = Path_Finder("PermakPath") + pTmppath + to_string(gln_Local) + "/";
	string name1 = tmp_path + "perm_" + to_string(tvs) + ".out";
	dr = wstring(tmp_path.begin(), tmp_path.end());
	CreateDirectory(dr.c_str(), NULL);


	ofstream out_tmp(name1);


	{
		for (unsigned int tvel = 1; tvel < MAX_SIZE_TVEL + 1; tvel++)
		{
			summa += ntCore[tvs].kk[tvel];
			if (ntCore[tvs].kk[tvel] == 0)
				emptyElements.insert(pair<int, int>(tvs, tvel));
			out_tmp << ntCore[tvs].kk[tvel] << endl;
		}
	}
	if ((tvs == 8) || (tvs == 12) || (tvs == 14) || (tvs == 18))
		summa = summa * (1 + (5 / 331.1));
	// NEW METHOD */


	ntCore[tvs].summa_wo_dev = summa;		// SUMMA
	ntCore[tvs].summa_w_dev = summa / 312.0;	// AVERAGE

	int number = 0;
	for (int tvel = 1; tvel < MAX_SIZE_TVEL + 1; tvel++)
	{
		ntCore[tvs].kk_in[tvel] = (ntCore[tvs].kk[tvel]) / (ntCore[tvs].summa_w_dev);
	}


	return summa;

}


int KK_KQ_Extracting(int gln_l)
{

	LoggerClass lg_Ex_1(__FUNCTION__);
	number.clear();
	thnumber.clear();
	number.clear();
	numtype.clear();
	celltype.clear();
	kk.clear();
	tvsnum.clear();
	


	string pp = Path_Finder("PermakPath");
	string kk_in=pp+pTtmt+ "perm_" + to_string(gln_l) + ".out";
	ifstream four(kk_in);
	string tt = pp+pRes + to_string(gln_l) + "/";
	tt += "KQ_sum.out";
	string KQ = tt;
	gln_Local = gln_l;


	numtype.push_back(0);
	celltype.push_back("0");
	kk.push_back(0);
	tvsnum.push_back(0);
	Sort();


	int err = checkPath(0);
	if (err)
	{
		printf("ERROR IN PATHS -> KK_KQ_EXTRACTION");
		system("pause");
		return 0;
	}

	DefineKK();
	Assigment();
	KkAssigment();

	// На этом шаге вся информация записана
	// Теперь расчеты
	double summa_kq[NUMBER_OF_FA + 1], summa = 0;
	string name;

	///////////////////////////////////////////////////////////
	// Вывод результатов


	summa_kq[0] = 0;
	for (int tvs = 1; tvs < NUMBER_OF_FA + 1; tvs++)
	{
		summa_kq[tvs] = Calculating(tvs);
		summa += summa_kq[tvs];
	}

	summa = summa / NUMBER_OF_FA;
	ofstream KQOut(KQ);

	for (int tvs = 1; tvs < NUMBER_OF_FA + 1; tvs++)
	{
		double res = summa_kq[tvs] / summa;
		if (tvs < 8)
		{
			TakeKq((to_string(tvs)), res);
		}
		KQOut << res << endl;
		GetKQKeff(true, false, true, false, tvs, res);
		/////////////////////////////////////////////////////////
		// Вывод KK
		string name = pp + pRes + to_string(gln_l) + "/" + "kk_N_" + to_string(tvs) + ".txt";
		ofstream out1(name);
	

		for (int i = 1; i < 332; i++)
		{
			//if (ntCore[tvs].mcu_kk[i]>0)
			out1 << ntCore[tvs].kk_in[i] << endl;
		}
		out1.close();

	}
	tt.clear();
	KQOut.close();


	/*string ty = Path_Finder("PermakPath") + pRes + to_string(gln_l) + "/";
	ty += "dKQ.out";
	ofstream DKQ(ty);
	double resultValue = 0;
	for (short i = 1; i != NUMBER_OF_FA + 1; i++)
	{
		resultValue = (GetKQKeff(0, 0, 1, 0, i, 0));
		resultValue = (resultValue - GetKQKeff(0, 0, 0, 0, i, 0)) / GetKQKeff(0, 0, 1, 0, i, 0);
		DKQ << (resultValue) << endl;;
	}

	DKQ.close();*/

	return 0;
}

void GetMaxDeviations(short tvs)
{
	LoggerClass Lg_2_1(__FUNCTION__);
	// Это путь к файлу, в который будем писать - общий и текущий
	string localMaxDeviationsPath = Path_Finder("PermakPath") + pRes + "/commonDevs.txt";
	string localCurrentMaxDeviationsPath = Path_Finder("PermakPath") + pRes + to_string(gln_Local) + "/" + "current.txt";

	// Открываем файлы
	ofstream localMax(localMaxDeviationsPath, ios_base::app);
	ofstream localCurrentMax(localCurrentMaxDeviationsPath, ios_base::app);

	// Поиск отклонений
	short t1 = -1, t2 = -1, t3 = -1, t4 = -1;

	for (short tvel = 1; tvel < MAX_SIZE_TVEL + 1; tvel++)
	{
		if (tvel >= 218 && tvel <= 271)
		{
			if (abs(ntCore[tvs].delta_mcu_kk[tvel]) > ntCore[tvs].maxDevSecondRow) {
				ntCore[tvs].maxDevSecondRow = abs(ntCore[tvs].delta_mcu_kk[tvel]);
				t1 = tvel;
			}
		}
		if (tvel >= 272 && tvel <= 331)
		{
			if (abs(ntCore[tvs].delta_mcu_kk[tvel]) > ntCore[tvs].maxDevPeriphery) {
				ntCore[tvs].maxDevPeriphery = abs(ntCore[tvs].delta_mcu_kk[tvel]);
				t2 = tvel;
			}
		}
		if (((ntCore[tvs].delta_mcu_kk[tvel]) < ntCore[tvs].maxUnderestimation)) {
			ntCore[tvs].maxUnderestimation = ntCore[tvs].delta_mcu_kk[tvel];
			t3 = tvel;
		}
		if (((ntCore[tvs].delta_mcu_kk[tvel]) > ntCore[tvs].maxRevaluation)) {
			ntCore[tvs].maxRevaluation = ntCore[tvs].delta_mcu_kk[tvel];
			t4 = tvel;
		}
	}

	localMax << "state " << gln_Local << "\t" << "FA " << tvs << " in " << t3 << " tvel ->\t Max Underestimation " << ntCore[tvs].maxUnderestimation << "\n";
	localMax << "state " << gln_Local << "\t" << "FA " << tvs << " in " << t4 << " tvel ->\t Max Revaluation     " << ntCore[tvs].maxRevaluation << "\n";
	localMax << "state " << gln_Local << "\t" << "FA " << tvs << " in " << t1 << " tvel ->\t Max Dev Second Row  " << ntCore[tvs].maxDevSecondRow << "\n";
	localMax << "state " << gln_Local << "\t" << "FA " << tvs << " in " << t2 << " tvel ->\t Max Dev Periphery     " << ntCore[tvs].maxDevPeriphery << "\n\n";

	localCurrentMax << "state " << gln_Local << "\t" << "FA " << tvs << "\t Max Underestimation " << ntCore[tvs].maxUnderestimation << "\n";
	localCurrentMax << "state " << gln_Local << "\t" << "FA " << tvs << "\t Max Revaluation     " << ntCore[tvs].maxRevaluation << "\n";
	localCurrentMax << "state " << gln_Local << "\t" << "FA " << tvs << "\t Max Dev Second Row  " << ntCore[tvs].maxDevSecondRow << "\n";
	localCurrentMax << "state " << gln_Local << "\t" << "FA " << tvs << "\t Max Dev Periphery     " << ntCore[tvs].maxDevPeriphery << "\n\n";

	localMax.close();
	localCurrentMax.close();

	ntCore[tvs].maxUnderestimation = 1000;
	ntCore[tvs].maxRevaluation = -1000;
	ntCore[tvs].maxDevSecondRow = -10;
	ntCore[tvs].maxDevPeriphery = -10;
}

void GetMcuKkValues(short tvs, short tvel, double value)
{
	ntCore[tvs].delta_mcu_kk[tvel] = value;
}

void ClearPermakERs()
{
	LoggerClass Lg_2_1(__FUNCTION__);
	number.clear();
	thnumber.clear();
	number.clear();
	numtype.clear();
	celltype.clear();
	kk.clear();
	tvsnum.clear();
	inputTypeSQL = false;
	for (short i = 0; i < MAX_SIZE_TVEL + 1; i++)
		frnum[MAX_SIZE_TVEL + 1] = 0;

	for (short tvs = 0; tvs < NUMBER_OF_FA + 1;tvs++)
	{
		for (short tvel = 0; tvel < MAX_SIZE_TVEL + 1; tvel++)
		{
			ntCore[tvs].frnum[tvel];
			ntCore[tvs].kk[tvel];
			ntCore[tvs].kq;
			ntCore[tvs].kk_in[tvel];
			ntCore[tvs].summa_w_dev = 0;
			ntCore[tvs].summa_wo_dev = 0;
			ntCore[tvs].mcu_kk[tvel];
			ntCore[tvs].delta_mcu_kk[tvel];
			ntCore[tvs].maxUnderestimation = 1000;
			ntCore[tvs].maxRevaluation = -1000;
			ntCore[tvs].maxDevPeriphery = -1000;
			ntCore[tvs].maxDevSecondRow = -1000;
		}
	}



}



