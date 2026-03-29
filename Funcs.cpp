#include "funcs.h"

void saveData(const vector<unsigned char>& data, string filename) {

	ofstream MyData(filename, std::ios::binary | std::ios::trunc);
	if (MyData.is_open()) {
		MyData.write(reinterpret_cast<const char*>(data.data()), data.size());
		if (MyData.good()) {
			cout << "\nДанные сохранены успешно\n";
		}
		MyData.close();
	}
	else cout << "\nОшибка создания файла!\n";
}

vector<unsigned char> loadRaw(const string& filename) {

	vector<unsigned char> data;

	ifstream file(filename, ios::binary);

	if (!file.is_open()) {
		cout << "Ошибка открытия файла: " << filename << endl;
		return data;
	}

	file.seekg(0, ios::end);
	unsigned size = file.tellg();
	file.seekg(0, ios::beg);

	data.resize(size);
	file.read(reinterpret_cast<char*>(data.data()), size);

	return data;
}

void printHEX(const vector<unsigned char>& data) {

	cout << "\nБайты (HEX): ";
	for (unsigned char symbol : data) {
		cout << "0x" << hex << setw(2) << setfill('0')
			<< (int)symbol << " ";
	}
	cout << dec << endl;


}

vector <unsigned char> readText(ifstream& file) {

	vector <unsigned char> textData;
	char sym;

	while (file.get(sym)) {
		textData.push_back(sym);
	}
	return textData;
}
