#pragma once
#include "funcs.h"

vector<unsigned char> BWT(const vector<unsigned char>& data);
vector<unsigned char> iBWT(const vector<unsigned char>& last_column);
bool compareShifts(int a, int b, const vector<unsigned char>& data);
vector<unsigned char> naiveBWT(const vector<unsigned char>& data);
vector<unsigned char> naiveiBWT(const vector<unsigned char>& data);
vector<unsigned char> blocksBWT(const vector<unsigned char>& data);
void printMatrix(vector<vector<unsigned char>> matrix);
vector<unsigned char> blocksiBWT(const vector<unsigned char>& data);
vector<int32_t> buildSuffArray(const vector <unsigned char>& data);
bool compareSuffs(int a, int b, const vector <unsigned char>& data);
vector<unsigned char> suffixArrayToBWT(const vector<int32_t>& suffix_array, const vector<unsigned char>& data);
