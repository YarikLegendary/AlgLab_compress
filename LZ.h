#pragma once
#include "funcs.h"

struct info {
	int index;
	unsigned char next;
};

vector <uint8_t> encodeLZ77(const vector <unsigned char>& data);
vector <unsigned char> decodeLZ77(const vector <uint8_t>& data);
vector <uint8_t> encodeLZSS(const vector<uint8_t>& data);
vector <unsigned char> decodeLZSS(const vector <uint8_t>& data);
vector <info> encodeLZ78(const vector <unsigned char>& data);
vector <unsigned char> decodeLZ78(const vector <info>& data);
vector<int> encodeLZW(const vector<unsigned char>& data);
vector<unsigned char> decodeLZW(const vector<int>& codes);

