#pragma once
#include <string>
#include "constants.h"
#include <fstream>
#include <iostream>
#include <codecvt>
#include <io.h>
#include <fcntl.h>
#include <algorithm>

using namespace std;

// function to select only russian letters from text
bool only_ru_letter(const wchar_t c);

/*
Function to delete all useless symbols from text like spaces and punctuation marks
and replaces unused letters
*/
wstring text_transform(const wchar_t *text_path);

/*
Break the text into bigrams. If symbols in bigramms are equals, or bigramm has only one
symbol, added ADDITIONAL_SYMBOL
*/
wstring bigram_transform(const wstring &wstr);

void write_to_file(const wchar_t *file, const wchar_t* key, const wchar_t* text);

wstring get_text_to_encrypt(const wchar_t* file);