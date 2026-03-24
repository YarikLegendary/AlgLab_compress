#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "funcs_img.h"
#include "funcs_text.h"
#include "compress.h"
#include "Huffman.h"
#include "Arithmetic Coding.h"
#include "BWT.h"
#include "LZ.h"

void compressor1() {


}

int oimg() {

	int width, height, channels;

	string filename = "rgbimage.jpg";

	unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &channels, 3);
	if (width < 1 || height < 1) return 1 ;

	if (!imageData) {
		cout << "Ошибка загрузки файла " << filename << endl;
		cout << "Причина: " << stbi_failure_reason() << endl;
		return 1;
	}
	vector<unsigned char> image(imageData, imageData + width * height * 3);
	stbi_image_free(imageData);
}

int main() {

	setlocale(LC_ALL, "Ru");

	ifstream f("Text.txt", ios::in);
	if (!f.is_open()) {
		cerr << "Error!"; return 1;
	}
	vector <unsigned char> data = readText(f);


	cout << "\nData size: " << data.size() << " byte";

	clock_t start = clock();

	auto encoded = encodeLZSS(data);
	
	clock_t end = clock();

	cout << "\nAfter LZSS:" << encoded.size() << " byte" << endl;

	auto decoded = decodeLZSS(encoded);

	cout << "Decodeed data size: " << decoded.size() << " byte" << endl;

	double k = (double)data.size() / (double)encoded.size();
	cout << "\nCoef: " << setprecision(2) << k << endl;

	double taken_time = double(end - start) / CLOCKS_PER_SEC;

	cout << "\Encoded Time: " << taken_time << " sec" << endl;

	return 0;
}