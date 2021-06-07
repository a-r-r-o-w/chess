#ifndef chess_vector_h
#define chess_vector_h

#include "stdlib.h"

#include "exceptions.h"

#define vector(T)                                                           \
                                                                            \
typedef struct {                                                            \
    T*   data;                                                              \
    int  size;                                                              \
    int  reserved_size;                                                     \
} vector_##T;                                                               \
                                                                            \
void vector_##T##_constructor (vector_##T*, int, T);                        \
T    vector_##T##_at          (vector_##T*, int);                           \
void vector_##T##_set         (vector_##T*, int, T);                        \
void vector_##T##_pushback    (vector_##T*, T);                             \
void vector_##T##_destructor  (vector_##T*);                                \
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
    vec->data = malloc(p2 * sizeof(T));                                     \
                                                                            \
    if (vec->data != NULL) {                                                \
        for (int i = 0; i < size; ++i)                                      \
            vec->data[i] = value;                                           \
        vec->size = size;                                                   \
        vec->reserved_size = p2;                                            \
    }                                                                       \
    else                                                                    \
        exception("memory could not be allocated", __FILE__, __LINE__);     \
}                                                                           \
                                                                            \
T vector_##T##_at (vector_##T* vec, int index) {                            \
    if (vec->size < index)                                                  \
        exception("out of bounds", __FILE__, __LINE__);                     \
                                                                            \
    return vec->data[index];                                                \
}                                                                           \
                                                                            \
void vector_##T##_set (vector_##T* vec, int index, T value) {               \
    if (vec->size < index) {                                                \
        vector_##T##_destructor(vec);                                       \
        exception("out of bounds", __FILE__, __LINE__);                     \
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
        T* newdata = malloc(p2 * sizeof(T));                                \
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
            exception("memory could not be allocated", __FILE__, __LINE__); \
        }                                                                   \
    }                                                                       \
}                                                                           \
                                                                            \
T vector_##T##_popback (vector_##T* vec) {                                  \
    if (vec->size == 0) {                                                   \
        vector_##T##_destructor(vec);                                       \
        exception("vector is empty", __FILE__, __LINE__);                   \
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
    vec->size = 0;                                                          \
    vec->reserved_size = 0;                                                 \
}

vector(int)
vector(char)
vector(float)
vector(double)

vector(vector_char)

#endif
