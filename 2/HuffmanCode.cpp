#include "HuffmanCode.h"

#include <iostream>
#include <fstream>
#include <map>

#define MENU_EXIT '0'		// Константа для определения символа выхода из программы

// Функция для вывода пунктов меню на экран
static void menuOptions(std::string text, std::string alpha, std::string freq, std::string coded) {
	std::cout << "\"MENU\"" << std::endl;
	std::cout << "1. Encode data (from \"" + text + "\")" << std::endl;
	std::cout << "2. Encode data (from \"" + alpha + "\" and \"" + freq + "\" files)" << std::endl;
	std::cout << "3. Decode data (from \"" + coded + "\" file)" << std::endl;
	std::cout << "4. Print table" << std::endl;
	std::cout << "5. Clear table" << std::endl;
	std::cout << "6. Print file-names" << std::endl;
	std::cout << "7. Change file-names" << std::endl;
	std::cout << "8. Print menu" << std::endl;
	std::cout << "9. Clear screen" << std::endl;
	std::cout << MENU_EXIT << ". Exit" << std::endl;
}

// Вызов консольного меню
void HuffmanCode::menu() {
	std::uint8_t choice;						// Пункт меню, выбранный пользователем

	// Стандартные имена файлов для работы с данными
	std::string text_file("text.txt");			// Входной файл с текстом
	std::string alpha_file("alpha.txt");		// Входной файл с алфавитом
	std::string freq_file("freq.txt");			// Входной файл с вероятностями
	std::string codes_file("codes.txt");		// Выходной файл с кодами
	std::string coded_file("coded.txt");		// Входной файл с закодированным текстом
	std::string decoded_file("decoded.txt");	// Выходной файл с декодированным текстом

	menuOptions(text_file, alpha_file, freq_file, coded_file);

	do {
		std::cout << std::string(48, '-') << std::endl;
		std::cout << "Enter menu item: ";
		std::cin >> choice;
		std::cout << std::endl;

		switch (choice) {
			case '1': {	// Кодирование данных
				encodeText(text_file, coded_file);
				std::cout << "Data encoded and saved to \"" + coded_file + "\"" << std::endl;
				break;
			}
			case '2': {	// Кодирование данных
				encodeAlphabet(alpha_file, freq_file, codes_file);
				std::cout << "Data encoded and saved to \"" + codes_file + "\"" << std::endl;
				break;
			}
			case '3': {	// Декодирование данных
				if (code_tree) {
					decode(coded_file, decoded_file);
					std::cout << "Data decoded and saved to \"" + decoded_file + "\"" << std::endl;
				}
				else
					std::cerr << "Error: codes doesn't exist!" << std::endl;
				break;
			}
			case '4': {	// Вывод таблицы
				printTable();
				break;
			}
			case '5': {	// Очистка таблицы
				clearTable();
				std::cout << "Table is clear!" << std::endl;
				break;
			}
			case '6': {	// Вывод названий файлов
				std::cout << "Text file name:\t" + alpha_file << std::endl;
				std::cout << "Alphabet file name:\t" + alpha_file << std::endl;
				std::cout << "Frequency file name:\t" + freq_file << std::endl;
				std::cout << "Codes file name:\t" + codes_file << std::endl;
				std::cout << "Coded file name:\t" + coded_file << std::endl;
				std::cout << "Decoded file name:\t" + decoded_file << std::endl;
				break;
			}
			case '7': {	// Изменение названий файлов
				std::cout << "Enter new text file name: ";
				std::cin >> text_file;
				std::cout << "Enter new alphabet file name: ";
				std::cin >> alpha_file;
				std::cout << "Enter new frequency file name: ";
				std::cin >> freq_file;
				std::cout << "Enter new codes file name: ";
				std::cin >> codes_file;
				std::cout << "Enter new coded file name: ";
				std::cin >> coded_file;
				std::cout << "Enter new decoded file name: ";
				std::cin >> decoded_file;
				break;
			}
			case '8': {	// Вывод пунктов меню
				menuOptions(text_file, alpha_file, freq_file, coded_file);
				break;
			}
			case '9': {	// Очистка экрана
				system("clear||cls");
				menuOptions(text_file, alpha_file, freq_file, coded_file);
				break;
			}
			case MENU_EXIT: break;
			default: std::cerr << "Error: invalid menu item!" << std::endl;
		}

	} while (choice != MENU_EXIT);

	clearTable();
}

