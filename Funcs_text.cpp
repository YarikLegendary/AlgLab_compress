#include "funcs_text.h"
#include "compress.h"

bool processText(string& filename) {

	ifstream file(filename, ios::in);
	if (!file.is_open()) {
		cout << "Ошибка открытя файла!";
		return false;
	}

	vector <unsigned char> text = readText(file);

	if (text.empty()) {
		cout << "Файл пуст!" << endl;
		return false;
	}

	cout << "\nCохранить текст в raw? (Да - 1, Нет - 2): ";
	int option; cin >> option;
	if (option == 1) saveData(text);

	cout << "\nВыполнить RLE сжатие? (Да - 1, Нет - 2): ";
	cin >> option;

	if (option == 1) {
		vector<unsigned char> compressedText = encodeRLE(text);
		cout << "\nСжатие выполнено" << endl;
		printHEX(compressedText);

		cout << "\nСохранить сжатые данные? (Да - 1, Нет - 2): ";
		cin >> option;
		if (option == 1) {
			saveData(compressedText);
		}
	}

	return true;
}
vector <unsigned char> readText(ifstream& file) {

	vector <unsigned char> textData;
	char sym;

	while (file.get(sym)) {
		textData.push_back(sym);
	}

	//cout << "\nДанные: ";
	//for (auto sym : textData) {
	//	cout << sym;
	//}
	//cout << "\n";
	
	//printHEX(textData);

	return textData;
}

