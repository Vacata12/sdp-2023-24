#include "List.h"
#include<iostream>

List::List(const List& other)
{
	copyFrom(other);
}
List& List::operator=(const List& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}
List::~List()
{
	free();
}

List::iterator List::begin()
{
	return iterator(head, *this);
}
List::iterator List::end() const
{
	return iterator(nullptr, *this);
}

const Student& List::front() const
{
	return head->data;
}
const Student& List::back() const
{
	return tail->data;
}
const Student& List::getAt(const size_t& pos) const
{
	if (pos >= size)
		throw std::invalid_argument("Index out of range!\n");
	Node* iter = head;
	for (unsigned i = 0; i < pos; ++i, iter = iter->next);
	return iter->data;
}

bool List::isEmpty() const
{
	return size == 0;
}
size_t List::getSize() const
{
	return size;
}

void List::pushBack(const Student& st)
{
	Node* push = new Node(st);
	if (!tail)
	{
		head = tail = push;
		size++;
		return;
	}
	tail->next = push;
	tail = push;
	size++;
}
void List::pushFront(const Student& st)
{
	Node* push = new Node(st);
	if (!head)
	{
		head = tail = push;
		size++;
		return;
	}
	push->next = head;
	head = push;
	++size;
}
void List::pushAt(const size_t& pos, const Student& st)
{
	if (pos > size)
		throw std::invalid_argument("Index out of range!\n");
	Node* push = new Node(st);
	if (pos == 0)
		pushFront(st);
	else if (pos == size)
		pushBack(st);
	else
	{
		Node* iter = head;
		for (int i = 1; i < pos; ++i, iter = iter->next);
		Node* tmp = iter->next;
		iter->next = push;
		push->next = tmp;
		size++;
	}
}

void List::pushAfter(const Student& value, iterator it)
{
	Node* tmp = it.ptr->next;
	if (&it.owner != this)
	{
		throw std::invalid_argument("That iter is not from that list!\n");
		return;
	}
	it.ptr->next = new Node(value);
	it.ptr->next->next = tmp;
	if (it.ptr == tail)
		tail = it.ptr->next;
	++size;
}

void List::popBack()
{
	popAt(0);
}
void List::popFront()
{
	popAt(size - 1);
}
void List::popAt(const size_t& pos)
{
	if (pos > size)
		throw std::invalid_argument("Index out of range!\n");
	if (pos == 0)
	{
		Node* toDelete = head;
		head = head->next;
		--size;
		if (head == nullptr)
			tail = nullptr;
		delete toDelete;
		return;
	}
	Node* iter = head;
	for (size_t i = 0; i < pos - 1; ++i, iter = iter->next);
	Node* toDelete = iter->next;
	iter->next = toDelete->next;
	delete toDelete;
	--size;
	if (size == 1)
		tail = head;
}

void List::copyFrom(const List& other)
{
	Node* iter = other.head;
	while (iter)
	{
		this->pushBack(iter->data);
		iter = iter->next;
	}
	size = other.size;
}
void List::free()
{
	Node* toDelete = head;
	Node* iter = head;
	while (iter)
	{
		iter = iter->next;
		delete toDelete;
		toDelete = iter;
	}
	head = tail = nullptr;
	size = 0;
}
