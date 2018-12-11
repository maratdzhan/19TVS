#pragma once
#include <string>

int KK_KQ_Extracting(int gln_l);


double GetKQKeff(bool write, bool getKeff, bool permak, bool clear, int returnAssamblyNumber, double acceptedValue);
void GetMaxDeviations(short tvs);
void GetMcuKkValues(short tvs, short tvel, double value);
void ClearPermakERs();

