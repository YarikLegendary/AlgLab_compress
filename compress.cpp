#include "compress.h"

vector <unsigned char> encodeRLE(const vector <unsigned char>& data) {

	vector <unsigned char> CompressedText;

	for (int i = 0; i < data.size();) {

		// Определим тип последовательности
		int run_length = 1;
		while (i + run_length < data.size() && data[i] == data[i + run_length] && run_length < 127) {
			run_length++;
		}

		// Повторяющаяся последовательность
		if (run_length > 2) {
			CompressedText.push_back(static_cast<unsigned char>(run_length));
			CompressedText.push_back(data[i]);
			i += run_length;
		}
		else { // Неповторяющаяся последовательность

			int start = i;
			int nonrun_length = 0;

			while (i + nonrun_length < data.size() && nonrun_length < 127) {
				if (i + nonrun_length + 2 < data.size() &&
					data[i + nonrun_length] == data[i + nonrun_length + 1] &&
					data[i + nonrun_length] == data[i + nonrun_length + 2]) {
					break;
				}
				nonrun_length++;
			}

			CompressedText.push_back(static_cast<unsigned char>(0x80 | nonrun_length));

			for (int k = start; k < start + nonrun_length; k++) {
				CompressedText.push_back(data[k]);
			}
			i = start + nonrun_length;
		}
	}
	return CompressedText;
}

vector <unsigned char> decodeRLE(const vector<unsigned char>& data) {

	vector <unsigned char> newData;

	for (int i = 0; i < data.size(); ) {

		if (data[i] & 0x80) { 

			int run_length = data[i] - 0x80;

			if (i + 1 + run_length > data.size()) {
				break;  // Проверка границ
			}

			for (int k = 0; k < run_length; k++) {
				newData.push_back(data[i + 1 + k]);
			}
			i += 1 + run_length;
		}
		else {

			int quantity = data[i];

			if (i + 2 > data.size()) {
				break;
			}// Проверка границ

			unsigned char symbol = data[i + 1];

			for (int k = 0; k < quantity; k++) {
				newData.push_back(symbol);
			}
			i += 2;
		}
	}

	return newData;
}

vector<unsigned char> MTF(const vector<unsigned char>& data) {

	list<unsigned char> alphabet;

	for (int i = 0; i < 256; i++) {
		alphabet.push_back(static_cast<unsigned char>(i));
	}

	vector<unsigned char> encodeResult;
	encodeResult.reserve(data.size());

	for (int i = 0; i < data.size();i++) {

		auto ptr = find(alphabet.begin(), alphabet.end(), data[i]);
		int pos = distance(alphabet.begin(), ptr);

		encodeResult.push_back(static_cast<unsigned char>(pos));

		alphabet.erase(ptr);
		alphabet.push_front(data[i]);
	}

	return encodeResult;
}

vector<unsigned char> decodeMTF(const vector<unsigned char>& data) {

	list<unsigned char> alphabet;

	for (int i = 0; i < 256; i++) {
		alphabet.push_back(static_cast<unsigned char>(i));
	}
	vector<unsigned char> decodeResult;
	decodeResult.reserve(data.size());

	for (int i = 0; i < data.size();i++) {

		auto ptr = alphabet.begin();
		int pos = static_cast<int>(data[i]);

		advance(ptr, pos);

		unsigned char sym = *ptr;
		decodeResult.push_back(sym);

		alphabet.erase(ptr);
		alphabet.push_front(sym);
	}

	return decodeResult;
}

vector <unsigned char> encodeRLERGB(const vector <unsigned char>& data) {

	vector <unsigned char> CompressedText;

	for (int i = 0; i < data.size();) {

		// Определим тип последовательности (сравниваем по 3 байта)
		int length = 1;
		while (i + length * 3 < data.size() &&
			data[i] == data[i + length * 3] &&
			data[i + 1] == data[i + length * 3 + 1] &&
			data[i + 2] == data[i + length * 3 + 2]) {
			length++;
		}

		// Повторяющаяся последовательность
		if (length > 1) {
			CompressedText.push_back(static_cast<unsigned char>(length));
			CompressedText.push_back(data[i]);     // R
			CompressedText.push_back(data[i + 1]);   // G
			CompressedText.push_back(data[i + 2]);   // B
			i += length * 3;
		}
		else { // Неповторяющаяся последовательность
			int start = i;
			int nonrun_length = 0;
			while (i + 3 < data.size() && nonrun_length < 127) {
				if (i + 6 < data.size() &&
					data[i] == data[i + 3] && data[i + 1] == data[i + 4] && data[i + 2] == data[i + 5]) {
					break;
				}
				i += 3;
				nonrun_length++;
			}
			if (i < data.size() && nonrun_length < 127) {
				nonrun_length++;
				i += 3;
			}
			//length = (i - start) / 3 + 1;
			CompressedText.push_back(static_cast<unsigned char>(0x80 | nonrun_length));

			for (int k = start; k < start + nonrun_length * 3; k += 3) {
				CompressedText.push_back(data[k]);     // R
				CompressedText.push_back(data[k + 1]);   // G
				CompressedText.push_back(data[k + 2]);   // B
			}
		}
	}
	return CompressedText;
}

vector <unsigned char> decodeRLERGB(const vector<unsigned char>& data) {

	vector <unsigned char> newData;

	for (int i = 0; i < data.size(); ) {

		if (data[i] & 0x80) {

			int length = data[i] - 0x80;

			if (i + 1 + length * 3 > data.size()) {
				break;
			}

			for (int k = 0; k < length; k++) {
				newData.push_back(data[i + 1 + k * 3]);     // R
				newData.push_back(data[i + 2 + k * 3]);     // G
				newData.push_back(data[i + 3 + k * 3]);     // B
			}
			i += 1 + length * 3;
		}
		else {
			int quantity = data[i];

			if (i + 3 > data.size()) {
				break;
			}

			unsigned char r = data[i + 1];
			unsigned char g = data[i + 2];
			unsigned char b = data[i + 3];

			for (int k = 0; k < quantity; k++) {
				newData.push_back(r);
				newData.push_back(g);
				newData.push_back(b);
			}
			i += 4;
		}
	}

	return newData;
}
