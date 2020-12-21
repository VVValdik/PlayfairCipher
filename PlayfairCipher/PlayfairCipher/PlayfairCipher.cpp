#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>
#include <codecvt>
#include <io.h>
#include <fcntl.h>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include "quad_stats_ru.h"
#include "key.h"
#include "constants.h"
#include "text_transform.h"

using namespace std;

/*
Calculating the fitness of text to real text.
The higher the value, the more the text matches the real one
*/
double calculate_fitness(const wchar_t *text, const size_t text_len);

// Function to encrypt/decrypt text with a given key
void playfair_cipher(const wchar_t *text, const size_t text_len,
	const wchar_t *key, wchar_t *output, const int OFFSET);

/*
Annealing algorithm
Searching best key to decrypt given text
*/
float annealing_algorithm(const wchar_t *text, const size_t text_len, wchar_t *best_key)
{
	wchar_t *decrypted_text = new wchar_t[text_len + 1];

	double fitness = 0;
	double max_fitness = 0;
	double best_fitness = 0;

	wchar_t current_key[ELEMENTS + 1];
	wchar_t max_key[ELEMENTS + 1];

	wcscpy(current_key, best_key);
	wcscpy(max_key, best_key);

	playfair_cipher(text, text_len, max_key, decrypted_text, DECRYPTION_OFFSET);
	max_fitness = calculate_fitness(decrypted_text, text_len);
	best_fitness = max_fitness;

	for (float t = TEMPERATURE; t >= 0; t -= TEMPERATURE_STEP)
	{
		for (int i = 0; i < ITERATIONS; i++)
		{
			wcscpy(current_key, max_key);
			change_key(current_key);

			playfair_cipher(text, text_len, current_key, decrypted_text, DECRYPTION_OFFSET);
			fitness = calculate_fitness(decrypted_text, text_len);

			double fitness_difference = fitness - max_fitness;

			if (fitness_difference >= 0)
			{
				max_fitness = fitness;
				wcscpy(max_key, current_key);
			}
			else if (t > 0)
			{
				double probability = exp(fitness_difference / t);

				if (probability > 1.0 * rand() / RAND_MAX)
				{
					max_fitness = fitness;
					wcscpy(max_key, current_key);
				}
			}

			if (max_fitness > best_fitness)
			{
				best_fitness = max_fitness;
				wcscpy(best_key, max_key);
			}
		}
	}

	delete decrypted_text;
	return best_fitness;
}

void main()
{
	srand(time(NULL));
	_setmode(_fileno(stdout), _O_U16TEXT);

	// use this function to generate statistics for other languages
	//generate_quadgram_statistics();

	const wchar_t keyword_path[] = L"keyword.txt";
	const wchar_t source_text_path[] = L"source.txt";
	const wchar_t encrypted_path[] = L"encrypted.txt";
	const wchar_t decrypted_path[] = L"decrypted.txt";

	wchar_t alphabet[] = L"АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

	wchar_t cipher_key[ELEMENTS + 1];
	generate_key(keyword_path, alphabet, cipher_key);

	wchar_t start_key[ELEMENTS + 1];
	wcscpy(start_key, alphabet);
	shuffle_key(start_key);

	wstring wstr_to_encrypt = get_text_to_encrypt(source_text_path);
	wchar_t *text_to_encrypt = new wchar_t[wstr_to_encrypt.size() + 1];
	wcscpy(text_to_encrypt, wstr_to_encrypt.c_str());

	size_t text_len = wcslen(text_to_encrypt);
	wchar_t *encrypted_text = new wchar_t[text_len + 1];
	wcscpy(encrypted_text, text_to_encrypt);
	playfair_cipher(text_to_encrypt, text_len, cipher_key, encrypted_text, ENCRYPTION_OFFSET);

	wcout << L"Ключ шифра:\n";
	print_key(cipher_key);
	write_to_file(encrypted_path, cipher_key, encrypted_text);

	wcout << L"\nНачальный ключ:\n";
	print_key(start_key);
	wcout << L"\nСчитано с файла: " << text_to_encrypt << L"\n";
	wcout << L"\nЗашифрованный текст: " << encrypted_text << L"\n";
	wcout << L"Длина текста: " << text_len << L"\n\n";

	double fitness = 0;
	double max_fitness = -99999999;
	wchar_t *decrypted_text = new wchar_t[text_len + 1];

	int iteration = 0;
	while (true)
	{
		wcout << L"Итерация " << iteration << L"\n";

		auto start = chrono::high_resolution_clock::now();

		fitness = annealing_algorithm(encrypted_text, text_len, start_key);

		auto stop = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
		wcout << L"Время выполнения алгоритма имитации отжига: " << duration.count() << L" мс\n";

		if (fitness > max_fitness)
		{
			max_fitness = fitness;
			playfair_cipher(encrypted_text, text_len, start_key, decrypted_text, DECRYPTION_OFFSET);

			wcout << L"Новый найденный ключ:\n";
			print_key(start_key);
			wcout << L"Расшифрованный текст: " << decrypted_text << L"\n\n";

			write_to_file(decrypted_path, start_key, decrypted_text);
		}

		iteration++;
	}
}

double calculate_fitness(const wchar_t *text, const size_t text_len)
{
	double fitness = 0;

	for (int i = 0; i < text_len - 3; i++)
	{
		int sym1 = text[i] - APLHABET_FIRST_LETTER;
		int sym2 = text[i + 1] - APLHABET_FIRST_LETTER;
		int sym3 = text[i + 2] - APLHABET_FIRST_LETTER;
		int sym4 = text[i + 3] - APLHABET_FIRST_LETTER;

		// буква Ё находится отдельно от алфавита, увеличиваем значение на 1
		if (sym1 > 5) sym1++;
		if (sym2 > 5) sym2++;
		if (sym3 > 5) sym3++;
		if (sym4 > 5) sym4++;

		fitness += qgram_ru[35937 * sym1 + 1089 * sym2 + 33 * sym3 + sym4];
	}

	return fitness;
}

void playfair_cipher(const wchar_t *text, const size_t text_len,
	const wchar_t *key, wchar_t *output, const int OFFSET)
{
	for (int i = 0; i < text_len; i += 2)
	{
		wchar_t sym1 = text[i];
		wchar_t sym2 = text[i + 1];

		int sym1_index = wcschr(key, sym1) - key;
		int sym2_index = wcschr(key, sym2) - key;

		int sym1i = sym1_index / COLS;
		int sym1j = sym1_index % COLS;

		int sym2i = sym2_index / COLS;
		int sym2j = sym2_index % COLS;

		if (sym1i == sym2i)
		{
			output[i] = key[sym1i * COLS + ((sym1j + OFFSET + COLS) % COLS)];
			output[i + 1] = key[sym2i * COLS + ((sym2j + OFFSET + COLS) % COLS)];
		}
		else if (sym1j == sym2j)
		{
			output[i] = key[((sym1i + OFFSET + ROWS) % ROWS) * COLS + sym1j];
			output[i + 1] = key[((sym2i + OFFSET + ROWS) % ROWS) * COLS + sym2j];
		}
		else
		{
			output[i] = key[sym1i * COLS + sym2j];
			output[i + 1] = key[sym2i * COLS + sym1j];
		}
	}

	output[text_len] = L'\0';
}