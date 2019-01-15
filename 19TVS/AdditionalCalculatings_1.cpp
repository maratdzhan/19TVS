#include "stdafx.h"
#include "Error.h"
#include "Path_Finder.h"
#include "19TVS.h"
#include "GetParam.h"
#include "NDMaking.h"
#include "AdditionalCalculatings_1.h"
#include "SessionInfo.h"
#include "KK_KQ_Extracting.h"

#include <string>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <iomanip>


using namespace std;

// std::string RetWithOutLastSym(std::string str, char(c));
// int GetParam(string s, int num);

/////////////////////////////////////////////////////////////////////////////////////
// В данном файле мы проводим следующую процедуру:
// 1) Считываем набор KK для MCU
// 2) Считываем набор KK для PERMAK-A
// 3) Считываем набор KQ для MCU
// 4) Считываем набор KQ для PERMAK-A
// 5) Перемножаем KK(M)*KQ(M)
// 6) Перемножаем KK(P)*KQ(P) 
// 7) Получаем (KR(P)-KR(M)/KR(M))
// 8) Записываем в файл KR(M) в нумерации PERMAK-A
// 9) Записываем delta(m-p) в нумерации PERMAKA-A
/////////////////////////////////////////////////////////////////////////////////////


// в коде пометки /**/ - это говно, которое потом надо удалить

string mcu_res_kk_file;
string mcu_res_kq_file;
string mcu_res_kr_file;
string permak_res_kk_file;
string permak_res_kq_file;
string permak_res_kr_file;
string delta_res_kr_file;
string tmp_string;
string tmp_string_p;
string tmp_string_m;
string textStr;

vector<double> mcu_kk;
vector<double> mcu_kq;
vector<double> permak_kk;
vector<double> permak_kq;
vector<double> mcu_kr;
vector<double> permak_kr;
vector<double> delta_kr;




double DSGroups[3][2][6]; // для углов и для плоских зазоров по 2 типа расчета и для 6-ти параметров [DISCREP_T][CEN][EDGE]

int tvs_mod = 0;

// Additional_calculatings_2

const int MAX_GROUP_NUMBER = 30;

struct Group
{
	bool table_one;
	bool nonEmpty;
	int _gtvs[10];
	int _gtvel[10];

	double _kkp[10];
	double _kkm[10];

	double _discrepancy;
};
Group observedGroup[MAX_GROUP_NUMBER];

struct Table
{
	string stateName;
	int gln_mcu;
	int edgeNumber[6];
	double gapSize[6];
	double cb;
	double gamCb;
	double gam;
	double Kinf[6];
	double descrepancy_1;
	double descrepancy_2;
};

Table Table1[8];

//

const int FA_NUMBER = 163;
const int CELL_NUMBER = 331;
const int FR_NUMBER = 312;
const int COUNTED_FA = 7;
const wchar_t * LocalName = L"Additional_Calculatings";

bool tParam;

void StartFunction(int inputStateNumber)
{
	mcu_kk.push_back(0);
	mcu_kq.push_back(0);
	mcu_kr.push_back(0);
	permak_kk.push_back(0);
	permak_kq.push_back(0);
	permak_kr.push_back(0);
	delta_kr.push_back(0);
	tmp_string = Path_Finder("PermakPath");
	tmp_string += "bin/res/ntc/result/";
	tmp_string += to_string(inputStateNumber);
	tmp_string += "/";
	// Полный путь к папке готов
	tmp_string_m=tmp_string + "mcu/";
	tmp_string_p = tmp_string;

}

void ClearFunction()
{
	mcu_res_kk_file.clear();
	mcu_res_kq_file.clear();
	mcu_res_kr_file.clear();
	permak_res_kk_file.clear();
	permak_res_kq_file.clear();
	delta_res_kr_file.clear();
	tmp_string.clear();
	tmp_string_p.clear();
	tmp_string_m.clear();

	mcu_kk.clear();
	mcu_kq.clear();
	permak_kk.clear();
	permak_kq.clear();
	mcu_kr.clear();
	permak_kr.clear();
	delta_kr.clear();
}

