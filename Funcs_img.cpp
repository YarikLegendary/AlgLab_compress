#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "funcs_img.h"
#include "compress.h"

const char* color_Options() {

	cout << "\nВыберете цвет изображения: (1 - Black/White, 2 - Graysacle, 3 - RGB): ";
	int colorNum; cin >> colorNum; 

	if (colorNum == 3) return "RGB";
	else if (colorNum == 2) return "GS";
	else if (colorNum == 1) return "BW";

	return "Nun";
}

int view_Option() {

	cout << "\nВыберете отображение (1 - Цифры, 2 - Символы): ";
	int showNum; cin >> showNum;

	if (showNum == 1) return showNum;
	else if (showNum == 2) return showNum - 2;

	return -1;
}

bool printImage(const vector<unsigned char>& image, int height, int width, const char* color) {

	const char* charsGS = " .:-=+*#%@";
	const char* charsBW = "#.";

	int ViewOpt = view_Option();
	if (ViewOpt == -1) return false;

	cout << "\n=== ЗАГРУЖЕННЫЕ ДАННЫЕ ===\n" << endl;

	for (int y = 0; y < height; y++) {

		for (int x = 0; x < width; x++) {

			if (color == "GS") {
				unsigned char pixel = image[y * width + x];
				if (ViewOpt) {
					cout << (int)pixel << ' ';
				}
				else {
					int index = (pixel * 9) / 255;
					cout << charsGS[index];
				}
			}
			else if (color == "BW")  {
				unsigned char pixel = image[y * width + x];

				if (ViewOpt){
					cout << (int)pixel << ' ';
				}
				else {
					int index = pixel / 255;
					cout << charsBW[index];
				}
			}
			else {

				cout << "RGB ";

			}
		}
		cout << endl;
	}

	return true;
}

bool processImage(string& filename) {

	// Информейшен
	int width, height, channels;

	const char* color = color_Options();

	if (color == "Nun") return false;

	// Крутая функция из библиотеки stb_image
	unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &channels, (color == "RGB") ? 3 : 1);
	if (width < 1 || height < 1) return false;

	if (!imageData) {
		cout << "Ошибка загрузки файла " << filename << endl;
		cout << "Причина: " << stbi_failure_reason() << endl;
		return false;
	}

	// Копируем в вектор, так поудобнее, указываем два указателя на начало и конец, данные копируются
	vector<unsigned char> image(imageData, imageData + width * height * ((color == "RGB") ? 3 : 1));

	//Небольшая очистка
	stbi_image_free(imageData);

	// Выводим данные
	if(!printImage(image, height, width, color)) return false;
	printHEX(image);
	
	// Сохранение в raw если захочется
	cout << "\nCохранить изображение в raw? (Да - 1, Нет - 2): ";
	int option; cin >> option;
	if (option == 1) saveData(image);

	cout << "\nВыполнить RLE сжатие? (Да - 1, Нет - 2): ";
	cin >> option;

	if (option == 1) {
		vector<unsigned char> compressedImage = encodeRLE(image);
		cout << "\nСжатие выполнено" << endl;
		printHEX(compressedImage);

		cout << "\nCохранить изображение в raw? (Да - 1, Нет - 2): ";
		cin >> option;
		if (option == 1) saveData(compressedImage);

	}

	return true;
}
