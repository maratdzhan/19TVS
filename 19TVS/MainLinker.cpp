#include "stdafx.h"
#include "19TVS.h"
#include "MainLinker.h"
#include "ToUpperFunct.h"
#include "Path_Finder.h"
#include "SessionInfo.h"
#include "Error.h"

#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <xstring>
#include <stdlib.h>
#include <winnt.h>

using namespace std;



const int matrix_types_number = 1000;
const int MOD_SIZE = 19 + 1;
const int MOD_PLANE_SIZE = 43;

bool complexity=0;
bool firstLaunch = true;
unsigned short position = 0;

const string EOK = "EOK";
//_declspec(dllimport) float CDefine(float h_TVS, float angle, int tvs, bool x);

const bool DUPLICATION = true;
////////////////////////
int calcNum = 0;
string calcNumStr;
// ����� ��:
string path;
string ParametersFile;
// ������ �:
string path2 = "D:/Vasiliev/PERMAK-A/Local_new/PROJ/B01/";
// ������ �������� �:
string path3 = "D:/Vasiliev_Others/CppTestDir/bin/res/";
////////////////////////

string InputFileName = path+ "bin/data/nt/" + "matrix_19.TXT";
string OutputFileName = path2 + "matrix.per";
string permparFile = path + "bin/data/nt/" + "permpar_input_19.dat";
string permparDir = path2 + "permpar";
string duplicatePermpar = path3 + "permpar_test_" + calcNumStr;
string duplicateMaxtrix = path3 + "matrix_test_" + calcNumStr + ".per";

///////////////////////
vector<string> ntMxMain;
vector<string> topStr_num;
vector<string> GXStr_num;
vector<string> CTStr_num;
vector<string> PLNStr_num;
vector<string> CRNStr_num;
vector<string> topStr_type;
vector<string> GXStr_type;
vector<string> CTStr_type;
vector<string> PLNStr_type;
vector<string> CRNStr_type;
vector<string> matrix;
vector<string> mapkasHistory;
vector<string> newStr_1;
vector<int> numHistory;
vector<string> strHistory;

int localHistoryNumberCounter = 0;


string wordFind_1_mapkas = "KASSET";

const int const_name_size = 1500;
int CONST_NAME[const_name_size];


string StrCutting(string inputLine, int BSpos)
{
	// BSpos - Bad Symbol Position
	string result;

	for (int pos = 0; pos < BSpos - 1; pos++)
	{
		if (inputLine[pos] != ' ')
		{
			result += inputLine[pos];
		}
	}

	return result;
}

 
int ConstRef(string sootv)
{
	//���������� ����� �������� (��� ��������)
	if (!sootv.empty())
	{
		for (unsigned int num = 0; num < newStr_1.size(); num++)
		{

			if (sootv == newStr_1[num])
			{
				return (num);
			}
		}
	}
	Error(L"WRONG CONSTANT NAME IN MXPARAM",L"CONSTREF -> MAINLINKER");
	return -445;
}

int TextStrFunc(string inputLine, string num)
{
	unsigned int i = 0;
	string summary,tmpVal;
	// ��� ��� ��������? � �����?
	// � ��� ���� �������� ������ inputLine
	// ������� ����� ��������� ������ ���� "CR01"
	// �� ����, ��� ��� �� ������
	summary += inputLine + num;
	int numi = stoi(num);

	int testVAL = inputLine.find("PL");

	{
		if (inputLine.find("TO")!=-1)
		{
			for (i = 0; i < topStr_num.size(); i++)
			{
				if (summary == topStr_num[i])
				{
					tmpVal += topStr_type[i];
					break;
				}
			}
		}
		if (inputLine.find("GX")!=-1)
		{
			for (i = 0; i < GXStr_num.size(); i++)
			{
				if (summary == GXStr_num[i])
				{
					tmpVal += GXStr_type[i];
					break;
				}
			}
		}
		if (inputLine.find( "CT")!=-1)
		{
			for (i = 0; i < CTStr_num.size(); i++)
			{
				if (summary == CTStr_num[i])
				{
					tmpVal += CTStr_type[i];
					break;
				}
			}
		}
		if (inputLine.find("PL")!=-1)
		{
			for (i = 1; i < PLNStr_num.size(); i++)
			{
				testVAL = PLNStr_num[i].find(num);
				if (PLNStr_num[i].find(num)!=-1)
				{
					tmpVal += PLNStr_type[i];
					break;
				}
			}
		}
		if (inputLine.find("CR")!=-1)
		{
			for (i = 0; i < CRNStr_num.size(); i++)
			{
				//if (summary == CRNStr_num[i]) ���� ���
				/*����� ���:*/
				testVAL = CRNStr_num[i].find(num);
				if (CRNStr_num[i].find(num) != -1)
				{
					tmpVal += CRNStr_type[i];
					break;
				}
			}
		}
	}

	for (int j = 0; j < localHistoryNumberCounter; j++)
	{
		if (tmpVal==strHistory[j])
		{
			return j;
		}
	}


	return 150;
}

