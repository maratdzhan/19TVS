#pragma once


void FirstLaunchNumeration();
int FRNumber(int tvs, int tvel, int type);
void newMcuMatrixNumeration();
double RetCoordinates(int tvs, bool x, int gstate);
double FakeTvel(int tvs, int tvel);
void FakeTvelDef(bool clearing);
int ActuallyTvel(int tvs, int tvel, bool finding);
void ClearingInitial();
void Clearing_Temporary(); 