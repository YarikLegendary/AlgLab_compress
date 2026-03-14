#pragma once
#include "funcs.h"

vector <unsigned char> encodeRLE(vector <unsigned char>& data);
vector <unsigned char> decodeRLE(const vector<unsigned char>& data);
double entropy(const vector<unsigned char>& data, int quantBlocks);
vector<int> MTF(const vector<unsigned char>& data);
vector<unsigned char> decodeMTF(const vector<int>& data);