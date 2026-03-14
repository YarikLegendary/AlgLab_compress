#include "LZ.h"
#include <cstdint>
const unsigned char END_MARKER = '$';

vector <uint8_t> encodeLZ77(const vector <unsigned char>& data, int MAX_WINDOW_SIZE = 255) {

	int data_size = data.size();
	vector <uint8_t> result;

	for (int pos = 0; pos < data_size;) {

		uint8_t best_offset = 0;
		uint8_t best_length = 0;
		unsigned char next_s;

		int search_start = max(0, pos - MAX_WINDOW_SIZE);
		int max_length = min(MAX_WINDOW_SIZE, data_size - pos);

		for (int i = search_start; i < pos ; i++) {

			int length = 0;

			while (length < max_length &&
				   pos + length < data_size &&
				   data[length + i] == data[pos + length])
			{
					length++;
			}
			if (length > best_length) {
				best_length = length;
				best_offset = pos - i;
			}
		}

		if (best_length >= 2) {
			if (pos + best_length < data_size)
				next_s = data[pos + best_length];
			else next_s = END_MARKER; // Конец данных
			
			result.push_back(best_offset);
			result.push_back(best_length);
			result.push_back(next_s);

			pos += best_length + 1;
		}
		else {
			result.push_back(0);
			result.push_back(0);
			result.push_back(data[pos]);

			pos++;
		}
	}
	return result;
}

vector <unsigned char> decodeLZ77(const vector <uint8_t>& data) {

	vector <unsigned char> result;

	for (int pos = 0; pos + 2 < data.size(); pos += 3) {

		uint8_t offset = data[pos];
		uint8_t length = data[pos + 1];
		unsigned char s = data[pos + 2];

		if (length >= 2) {

			int start = result.size() - offset;

			for (int i = 0; i < length; i++) {
				result.push_back(result[start + i]);
			}
			if (s != END_MARKER) {
				result.push_back(s);
			}
		}
		else result.push_back(s);
	}
	return result;
}

vector<uint8_t> encodeLZSS(const vector<uint8_t>& data, int MAX_WINDOW_SIZE = 255) {

	const int MIN_LEN = 2;

	int data_size = data.size();
	vector <uint8_t> result;

	for (int pos = 0; pos < data_size;) {

		uint8_t best_offset = 0;
		uint8_t best_length = 0;
		unsigned char next_s;

		int search_start = max(0, pos - MAX_WINDOW_SIZE);
		int max_length = min(MAX_WINDOW_SIZE, data_size - pos);

		for (int i = search_start; i < pos; i++) {

			int length = 0;

			while (length < max_length &&
				pos + length < data_size &&
				data[length + i] == data[pos + length])
			{
				length++;
			}
			if (length > best_length) {
				best_length = length;
				best_offset = pos - i;
			}
		}

		if (best_length >= MIN_LEN) {
			result.push_back(1); // Флаг если было повторение
			result.push_back(best_offset);
			result.push_back(best_length);
			pos += best_length;
		}
		else {
			result.push_back(0); // Флаг если не было
			result.push_back(data[pos]);
			pos++;
		}
	}
	return result;
}

vector <unsigned char> decodeLZSS(const vector <uint8_t>& data) {

	vector <unsigned char> result;

	for (int pos = 0; pos < data.size();) {
		
		uint8_t flag = data[pos++];

		if (flag == 0) {
			if (pos >= data.size()) break;
			result.push_back(data[pos++]);
		}
		else {

			if (pos + 1 >= data.size()) break;

			uint8_t offset = data[pos++];
			uint8_t length = data[pos++];

			int start = result.size() - offset;
			for (int i = 0; i < length; i++) {
				result.push_back(result[start + i]);
			}
		}
	}
	return result;
}

vector <info> encodeLZ78(const vector <unsigned char>& data) {

	map<string, int> dictionary;
	vector <info> result;

	string current = "";
	dictionary[current] = 0;
	int next_index = 1;

	for (unsigned char s : data) {

		string new_str = current + (char)s;

		if (dictionary.count(new_str)) {
			current = new_str;
		}
		else {

			result.push_back({ dictionary[current], s });
			dictionary[new_str] = next_index++;
			current = "";
		}
	}
	if (!current.empty()) {
		result.push_back({ dictionary[current], 0 });
	}

	return result;
}

vector <unsigned char> decodeLZ78(const vector <info>& data) {

	vector<unsigned char> result;

	vector<string> dictionary;

	dictionary.push_back("");

	for (const auto& p : data) {

		string str;
		
		if (p.index == 0) {

			str = string(1, p.next);
		}
		else str = dictionary[p.index] + (char)p.next;

		for (unsigned char s : str) {
			result.push_back(s);
		}
		if (p.next != 0) {
			dictionary.push_back(str);
		}
	}

	return result;
}