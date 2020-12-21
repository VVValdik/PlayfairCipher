#include "text_transform.h"

wstring bigram_transform(const wstring &wstr)
{
	wstring new_text = L"";
	int index = 0;
	while (index < wstr.size())
	{
		wchar_t sym1 = wstr[index];
		index++;
		wchar_t sym2;

		if (index < wstr.size())
		{
			sym2 = wstr[index];
		}
		else
		{
			sym2 = ADDITIONAL_SYMBOL;
			new_text += sym1;
			new_text += sym2;
			break;
		}

		if (sym1 == sym2)
		{
			sym2 = ADDITIONAL_SYMBOL;
			index--;
		}
		index++;

		new_text += sym1;
		new_text += sym2;
	}

	return new_text;
}

void write_to_file(const wchar_t *file, const wchar_t* key, const wchar_t* text)
{
	wofstream wof(file);
	wof.imbue(std::locale(locale::empty(), new codecvt_utf8<wchar_t>));

	wof << key << L"\n" << text;

	wof.close();
}

wstring text_transform(const wchar_t *text_path)
{
	wifstream wif(text_path);
	wif.imbue(std::locale(locale::empty(), new codecvt_utf8<wchar_t>));

	wstring wstr;
	wstring full_wstr = L"";

	auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());
	while (!wif.eof())
	{
		wif >> wstr;
		std::replace(wstr.begin(), wstr.end(), L'¨', L'Å');
		std::replace(wstr.begin(), wstr.end(), L'¸', L'Å');
		wstr.erase(std::remove_if(wstr.begin(), wstr.end(), only_ru_letter), wstr.end());
		f.toupper(&wstr[0], &wstr[0] + wstr.size());

		full_wstr += wstr;
	}

	wif.close();

	return full_wstr;
}

bool only_ru_letter(const wchar_t c)
{
	if ((c >= L'À' && c <= L'ß') || (c >= L'à' && c <= L'ÿ')) return false;

	return true;
}

wstring get_text_to_encrypt(const wchar_t* file)
{
	wstring wstr = text_transform(file);
	wstring bigrams_wstr = bigram_transform(wstr);

	return bigrams_wstr;
}