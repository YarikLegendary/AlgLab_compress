#pragma once
#include "funcs.h"

struct Node;

vector <unsigned char> encodeHuffman(const vector<unsigned char>& data);
void makeCodes(Node* root, string code, map<unsigned char, string>& codes);
vector <unsigned char> decodeHuffman(const vector<unsigned char>& compressed);
Node* buildTree(const map<unsigned char, int>& freqs);
vector<unsigned char> packBits(const string& bits);
string unpackBits(const vector<unsigned char>& packed, int totalBits);
vector<unsigned char> packModel(const map<unsigned char, int>& freqs);
map<unsigned char, int> unpackModel(const vector<unsigned char>& packedModel);