// Метод, отвечающий за кодирование данных. На вход подаётся файл с кодируемым текстом
void HuffmanCode::encodeText(const std::string text_file, const std::string output_file) {
	std::ifstream file(text_file);				// Входной файл с кодируемым текстом

	if (file) {
		custom_priority_queue<Node*, NodePtrsCompare> queue;
		std::map<std::string, std::uint32_t> freqs;

		// Подсчитываем вероятности исходного текста
		std::uint32_t chrs_count = 0;
		std::string chr;
		while (file >> chr) {
			++freqs[chr];
			++chrs_count;
		}

		// Формируем листовые узлы
		for (auto sign : freqs)
			queue.push(new Node(sign.first, static_cast<double>(sign.second) / chrs_count));

		file.close();

		// Создаём кодове дерево Хаффмана
		buildCodeTree(queue);
		// Создаём кодовую таблицу Хаффмана
		buildCodeTable(code_tree);

		std::string encoded_text;

		file.open(text_file);
		while (file >> chr) {
			bool find = true;

			for (auto i = code_table.begin(); i != code_table.end() && find; ++i)
				if (i->sign == chr) {
					encoded_text += i->code;
					find = false;
				}
		}
		file.close();

		std::cout << encoded_text << std::endl; // УБРАТЬ, ЧТОБЫ ПОТОМ ЗАПИСАТЬ В ФАЙЛ
	}
	else
		std::cerr << "Error: file(s) is not opened!" << std::endl;
}

// Метод, отвечающий за кодирование данных. На вход подаются 2 файла:
// 1) файл с кодируемым алфавитом;
// 2) Файл с вероятностями символов алфавита.
void HuffmanCode::encodeAlphabet(const std::string alphabet_file, const std::string frequency_file, const std::string output_file) {
	std::ifstream alph_file(alphabet_file);		// Входной файл с кодируемым алфавитом
	std::ifstream freq_file(frequency_file);	// Входной файл с вероятностями символов

	if (alph_file && freq_file) {
		// Создаём контейнер для хранения листовых узлов дерева Хаффмана
		custom_priority_queue<Node*, NodePtrsCompare> queue;

		// Формируем листовые узлы
		std::pair<std::string, double> sign;
		while (alph_file >> sign.first && freq_file >> sign.second)
			queue.push(new Node(sign.first, sign.second));

		alph_file.close();
		freq_file.close();

		// Создаём кодове дерево Хаффмана
		buildCodeTree(queue);

		// Создаём кодовую таблицу Хаффмана
		buildCodeTable(code_tree);

		// Записываем результат - кодовые слова, в выходной файл, 
		// по умолчанию "codes.txt"
		write(output_file, &elem_t::code);
	}
	else
		std::cerr << "Error: file(s) is not opened!" << std::endl;
}

// Построить кодовое дерево Хаффмана
void HuffmanCode::buildCodeTree(custom_priority_queue<Node*, NodePtrsCompare>& queue) {
	// Формируем дерево до тех пор, пока в очереди есть больше чем 1 узел
	// Извлекаем из очереди 2 узла и формируем родительский, 
	// с указателями на двух извлечённых потомков. Левый потомок -- первый элемент,
	// правый потомок -- второй элемент
	while (1 < queue.size())
		queue.push(makeParent(queue.peek(), queue.peek()));

	// Оставшийся узел становится корнем дерева Хаффмана
	code_tree = queue.peek();
}

// Сформировать родительский узел в дереве Хаффмана
Node* HuffmanCode::makeParent(Node* left, Node* right) {
	// Вероятность родителя равна сумме вероятностей потомков
	// Родитель указывает на двух потомков
	auto parent = new Node(left, right);

	return parent;
}

