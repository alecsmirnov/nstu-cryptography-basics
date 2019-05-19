#ifndef HUFFMANCODE_H
#define HUFFMANCODE_H

#include "Node.h"
#include "CustomPriorityQueue.h"

#include <string>
#include <vector>

namespace HuffmanStd {		// Элемент таблицы Хаффмана
struct elem_t {
	wchar_t sign;			// Символ алфавита
	double frequency;		// Вероятность символа
	std::wstring code;		// Кодовое слово
};
}

// Класс для работы с алгоритмом Хаффмана
class HuffmanCode {
public:
	// Кодирование текста и формирование таблицы Хаффмана. На вход подаётся текст,
	// представленный в виде строки
	std::wstring encodeText(std::wstring text);
	// Формирование кодовых слов и таблицы Хаффмана по алфавиту и вероятностяи
	// На вход подаётся алфавит и вероятности в виде строк текста
	void encodeAlphabet(std::wstring alphabet, std::wstring frequency);
	// Кодирование текста по уже существующей таблице Хаффмана
	std::wstring encodeTextByTable(std::wstring text);

	// Декодирование текста по существующей таблице Хаффмана
	std::wstring decodeText(std::wstring& text);

	// Прочитать текстовый файл и представить его содержимое в виде строки
	std::string readFile(std::string file_name) const;
	// Записать текст в текстовый файл по указанному имени
	void writeFile(std::string file_name, std::wstring text) const;

	// Получить копию кодовой таблицы Хаффмана
	std::vector<HuffmanStd::elem_t> getTable() const;

	// Получить среднюю длину кодового слова
	double getWordAverageLength() const;

	// Получить Энтропию (кол-во информации в сообщении)
	double getEntropy() const;

	// Получить избыточность неравномерного кода
	double getRedundancy() const;

	// Проверка на неравенство Крафта
	bool kraftInequality() const;

    void clearTable();

private:
	// Получение бита кратности по коду
	wchar_t getBit(std::wstring code) const;

	// Проверка алфавита на чётность
	bool isEvenAlphabet() const;

	// Построить кодовое дерево Хаффмана
	void buildCodeTree(custom_priority_queue<Node*, NodePtrsCompare>& queue);

	// Сформировать родительский узел в дереве Хаффмана
	Node* makeParent(Node* left, Node* right);		

	// Построить кодовую таблицу Хаффмана
	void buildCodeTable(Node* code_tree, std::wstring code = std::wstring());

	// Проверка узла, является ли он листом
	bool isLeaf(Node* tree_node) const;

private:
	Node* code_tree = nullptr;						// Кодовое дерево Хаффмана (для декодирования)
	std::vector<HuffmanStd::elem_t> code_table;		// Кодовая таблица Хаффмана (для кодирования)
};

#endif
