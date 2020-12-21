#pragma once
#include <unordered_map>
#include <vector>
#include <iostream>
#include <codecvt>
#include <io.h>
#include <fcntl.h>
#include <fstream>

using namespace std;

void generate_quadgram_statistics(const wchar_t* quadgramm_path, const wchar_t* output_path);