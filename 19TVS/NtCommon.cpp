#include "stdafx.h"
#include "19TVS.h"
#include "NtCommon.h"
#include "ToUpperFunct.h"
#include "Path_Finder.h"

#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

map<string, string> varName;
map<string, string> pathVarName;
map<string, string> mainPath;
vector<string> keffInRow;
map<string, double> KqInRows;
map<string,string> KqInColumns;





void WriteKq(bool InRow)
{
	ofstream kqFileCommon; 
	kqFileCommon.open(ReturnMainPath("CommonResNt") + "Kq.txt");
	

	if (InRow)
	{
		map<string, double>::iterator b = KqInRows.begin(), e = KqInRows.end();
		for (auto b = KqInRows.begin(); b != KqInRows.end(); b++)
		{
			kqFileCommon << b->first << b->second << endl;
		}
	}
	else
	{
		map<string, string>::iterator b = KqInColumns.begin(), e = KqInColumns.end();
		for (auto b = KqInColumns.begin(); b != KqInColumns.end(); b++)
		{
			kqFileCommon << b->first << "," << b->second << endl;
		}
	}
	kqFileCommon.close();
	KqInRows.clear();

}


void TakeKq(string state,double value)
{
	KqInRows[state] = value;
	KqInColumns[state] += ",";
	KqInColumns[state] += to_string(value);
}




void TakeKeff(string value)
{
		keffInRow.push_back((value));
}

void WriteKeff()
{
	ofstream keffFileCommon;
	keffFileCommon.open(ReturnMainPath("CommonResNt") + "Keff.txt");

	for (unsigned int a = 0; a < keffInRow.size(); a++)
	{
		keffFileCommon << keffInRow[a] << endl;
	}
	keffFileCommon.close();
	keffInRow.clear();
}





void CreateMainPath()
{
	mainPath[ToUpperFunc("permakpath")] = ToUpperFunc(Path_Finder("PermakPath"));
	mainPath[ToUpperFunc("sltifs")] = mainPath["PERMAKPATH"] + ToUpperFunc("bin/data/tests/slt.ifs");
	mainPath[ToUpperFunc("slink")] = mainPath["PERMAKPATH"] + ToUpperFunc("bin/data/tests/sl.ink");
	mainPath[ToUpperFunc("CommonResnt")] = mainPath["PERMAKPATH"] + ToUpperFunc("bin/res/ntc/Result/");

}

string ReturnMainPath(string inputString)
{
	string result = ToUpperFunc(inputString);

	return mainPath[result];
}





void PathMaker(string value, string pathName)
{
	pathVarName[pathName] = value;
		
}


void TakeParameter(string value, int valueType, string parameterName, int index1, int index2, int index3, bool is_path)
{
	if (is_path)
		PathMaker(ToUpperFunc(value),ToUpperFunc(parameterName));


}


void TakeForExtraction(int intVal, double doubleVal, string name, string description)
{

}

