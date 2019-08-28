#ifndef HUFFMANCODE_H
#define HUFFMANCODE_H

#include "Node.h"
#include "CustomPriorityQueue.h"

#include <vector>
#include <iostream>
#include <fstream>

// Элемент таблицы Хаффмана
struct elem_t {	
	std::string sign;			// Символ алфавита
	double frequency;			// Вероятность символа
	std::string code;			// Кодовое слово
};

// Класс для работы с алгоритмом Хаффмана
class HuffmanCode {
public:
	void menu();				// Вывзов консольного меню

	// Метод, отвечающий за кодирование данных. На вход подаётся файл с кодируемым текстом
	void encodeText(const std::string text_file, const std::string output_file);

	// Метод, отвечающий за кодирование данных. На вход подаются 2 файла:
	// 1) файл с кодируемым алфавитом;
	// 2) Файл с вероятностями символов алфавита.
	void encodeAlphabet(const std::string alphabet_file = "alpha.txt", const std::string frequency_file = "freq.txt",
						const std::string output_file = "codes.txt");

	// Метод, отвечающий за декодирование данных. На вход подаётся фай с закодированным текстом
	void decode(const std::string coded_file = "coded.txt", const std::string output_file = "decoded.txt");

	void printTable() const;	// Вывести таблицу Хаффмана
	void clearTable();			// Очистить таблицу Хаффмана (удалить все данные)

private:
	// Построить кодовое дерево Хаффмана
	void buildCodeTree(custom_priority_queue<Node*, NodePtrsCompare>& queue);

	// Сформировать родительский узел в дереве Хаффмана
	Node* makeParent(Node* left, Node* right);		

	// Построить кодовую таблицу Хаффмана
	void buildCodeTable(Node* code_tree, std::string code = std::string());

	// Проверка узла, является ли он "листом"
	bool isLeaf(Node* tree_node) const;

	// Универсальный метод для записи элементов таблицы в файл
	void write(const std::string out_file, const std::string elem_t::* field) const;

	// Получить среднюю длину кодового слова
	double getWordAverageLength() const;

	// Получить Энтропию (кол-во информации в сообщении)
	double getEntropy() const;

	// Получить избыточность неравномерного кода
	double getRedundancy() const;

	// Проверка на неравенство Крафта
	bool kraftInequality() const;

private:
	Node* code_tree = nullptr;			// Кодовое дерево Хаффмана (для декодирования)
	std::vector<elem_t> code_table;		// Кодовая таблица Хаффмана (для кодирования)
};

#endif
