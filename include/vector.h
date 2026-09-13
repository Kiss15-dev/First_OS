#ifndef VECTOR_H
#define VECTOR_H

#include "types.h"

typedef struct {
	size_t capacity;
	size_t size;
	size_t elem_size;
	void *ptr;
} Vector;

Vector* create_vector(Vector *vector, size_t elem_size);
void free_vector(Vector *vector);
void shrink_to_fit_vector(Vector *vector);
void* push_back_vector(Vector *vector, const void *elem);
void* insert_vector(Vector *vector, int index, const void* elem);
void* erase_vector(Vector *vector, int index);
void* pop_back_vector(Vector *vector);
void* get_elem_vector_vector( Vector *vector, int index);
void* set_elem_vector(Vector *vector, int index, const void* new_elem);
size_t get_capacity_vector(Vector *vector);
size_t get_size_vector(Vector *vector);
size_t get_elem_size_vector( Vector *vector);
int empty_vector(Vector *vector);
void* get_last_elem_vector(Vector *vector);

#endif