#include "funcs_img.h"
#include "funcs_text.h"
#include "compress.h"
#include "Huffman.h"
#include "Arithmetic Coding.h"
#include "BWT.h"
#include "LZ.h"

int main() {

	setlocale(LC_ALL, "Ru");
	
	string text = "abc$";

	cout << "Исходная строка: " << text;

	vector<unsigned char> data(text.begin(), text.end());

	vector<unsigned char> encoded = naiveBWT(data);

	vector<unsigned char> decoded = naiveiBWT(encoded);\

	return 0;
}


//////////////////////////////

//cout << "Открыть .PNG/JPG - 1, Открыть .txt - 2 , Открыть .RAW - 3. Выберите опцию: ";
//int option; cin >> option;

//if (option == 1) {

//	string filename1 = "TESTIMG4.png";
//	cout << "\nЗагружаемое изображение: " << filename1 << endl;

//	if (!processImage(filename1)) {
//		cout << "\nКакие то-неплоадки :(\n";
//		return 1;
//	};
//}
//else if (option == 2) {
//	string filename2 = "text.txt";
//	cout << "\nЗагружаемый текст: " << filename2 << endl;

//	if (!processText(filename2)) {
//		cout << "\nКакие то-неплоадки :(\n";
//		return 1;
//	};
//}
//else if (option == 3) {

//	string filename3 = "data.raw";
//	cout << "\nЗагружаемый файл: " << filename3 << endl;

//	printHEX(loadRaw(filename3));

//	cout << "\nDecompress? (Да - 1, Нет - 2): ";
//	int option; cin >> option;
//	if (option == 1) {
//		decodeRLE(loadRaw(filename3));
//	}

//}
//else cout << "\nВыбрано неверное значение, повоторите попытку\n";