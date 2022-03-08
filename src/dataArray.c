#include "dataArray.h"

#include <assert.h>
#include <stdio.h>

#define DA_CAPACITY_COEFF 2;

void DA_init(DataArray* da, size_t capacity) {
  da->capacity = capacity;
  da->length = 0;
  da->data = (size_t*)malloc(capacity * sizeof(size_t));
}

void DA_insert_back(DataArray* da, size_t value){
    if(da->length == da->capacity){
        //fprintf(stderr, "DEBUG: capacity %ld ; len %ld\n", da->capacity, da->length);

        da->capacity = da->capacity * DA_CAPACITY_COEFF;
        da->data = realloc(da->data, da->capacity*sizeof(size_t));
        assert(da->data != NULL);
    }
    da->data[da->length] = value;
    da->length ++;
}

size_t DA_length(const DataArray* da){
    return da->length;
}

size_t DA_value(const DataArray* da, size_t pos){
    return da->data[pos];
}

void DA_free(DataArray* da) {
    if(da != NULL){
        free(da->data);
        da->capacity = 0;
        da->length = 0;
    }
}
