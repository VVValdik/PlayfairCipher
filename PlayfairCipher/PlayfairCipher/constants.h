#pragma once

const size_t ROWS = 4;
const size_t COLS = 8;
const size_t ELEMENTS = ROWS * COLS;

const wchar_t ALPHABET_START_LETTER = L'À';
const wchar_t ALPHABET_END_LETTER = L'ß';

const wchar_t APLHABET_FIRST_LETTER = L'À';
const wchar_t ADDITIONAL_SYMBOL = L'Ú';

// for annealing algorithm
const float TEMPERATURE = 20.0;
const float TEMPERATURE_STEP = 0.2;
const size_t ITERATIONS = 10000;

// constants for encryption/decryption
const int ENCRYPTION_OFFSET = 1;
const int DECRYPTION_OFFSET = -1;