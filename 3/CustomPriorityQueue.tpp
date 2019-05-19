#include "CustomPriorityQueue.h"

// Объединить двух операций в одну:
// взятие верхнего элемента из стека, удаление верхнего элемента стека
template<typename T, typename Compare>
T custom_priority_queue<T, Compare>::peek() {
	T top = this->top();		
	this->pop();

	return top;
}