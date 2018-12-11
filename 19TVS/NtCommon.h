#pragma once

void CreateMainPath();
void TakeParameter(std::string value, int valueType, std::string parameterName, int index1, int index2, int index3, bool is_path);
std::string ReturnMainPath(std::string inputString);

void TakeKeff(std::string value);
void WriteKeff();
void TakeKq(std::string state, double value);
void WriteKq(bool inRow); 