#include "array_edited.h"
#include <vector>

const int INF = -1000000;
const float FINF = 1000000;

template<typename T>
inline ArrayIterator<T>::ArrayIterator(PointerType ptr) {
    it_ptr = ptr;
}

template<typename T>
inline ArrayIterator<T>& ArrayIterator<T>::operator++() {
    it_ptr++;
    return *this;
}

template<typename T>
inline ArrayIterator<T> ArrayIterator<T>::operator++(int) {
    ArrayIterator iterator_cpy = *this;
    ++(*this);
    return iterator_cpy;
}

template<typename T>
inline bool ArrayIterator<T>::operator==(ArrayIterator other) {
    return (it_ptr == other.it_ptr);
}

template<typename T>
inline bool ArrayIterator<T>::operator!=(ArrayIterator other) {
    return !(it_ptr == other.it_ptr);
}

// integer array
template<>
inline Array<Integer>::Array(int a[], int a_size, int arr_size, int _party, int _int_size) {
    int_size = _int_size;
    party = _party;
    array_size = arr_size;
    for (int i=0; i<arr_size; i++) {
        //Integer integer_num {int_size, a[i], party};
        if (i<a_size) {
            Integer val = Integer (int_size, a[i], party);
            arr.push_back(val);
        }
        else {
            Integer val = Integer (int_size, INF, party);
            arr.push_back(val);
        }
    }
}

// initializes array to all INF's 
template <>
inline Array<Integer>::Array(int arr_size, int _party, int _int_size) {
    int_size = _int_size;
    party = _party;
    array_size = arr_size;
    for (int i=0; i<arr_size; i++) {
        Integer val = Integer (int_size, INF, party);
        arr.push_back(val);
    }
}


// get array size
template <typename T>
inline size_t Array<T>::get_size() const {
    return arr.size();
}


// append
template <>
inline void Array<Integer>::append(Integer val) {
    //Integer val = Integer (int_size, num, party);
    arr.push_back(val);
}


// index
template <typename T>
inline T Array<T>::operator[](size_t ind) {
    Integer sum = Integer(int_size, 0, party);
    Integer ZERO = Integer(int_size, 0, party);
    for (int i=0; i<arr.size(); i++) {
        Bit cond = i==ind;
        sum = sum + ZERO.If(cond, arr[i]);
    }
    return sum;

}

template <typename T>
inline ArrayIterator<T> Array<T>::begin() {
    return ArrayIterator<T>(arr.data());

}

template <typename T>
inline ArrayIterator<T> Array<T>::end() {
    return ArrayIterator<T>(arr.data()+arr.size());
}