#pragma once
#include <string>
#include <iostream>

//custom iterator for our queue(FIFO front) so we can iterate over its elementes
template<typename queue>
class const_queueIterator
{
public:
	//defines an alias for the underlying container(node) within the queue class
	using _node = typename queue::node;

	//defines an alias for the value type stored within the underlying container(node) of our queue
	using ValueType = _node::ValueType;

	//defines an alias for a pointer to a our queue underlying container(node)
	using NodePointer = _node*;

	//defines an alias for a pointer to the value type stored in the underlying container(node) of our queue
	using PointerType = ValueType*;

	//defines an alias for a reference to the value type stored in the underlying container(node) of our queue
	// (const ensures the value isn't modified)
	using ReferenceType = const ValueType&;

	const_queueIterator(const NodePointer ptr)
		: m_Ptr(ptr) { }


	//increments operator
	//operator for ++iterator
	const_queueIterator& operator++();

	//increments operator
	//operator for iterator++
	const_queueIterator& operator++(int);

	//returns reference to value stored in our node m_Ptr->value
	ReferenceType operator*() const;

	//allows us to acces data of nodes data, usefel when it holds data of more complex types
	PointerType operator->() const;

	//operator for comaprisons between given iterators
	bool operator==(const const_queueIterator& other) const;

	//operator for comaprisons between given iterators
	bool operator!=(const const_queueIterator& other) const;

private:
	//stores pointer to the underlying container of our queue
	NodePointer m_Ptr;
};

template<typename T>
class queue
{
private:
	struct node;
public:
	//defines an alias for the value type of our queue
	using ValueType = T;
	//defines an alias for the iterator of our queue
	using const_Iterator = const_queueIterator<queue<T>>;
public:
	//default constructor
	queue();

	//returns element in the front
	T const& front() const;

	//pops front element
	void pop_front();

	//pushes back an elemnt
	void push_back( T value );

	//counstructs element directly in our queue and then saveing it in queue instead of construcing it then copying
	template<typename... Args>
	T& emplaceBack(Args&&... args);

	//returns size of our queue
	size_t size();

	//returns if its empty or not
	bool empty();

	//returns iterator to the begining(1st element) of our queue
	const_Iterator begin();

	//returns adress past our last element as for us to know when to stop iterating
	const_Iterator end();

	//clears our stack so there are no data left
	void clear();

	//ensures safe memory freeing
	~queue();

	//private members
private:
	//declare our custom iterator as friend as to have him access our private members
	template<typename>
	friend class const_queueIterator;

	size_t m_size = 0;

	//our underlying container
	struct node
	{
		using ValueType = T;
		T m_Value;

		node* m_next = nullptr;
	};

	//store references to head and tail
	node* m_head = nullptr;
	node* m_tail = nullptr;
};



