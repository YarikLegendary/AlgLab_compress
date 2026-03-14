#pragma once
#include "funcs.h"

bool printImage(const vector<unsigned char>& image, int height, int width, const char* color);

bool processImage(string& filename);

const char* color_Options();

int view_Option();
