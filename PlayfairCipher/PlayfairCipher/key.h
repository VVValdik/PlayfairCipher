#pragma once
#include "constants.h"
#include <ctime>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <io.h>
#include <fcntl.h>

using namespace std;

// Randomly chenge key: swap symbols, rows, cols, etc.
void change_key(wchar_t *key);

// Swap two random symbols in key
void swap_symbols(wchar_t *key);

// Swap two random rows in key
void swap_rows(wchar_t *key);

// Swap two random columns in key
void swap_cols(wchar_t *key);

// Reverse all rows in key
void reverse_rows(wchar_t *key);

// Reverse all columns in key
void reverse_cols(wchar_t *key);

// Reverse full key
void reverse_key(wchar_t *key);

// Print key on console as matrix ROWS x COLS
void print_key(const wchar_t *key);

void shuffle_key(wchar_t *key);

// Generate key using keyword from file
void generate_key(const wchar_t* fname, const wchar_t* alphabet, wchar_t *cipher_key);