int PermparMaking()
{
	ifstream permparInp(path + permparFile);
	ofstream permparOutp(permparDir);

	std::string s, word_pln, word_crn, word_ost;
	char sym = ',';
	bool thisLine = 0;

	if (permparInp.is_open() != 1)
	{
		Error(L"Error opening PERMPAR_INP.DAT", L"MainLinker -> PermparMaking");
		return 5;
	}
	else
	{
		if (complexity)
		{
			do
			{
				getline(permparInp, s);
				if (s.find(wordFind_1_mapkas) != -1)
				{
					if (mapkasHistory.size() != 0)
					{
						for (unsigned short num = 0; num < mapkasHistory.size(); num++)
						{
							permparOutp << mapkasHistory[num];
						}
					}

					permparOutp << position << sym << position << sym;

					for (int num = 1; num < localHistoryNumberCounter; num++)
					{
						string z = strHistory[num];
						if (!z.empty())
						{
							permparOutp << ConstRef(z) << ',';
						}
					}
					permparOutp << (100 - localHistoryNumberCounter - 1) << "*0," << endl;
					s.clear();
				}
				else
				{
					permparOutp << s << "\n";
				}
			} while (permparInp.eof() != 1);
		}
		else
		{ 
			string toHistory;
			toHistory += to_string(position);
			toHistory += ",";
			toHistory += to_string(position);
			toHistory += ",";
			for (int num = 1; num < localHistoryNumberCounter; num++)
			{
				string z = strHistory[num];
				if (!z.empty())
				{
					toHistory += to_string(ConstRef(z));
					toHistory+=sym;
				}
			}
			toHistory += to_string((100 - localHistoryNumberCounter - 1));
			toHistory += "*0,";
			toHistory += "\n";
			mapkasHistory.push_back(toHistory);
		}
	}

	permparInp.close();
	permparOutp.close();
	return 0;

}

void MatrixForming()
{
	ofstream OutputFile(OutputFileName);

	for (unsigned short i = 0; i != matrix.size(); i++)
	{
		OutputFile << matrix[i] << endl;
	}

	OutputFile.close();
}

