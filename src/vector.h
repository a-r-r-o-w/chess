/**
 * Chess
 * 
 * Written by:
 *    Aryan V S         (PES1UG20CS083)
 *    Aryansh Bhargavan (PES1UG20CS084)
 *    Avanish Bhat      (PES1UG20CS092)
 */

#ifndef chess_vector_h
#define chess_vector_h

#include "stdlib.h"
#include "stdint.h"

#include "exception.h"

/**
 * Macro based implementation of a dynamic array of arbitrary type
 * similar to C++ std::vector
 */

#define vector_t(T)                                                         \
                                                                            \
typedef struct {                                                            \
    T*   data;                                                              \
    int  size;                                                              \
    int  reserved_size;                                                     \
} vector_##T;                                                               \
                                                                            \
void     vector_##T##_constructor (vector_##T*, int, T);                    \
T        vector_##T##_at          (vector_##T*, int);                       \
void     vector_##T##_set         (vector_##T*, int, T);                    \
void     vector_##T##_pushback    (vector_##T*, T);                         \
T        vector_##T##_popback     (vector_##T*);                            \
const T* vector_##T##_getdata     (vector_##T*);                            \
void     vector_##T##_destructor  (vector_##T*);                            \
                                                                            \
void vector_##T##_constructor (vector_##T* vec, int size, T value) {        \
    vec->data          = NULL;                                              \
    vec->size          = 0;                                                 \
    vec->reserved_size = 0;                                                 \
                                                                            \
    int p2 = 1;                                                             \
    while (p2 < size)                                                       \
        p2 *= 2;                                                            \
                                                                            \
    vec->data = (T*)malloc(p2 * sizeof(T));                                 \
                                                                            \
    if (vec->data != NULL) {                                                \
        for (int i = 0; i < size; ++i)                                      \
            vec->data[i] = value;                                           \
        vec->size = size;                                                   \
        vec->reserved_size = p2;                                            \
    }                                                                       \
    else                                                                    \
        EXCEPTION("memory could not be allocated");                         \
}                                                                           \
                                                                            \
T vector_##T##_at (vector_##T* vec, int index) {                            \
    if (vec->size < index) {                                                \
        vector_##T##_destructor(vec);                                       \
        EXCEPTION("out of bounds");                                         \
    }                                                                       \
                                                                            \
    return vec->data[index];                                                \
}                                                                           \
                                                                            \
void vector_##T##_set (vector_##T* vec, int index, T value) {               \
    if (vec->size < index) {                                                \
        vector_##T##_destructor(vec);                                       \
        EXCEPTION("out of bounds");                                         \
    }                                                                       \
                                                                            \
    vec->data[index] = value;                                               \
}                                                                           \
                                                                            \
void vector_##T##_pushback (vector_##T* vec, T value) {                     \
    if (vec->size < vec->reserved_size) {                                   \
        vec->data[vec->size] = value;                                       \
        vec->size += 1;                                                     \
    }                                                                       \
    else {                                                                  \
        int p2 = vec->reserved_size * 2;                                    \
        T* newdata = (T*)malloc(p2 * sizeof(T));                            \
                                                                            \
        if (newdata != NULL) {                                              \
            for (int i = 0; i < vec->size; ++i)                             \
                newdata[i] = vec->data[i];                                  \
            free(vec->data);                                                \
            vec->data = newdata;                                            \
            vec->data[vec->size] = value;                                   \
            vec->size += 1;                                                 \
            vec->reserved_size = p2;                                        \
        }                                                                   \
        else {                                                              \
            vector_##T##_destructor(vec);                                   \
            EXCEPTION("memory could not be allocated");                     \
        }                                                                   \
    }                                                                       \
}                                                                           \
                                                                            \
T vector_##T##_popback (vector_##T* vec) {                                  \
    if (vec->size == 0) {                                                   \
        vector_##T##_destructor(vec);                                       \
        EXCEPTION("vector is empty");                                       \
    }                                                                       \
                                                                            \
    vec->size -= 1;                                                         \
    return vec->data[vec->size];                                            \
}                                                                           \
                                                                            \
const T* vector_##T##_getdata (vector_##T* vec) {                           \
    return (const T*)(vec->data);                                           \
}                                                                           \
                                                                            \
void vector_##T##_destructor (vector_##T* vec) {                            \
    free(vec->data);                                                        \
    vec->data = NULL;                                                       \
    vec->size = 0;                                                          \
    vec->reserved_size = 0;                                                 \
}

#define vector(type) vector_##type

vector_t(int)
vector_t(char)

#endif
