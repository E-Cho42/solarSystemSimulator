#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "List.hpp"

#include <exception>
#include <iostream>
#include <string> 
#include <sstream> // Needed for joining arrays into strings

template<typename T>
class Array final : public IList<T> {
public:
    Array();
    ~Array();

    int size() const override;
    T get(const int POS) const override;
    void set(const int POS, const T VALUE) override;
    void insert(int POS, const T VALUE) override;
    void remove(int POS) override;
    T min() const override;
    T max() const override;
    int find(const T VALUE) const override;
    void sort() override;
    int search(const T VALUE) const override;

private:
    int _size;
    T* _pArray;

    // Helper function to print a portion of the array
    void printArray(T* arr, int start, int end) const;

    // Merge sort helper function
    void mergeSort(T* arr, int left, int right);
};

//---------------------------------------------------------

template<typename T>
Array<T>::Array() {
    // set size to zero
    _size = 0;

    // set array to be a nullptr
    _pArray = nullptr;
}

template<typename T>
Array<T>::~Array() {
    // deallocate current array
    delete[] _pArray;
}

template<typename T>
int Array<T>::size() const { 
    return _size;
}

template<typename T>
T Array<T>::get(const int POS) const {
    // if POS is out of range, throw std::out_of_range exception
    if (POS < 0 || POS >= _size) {
        throw std::out_of_range("Index is out of range");
    }

    // return value at POS within array
    return _pArray[POS];
}

template<typename T>
void Array<T>::set(const int POS, const T VALUE) {
    // if POS is out of range, throw std::out_of_range exception
    if (POS < 0 || POS >= _size) {
        throw std::out_of_range("Index is out of range");
    }

    // set VALUE at POS within array    
    _pArray[POS] = VALUE;
}

template<typename T>
void Array<T>::insert(int POS, const T VALUE) {
    // if POS is before zero, clamp to zero
    if (POS < 0) {
        POS = 0;
    }

    // if POS is after size, clamp to size
    if (POS >= _size) {
        POS = _size;
    }

    // create new array of size + 1
    T* _newArray = new T[_size + 1];

    // copy elements 0 to POS from old array to new array
    for (int i = 0; i < POS; i++) {
        _newArray[i] = _pArray[i];
    }

    // set element at POS in new array to VALUE
    _newArray[POS] = VALUE;

    // copy elements POS to size from old array to new array
    for (int i = POS; i < _size; i++) {
        _newArray[i + 1] = _pArray[i];
    }

    // delete old array
    delete[] _pArray;

    // set old array to be new array
    _pArray = _newArray;

    // increment size
    _size++;
}

template<typename T>
void Array<T>::remove(int POS) {
    // if array is empty, throw std::out_of_range exception
    if (_size == 0 || _pArray == nullptr) {
        throw std::out_of_range("Index is out of range");
    }

    // if POS is before zero, clamp to zero
    if (POS < 0) {
        POS = 0;
    }

    // if POS is after size, clamp to size
    if (POS >= _size) {
        POS = _size - 1;
    }

    // create new array of size - 1
    T* _newArray = new T[_size - 1];

    // copy elements from 0 to POS from old array to new array
    for (int i = 0; i < POS; i++) {
        _newArray[i] = _pArray[i];
    }

    // copy elements from POS+1 to size from old array to new array
    for (int i = POS + 1; i < _size; i++) {
        _newArray[i - 1] = _pArray[i];
    }

    // delete old array
    delete[] _pArray;

    // set old array to be new array
    _pArray = _newArray;

    // decrement size
    _size--;
}

template<typename T>
T Array<T>::max() const {
    // if array is empty, throw std::out_of_range exception
    if (_size == 0 || _pArray == nullptr) {
        throw std::out_of_range("Index is out of range");
    }

    // find max value within array
    T max = _pArray[0];

    for (int i = 0; i < _size; i++) {
        if (_pArray[i] > max) {
            max = _pArray[i];
        }
    }

    // return max value
    return max;
}

template<typename T>
T Array<T>::min() const {
    // if array is empty, throw std::out_of_range exception
    if (_size == 0 || _pArray == nullptr) {
        throw std::out_of_range("Index is out of range");
    }

    // find min value within array
    T min = _pArray[0];

    for (int i = 0; i < _size; i++) {
        if (min > _pArray[i]) {
            min = _pArray[i];
        }
    }

    // return min value
    return min;
}

template<typename T>
int Array<T>::find(const T VALUE) const {
    // search for first occurrence of VALUE
    int index = -1;
    for (int i = 0; i < _size; i++) {
        if (_pArray[i] == VALUE) {
            index = i;
            break;
        }
    }

    // return index of first occurrence if found
    return index;
}

template<typename T>
void Array<T>::printArray(T* arr, int start, int end) const {
    for (int i = start; i <= end; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

template<typename T>
void Array<T>::sort() {
    std::cout << "Sorting an array:\nInitial array: ";
    printArray(_pArray, 0, _size - 1);

    if (_size > 1) {
        mergeSort(_pArray, 0, _size - 1);
    }
}

template<typename T>
void Array<T>::mergeSort(T* arr, int left, int right) {
    if (left >= right) return;

    std::cout << "\nInitial array: ";
    printArray(arr, left, right);

    int mid = left + (right - left) / 2;

    std::cout << "Left:  ";
    printArray(arr, left, mid);
    std::cout << "Right: ";
    printArray(arr, mid + 1, right);

    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    // Merging
    int n1 = mid - left + 1;
    int n2 = right - mid;

    T* L = new T[n1];
    T* R = new T[n2];

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    std::cout << "Merged: ";
    printArray(arr, left, right);

    delete[] L;
    delete[] R;
}


template<typename T>
int Array<T>::search(const T VALUE) const {
    // First, ensure the array is sorted before performing binary search
    
    std::cout << "Searching for " << VALUE << " in the sorted array:\n";

    // Perform binary search
    int low = 0;
    int high = _size - 1;
    int mid;
    
    while (low <= high) {
        mid = low + (high - low) / 2; // To prevent overflow

        // Check if the target value is at the middle
        if (_pArray[mid] == VALUE) {
            std::cout << "Found " << VALUE << " at index " << mid << "\n";
            return mid;
        }

        // If the value is smaller, ignore the right half
        if (_pArray[mid] > VALUE) {
            high = mid - 1;
        }
        // If the value is larger, ignore the left half
        else {
            low = mid + 1;
        }
    }

    // If we reach here, the value was not present
    std::cout << VALUE << " not found in the array.\n";

    return -1;
}

#endif//ARRAY_H