void MatrixFilling()
{
	ifstream InputFile(path + InputFileName);
	
	string inputTextString,resultString;
	string cType;
	string toVector;
	

	if (InputFile.is_open() != 1)
	{
		Error(L"ERROR OPENING INPUT FILE", L"MainLinker -> WrittingFunction -> MATRIX");
	}
	else
	{
		// ���� ���� ��������, �� ��������� ���� �� ��������� ����� �����
		while (InputFile.eof() != 1)
		{
			getline(InputFile, inputTextString);
			inputTextString = ToUpperFunc(inputTextString);
			// ���� ��������� ������ "MINICORE_VMN"
			// �� �������� ��������� ���� �������
			if (inputTextString.find("MINICORE_VMN") == -1)
			{
				if (firstLaunch)
				matrix.push_back(inputTextString);
			}
			else
			{
				// �������� ����, ������� ���������� ����� ����� ��������� ����� �����
				firstLaunch = false;
				matrix.push_back(to_string(position) + "    MINICORE_VMN   \n");
				// ������� ��������� ������� - ������ EOK
				while (inputTextString.find(EOK) == -1)
				{
					getline(InputFile, inputTextString);
					inputTextString= ToUpperFunc(inputTextString);
					// ���� �� ����� ����� ���������:
					if (inputTextString.find(EOK) == -1)
					{
						// �� ����� ������ �������� ��������� �������
						for (unsigned int pos = 0; pos < inputTextString.size() + 1; pos++)
						{
							// ���� !������ � !\0 � !\n, ����� ����������� ������ �� ���� ������
							if ((char(inputTextString[pos]) != 32) 
								&& (char(inputTextString[pos]) != 0) 
								&& (char(inputTextString[pos]) != 10))
							{
								resultString += inputTextString[pos];
							}
							// ���� ����� �� !%, �����
							else
							{
								// �������, ������ �� ������, ���� ��� - �� � ��� ����� �����-�� �����
								if ((resultString.empty() != 1) && (resultString.find(EOK) == -1))
								{
									// ���������� ����� ����� �����, ���������� �� � ����� �������
									// � ������� ������
									int kk;
									string tk;
									tk.clear();
									// ������� ����� �������
									// � ��� �����
									tk += resultString[2];
									tk += resultString[3];
									{
										// TextStrFunc ��������� �������� �� ���������� - ���������� �� ����� ������� 
										// ������. ���� ����������, �� kk �������� �������� ����� �����.
										// ���� �� ���������� - ������������ ����� ���.
										if (resultString.find("TO") != -1)
										{
											cType = "TO";
											kk = TextStrFunc("TO", tk);
										}
										if (resultString.find("GX") != -1)
										{
											cType = "GX";
											kk = TextStrFunc("GX", tk);
										}
										if (resultString.find("CT") != -1)
										{
											cType = "CT";
											kk = TextStrFunc("CT", tk);
										}
										if (resultString.find("PL") != -1)
										{ 
											cType = "PL";
											kk = TextStrFunc("PL", tk);
										}
										if (resultString.find("CR") != -1)
										{
											cType = "CR";
											kk = TextStrFunc("CR", tk);
										}
									}
									if (kk == 150)
									{
										// ���� �������� �� ���� ��������� �������� ��� � �������
										kk = TextStrFunc(cType, "01");
									}

									if (resultString.find(EOK) == -1)
									{
										toVector+= "  " +to_string(kk) + " ";
									}
									resultString.clear();
								}
								// � ���� ������ �� ��� ���� ������ ������� ����� ���������, �� � ����������
								else
								{
									toVector+= " ";
								}
							}
						}
						matrix.push_back(toVector);
						toVector.clear();
					}
				}
			}
		}
	}
	InputFile.close();
}

void WrittingFunc()
{
	MatrixFilling();
	if (complexity)
	{
		firstLaunch = true;
		MatrixForming();
	}
}

void History(string conName)
{

	int myMatrixCount = 0;
	bool tFind = false;


	// ���� ����������
	for (unsigned int i = 0; i < strHistory.size(); i++)
	{
		if (conName == strHistory[i])
		{
			tFind = true;
		}
	}

	// ���� �� �������
	if (!tFind)
	{
		strHistory.push_back(conName);
		localHistoryNumberCounter++;
	}

}

void StructWork(string Str1, int param)
{
	int cpos = 0;
	int fse = 0;
	string st1_l , st2_l, st2Tmp, st1Len;
	int numLen = 0;
	unsigned int semicolon_pos = (Str1.find(";"));
	for (unsigned int pos = 0; pos < Str1.size(); pos++)
	{
		if ((Str1[pos] != ' ') && (Str1[pos] != ';') && (pos<semicolon_pos))
		{
			if (Str1[pos] == ',')
			{
				cpos = pos;
				continue;
			}
			if (cpos == 0)
			{
				st1_l += Str1[pos];
			}
			else
			{
				st2_l += Str1[pos];
			}
			if (Str1[pos] == ',')
			{
				cpos = pos;
			}
		}
	}
		// ����� �������
		// ���������� 
		// st1 � ��� ���������
		// st2 � ��� ���������
		{
			ntMxMain.push_back(st1_l);
			// ������ �� ���� �������
				// ���������� ����� ������ �����
				switch (param) {
				case 1:
					topStr_num.push_back(st1_l);
					topStr_type.push_back(st2_l);
					break;
				case 2:
					GXStr_num.push_back(st1_l);
					GXStr_type.push_back(st2_l);
					break;
				case 3:
					CTStr_num.push_back(st1_l);
					CTStr_type.push_back(st2_l);
					break;
				case 4:
					PLNStr_num.push_back(st1_l);
					PLNStr_type.push_back(st2_l);
					break;
				case 5:
					CRNStr_num.push_back(st1_l);
					CRNStr_type.push_back(st2_l);
					break;
				default:
				//	MessageBox(NULL, L"ERROR TYPE IN MXParam", L"ERROR", MB_OK);
					break;
				}
			}
			// ���� ��� ���� �������

	History(st2_l);
}

