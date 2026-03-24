#pragma once
#include "funcs.h"

struct Node;
struct MyCompare;

Node* buildHuffmanTree(const vector<unsigned char>& data);
void buildCode(Node* root, vector<bool>& code, map<unsigned char, vector<bool>>& table);
vector <unsigned char> encodeHuf(const vector<unsigned char>& data);
vector <unsigned char> decodeHuf(const vector<unsigned char>& compressed);