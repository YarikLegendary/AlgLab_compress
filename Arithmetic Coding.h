#pragma once
#include "funcs.h"

pair <map<unsigned char, double>, vector<unsigned char>> calcProbs(const vector<unsigned char>& data);
double encodeArithSimple(double lower, double upper, int index, const map<unsigned char, double>& probs,
	const vector<unsigned char>& symbols, const vector<unsigned char>& data);
double encodeArithmetic(const vector<unsigned char>& data);