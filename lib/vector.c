#include "string.h"
#include "vector.h"
#include "allocator.h"

Vector* create_vector(Vector *vector, size_t elem_size) {
	if (vector != NULL) {
		free_vector(vector);
	}

	vector = (Vector*)kmalloc(sizeof(Vector));

	if (vector == NULL) return NULL;

	vector->capacity = 32;
	vector->size = 0;
	vector->elem_size = elem_size;
	vector->ptr = kmalloc(vector->elem_size * vector->capacity);

	return vector;
}

void free_vector(Vector *vector) {
	if (vector != NULL) {
		kfree(vector->ptr);
		kfree(vector);

		vector->ptr = NULL;
		vector->size = 0;
		vector->capacity = 0;
		vector->elem_size = 0;
	}
}

static void reserve_vector(Vector *vector, size_t new_capacity) {
	if (vector != NULL) {
		void *buff_ptr = krealloc(vector->ptr, new_capacity);

		if (buff_ptr != NULL) {
			vector->capacity = new_capacity;
			vector->ptr = buff_ptr;
		}
	}
}

void shrink_to_fit_vector(Vector *vector) {
	if (vector != NULL) {
		if (vector->capacity != vector->size) {
			void *buff_ptr = krealloc(vector->ptr, vector->size);

			if (buff_ptr != NULL) {
				vector->ptr = buff_ptr;
				vector->capacity = vector->size;
			}
		}
	}
}

void* push_back_vector(Vector *vector, const void *elem) {
	if (vector != NULL) {
		if (elem != NULL) {
			if (vector->size == vector->capacity) {
				reserve_vector(vector, vector->capacity * 2);
			}

			vector->size++;
			my_memcpy((uint8_t*)vector->ptr + (vector->size * vector->elem_size), elem, vector->elem_size);
		}
	}

	return NULL;
}

void* insert_vector(Vector *vector, int index, const void* elem) {
	if (vector != NULL) {
		if (vector->size == vector->capacity) {
			reserve_vector(vector, vector->capacity * 2);
		}

		if (index < vector->size && index >= 0) {
			vector->size++;

			for (int i = 0; i < vector->size - index - 1; i++) {
				my_memcpy((uint8_t*)vector->ptr + vector->elem_size * (vector->size - 1 - i), (uint8_t*)vector->ptr + vector->elem_size * (vector->size - 2 - i), vector->elem_size);
			}

			my_memcpy((uint8_t*)vector->ptr + index * vector->elem_size, elem, vector->elem_size);
			return vector;
		}
	}

	return NULL;
}

void* erase_vector(Vector *vector, int index) {
	if (vector != NULL) {
		if (index < vector->size && index > -1) {
			for (int i = 0; i < vector->size - index - 1; i++) {
				my_memcpy((uint8_t*)vector->ptr + vector->elem_size * (index + i), (uint8_t*)vector->ptr + vector->elem_size * (index + 1 + i), vector->elem_size);
			}

			vector->size--;
			return vector;
		}
	}

	return NULL;
}

void* pop_back_vector(Vector *vector) {
	if (vector != NULL) {
		if (vector->size > 0) {
			vector->size--;
		}
	}

	return NULL;
}

void* get_elem_vector(Vector *vector, int index) {
	if (vector != NULL) {
		if (index < vector->size && index > -1) {
			return (uint8_t*)vector->ptr + index * vector->elem_size;
		}
	}

	return NULL;
}

void* set_elem_vector(Vector *vector, int index, const void* new_elem) {
	if (vector != NULL) {
		if (index < vector->size && index > -1) {
			my_memcpy((uint8_t*)vector->ptr + index * vector->elem_size, new_elem, vector->elem_size);
		}
	}
	return NULL;	
}

size_t get_capacity_vector(Vector *vector) {
	if (vector != NULL) return vector->capacity;
}

size_t get_size_vector(Vector *vector) {
	if (vector != NULL) return vector->size;
}

size_t get_elem_size_vector(Vector *vector) {
	if (vector != NULL) return vector->elem_size;
} 

int empty_vector(Vector *vector) {
	if (vector != NULL) {
		if (vector->size == 0) return 1;
		return 0;
	}

	return -1;
}

void* get_last_elem_vector(Vector *vector) {
	if (vector != NULL && vector->size > 0) return (uint8_t*)vector->ptr + (vector->size - 1) * vector->elem_size;
}