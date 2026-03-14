#include "Arithmetic Coding.h"

pair <map<unsigned char, double>, vector<unsigned char>> calcProbs(const vector<unsigned char>& data) {

	// Количество вхождений
	map<unsigned char, int> freqs;
	for (auto s : data) freqs[s]++;

	//Сортировка символов, оставляем по одному представителю
	vector<unsigned char> symbols;
	for (auto& s : freqs) symbols.push_back(s.first);

	map<unsigned char, double> probs;

	double prob = 0.0;

	for (unsigned char sym : symbols) {
		probs[sym] = prob; // Нижняя граница интервала
		prob += (double)freqs[sym] / data.size(); // Верхнаяя граница для sym
	}
	return { probs, symbols }; // Возвращаем вероятностное распределение символов
}


double encodeArithSimple(double lower, double upper, int index, const map<unsigned char, double>& probs,
	const vector<unsigned char>& symbols, const vector<unsigned char>& data) {

	if (index == data.size()) {
		return (lower + upper) / 2; 
	}

	// Ищем индексы
	unsigned char curS = data[index];
	double sLower = probs.at(curS);

	double sUpper = 1.0;
	for (int i = 0; i < symbols.size() - 1; i++) {
		if (symbols[i] == curS) {
			sUpper = probs.at(symbols[i + 1]);
			break;
		}
	}
	double range = upper - lower;

	double newLower = lower + range * sLower;
	double newUpper = lower + range * sUpper;
	
	return encodeArithSimple(newLower, newUpper, index + 1, probs, symbols, data);
}	

double encodeArithmetic(const vector<unsigned char>& data) {

	if (data.size() > 20) {
		cout << "\nВозможна большая неточность! Алгоритм сработает неверно :(\n";
		return 0.0;
	}

	pair <map<unsigned char, double>, vector<unsigned char>> result = calcProbs(data);
	map<unsigned char, double> probs = result.first;
	vector<unsigned char> symbols = result.second;

	return encodeArithSimple(0.0, 1.0, 0, probs, symbols, data);
}