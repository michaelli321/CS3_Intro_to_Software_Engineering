#ifndef __SORT_H
#define __SORT_H

#include <stddef.h>
#include "comparator.h"

void ascending_int_sort_whole(int arr[], size_t nelements);
void ascending_int_sort(int arr[], size_t lo, size_t hi);
void descending_int_sort(int arr[], size_t lo, size_t hi);

void int_sort(int arr[], size_t lo, size_t hi, IntComparator compare);
void string_sort(char *arr[], size_t lo, size_t hi, StringComparator compare);

void sort(void *arr[], size_t lo, size_t hi, Comparator compare);

#endif /* __SORT_H */
