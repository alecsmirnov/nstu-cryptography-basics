#include "HuffmanCode.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <map>

#define chrToInt(x) (x - '0')
#define intToChr(x) (x + '0')

#define isPowerOf2(x) (0 < x && !(x & (x - 1)))

#define EVEN 0

using namespace HuffmanStd;

// Прочитать текстовый файл и представить его содержимое в виде строки
std::string HuffmanCode::readFile(std::string file_name) const {
	std::ifstream file(file_name);
	std::string str_file;

	if (file) {
		std::string line;
		while(std::getline(file, line))			// Построчно читаем файл
			str_file += line + "\n";			// Записываем содержимое файла в строку

		str_file.resize(str_file.length() - 1);	// Убираем последний символ новой строки

		file.close();
	}
	else
		std::cerr << "Error: " + file_name + " is not opened!" << std::endl;

	return str_file;
}

// Записать текст в текстовый файл по указанному имени
void HuffmanCode::writeFile(std::string file_name, std::wstring text) const {
	std::wofstream file(file_name, std::ios_base::trunc);
	file << text;
	file.close();
}

// Получить копию кодовой таблицы Хаффмана
std::vector<elem_t> HuffmanCode::getTable() const {
	return code_table;
}

wchar_t HuffmanCode::getBit(std::wstring code) const {
	wchar_t bit = 0;

	if (!code.empty()) {
		bit = chrToInt(code[0]);

		for (std::size_t i = 1; i != code.size(); ++i)
			bit = (bit ^ chrToInt(code[i]));
	}

	return intToChr(bit);
}

bool HuffmanCode::isEvenAlphabet() const {
	return isPowerOf2(code_table.size());
}

// Кодирование текста и формирование таблицы Хаффмана. На вход подаётся текст,
// представленный в виде строки
std::wstring HuffmanCode::encodeText(std::wstring text) {
	std::wstring encoded_text;

	custom_priority_queue<Node*, NodePtrsCompare> queue;	// Узлы для формирования дерева
	std::map<wchar_t, std::uint32_t> freqs;					// Вероятности символов текста

	// Подсчитываем вероятности исходного текста
	for (auto chr : text)
		++freqs[chr];

	// Формируем листовые узлы
	double freq = 0;
	for (auto sign : freqs) {
		freq = static_cast<double>(sign.second) / text.size();

		code_table.push_back({sign.first, freq, std::wstring()});
		queue.push(new Node(sign.first, freq));
	}

	if (isEvenAlphabet()) {
		// Создаём кодове дерево Хаффмана
		buildCodeTree(queue);
		// Создаём кодовую таблицу Хаффмана
		buildCodeTable(code_tree);

		// Посимвольное кодирование текста
		for (auto chr : text) {					// Последовательный просмотр текста
			bool find = true;
			for (auto it = code_table.begin(); it != code_table.end() && find; ++it)
				if (it->sign == chr) {			// Поиск символа в таблице
					encoded_text += it->code;	// Запись соответствующего ему кода в строку

					encoded_text += getBit(it->code);

					find = false;
				}
		}
	}
	else
		clearTable();

	return encoded_text;
}

// Формирование кодовых слов и таблицы Хаффмана по алфавиту и вероятностяи
// На вход подаётся алфавит и вероятности в виде строк текста
void HuffmanCode::encodeAlphabet(std::wstring alphabet, std::wstring frequency) {
	std::wistringstream ss_alpha(alphabet);		// Строки в виде потока
	std::wistringstream ss_freq(frequency);		// Для обработки подобно файлу

	custom_priority_queue<Node*, NodePtrsCompare> queue;	// Узлы дерева Хаффмана

	// Формирование пары (символ, вероятность) и кодовой таблицы
	std::pair<wchar_t, double> sign;
	while (ss_alpha >> sign.first && ss_freq >> sign.second)
		if (sign.second != 0) {
			code_table.push_back({sign.first, sign.second, std::wstring()});
			queue.push(new Node(sign.first, sign.second));
		}

	if (isEvenAlphabet()) {
		// Формирование дерева Хаффмана и заполнение кодовой таблицы
		buildCodeTree(queue);
		buildCodeTable(code_tree);
	}
	else
		clearTable();
}

