#ifndef CUSTOMPRIORITYQUEUE_H
#define CUSTOMPRIORITYQUEUE_H

#include <queue>

// Наследование от класса из стандартной библиотеки язык
// Необходимо для того, чтобы объединить две операции:
// взятие верхнего элемента из стека, удаление верхнего элемента стека, в одну
template<typename T, typename Compare>
class custom_priority_queue : public std::priority_queue<T, std::vector<T>, Compare> {
public:
	T peek();
};

#include "CustomPriorityQueue.tpp"

#endif