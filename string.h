#ifndef STRING_H
#define STRING_H

#include "types.h"
#include "vector.h"

void *my_memcpy(void *dest, const void *scr, size_t n);
void* my_memset(void *scr, int c, size_t n);
uint8_t my_memcmp(const void *scr1, const void *scr2, size_t n);
void* my_memchr(const void* scr, int c, size_t n);
size_t my_strlen(char* str);
void* str_append(Vector *vector, char* str);
void str_lower(char* str);
void str_upper(char* str);
void* str_terminate(Vector *vector);
int strcmp(char *str1, char *str2);
int strncmp(char *str1, char *str2, size_t n);

#endif