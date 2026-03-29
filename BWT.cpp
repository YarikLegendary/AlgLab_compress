#include "BWT.h"
const size_t BLOCK_SIZE = 1024 * 128;

vector<unsigned char> naiveBWT(const vector<unsigned char>& data) {

	vector<vector<unsigned char>> matrix;

	for (int i = 0; i < data.size(); i++) {

		vector<unsigned char> row;
		row.reserve(data.size());

		row.insert(row.end(), data.begin() + i, data.end());
		row.insert(row.end(), data.begin(), data.begin() + i);

		matrix.push_back(row);

	}

	cout << "\n\nМатрица сдвигов:" << endl;
	printMatrix(matrix);

	sort(matrix.begin(), matrix.end()); // сортируем лексикографически

	cout << "\nОтсортированная матрица сдвигов:" << endl;
	printMatrix(matrix);

	vector<unsigned char> result;

	for (const auto& c : matrix){
		result.push_back(c.back());
	}

	cout << "\nПоследний столбец: ";
	for (auto s : result) {
		cout << s;
	}
	cout << endl;

	return result;
}

vector<unsigned char> naiveiBWT(const vector<unsigned char>& data) {

	vector<vector<unsigned char>> matrix(data.size());

	for (int i = 0; i < data.size();i++) {
		for (int j = 0; j < data.size(); j++) {
			matrix[j].insert(matrix[j].begin(), data[j]);
		}
		sort(matrix.begin(), matrix.end());
	}
	vector<unsigned char> result(data.size());

	for (const auto& row : matrix) {
		if (!row.empty() && row.back() == '$') {
			cout << "\nИсходная строка: ";
			for (auto s : row) {
				cout << s;
			}
			cout << endl;

			return row;
		}
	}

	return matrix[0];
}

vector<unsigned char> BWT(const vector<unsigned char>& data) {

	if (data.empty()) return data;

	vector<unsigned char> extended(data.size() + 1);
	copy(data.begin(), data.end(), extended.begin());
	extended.back() = 0;

	// Массив перестановок {0, 1, 2, 3 ... } условно
	vector<int> shifts (extended.size());

	for (int i = 0; i < extended.size(); i++) shifts[i] = i;

	// Сортируем по СТРОКАМ (compareShifts) индксы в массиве shifts 
	sort(shifts.begin(), shifts.end(),
		[&](int a, int b) { return compareShifts(a, b, extended); });

	// Берем последние индексы переставленных строк
	vector<unsigned char> result;
	result.reserve(extended.size());

	for (int start : shifts) {
		int lastIndex = (start + extended.size() - 1) % extended.size();
		result.push_back(extended[lastIndex]);
	}

	return result;
}

vector<unsigned char> iBWT(const vector<unsigned char>& last_column) {

	if (last_column.empty()) return last_column;

	int n = last_column.size();

	vector<int> count(256, 0);
	for (unsigned char c : last_column) {
		count[c]++;
	}

	vector<int> start_pos(256, 0);
	int sum = 0;
	for (int i = 0; i < 256; i++) {
		start_pos[i] = sum;
		sum += count[i];
	}

	vector<int> next(n);
	vector<int> current_pos = start_pos; 

	for (int i = 0; i < n; i++) {
		unsigned char c = last_column[i];
		next[current_pos[c]++] = i;
	}

	int original_row = 0;
	for (int i = 0; i < n; i++) {
		if (last_column[i] == 0) { 
			original_row = i;
			break;
		}
	}
	vector<unsigned char> result;
	int current = original_row;

	for (int i = 0; i < n; i++) {
		current = next[current];
		result.push_back(last_column[current]);
	}
	result.pop_back();

	return result;
}

vector<unsigned char> blocksBWT(const vector<unsigned char>& data) {

	size_t total_size = data.size();

	vector<unsigned char> result;
	
	for (size_t offset = 0; offset < total_size; offset += BLOCK_SIZE) {

		size_t block_size = min(BLOCK_SIZE, total_size - offset);

		vector<unsigned char> block(data.begin() + offset, data.begin() + offset + block_size);

		vector<unsigned char> processed_block = BWT(block);

		result.insert(result.end(), processed_block.begin(), processed_block.end());

	}

	return result;
}

vector<unsigned char> blocksiBWT(const vector<unsigned char>& data) {

	size_t total_size = data.size();

	vector<unsigned char> result;

	size_t encoded_block_size = BLOCK_SIZE + 1;

	for (size_t offset = 0; offset < total_size; offset += encoded_block_size) {

		size_t block_size = min(encoded_block_size, total_size - offset);

		vector<unsigned char> block(data.begin() + offset, data.begin() + offset + block_size);

		vector<unsigned char> processed_block = iBWT(block);

		result.insert(result.end(), processed_block.begin(), processed_block.end());

	}

	return result;
}

vector<int32_t> buildSuffArray(const vector <unsigned char>& data) {

	vector <int32_t> suff_array;

	for (int i = 0; i < data.size() ; i++) {
		suff_array.push_back(i);
	}

	sort(suff_array.begin(), suff_array.end(), [&](int a, int b) {return compareSuffs(a, b, data);});

	return suff_array;
}

bool compareSuffs(int a, int b, const vector <unsigned char>& data) {

	for (int i = 0;;i++) {

		if (i + a >= data.size() && i + b >= data.size()) return false;

		if (a + i >= data.size()) return true;
		if (b + i >= data.size()) return false;

		if (data[i + a] != data[i + b]) {
			return data[i + a] < data[i + b];
		}
	}

}

vector<unsigned char> suffixArrayToBWT(const vector<int32_t>& suffix_array, const vector<unsigned char>& data) {

	vector<unsigned char> last_column(data.size());

	for (int i = 0; i < suffix_array.size(); i++) {

		int start = suffix_array[i];

		if (start == 0) {
			last_column[i] = data[data.size() - 1];
		}
		else last_column[i] = data[start - 1];

	}

	return last_column;
}

bool compareShifts(int a, int b, const vector<unsigned char>& data) {

	// Поэлеметно пробегаемся

	for (int i = 0; i < data.size(); i++) {

		// Берем по очереди символы a и b перестановки
		unsigned char sym_a = data[(a + i) % data.size()];
		unsigned char sym_b = data[(b + i) % data.size()];

		if (sym_a != sym_b) {
			return sym_a < sym_b; // Возвращаем true для swapa строк
		}
	}
	return false;
}

void printMatrix(vector<vector<unsigned char>> matrix) {
	for (const auto& row : matrix) {
		for (char s : row) {
			cout << s;
		}
		cout << endl;
	}
}
