#pragma once
#include "funcs.h"

vector<unsigned char> BWT(const vector<unsigned char>& data);
bool compareShifts(int a, int b, const vector<unsigned char>& data);
vector<unsigned char> naiveBWT(const vector<unsigned char>& data);
vector<unsigned char> naiveiBWT(const vector<unsigned char>& data);
void printMatrix(vector<vector<unsigned char>> matrix);