#pragma once
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
using namespace std;

void saveData(const vector<unsigned char>& data);
vector<unsigned char> loadRaw(const string& filename);
void printHEX(const vector<unsigned char>& data);