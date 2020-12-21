#include "statistics.h"

void generate_quadgram_statistics(const wchar_t* quadgramm_path, const wchar_t* output_path)
{
	unordered_map<wstring, long> frequency_table;

	wchar_t alphabet[] = L"юабцдеефгхийклмнопярстужвьызшэщчъ";
	size_t alphabet_len = wcslen(alphabet);

	vector<wstring> keys;

	for (wchar_t i1 = 0; i1 < alphabet_len; i1++)
	{
		for (wchar_t i2 = 0; i2 < alphabet_len; i2++)
		{
			for (wchar_t i3 = 0; i3 < alphabet_len; i3++)
			{
				for (wchar_t i4 = 0; i4 < alphabet_len; i4++)
				{
					wstring key = L"";
					key += alphabet[i1];
					key += alphabet[i2];
					key += alphabet[i3];
					key += alphabet[i4];

					frequency_table[key] = 0;

					keys.push_back(key);
				}
			}
		}
	}
	wcout << "Size is " << keys.size() << endl;
	wifstream wif(quadgramm_path);
	wif.imbue(std::locale(locale::empty(), new codecvt_utf8<wchar_t>));

	wstring quadgram_key;
	long frequency;
	long long quadgram_count = 0;
	while (!wif.eof())
	{
		wif >> quadgram_key >> frequency;
		quadgram_count += frequency;
		frequency_table[quadgram_key] = frequency;
	}
	wif.close();

	wofstream wof(output_path);
	wof.imbue(std::locale(locale::empty(), new codecvt_utf8<wchar_t>));

	for (int i = 0; i < keys.size(); i++)
	{
		if (frequency_table[keys[i]] == 0)
		{
			wof << log(0.01 / (double)quadgram_count) << ",\n";
		}
		else
		{
			wof << log(frequency_table[keys[i]] / (double)quadgram_count) << ",\n";
		}
	}

	wof.close();
	wcout << "Max is " << quadgram_count << "\n\n";
}