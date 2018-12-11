#pragma once
#include <string>

using namespace std;

int GetSessionInfo(HWND hWnd_1);
void NtConfig();
double ReturnCornerGapSize(short tvs1, short tvs2, short tvs3);
void DefineKinf(short cycleNumber, short timeNumber);
double ReturnKinfValue(short tvs1, short tvs2, short tvs3);
int GetParam(string s, int num);

void GetTableParam(int table_num, int param, int inner_num, double value);
double ReturnTableParam(int table_num, int param, int inner_num, double value);