void GetKK(int tvs)
{
	permak_res_kk_file += tmp_string_p;
	permak_res_kk_file += "kk_N_";
	permak_res_kk_file += to_string(tvs);
	permak_res_kk_file += ".txt";
	mcu_res_kk_file += tmp_string_m;
	mcu_res_kk_file += to_string(tvs);
	mcu_res_kk_file += ".txt";
	ifstream inP(permak_res_kk_file);
	ifstream inM(mcu_res_kk_file);

	int err = 0;
	if (!inP.is_open())
	{
		Error(L"Error opening permak result file", LocalName);
		err++;
	}
	if (!inM.is_open())
	{
		Error(L"Error opening mcu result file", LocalName);
		err++;
	}
	if (err == 0)
	{
		while (!inP.eof())
		{
			getline(inP, textStr);
			if (!textStr.empty())
			{
				permak_kk.push_back(stod(textStr));
			}
		}
		while (!inM.eof())
		{
			getline(inM, textStr);
			if (!textStr.empty())
			{
				mcu_kk.push_back(stod(textStr));
			}
		}
	}


	permak_res_kk_file.clear();
	mcu_res_kk_file.clear();
	inP.close();
	inM.close();
}

void GetKQ(int tvs,bool tParam)
{
	if (tParam)
	{
		permak_res_kq_file += tmp_string_p;
		permak_res_kq_file += "KQ_sum.out";
		mcu_res_kq_file += tmp_string_m;
		mcu_res_kq_file += "KQ.txt";
		ifstream inP(permak_res_kq_file);
		ifstream inM(mcu_res_kq_file);
		
		int err = 0;
		if (!inP.is_open())
		{
			Error(L"Error opening permak KQ result file", LocalName);
			err++;
		}
		if (!inM.is_open())
		{
			Error(L"Error opening mcu KQ result file", LocalName);
			err++;
		}
		if (err == 0)
		{
			while (!inP.eof())
			{
				getline(inP, textStr);
				if (!textStr.empty())
				{
					permak_kq.push_back(stod(textStr));
				}
			}
			while (!inM.eof())
			{
				getline(inM, textStr);
				if (!textStr.empty())
				{
					mcu_kq.push_back(stod(textStr));
				}
			}
		}


		tParam=false; // was ==
		permak_res_kq_file.clear();
		mcu_res_kq_file.clear();
		inP.close();
		inM.close();
	}
}

void GetKR(int tvs)
{
	for (int tvel = 1; tvel < CELL_NUMBER+1; tvel++)
	{
		permak_kr.push_back((permak_kk[tvel] * permak_kq[tvs]));
		mcu_kr.push_back(mcu_kk[tvel] * mcu_kq[tvs]);
	}
}

void GetDeltaKK(int tvs)
{
	delta_res_kr_file = tmp_string_m + "delta_" + to_string(tvs) + ".txt";
	for (int tvel = 1; tvel < CELL_NUMBER + 1; tvel++)
	{
		if (mcu_kr[tvel] != 0)
			//			delta_kr.push_back(100 * (permak_kr[tvel] - mcu_kr[tvel]) / mcu_kr[tvel]);
			delta_kr.push_back(100 * (permak_kk[tvel] - mcu_kk[tvel]) / mcu_kk[tvel]);
		else
			delta_kr.push_back(0);
		GetMcuKkValues(tvs, tvel, delta_kr[delta_kr.size() - 1]);
	}
	
}