void Reset()
{
	newStr_1.clear();
	numHistory.clear();
	strHistory.clear();
	ntMxMain.clear();
	topStr_num.clear();
	GXStr_num.clear();
	CTStr_num.clear();
	PLNStr_num.clear();
	CRNStr_num.clear();
	topStr_type.clear();
	GXStr_type.clear();
	CTStr_type.clear();
	PLNStr_type.clear();
	CRNStr_type.clear();
	path.clear();
	ParametersFile.clear();

	localHistoryNumberCounter = 0;

	newStr_1.push_back("0");
	numHistory.push_back(0);
	strHistory.push_back("0");
	ntMxMain.push_back("0");
	topStr_num.push_back("0");
	GXStr_num.push_back("0");
	CTStr_num.push_back("0");
	PLNStr_num.push_back("0");
	CRNStr_num.push_back("0");
	topStr_type.push_back("0");
	GXStr_type.push_back("0");
	CTStr_type.push_back("0");
	PLNStr_type.push_back("0");
	CRNStr_type.push_back("0");
}


int ReadingInputData(int gln)
{
	ParametersFile = path + "bin/data/tests/MXParamSost/" + "MX_Param_" + to_string(gln) + ".dat";
	ifstream Inp_MX(ParametersFile);
	string rid, st1;
	

	if (Inp_MX.is_open() != 1)
	{
		Error(L"ERROR OPENING MXPARAM", L"MainLinker -> ReadingInputData");
		return 1;
	}
	else
	{
		// ���� ���� ������ �������
		while (!Inp_MX.eof())
		{
			// ���� ��� ������� "�����"
			getline(Inp_MX, rid);
			if (rid.find("IDENTICAL") != -1)
			{
				// ���� ������� ������� "IDENTICAL"
				// ���� �� ����� ���������� ��� ���� "IDENTICAL"
				while (rid.find("&&EI") == -1)
				{
					getline(Inp_MX, rid);
					// ���������� ������������ ��������
					if (!rid.empty())
					{
						// ���� case - "���������� ��� ������� - TOP, GX, CT, PLN, CRN" 
						// � ���������� �� ����� � StructWork
						int parameterName = -446;
						{
							if ((rid.find("TO") != -1))
							{
								parameterName = 1;
							}
							if ((rid.find("GX") != -1))
							{
								parameterName = 2;
							}
							if ((rid.find("CT") != -1))
							{
								parameterName = 3;
							}
							if ((rid.find("PLN") != -1) || (rid.find("P") != -1))
							{
								parameterName = 4;
							}
							if ((rid.find("CR") != -1))
							{
								if (rid.find("CEN") == -1)
								{
									parameterName = 5;
								}
							}
						}
						// Top - 1
						// GX  - 2
						// CT  - 3
						// PLN - 4
						// CRN - 5
						// �������� � �������
						StructWork(rid, parameterName);
					}
				}
			}

		}
	}

	return 0;
}

