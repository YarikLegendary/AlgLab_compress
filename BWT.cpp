#include "BWT.h"

vector<unsigned char> naiveBWT(const vector<unsigned char>& data) {

	vector<vector<unsigned char>> matrix;

	for (int i = 0; i < data.size(); i++) {

		vector<unsigned char> row(data.size());

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

	// Массив перестановок {0, 1, 2, 3 ... } условно
	vector<int> shifts (data.size());

	for (int i = 0; i < data.size(); i++) shifts[i] = i;

	// Сортируем по СТРОКАМ (compareShifts) индксы в массиве shifts 

	// ТУТ ПО ИДЕИ СОРТИРОВКА ПОДСЧЕТОМ ИЛИ КАК?
	sort(shifts.begin(), shifts.end(),
		[&](int a, int b) { return compareShifts(a, b, data); });

	// Берем последние индексы переставленных строк
	vector<unsigned char> result;
	for (int start : shifts) {
		int lastIndex = (start + data.size() - 1) % data.size();
		result.push_back(data[lastIndex]);
	}
	return result;

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

vector<unsigned char> inverseBWT_counting_sort(const vector<unsigned char>& last_column) {
	int n = last_column.size();

	// Шаг 1: СОРТИРОВКА ПОДСЧЕТОМ
	const int ALPHABET_SIZE = 256;
	vector<int> count(ALPHABET_SIZE, 0);
	vector<int> start_pos(ALPHABET_SIZE, 0);

	// Подсчет частот
	for (unsigned char c : last_column) {
		count[c]++;
	}

	// Вычисление начальных позиций
	int sum = 0;
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		start_pos[i] = sum;
		sum += count[i];
	}

	// Шаг 2: Построение массива перестановок next[]
	vector<int> next(n);
	vector<int> next_pos = start_pos; // копия

	for (int i = 0; i < n; i++) {
		unsigned char c = last_column[i];
		next[next_pos[c]++] = i;
	}

	// Шаг 3: Поиск строки с оригиналом
	// В реальном BWT нужно знать индекс оригинальной строки
	// Для простоты начнем с первой строки
	int row = 0;

	// Шаг 4: Восстановление
	vector<unsigned char> result;
	for (int i = 0; i < n; i++) {
		row = next[row];
		result.push_back(last_column[row]);
	}

	return result;
}


void printMatrix(vector<vector<unsigned char>> matrix) {
	for (const auto& row : matrix) {
		for (char s : row) {
			cout << s;
		}
		cout << endl;
	}
}
