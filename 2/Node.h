#ifndef NODE_H
#define NODE_H

#include <string>

// Узел кодового дерева Хаффмана 
struct Node {
	// Конструктор для листового узла
	Node(std::string symbol, double frequency = 0, Node* left = nullptr, Node * right = nullptr) :
		symbol(symbol), frequency(frequency), left(left), right(right) {}

	// Конструктор для родительского узла
	Node(Node* left, Node* right) :
		Node(left->symbol + right->symbol, left->frequency + right->frequency, left, right) {}

	~Node() {delete left; delete right;}

	std::string symbol;		// Кодируемый символ алфавита
	double frequency;		// Вероятность кодируемого символа

	Node* left;				// Левый потомок
	Node* right;			// Правый потомок
};

// Описание структуры сравнения двух указателей на узлы кодового дерева
// Используется для стандартных контейнеров языка,
// в данном случае очередь с минимальным приоритетом
struct NodePtrsCompare {	
	bool operator()(const Node* left, const Node* right) const {
		return left->frequency > right->frequency;
	}
};

#endif