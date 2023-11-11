#pragma once
#include <iostream>

template<typename T>
class DynamicArray
{
private:
    T* arr;
    size_t size;
    size_t capacity;
public:
    DynamicArray();
    DynamicArray(const size_t& capacity);
    DynamicArray(std::initializer_list<T> list);
    DynamicArray(const DynamicArray<T>& other);
    DynamicArray<T>& operator=(const DynamicArray<T>& other);
    ~DynamicArray();
private:
    void copyFrom(const DynamicArray<T>& other);
    void free();
    void resize(size_t newCap);
public:
    void pushBack(const T& newElem); // add a new element in the end
    void popBack(); // removes the last element

    size_t getSize() const;
    bool isEmpty() const;

    void reverse();

    const T& getLowest() const;
    const T& getMax() const;

    const T& operator[](size_t index) const;
    T& operator[](size_t index);
};

template<typename T>
DynamicArray<T>::DynamicArray() : size(0), capacity(4)
{
    arr = new T[capacity];
} 
template<typename T>
DynamicArray<T>::DynamicArray(const size_t& cap) : size(0), capacity(cap)
{
    arr = new T[capacity];
}
// This constructor allow to make array like that DynamicArray<T> name = {value, value, ..};
template<typename T>
DynamicArray<T>::DynamicArray(std::initializer_list<T> list)
{
    size = list.size();
    arr = new T[size];
    int index = 0;
    for(T el : list)
    {
        arr[index] = el;
        index++;
    }
}
template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other)
{
    copyFrom(other);
}
template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other)
{
    if(this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}
template<typename T>
DynamicArray<T>::~DynamicArray()
{
    free();
}
template<typename T>
void DynamicArray<T>::copyFrom(const DynamicArray<T>& other)
{
    this->size = other.size;
    this->capacity = other.capacity;
    this->arr = new T[other.capacity];
    for (size_t i = 0; i < capacity; i++)
    {
        this->arr[i] = other.arr[i];
    }
    
}
template<typename T>
void DynamicArray<T>::free()
{
    delete[] arr;
}
template<typename T>
void DynamicArray<T>::resize(size_t newCap)
{
    T* temp = arr;
    arr = new T[newCap];
    for (size_t i = 0; i < size; i++)
    {
        this->arr[i] = temp[i];
    }
    capacity = newCap;
    delete[] temp;
}

template<typename T>
void DynamicArray<T>::pushBack(const T& newElem)
{
    if(capacity == size)
        resize(capacity * 2);
    arr[size++] = newElem;
}
template<typename T>
void DynamicArray<T>::popBack()
{
    if(size)
        size--;
    else
        throw std::length_error("Already empty!\n");
    if (size * 4 <= capacity && capacity > 1)
		resize(capacity / 2);
}

template<typename T>
size_t DynamicArray<T>::getSize() const
{
    return size;
}
template<typename T>
bool DynamicArray<T>::isEmpty() const
{
    return size == 0;
}

template<typename T>
void DynamicArray<T>::reverse()
{
    unsigned _size = size / 2;
    for (size_t i = 0, j = (size - 1); i < _size; i++, j--)
    {
        std::swap(arr[i], arr[j]);
    }
}

template<typename T>
const T& DynamicArray<T>::getLowest() const
{
    T lowest = arr[0];
    for (size_t i = 1; i < size; i++)
    {
        if(arr[i] > lowest)
            lowest = arr[i];
    }
    return lowest;
}
template<typename T>
const T& DynamicArray<T>::getMax() const
{
    T max = arr[0];
    for (size_t i = 1; i < size; i++)
    {
        if(arr[i] > max)
            max = arr[i];
    }
    return max;
}

template<typename T>
const T& DynamicArray<T>::operator[](size_t index) const
{
    if(index > size)
        throw std::out_of_range("Out of range!\n");
    return this->arr[index];
}
template<typename T>
T& DynamicArray<T>::operator[](size_t index)
{
    if(index > size)
        throw std::out_of_range("Out of range!\n");
    return this->arr[index];
}


