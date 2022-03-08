
#ifndef _DATA_ARRAY_H_
#define _DATA_ARRAY_H_

#include "stdlib.h"

typedef struct DA {
    size_t capacity;
    size_t length;
    size_t * data;
} DataArray;

void DA_init(DataArray* da, size_t capacity);

void DA_insert_back(DataArray* da, size_t value);

size_t DA_length(const DataArray* da);

size_t DA_value(const DataArray* da, size_t pos);

void DA_free(DataArray * da);


#endif //_DATA_ARRAY_H_
