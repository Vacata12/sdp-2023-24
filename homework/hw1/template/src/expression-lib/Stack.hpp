#pragma once
#include <iostream>

template<typename T>
class Stack
{
private:
    T* stack;
    size_t currElement;
    size_t size;
public:
    Stack();
    Stack(const size_t& size);
    Stack(const Stack& ohter);
    Stack<T>& operator=(const Stack& other);
    ~Stack();

    size_t getSize() const;
    size_t getCurrSize() const; // function that returns the number of elements in the stack
    bool isEmpty() const;

    void push(const T& newElem); // adds new element at the end of the stack
    void pop(); // removes the newest element

    const T& operator[](size_t index) const;
    T& operator[](size_t index);

    const T& top() const; // return first element in the stack
    const T& peek() const; // return the newest element in the stack

private:
    void free();
    void copyFrom(const Stack& other);
    void resize(const size_t& newSize);

};




template<typename T>
Stack<T>::Stack() : currElement(0), size(4)
{
    stack = new T[size];
}
template<typename T>
Stack<T>::Stack(const size_t& size)
{
    currElement = 0;
    this->size = size;
    stack = new T[size];
}
template<typename T>
Stack<T>::Stack(const Stack& other)
{
    copyFrom(other);
}
template<typename T>
Stack<T>& Stack<T>::operator=(const Stack& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}
template<typename T>
Stack<T>::~Stack()
{
    free();
}

template<typename T>
size_t Stack<T>::getSize() const
{
    return size;
}
template<typename T>
size_t Stack<T>::getCurrSize() const
{
    return currElement;
}
template<typename T>
bool Stack<T>::isEmpty() const
{
    return currElement == 0;
}

template<typename T>
void Stack<T>::push(const T& newElem)
{
    if (currElement >= size)
        resize(size * 2);
    stack[currElement++] = newElem;
}
template<typename T>
void Stack<T>::pop()
{
    if (currElement == 0)
        throw std::length_error("It's already empty!\n");
    currElement -= 1;
    if (currElement * 4 <= size && size > 1)
        resize(size / 2);
}

template<typename T>
const T& Stack<T>::operator[](size_t index) const
{
    if (index > size)
        throw std::out_of_range("Out of range!\n");
    return stack[index];
}
template<typename T>
T& Stack<T>::operator[](size_t index)
{
    if (index > size)
        throw std::out_of_range("Out of range!\n");
    return stack[index];
}

template<typename T>
const T& Stack<T>::top() const
{
    return stack[0];
}
template<typename T>
const T& Stack<T>::peek() const
{
    if (isEmpty())
        throw std::out_of_range("The stack is empty!\n");
    size_t index = currElement - 1;
    return stack[index];
}

template<typename T>
void Stack<T>::free()
{
    delete[] stack;
}
template<typename T>
void Stack<T>::copyFrom(const Stack& other)
{
    stack = new T[other.size];
    for (size_t i = 0; i < other.size; i++)
    {
        stack[i] = other.stack[i];
    }
    currElement = other.currElement;

}
template<typename T>
void Stack<T>::resize(const size_t& newSize)
{
    T* temp = stack;
    stack = new T[newSize];
    for (size_t i = 0; i < currElement; i++)
    {
        stack[i] = temp[i];
    }
    size = newSize;
    delete[] temp;
}
