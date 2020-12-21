#include "key.h"

void change_key(wchar_t *key)
{
	int variant = rand() % 50;

	switch (variant)
	{
	case 0:
		swap_rows(key);
		break;
	case 1:
		swap_cols(key);
		break;
	case 2:
		reverse_key(key);
		break;
	case 3:
		reverse_rows(key);
		break;
	case 4:
		reverse_cols(key);
		break;
	default:
		swap_symbols(key);
		break;
	}
}

void swap_symbols(wchar_t *key)
{
	int index1 = rand() % ELEMENTS;
	int index2 = rand() % ELEMENTS;

	swap(key[index1], key[index2]);
}

void swap_rows(wchar_t *key)
{
	int row1 = rand() % ROWS;
	int row2 = rand() % ROWS;

	for (int j = 0; j < COLS; j++)
	{
		int index1 = row1 * COLS + j;
		int index2 = row2 * COLS + j;

		swap(key[index1], key[index2]);
	}
}

void swap_cols(wchar_t *key)
{
	int col1 = rand() % COLS;
	int col2 = rand() % COLS;

	for (int i = 0; i < ROWS; i++)
	{
		int index1 = i * COLS + col1;
		int index2 = i * COLS + col2;

		swap(key[index1], key[index2]);
	}
}

void reverse_rows(wchar_t *key)
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS / 2; j++)
		{
			int index1 = i * COLS + j;
			int index2 = i * COLS + (COLS - 1 - j);

			swap(key[index1], key[index2]);
		}
	}
}

void reverse_cols(wchar_t *key)
{
	for (int j = 0; j < COLS; j++)
	{
		for (int i = 0; i < ROWS / 2; i++)
		{
			int index1 = i * COLS + j;
			int index2 = (ROWS - 1 - i) * COLS + j;

			swap(key[index1], key[index2]);
		}
	}
}

void reverse_key(wchar_t *key)
{
	for (int i = 0; i < ELEMENTS / 2; i++)
	{
		int indexToSwap = ELEMENTS - 1 - i;
		swap(key[i], key[indexToSwap]);
	}
}

void print_key(const wchar_t *key)
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			wcout << key[i * COLS + j] << L" ";
		}
		wcout << L"\n";
	}
}

void shuffle_key(wchar_t *key)
{
	for (int i = 0; i < ELEMENTS; i++)
	{
		int rand_index = rand() % ELEMENTS;

		swap(key[i], key[rand_index]);
	}
}

void generate_key(const wchar_t* fname, const wchar_t* alphabet, wchar_t *cipher_key)
{
	wifstream wif(fname);
	wif.imbue(std::locale(locale::empty(), new codecvt_utf8<wchar_t>));

	wstring key, temp;
	wif >> temp;

	for (int i = 0; i < temp.size(); i++)
	{
		if (key.find(temp[i]) == wstring::npos)
		{
			key += temp[i];
		}
	}

	for (int i = 0; i < ELEMENTS; i++)
	{
		if (key.find(alphabet[i]) == wstring::npos)
		{
			key += alphabet[i];
		}
	}

	wcscpy(cipher_key, key.c_str());
}