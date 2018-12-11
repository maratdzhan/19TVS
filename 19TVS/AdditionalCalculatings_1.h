#pragma once
#include <string>



void KrCalculatings(int gln);
std::wstring ReturnTableValues(bool header, int paramNumber, int tvs);
void GetStateName(int gln);

void Extraction1209(double value, std::string description, bool center, bool kq);
void Writting1209();