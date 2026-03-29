#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "funcs_img.h"
#include "compress.h"
#include "Huffman.h"
#include "Arithmetic Coding.h"
#include "BWT.h"
#include "LZ.h"

// HA
void compressor1(const vector<unsigned char>& data) {

	vector <unsigned char> encoded = encodeHuf(data);

	cout << "\nAfter Huffman: " << encoded.size() << " byte" << endl;

	vector <unsigned char> decoded = decodeHuf(encoded);

	cout << "\nAfter decoding: " << decoded.size() << " byte" << endl;

	if (data.size() == decoded.size()) cout << "\nSame size! "; else cout << "\nFAIL\n";

	if (data != decoded) {
		cout << " !Wrong data!\n";
	}
	else cout << "Decoded data is correct!\n";

	double k = (double)data.size() / (double)encoded.size();
	cout << "\nCoef: " << setprecision(2) << k << endl;
} 

// RLE
void compressor2(const vector<unsigned char>& data) {

	vector <unsigned char> encoded = encodeRLE(data);

	cout << "\nAfter RLE: " << encoded.size() << " byte" << endl;

	vector <unsigned char> decoded = decodeRLE(encoded);

	cout << "\nAfter decoding: " << decoded.size() << " byte" << endl;

	if (data.size() == decoded.size()) cout << "\nSame size! "; else cout << "\nFail";

	if (data != decoded) {
		cout << " !Wrong data!\n";
	}
	else cout << "Decoded data is correct!\n";

	double k = (double)data.size() / (double)encoded.size();
	cout << "\nCoef: " << setprecision(2) << k << endl;
}

// BWT + RLE
void compressor3(const vector<unsigned char>& data) {

	vector <unsigned char> encodedBWT = blocksBWT(data);
	vector <unsigned char> encodedRLE = encodeRLE(encodedBWT);
	cout << "\nAfter BWT+RLE: " << encodedRLE.size() << " byte" << endl;

	vector <unsigned char> decodedRLE = decodeRLE(encodedRLE);
	vector <unsigned char> decodedBWT = blocksiBWT(decodedRLE);

	cout << "\nAfter decoding: " << decodedBWT.size() << " byte" << endl;

	if (data.size() == decodedBWT.size()) cout << "\nSame size! "; else cout << "\nFAIL! ";

	if (data != decodedBWT) {
		cout << " !Wrong data!\n"; 
	}
	else cout << "Decoded data is correct!\n";

	double k = (double)data.size() / (double)encodedRLE.size();
	cout << "\nCoef: " << setprecision(2) << k << endl;

}

// BWT + MTF + HA
void compressor4(const vector<unsigned char>& data) {

	vector <unsigned char> encodedBWT = blocksBWT(data);
	vector <unsigned char> encodedMTF = MTF(encodedBWT);
	vector <unsigned char> encodedHuf = encodeHuf(encodedMTF);
	cout << "\nAfter BWT + MTF + HA: " << encodedHuf.size() << " byte" << endl;

	vector <unsigned char> decodedHuf = decodeHuf(encodedHuf);
	vector <unsigned char> decodedMTF = decodeMTF(decodedHuf);
	vector <unsigned char> decodedBWT = blocksiBWT(decodedMTF);

	cout << "\nAfter decoding: " << decodedBWT.size() << " byte" << endl;

	if (data.size() == decodedBWT.size()) cout << "\nSame size! "; else cout << "\nFAIL! ";
	if (data != decodedBWT) {
		cout << " !Wrong data!\n";
	}
	else cout << "Decoded data is correct!\n";

	double k = (double)data.size() / (double)encodedHuf.size();
	cout << "\nCoef: " << setprecision(2) << k << endl;

}

// BWT + MTF + RLE + HA

void compressor5(const vector<unsigned char>& data) {

	vector <unsigned char> encodedBWT = blocksBWT(data);
	vector <unsigned char> encodedMTF = MTF(encodedBWT);
	vector <unsigned char> encodedRLE = encodeRLE(encodedMTF);
	vector <unsigned char> encodedHuf = encodeHuf(encodedRLE);
	cout << "\nAfter BWT + MTF + RLE + HA: " << encodedHuf.size() << " byte" << endl;

	vector <unsigned char> decodedHuf = decodeHuf(encodedHuf);
	vector <unsigned char> decodedRLE = decodeRLE(decodedHuf);
	vector <unsigned char> decodedMTF = decodeMTF(decodedRLE);
	vector <unsigned char> decodedBWT = blocksiBWT(decodedMTF);

	cout << "\nAfter decoding: " << decodedBWT.size() << " byte" << endl;

	if (data.size() == decodedBWT.size()) cout << "\nSame size! "; else cout << "\nFAIL! ";
	if (data != decodedBWT) {
		cout << " !Wrong data!\n";
	}
	else cout << "Decoded data is correct!\n";

	double k = (double)data.size() / (double)encodedHuf.size();
	cout << "\nCoef: " << setprecision(2) << k << endl;

}

// LZSS

void compressor6(const vector<unsigned char>& data) {

	vector <unsigned char> encoded = encodeLZSS(data);

	cout << "\nAfter LZSS: " << encoded.size() << " byte" << endl;

	vector <unsigned char> decoded = decodeLZSS(encoded);

	cout << "\nAfter decoding: " << decoded.size() << " byte" << endl;

	if (data.size() == decoded.size()) cout << "\nSame size\n"; else cout << "\nFAIL\n";

	for (int i = 0; i < data.size(); i++) {
		if (data[i] != decoded[i]) { cout << "\n!WRONG DATA DECODED!\n"; break; }
	}

	double k = (double)data.size() / (double)encoded.size();
	cout << "\nCoef: " << setprecision(2) << k << endl;
}

