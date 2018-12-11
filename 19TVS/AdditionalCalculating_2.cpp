#include "stdafx.h"
#include "AdditionalCalculatings_1.h"
#include "SessionInfo.h"
#include "Error.h"
#include "Path_Finder.h"

#include <stdio.h>
#include <map>


// Этот файл принимает и возвращает параметры для таблицы 1 и таблицы 2

struct Table_param
{
	int global_number;
	double gap_size[6];
	double cb;
	double gamCb;
	double gam;
	double Kinf[6];
	double descrepancy_1[6];
	double descrepancy_2[6];
	bool table_here;
};

Table_param Table_pp[2];


enum paramType
{
	null,
	gln,
	gap_size,
	cb,
	gamcb,
	gam,
	kinf,
	d1,
	d2,
}; 



void GetTableParam(int table_num, int param, int inner_num, double value)
{

	Table_pp[0].table_here = true;

	switch (param)
	{
	case gln:
		Table_pp[table_num - 1].global_number = (int)value;
		break;
	case gap_size:
		Table_pp[table_num - 1].gap_size[inner_num-1] = value;
		break;
	case cb:
		Table_pp[table_num - 1].cb = value;
		Table_pp[table_num - 1].gamCb = Table_pp[table_num - 1].cb*Table_pp[table_num - 1].gam / 0.7241;
		break;
	case gam:
		Table_pp[table_num - 1].gam = value;
		Table_pp[table_num - 1].gamCb = Table_pp[table_num - 1].cb*Table_pp[table_num - 1].gam / 0.7241;
		break;
	case d1:
		Table_pp[table_num - 1].descrepancy_1[inner_num-1] = value;
		break;
	case d2:
		Table_pp[table_num - 1].descrepancy_2[inner_num - 1] = value;
		break;
	default:
		Error(L"ERROR TYPE FOR TABLE", L"GetTableParam -> AdditionalCalc_2");
		break;
	}
}

double ReturnTableParam(int table_num, int param, int inner_num, double value)
{
	switch (param)
	{
	case null:
		return Table_pp[table_num-1].table_here;
		break;
	case gln:
		return Table_pp[table_num - 1].global_number;
		break;
	case gap_size:
		return Table_pp[table_num - 1].gap_size[inner_num - 1];
		break;
	case cb:
		return Table_pp[table_num - 1].cb;
		break;
	case gamcb:
		return Table_pp[table_num - 1].gamCb;
		break;
	case gam:
		return Table_pp[table_num - 1].gam;
		break;
	case kinf:
		return 
			9;
		break;
	case d1:
		return Table_pp[table_num - 1].descrepancy_1[inner_num - 1];
		break;
	case d2:
		return Table_pp[table_num - 1].descrepancy_2[inner_num - 1];
		break;
	default:
		Error(L"NOTHING TO RETURN -> ERROR TYPE FOR TABLE", L"ReturnTableParam -> AdditionalCalc_2");
		break;
	}
	return -555;
}



map <string, double> forResultCen;
map <string, double> forResultCor;
map <string, double> forResultKq;
map <string, double> forResultKqDev;

void Extraction1209(double value, string description, bool center, bool kq)
{

		if (kq)
		{
			if (center)
				forResultKq[description] = (value);
			else
				forResultKqDev[description] = (value);
		}
		else
		{
			if (center)
				forResultCen[description] = (value);
			else
				forResultCor[description] = (value);
		}



}

double aver_kk_cen = 0;
double aver_kk_corn = 0;
double max_kk_cen = 0;
double max_kk_corn = 0;
string max_pos_cen;
string max_pos_corn;

void Processing()
{
	
	for (auto i : forResultCen)
	{
		aver_kk_cen += i.second;
		if (i.second > max_kk_cen)
		{
			max_pos_cen = i.first;
			max_kk_cen = i.second;
		}
	}
	aver_kk_cen = aver_kk_cen / forResultCen.size();

	for (auto i : forResultCor)
	{
		aver_kk_corn += i.second;
		if (i.second > max_kk_corn)
		{
			max_kk_corn = i.second;
			max_pos_corn = i.first;
		}
	}
	aver_kk_corn = aver_kk_corn / forResultCor.size();


}


void Writting1209()
{
	Processing();
	SYSTEMTIME st;
	GetLocalTime(&st);
	char sp = 45;
	string outputString = to_string(st.wYear) + sp + to_string(st.wMonth) + sp + to_string(st.wDay) + sp + to_string(st.wHour) + sp + to_string(st.wMinute) + sp + to_string(st.wSecond)+"; ";
	string values;
	ofstream out(Path_Finder("TMP")+"dkk_cen.txt",iostream::app);
	values.clear();
	/*for (auto i : forResultCen)
	{
		values = outputString + i.first + "  ;  " + to_string(i.second);
		out << values << endl;
	}
	out << endl;
	*/
	out <<
		"state: "<< stoi(ReturnTableValues(false, 2, 1))<<
		" ; max: "<<max_pos_cen<<
		" ; max value: "<< max_kk_cen<<
		" ; aver: "<<aver_kk_cen<<
		endl<<endl;
	out.close();
	out.open(Path_Finder("TMP") + "dkk_cor.txt", iostream::app);

	/*for (auto i : forResultCor)
	{
		values = outputString + i.first + "  ;  " + to_string(i.second);
		out << values << endl;
	}
	out << endl;
	*/
	out <<
		"state: " << stoi(ReturnTableValues(false, 2, 1)) <<
		" ;max " << max_pos_corn <<
		" ; max: " << max_kk_corn <<
		" ; aver: " << aver_kk_corn << endl << endl;
	out.close();
	out.open(Path_Finder("TMP") + "kq.txt", iostream::app);
	/*for (auto i : forResultKq)
	{
		values = outputString + i.first + "  ;  " + to_string(i.second);
		out << values << endl;
	}*/
	out << endl;
	out.close();
	out.open(Path_Finder("TMP") + "dkq.txt", iostream::app);
	/*for (auto i : forResultKqDev)
	{
		values = outputString + i.first + "  ;  " + to_string(i.second);
		out << values << endl;
	}*/
	out << endl;
	out.close();

	forResultCen.clear();
	forResultCor.clear();
	forResultKq.clear();
	forResultKqDev.clear();
	aver_kk_cen = 0;
	aver_kk_corn = 0;
	max_kk_cen = 0;
	max_kk_corn = 0;
	max_pos_cen.clear();
	max_pos_corn.clear();
}