#pragma once
#include "Student.h"

class List
{
private:
	struct Node
	{
		Student data;
		Node* next;
		Node(const Student& st) { data = st; next = nullptr; }
	};
	Node* head = nullptr;
	Node* tail = nullptr;
	size_t size = 0;
public:
	List() = default;
	List(const List& other);
	List& operator=(const List& other);
	~List();

	class iterator;

	iterator begin();
	iterator end() const;

	const Student& front() const;
	const Student& back() const;
	const Student& getAt(const size_t& pos) const;

	bool isEmpty() const;
	size_t getSize() const;

	void pushBack(const Student& st);
	void pushFront(const Student& st);
	void pushAt(const size_t& pos, const Student& st);
	void pushAfter(const Student& value, iterator it);

	void popBack();
	void popFront();
	void popAt(const size_t& pos);
private:
	void copyFrom(const List& other);
	void free();
};

class List::iterator
{
	friend class List;

	List::Node* ptr;
	const List& owner;
	iterator(List::Node* ptr, const List& creator)
		: ptr(ptr), owner(creator) {}
public:
	iterator(const iterator& other) = default;
	iterator& operator=(const iterator& other) = default;
	~iterator() = default;

	bool valid() const { return ptr; }
	operator bool() const { return ptr; }

	Student& operator*() { return ptr->data; }
	Student* operator->() { return &ptr->data; }
	const Student& operator*() const { return ptr->data; }
	const Student* operator->() const { return &ptr->data; }

	iterator& operator++()
	{
		ptr = ptr->next;
		return *this;
	}
	iterator operator++(int)
	{
		iterator res = *this;
		++*this;
		return res;
	}

	bool operator == (const iterator& rhs) const
	{
		return ptr == rhs.ptr;
	}
	bool operator != (const iterator& rhs) const
	{
		return !(*this == rhs);
	}

	void startAgain() { ptr = owner.head; }
};
