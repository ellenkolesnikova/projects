// insert ifndef + define statements (figure out what they do)

#ifndef EMP_ARRAY_H__
#define EMP_ARRAY_H__


#include "emp-tool/circuits/bit.h"
#include "emp-tool/circuits/number.h"
#include "emp-tool/circuits/comparable.h"
#include "emp-tool/circuits/swappable.h"
#include "emp-tool/circuits/integer.h"
#include <vector>
#include <bitset>
#include <algorithm>
#include <math.h>

using std::vector;
namespace emp {
    template<typename T>
    class Array;

    template<typename T>
    class ArrayIterator { public:
        using ValueType = T;
        using PointerType = ValueType*;
        using ReferenceType = ValueType&;

        ArrayIterator(PointerType ptr);

        ArrayIterator& operator++();
        
        ArrayIterator operator++(int);

        bool operator==(ArrayIterator other);


        bool operator!=(ArrayIterator other);

        ReferenceType operator*() {
            return *it_ptr;
        }

        ReferenceType operator[](int index) {
            return *(it_ptr+index);
        }

        PointerType operator->() {
            return it_ptr;
        }
        



    private:
        PointerType it_ptr;

    };
    template<typename T>
    class Array { public:
        using ValueType = T;
        using Iterator = ArrayIterator<Array<T>>;

        vector<T> arr;
        int int_size;
        int party;
        int array_size;

        Array(int _party) {
            int_size = 32;
            party = _party;
        }
        Array(int a[], int a_size, int arr_size, int _party, int _int_size);

        Array(int arr_size, int party, int int_size);

        //Array(float a[], int _party);

        size_t get_size() const;

        void append(Integer val);

        T operator[](size_t ind);

        ArrayIterator<T> begin();

        ArrayIterator<T> end();


    };

    
    #include "array_edited.hpp"
}
// EMP_ARRAY_H__
#endif