// Построить кодовую таблицу Хаффмана
void HuffmanCode::buildCodeTable(Node* code_tree, std::string code) {
	// Рекурсивно проходим узлам дерева Хаффмана в прямом порядке, записывая бит информации
	// о каждой ветвии, по которой был совершён переход
	if (code_tree) {
		// Если узел - "лист", записываем информацию о символе и его код в  таблицу Хаффмана
		if (isLeaf(code_tree))
			code_table.push_back({code_tree->symbol, code_tree->frequency, code});

		// Вычисление кода каждого символа по дереву Хаффмана
		buildCodeTable(code_tree->left, code + "0");	// Переход по правым ветвям дерева
		buildCodeTable(code_tree->right, code + "1");	// Переход по левым вевтям дерева
	}
}

// Метод, отвечающий за декодирование данных. На вход подаётся фай с закодированным текстом
void HuffmanCode::decode(const std::string coded_file, const std::string output_file) {
	std::ifstream code_file(coded_file);				// Файл с закодированным текстом
	std::ofstream decode_file(output_file, std::ios_base::trunc);

	if (code_file && decode_file) {
		auto tree_ptr = code_tree;						// Указатель на корень кодового дерева

		// Декодирование текста с помощью кодового дерева Хаффмана
		// С первого символа текста, начиная с корня дерева, проходим по ветвям дерева
		// до тех пор, пока не встретим символ алфавита "лист-узел". Повторяем до конца текста
		std::uint8_t bit;
		while (code_file >> bit) {
			// Вычисляем ветвь дерева
			tree_ptr = bit == '0' ? tree_ptr->left : tree_ptr->right;

			if (isLeaf(tree_ptr)) {						// Определям, найден ли символ
				decode_file << tree_ptr->symbol;		// Записываем декодированный символ в файл

				tree_ptr = code_tree;					// Переходи обратно к корню дерева
			}
		}

		code_file.close();
		decode_file.close();
	}
	else
		std::cerr << "Error: file(s) is not opened!" << std::endl;
}

// Проверка узла, является ли он "листом"
bool HuffmanCode::isLeaf(Node* tree_node) const {
	return tree_node->left == nullptr && tree_node->right == nullptr;
}

// Универсальный метод для записи элементов таблицы в файл
void HuffmanCode::write(const std::string out_file, const std::string elem_t::* field) const {
	if (!code_table.empty()) {
		std::ofstream file(out_file, std::ios_base::trunc);

		for (std::size_t i = 0; i != code_table.size() - 1; ++i)	// Записываем символы,
			file << code_table[i].*field << " ";					// разделяя их пробелами

		file << code_table[code_table.size() - 1].code;

		file.close();
	}
	else
		std::cerr << "Error: table is empty! Can't write code!" << std::endl;
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

// Вывести таблицу Хаффмана
void HuffmanCode::printTable() const {
	if (!code_table.empty()) {
		std::cout << std::string(48, '_') << std::endl;
		std::cout << " Alphabet\t| Frequency\t| Code\t\t|" << std::endl;
		std::cout << std::string(16, '_') << "|" << std::string(15, '_') << "|" << std::string(15, '_') << "|" << std::endl;

		// Вывод элементов таблицы, состоящих из символа, его вероятности и кода
		for (auto sign : code_table)
			std::cout << " " << sign.sign << "\t\t| " << sign.frequency << "\t\t| " << sign.code << "\t\t|" << std::endl;

		std::cout << std::string(16, '_') << "|" << std::string(15, '_') << "|" << std::string(15, '_') << "|" << std::endl;

		std::cout << std::endl << "Code word average length: " << getWordAverageLength();
		std::cout << std::endl << "Entropy: " << getEntropy();
		std::cout << std::endl << "Redundancy: " << getRedundancy();
		std::cout << std::endl << "Kraft inequality: " << std::boolalpha << kraftInequality() << std::endl;
	}
	else
		std::cerr << "Error: table is empty!" << std::endl;
}

// Очистить таблицу Хаффмана (удалить все данные)
void HuffmanCode::clearTable() {
	std::vector<elem_t>().swap(code_table);

	delete code_tree;
	code_tree = nullptr;
}