void WriteResultDelta(int tvs)
{
	permak_res_kr_file = tmp_string_p;
	mcu_res_kr_file = tmp_string_m;

	textStr.clear();
	textStr += tmp_string_p;
	textStr += "/kr/";
	wstring dr = wstring(textStr.begin(), textStr.end());
	CreateDirectory(dr.c_str(), NULL);

	ofstream dkr(textStr+"delta_"+to_string(tvs)+".txt");
	ofstream pkr(textStr+"permak_"+to_string(tvs) + ".txt");
	ofstream mkr(textStr+"mcu_"+to_string(tvs) + ".txt");
	ofstream pointsCen(textStr + "points_cen_" + to_string(tvs) + ".txt");
	ofstream pointsCor(textStr + "points_cor_" + to_string(tvs) + ".txt");


	for (int tvel = 1; tvel < CELL_NUMBER + 1; tvel++)
	{
		dkr << delta_kr[tvel] << endl;
		pkr << permak_kr[tvel] << endl;
		mkr << mcu_kr[tvel] << endl;
		bool thisOneCen = false, thisOneCorn = false;
		if ((tvel == 277) || (tvel == 287) || (tvel == 297) || (tvel == 307) || (tvel == 317) || (tvel == 327)
			|| (tvel == 276) || (tvel == 286) || (tvel == 296) || (tvel == 306) || (tvel == 316) || (tvel == 326)
			|| (tvel == 278) || (tvel == 288) || (tvel == 298) || (tvel == 308) || (tvel == 318) || (tvel == 328))
			thisOneCen = true;
		if ((tvel == 272) || (tvel == 282) || (tvel == 292) || (tvel == 302) || (tvel == 312) || (tvel == 322))
			thisOneCorn = true;

		if (thisOneCen)
		{
			pointsCen << delta_kr[tvel] << endl;
			thisOneCen = false;
		}
		if (thisOneCorn)
		{
			pointsCor << delta_kr[tvel] << endl;
			thisOneCorn = false;
		}
	}

	pointsCen.close();
	pointsCor.close();
	dkr.close();
	pkr.close();
	mkr.close();
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// Additional_calculatings_2


void GetObservedRods()
{
	{
		string desciptionDescrepancy = ResultPathForMcu("", 2);

		ofstream desc(desciptionDescrepancy + "descrepancy_description.txt");
		string st1 = "группа 1 - 5 твэлов в центре грани";
		desc << st1 << endl;
		st1.clear();
		st1 = "группа 2 - 3 смежных угловых твэла";
		desc << st1 << endl;
		st1.clear();
		st1 = "группа 3 - 10 твэлов - по 5 на двух смежных гранях";
		desc << st1 << endl;
		st1.clear();
		st1 = "группа 4 - 1 угловой твэл";
		desc << st1 << endl;
		st1 = to_string(GetKQKeff(false, true, true, false, 1, 0));
		desc <<"Keff="<< st1 << endl;
		st1.clear();
		desc.close();
	}
	// GetGroups
	// Planes
	for (int i = 0; i <= 5; i++)
	{
		observedGroup[i].table_one = true;
		observedGroup[i].nonEmpty = true;
		observedGroup[2]._gtvel[i] = i + 275;
		observedGroup[3]._gtvel[i] = i + 285;
		observedGroup[4]._gtvel[i] = i + 295;
		observedGroup[5]._gtvel[i] = i + 305;
		observedGroup[0]._gtvel[i] = i + 315;
		observedGroup[1]._gtvel[i] = i + 325;
		observedGroup[0]._gtvs[i] = 1;
		observedGroup[1]._gtvs[i] = 1;
		observedGroup[2]._gtvs[i] = 1;
		observedGroup[3]._gtvs[i] = 1;
		observedGroup[4]._gtvs[i] = 1;
		observedGroup[5]._gtvs[i] = 1;
	}
	// Corners 
	{
		observedGroup[6].table_one = false; observedGroup[7].table_one = false; observedGroup[8].table_one = false; observedGroup[9].table_one = false;
		observedGroup[10].table_one = false; observedGroup[11].table_one = false;
		observedGroup[6]._gtvs[0]=1; observedGroup[6]._gtvs[1]=6; observedGroup[6]._gtvs[2]=5; observedGroup[6].nonEmpty = true;
		observedGroup[7]._gtvs[0]=1; observedGroup[7]._gtvs[1]=5; observedGroup[7]._gtvs[2]=4; observedGroup[7].nonEmpty = true;
		observedGroup[8]._gtvs[0]=1; observedGroup[8]._gtvs[1]=4; observedGroup[8]._gtvs[2]=3; observedGroup[8].nonEmpty = true;
		observedGroup[9]._gtvs[0]=1; observedGroup[9]._gtvs[1]=3; observedGroup[9]._gtvs[2]=2; observedGroup[9].nonEmpty = true;
		observedGroup[10]._gtvs[0]=1; observedGroup[10]._gtvs[1]=2; observedGroup[10]._gtvs[2]=7; observedGroup[10].nonEmpty = true;
		observedGroup[11]._gtvs[0]=1; observedGroup[11]._gtvs[1]=7; observedGroup[11]._gtvs[2]=6; observedGroup[11].nonEmpty = true;

		observedGroup[6]._gtvel[0] = 312; observedGroup[6]._gtvel[1] = 272; observedGroup[6]._gtvel[2] = 292;

		observedGroup[7]._gtvel[0] = 322; observedGroup[7]._gtvel[1] = 282; observedGroup[7]._gtvel[2] = 302;

		observedGroup[8]._gtvel[0] = 272; observedGroup[8]._gtvel[1] = 292; observedGroup[8]._gtvel[2] = 312;

		observedGroup[9]._gtvel[0] = 282; observedGroup[9]._gtvel[1] = 302; observedGroup[9]._gtvel[2] = 322;

		observedGroup[10]._gtvel[0] = 292; observedGroup[10]._gtvel[1] = 312; observedGroup[10]._gtvel[2] = 272;

		observedGroup[11]._gtvel[0] = 302; observedGroup[11]._gtvel[1] = 322; observedGroup[11]._gtvel[2] = 282;

	}
	// Planes with Neighbors
	{

		for (int i = 0; i < 5; i++)
		{
			observedGroup[12+i].nonEmpty = true;
			observedGroup[12+i].table_one = true;
			observedGroup[14]._gtvel[i] = i + 275; observedGroup[12]._gtvs[i] = 1;				observedGroup[14]._gtvel[i + 5] = i + 305; observedGroup[14]._gtvs[i + 5] = 3;
			observedGroup[15]._gtvel[i] = i + 285; observedGroup[13]._gtvs[i] = 1;				observedGroup[15]._gtvel[i + 5] = i + 315; observedGroup[15]._gtvs[i + 5] = 2;
			observedGroup[16]._gtvel[i] = i + 295; observedGroup[14]._gtvs[i] = 1;				observedGroup[16]._gtvel[i + 5] = i + 325; observedGroup[16]._gtvs[i + 5] = 7;
			observedGroup[17]._gtvel[i] = i + 305; observedGroup[15]._gtvs[i] = 1;				observedGroup[17]._gtvel[i + 5] = i + 275; observedGroup[17]._gtvs[i + 5] = 6;
			observedGroup[12]._gtvel[i] = i + 315; observedGroup[16]._gtvs[i] = 1;				observedGroup[12]._gtvel[i + 5] = i + 285; observedGroup[12]._gtvs[i + 5] = 5;
			observedGroup[13]._gtvel[i] = i + 325; observedGroup[17]._gtvs[i] = 1;				observedGroup[13]._gtvel[i + 5] = i + 295; observedGroup[13]._gtvs[i + 5] = 4;
		}
		observedGroup[17].nonEmpty = true;
		observedGroup[17].table_one = true;
	}
	// Single Corner
		
		
		observedGroup[18]._gtvel[0] = 312;
		observedGroup[19]._gtvel[0] = 322;
		observedGroup[20]._gtvel[0] = 272;
		observedGroup[21]._gtvel[0] = 282;
		observedGroup[22]._gtvel[0] = 292;
		observedGroup[23]._gtvel[0] = 302;
		for (int i = 0; i < 6; i++)
		{
			observedGroup[18 + i]._gtvs[i] = i+1;
			observedGroup[18 + i].table_one = false;
			observedGroup[18 + i].nonEmpty = true;
		}
		// with 1.6
		{
			for (int i = 0; i < 5; i++)
			{
				observedGroup[24+i].nonEmpty = true;
				observedGroup[24+i].table_one = true;
				observedGroup[24]._gtvel[i] = i + 285; observedGroup[24]._gtvs[i] = 4;				observedGroup[24]._gtvel[i + 5] = i + 305; observedGroup[24]._gtvs[i + 5] = 3;
				observedGroup[25]._gtvel[i] = i + 305; observedGroup[25]._gtvs[i] = 4;				observedGroup[25]._gtvel[i + 5] = i + 325; observedGroup[25]._gtvs[i + 5] = 5;
				observedGroup[26]._gtvel[i] = i + 285; observedGroup[26]._gtvs[i] = 4;
				observedGroup[27]._gtvel[i] = i + 305; observedGroup[27]._gtvs[i] = 4;
			}
			observedGroup[29].nonEmpty = true;
			observedGroup[29].table_one = true;

		}

}


void GetKkForDiscrepancyValue()
{
	// Прогоняем по всем группам
	// Если группа не пустая
	// По всем элементам "_gtvs"
	// Если tvs_mod (оператор внешнего условного цикла) равен текущей _gtvs = считываем велчину KK
	for (int groupNumber = 0; groupNumber < MAX_GROUP_NUMBER; groupNumber++)
	{
		if (observedGroup[groupNumber].nonEmpty == true)
		{
			for (int currentParam = 0; currentParam < 10; currentParam++)
			{
				if (tvs_mod == observedGroup[groupNumber]._gtvs[currentParam])
				{
					observedGroup[groupNumber]._kkp[currentParam] = permak_kk[observedGroup[groupNumber]._gtvel[currentParam]];
					observedGroup[groupNumber]._kkm[currentParam] = mcu_kk[observedGroup[groupNumber]._gtvel[currentParam]];
				}
			}
		}
	}
}

void GetCalculateDiscrepancy()
{
	short elementsCount = 0;
	for (int currentGroupNumber = 0; currentGroupNumber < MAX_GROUP_NUMBER; currentGroupNumber++)
	{

		for (short elements = 0; elements < 10; elements++)
		{
			if (observedGroup[currentGroupNumber]._gtvs[elements] != 0)
			{
				observedGroup[currentGroupNumber]._discrepancy +=
					observedGroup[currentGroupNumber]._kkp[elements] - observedGroup[currentGroupNumber]._kkm[elements];
				elementsCount++;
			}
		}
		observedGroup[currentGroupNumber]._discrepancy = observedGroup[currentGroupNumber]._discrepancy / elementsCount;
		elementsCount = 0;
	}
}

void GetStateName(int gln)
{
	LoggerClass lg_1(__FUNCTION__);
	string nn1 = Path_Finder("PermakPath") + "bin/data/tests/tests_description/Param.txt";
	ifstream stNameFile(nn1);
	nn1.clear();

	if (!stNameFile.is_open())
		Error(L"CANT READ PARAM.txt", L"GetStateName -> Add. calc. #1");
	else
	{
		do
		{
			// Нет учета, что строка пустая или лишние символы в ней.
			getline(stNameFile, nn1);
			if (GetParam(nn1, 1) == gln)
			{
				for (int i = 0; i < 8; i++)
				{
					Table1[i].stateName = GetParamS(nn1, 4);
					Table1[i].gln_mcu = GetParam(nn1, 1);
					Table1[i].cb= GetParamD(nn1, 2);
					Table1[i].gam = GetParamD(nn1, 3);
				}
				ifstream GSW(Path_Finder("PermakPath")+"bin/data/tests/tests_description/"+to_string(gln)+"_Gaps.txt");
				if (!GSW.is_open())
					Error(L"Error reading Gaps", L"GetStateName -> Add.Calc.#1");
				else
				{
					for (int tvs = 0; tvs < 8; tvs++)
					{
						getline(GSW, nn1);
						for (int edge = 1; edge < 7; edge++)
						{
							Table1[tvs].gapSize[edge - 1] = GetParamD(nn1, edge);
						}
					}
				}
				GSW.close();
			}
		} while (!stNameFile.eof());
	}
	stNameFile.close();
}

void WriteDiscrepancyResult()
{
	string openFilePath;
	int startNum = 0, lastNum = 6;
	for (int i = 1; i < 6; i++)
	{
		openFilePath = ResultPathForMcu("", 2) + "group_" + to_string(i) + "_discrepancy.txt";
		ofstream wrf(openFilePath);
		for (int groupNumber =startNum; groupNumber < i*lastNum; groupNumber++)
		{
			if (observedGroup[groupNumber].nonEmpty)
			{
				wrf << observedGroup[groupNumber]._discrepancy << endl;
			}
		}
		startNum += lastNum;
		openFilePath.clear();
		wrf.close();
	}
}

double ReturnKinf(int edge)
{
	if (edge == 0)
		return 3.33;
	if ((edge == 1) || (edge == 3) || (edge == 5))
	{
		return 3.80;
	}
	else
		return 3.20;
}

void GetTable(int gln_c)
{

	wstring tmwcb1 = ReturnTableValues(0, 3, 1);
	wstring tmwgam2 = ReturnTableValues(0, 4, 1);
	double tmp_cb_1 = (double)_wtof(tmwcb1.c_str());
	double tmp_gam_2 = (double)_wtof(tmwgam2.c_str());


	
	ofstream ofs_t(Path_Finder("TABLE2"),ios_base::app);
//	ofs_t << "#(MCU) EDGE GAP_SIZE CB CB(GAM) GAM Kinf Discrep1 Discrep2" << endl;
	for (short group = 6;  group <= 11; group++)
	{
		double gap_size = 
			ReturnCornerGapSize(observedGroup[group]._gtvs[0], observedGroup[group]._gtvs[1], observedGroup[group]._gtvs[2]);
		ofs_t << gln_c<< " ";
		ofs_t << tmp_cb_1 << " ";
		ofs_t << tmp_cb_1 * tmp_gam_2 << " ";
		ofs_t << tmp_gam_2 << " ";
		ofs_t << ReturnKinfValue(observedGroup[group]._gtvs[0], observedGroup[group]._gtvs[1], observedGroup[group]._gtvs[2]) << " ";
		ofs_t << setprecision(6) << gap_size << " ";
		ofs_t << observedGroup[group]._discrepancy;
		ofs_t << endl<<endl;
	}

	ofs_t.close();

}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void KrCalculatings(int inputStatenumber)
{
	tParam = true;
	for (tvs_mod = 1; tvs_mod < COUNTED_FA + 1; tvs_mod++)
	{
		StartFunction(inputStatenumber);
		GetKK(tvs_mod);
		GetKQ(tvs_mod,tParam);
		GetKR(tvs_mod);
		GetDeltaKK(tvs_mod);
		WriteResultDelta(tvs_mod);

		///////////////////////////_START_1
		// Additional_calculatings_2
		GetObservedRods(); // Работает хорошо
		GetKkForDiscrepancyValue(); // Работает хорошо
		
		GetStateName(inputStatenumber);
		///////////////////////////_END_1



		ClearFunction();
		GetMaxDeviations(tvs_mod);
	}
	///////////////////////////_START_2
	GetCalculateDiscrepancy();
	WriteDiscrepancyResult();
	GetTable(inputStatenumber);
	///////////////////////////_END_2
	tvs_mod = 0;
	

}

wstring ReturnTableValues(bool header,int paramNumber, int tvs)
{
	tvs = tvs - 1;
	string resultStr;
	int edge = paramNumber - 5;
	if (header)
	{
		switch (paramNumber)
		{
		case 1:
			resultStr = "Name";
			break;
		case 2:
			resultStr = "State #";
			break;
		case 3:
			resultStr = "Cb";
			break;
		case 4:
			resultStr = "Gam";
			break;
		case 5:
		case 6:
		case 7:
		case  8:
		case 9:
		case 10:
			resultStr.clear();
			resultStr += "gap("+to_string(6+edge-5)+")";
			break;
		default:
			resultStr = "ERROR";
			break;
		}
	}
	else
	{
		switch (paramNumber)
		{
		case 1:
			resultStr=Table1[tvs].stateName;
			break;
		case 2:
			resultStr = to_string(Table1[tvs].gln_mcu);
			break;
		case 3:
			resultStr = to_string(Table1[tvs].cb);
			break;
		case 4:
			resultStr = to_string(Table1[tvs].gam);
			break;
		case 5:
		case 6:
		case 7:
		case  8:
		case 9:
		case 10:
			resultStr = to_string(Table1[tvs].gapSize[5+edge-5]);
			break;
		default:
			resultStr="ERROR";
			break;
		}
	}
	
	if ((resultStr.length() > 6)&&(paramNumber>1))
	{
		int endStr=5;
		string tmpStr1 = resultStr;
		resultStr.clear();
		for (int i = 0; i < endStr; i++)
		{
			resultStr += tmpStr1[i];
		}
	}


	wstring wresult = wstring(resultStr.begin(), resultStr.end());
	return wresult;
}