// LZSS + HA 

void compressor7(const vector<unsigned char>& data) {

	vector <unsigned char> encodedLZSS = encodeLZSS(data);
	vector <unsigned char> encodedHuf = encodeHuf(encodedLZSS);

	cout << "\nAfter LZSS + HA: " << encodedHuf.size() << " byte" << endl;

	vector <unsigned char> decodedHuf = decodeHuf(encodedHuf);
	vector <unsigned char> decodedLZSS = decodeLZSS(decodedHuf);

	cout << "\nAfter decoding: " << decodedLZSS.size() << " byte" << endl;

	if (data.size() == decodedLZSS.size()) cout << "\nSame size\n"; else cout << "\nFAIL\n";

	for (int i = 0; i < data.size(); i++) {
		if (data[i] != decodedLZSS[i]) { cout << "\n!WRONG DATA DECODED!\n"; break; }
	}

	double k = (double)data.size() / (double)encodedHuf.size();
	cout << "\nCoef: " << setprecision(2) << k << endl;
}

// LZW

void compressor8(const vector<unsigned char>& data) {

	vector <int> encoded = encodeLZW(data);

	cout << "\nAfter LZW: " << encoded.size() << " byte" << endl;

	vector <unsigned char> decoded = decodeLZW(encoded);

	cout << "\nAfter decoding: " << decoded.size() << " byte" << endl;

	if (data.size() == decoded.size()) cout << "\nSame size\n"; else cout << "\nFAIL\n";

	for (int i = 0; i < data.size(); i++) {
		if (data[i] != decoded[i]) { cout << "\n!WRONG DATA DECODED!\n"; break; }
	}

	double k = (double)data.size() / (double)encoded.size();
	cout << "\nCoef: " << setprecision(2) << k << endl;
}

// LZW + HA 

void compressor9(const vector<unsigned char>& data) {

	vector<int> encodedLZW = encodeLZW(data);

	// Преобразование int в 2 байта
	vector<unsigned char> lzw_bytes;
	lzw_bytes.reserve(encodedLZW.size() * 2);

	for (int code : encodedLZW) {
		uint16_t code16 = static_cast<uint16_t>(code);
		lzw_bytes.push_back(static_cast<unsigned char>(code16 & 0xFF));
		lzw_bytes.push_back(static_cast<unsigned char>((code16 >> 8) & 0xFF));
	}

	// Хаффман
	vector<unsigned char> encodedHuf = encodeHuf(lzw_bytes);

	cout << "\nAfter LZW + HA: " << encodedHuf.size() << " byte" << endl;

	vector<unsigned char> decodedHuf = decodeHuf(encodedHuf);

	vector<int> decodedLZW_bytes;
	decodedLZW_bytes.reserve(decodedHuf.size() / 2);

	for (size_t i = 0; i + 1 < decodedHuf.size(); i += 2) {
		int code = decodedHuf[i] | (decodedHuf[i + 1] << 8);
		decodedLZW_bytes.push_back(code);
	}

	vector<unsigned char> decodedLZW = decodeLZW(decodedLZW_bytes);

	cout << "\nAfter decoding: " << decodedLZW.size() << " byte" << endl;

	if (data.size() == decodedLZW.size()) cout << "\nSame size\n"; else cout << "\nFAIL\n";

	for (int i = 0; i < data.size() ; i++) {
		if (data[i] != decodedLZW[i]) { cout << "\n!WRONG DATA DECODED!\n"; break; }
	}

	double k = (double)data.size() / (double)encodedHuf.size();
	cout << "\nCoef: " << setprecision(2) << k << endl;
}

vector<unsigned char> oimg() {

	int width, height, channels;

	string filename = "bwimage.jpg";

	unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &channels, 1);
	if (width < 1 || height < 3) return {};

	if (!imageData) {
		cout << "Ошибка загрузки файла " << filename << endl;
		cout << "Причина: " << stbi_failure_reason() << endl;
		return {};
	}
	vector<unsigned char> image(imageData, imageData + width * height * 1);
	stbi_image_free(imageData);

	return image;
}


int main() {

	setlocale(LC_ALL, "Ru");

	//ifstream f("Test.txt", ios::in);
	//if (!f.is_open()) {
	//	cerr << "Error!"; return 1;
	//}
	//vector <unsigned char> data = readText(f); f.close();

	//auto data = oimg();

	ifstream file("openvpn.exe", ios::binary);
	if (!file.is_open()) {
		cerr << "Error!"; return 1;
	}

	file.seekg(0, ios::end);
	size_t size = file.tellg();
	file.seekg(0, ios::beg);

	vector<unsigned char> data(size);
	file.read(reinterpret_cast<char*>(data.data()), size);

	cout << "\nData size: " << data.size() << " byte" << endl;

	clock_t start = clock();

	compressor9(data);
	
	clock_t end = clock();
	double taken_time = double(end - start) / CLOCKS_PER_SEC;

	cout << "\n---------------------\nTime taken: " << taken_time << " sec" << endl;


	return 0;
}