int FormatInputData(int calcNumStr)
{

	// �������� ALREADYEXISTS ��� ���������� ��������� �������;
	// ������ fill_file ����������, �� ������ ������ �������.
	ParametersFile = path + "bin/data/tests/MXParamSost/" + "MX_Param_" + to_string(calcNumStr) + ".dat";
	string aaI, aaO;
	vector<string> newStr;
	newStr.clear();
	ifstream MX(ParametersFile);
	unsigned int W = 50;
	bool endF = false;

	if (MX.is_open() != 1)
	{
		Error(L"NOT FIND MXPARAM", L"MainLinker -> FormatInputData");
		return 1;
	}
	else
	{
		do
		{
			{
				// ��������� ���������
				getline(MX, aaI);
				// ��������� � ������� �������
				aaO = ToUpperFunc(aaI);
				if ((!aaO.empty()) && (aaO.length() > 3))
				{
					if (!(aaO.find("PL") != -1)&&(!(aaO.find("CR")!=-1)))
					{
						newStr.push_back(aaO);
					}
					else
					{
						if ((aaO.find("PL01")!=-1) || (aaO.find("CR01")!=-1))
						{
							newStr.push_back(aaO);
						}
					}
				}
				// ���������� � ������ 
				
			}
			// ���� ��������� ����� "&EI" - ����� ������� �����
			if (aaO.find("EI") != -1)
			{
				// ��������� �� ����� ������
				//////////////////////////////////////////////////////newStr.push_back("\n");
				// ������ ������ ���� �� �������� "&&END2" 
				do
				{
					// �������
					getline(MX, aaI);
					aaO.clear();
					/*
					=0.0= ���� �� ������, ��:
					=0.1= ������� � ������� �������
					=1.1= ��������� �� ��� ��������� ��������
					=1.2= �������� ��������� ������ - �������� �� �������
					=2.1= �������� �� W
					*/

					// =0.0=
					if (aaI.size() > 0)
					{
						// =0.1=
						aaI = ToUpperFunc(aaI);
						// =1.1=
						// =1.2=

						{
							if (aaI.length() > W)
							{
								aaO += StrCutting(aaI, W);
							}
							else
							{
								aaO = aaI;
							}
						}
					}
					// ����� =NN.N=

					// � ����� ������, ���� ������ �� ������ ���������� � �����
					if (!aaO.empty())
					{
						newStr.push_back(aaO);
						newStr_1.push_back(aaO);
					}
					if (aaO.find("EOF") != -1)
					{
						endF = true;
					}
				} while (endF != 1);
			}
			aaO.clear();
			aaI.clear();
		} while (MX.eof() != 1);
	}

	MX.close();



	ofstream MX1(ParametersFile);

	for (unsigned int strNum = 0; strNum < newStr.size(); strNum++)
	{
		MX1 << newStr[strNum] << endl;
	}
	newStr.clear();
	MX1.close();

	return 0;
}

void FillMxFile(int gln)
{
	ParametersFile = path + "bin/data/tests/MXParamSost/" + "MX_Param_" + to_string(gln) + ".dat";
	string ParametersFile_w = ParametersFile;
	ifstream MxFile(ParametersFile);
	vector<string> fullMxFile;
	string tmp_str_fill_file_1;
	string tmp_str_fill_file_2;

	do
	{
		getline(MxFile, tmp_str_fill_file_1);
		if (tmp_str_fill_file_1.find("&EI")!=-1)
		{
			ParametersFile.clear();
			ParametersFile= Path_Finder("PermakPath") + "bin/ttmt/ttnt/tmp_gap.tmp";
			ifstream planeFile(ParametersFile);
			do
			{
				getline(planeFile, tmp_str_fill_file_2);
				fullMxFile.push_back(tmp_str_fill_file_2);
			} while (!planeFile.eof());
			planeFile.close();
		}
		fullMxFile.push_back(tmp_str_fill_file_1);
	} while (!MxFile.eof());
	MxFile.close();

	ofstream wParamFile(ParametersFile_w);
	for (unsigned int i = 0; i < fullMxFile.size(); i++)
	{
		wParamFile << fullMxFile[i] << endl;
	}

	wParamFile.close();
	ParametersFile_w.clear();
	ParametersFile.clear();
}

int GetParameters(int gln)
{
	int err = 0;
	
	err += FormatInputData(gln);
	FillMxFile(gln);
	err += ReadingInputData(gln);

	return err;
}

int MainLinker(int gln, bool main, unsigned short item)
{

	Reset();
	position = item+1;
	complexity = main;
	
	path += Path_Finder("PermakPath");
	calcNum = gln;
	calcNumStr = to_string(gln);
	int err = 0;


	err += GetParameters(gln);
	err += PermparMaking();
	WrittingFunc();

	if (main)
		matrix.clear();


	return err;
}

void ClearLinkerFunction()
{
	ntMxMain.clear();
	topStr_num.clear();
	GXStr_num.clear();
	CTStr_num.clear();
	PLNStr_num.clear();
	CRNStr_num.clear();
	topStr_type.clear();
	GXStr_type.clear();
	CTStr_type.clear();
	PLNStr_type.clear();
	CRNStr_type.clear();
	matrix.clear();
	mapkasHistory.clear();
	newStr_1.clear();
	numHistory.clear();
	strHistory.clear();
}



