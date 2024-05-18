#include "queue.h"

template<typename T>
queue<T>::queue()
{
	m_head = new node;
	m_head->m_next = nullptr;
}
template<typename T>
queue<T>::~queue()
{
	clear();
	delete m_head;
};

template<typename T>
size_t queue<T>::size() { return m_size; }

template<typename T>
bool queue<T>::empty() { return m_size == 0; }

template<typename T>
void queue<T>::push_back(T value)
{
	node* new_node = new node;
	new_node->m_Value = value;

	if (m_size++ == 0)
	{
		m_head->m_next = new_node;
		m_tail = new_node;
		return;
	}

	m_tail->m_next = new_node;
	m_tail = new_node;
}

template<typename T>
T const& queue<T>::front() const
{
	return m_head->m_next->m_Value;
}

template<typename T>
void queue<T>::pop_front()
{
	if (m_size == 0) { return; }

	node* tmp = m_head->m_next;
	m_head->m_next = tmp->m_next;

	if (tmp == m_tail)
	{
		--m_size;

		delete tmp;
		m_tail = nullptr;
		tmp = nullptr;
		return;
	}

	--m_size;
	delete tmp;
	tmp = nullptr;
}

template<typename T>
void queue<T>::clear()
{
	while (m_size != 0) { pop_front(); }
	m_head->m_next = nullptr;
}

template<typename T>
template<typename... Args>
T& queue<T>::emplaceBack(Args&&... args)
{
	node* new_node = new node;

	new_node->m_Value = T(std::forward<Args>(args)...);
	new_node->m_next = nullptr;

	if (m_size++ == 0)
	{
		m_head->m_next = new_node;
		m_tail = new_node;
		return new_node->m_Value;
	}

	m_tail->m_next = new_node;
	m_tail = new_node;

	return new_node->m_Value;
}

template<typename queue>
const_queueIterator<queue>& const_queueIterator<queue>::operator++()
{
	if (this->m_Ptr)
	{
		*this = m_Ptr->m_next;
	}
	return *this;
}

template<typename queue>
const_queueIterator<queue>& const_queueIterator<queue>::operator++(int)
{
	const_queueIterator iterator = *this;

	if (this->m_Ptr != nullptr)
	{
		*this->m_Ptr = this->m_Ptr->m_next;
	}
	return iterator;
}


template<typename queue>
const_queueIterator<queue>::ReferenceType const_queueIterator<queue>::operator*() const
{
	return this->m_Ptr->m_Value;
}

template<typename queue>
bool const_queueIterator<queue>::operator==(const const_queueIterator& other) const
{
	return this->m_Ptr == other.m_Ptr;
}

template<typename queue>
bool const_queueIterator<queue>::operator!=(const const_queueIterator& other) const
{
	return !(*this == other);
}


template<typename queue>
const_queueIterator<queue>::PointerType const_queueIterator<queue>::operator->() const
{
	return &(m_Ptr->m_Value);
}

template<typename T>
queue<T>::const_Iterator queue<T>::begin()
{
	return const_Iterator(m_head->m_next);
}

template<typename T>
queue<T>::const_Iterator queue<T>::end()
{
	//return adress past our last element (which is nulptr in our case)
	return const_Iterator(m_tail->m_next);
}