// Кодирование текста по уже существующей таблице Хаффмана
std::wstring HuffmanCode::encodeTextByTable(std::wstring text) {
	std::wstring encoded_text;

	// Поиск символа в таблице Хаффмана и построение кодируемой строки
	for (auto chr : text) {				// Последовательный просмотр текста
		bool find = true;
		for (auto i = code_table.begin(); i != code_table.end() && find; ++i)
			if (i->sign == chr) {			// Поиск символа в таблице
				encoded_text += i->code;	// Запись соответствующего ему кода в строку

				encoded_text += getBit(i->code);

				find = false;
			}
	}

	return encoded_text;
}

// Построить кодовое дерево Хаффмана
void HuffmanCode::buildCodeTree(custom_priority_queue<Node*, NodePtrsCompare>& queue) {
	// Формируем родителя для двух минимальных по весу узлов до тех пор,
	// пока в очереди с приоритетом (минимльным) не останется один узел
	while (1 < queue.size())
		queue.push(makeParent(queue.peek(), queue.peek()));

	code_tree = queue.peek();
}

// Сформировать родительский узел в дереве Хаффмана
Node* HuffmanCode::makeParent(Node* left, Node* right) {
	auto parent = new Node(left, right);

	return parent;
}

// Построить кодовую таблицу Хаффмана
void HuffmanCode::buildCodeTable(Node* code_tree, std::wstring code) {
	// Рекурсивно проходимся от корня дерева Хаффмана
	// до листовых узлов. Каждый код листового узла записывается таблицу Хаффмана
	if (code_tree) {
		if (isLeaf(code_tree)) {
			// Если узел -- лист, находим соответсвующий таблице елемент
			// и заполняем для него кодовое слово
			bool find = true;
			for (auto it = code_table.begin(); it != code_table.end() && find; ++it)
				if (it->sign == code_tree->symbol) {
					it->frequency = code_tree->frequency;
					it->code = code;

					find = false;
				}
		}

		// Рекурсивный обход дерева с формированием битовых кодов:
		// 0 -- для левых потомков, 1 -- для правых
		buildCodeTable(code_tree->left, code + L"0");
		buildCodeTable(code_tree->right, code + L"1");
	}
}

// Декодирование текста по существующей таблице Хаффмана
std::wstring HuffmanCode::decodeText(std::wstring& text) {
	std::wstring decoded_text;

	// Декодирование текста с помощью кодового дерева Хаффмана
	// С первого символа текста, начиная с корня дерева, проходим по ветвям дерева
	// до тех пор, пока не встретим символ алфавита "лист-узел". Повторяем до конца текста
	std::uint8_t word_len = code_table[0].code.length();

	for (std::size_t i = 0; i != text.size(); ++i) {
		wchar_t bit = chrToInt(text[i]);
		for (std::uint8_t j = 1; j != word_len + 1; ++j)
			bit = (bit ^ chrToInt(text[i + j]));

		if (bit == EVEN) {
			auto tree_ptr = code_tree;
			while (!isLeaf(tree_ptr))
				tree_ptr = text[i++] == L'0' ? tree_ptr->left : tree_ptr->right;

			decoded_text += tree_ptr->symbol;
			tree_ptr = code_tree;
		}
		else {
			for (std::uint8_t j = 0; j != word_len + 1; ++j)
				text[i + j] = 'X';

			i += word_len;
		}
	}

	return decoded_text;
}

// Проверка узла, является ли он листом (не имеет потомков)
bool HuffmanCode::isLeaf(Node* tree_node) const {
	return tree_node->left == nullptr && tree_node->right == nullptr;
}

// Получить среднюю длину кодового слова
double HuffmanCode::getWordAverageLength() const {
	double len = 0;
	for (auto sign : code_table)
		len += sign.frequency * sign.code.length();				

	return len;
}

// Получить Энтропию (кол-во информации в сообщении)
double HuffmanCode::getEntropy() const {
	double entropy = 0;

	for (auto sign : code_table)
		entropy += sign.frequency * log2(1 / sign.frequency);

	return entropy;
}

// Получить избыточность неравномерного кода
double HuffmanCode::getRedundancy() const {
	return getWordAverageLength() - getEntropy();
}

// Проверка на неравенство Крафта
bool HuffmanCode::kraftInequality() const {
	double sum = 0;

	for (auto sign : code_table)
		sum += 1 / std::pow(2, sign.code.length());

	return sum <= 1;
}

// Удаление таблицы и дерева Хаффмана
void HuffmanCode::clearTable() {
	std::vector<elem_t>().swap(code_table);

	delete code_tree;
	code_tree = nullptr;
}
