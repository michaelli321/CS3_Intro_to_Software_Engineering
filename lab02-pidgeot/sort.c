#include "sort.h"

void ascending_int_sort_whole(int arr[], size_t nelements) {
	ascending_int_sort(arr, 0, nelements);
}

void ascending_int_sort(int arr[], size_t lo, size_t hi) {
	int_sort(arr, lo, hi, int_asc);
}

void descending_int_sort(int arr[], size_t lo, size_t hi) {
	int_sort(arr, lo, hi, int_desc);
}

void int_sort(int arr[], size_t lo, size_t hi, IntComparator compare) {
	for (size_t i = lo; i < hi-1; i++) {
		if (compare(arr[i], arr[i+1]) > 0) {
			int j = i;
			while(1) {
				int temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;

				j -= 1;
				if (j < 0 || compare(arr[j], arr[j+1]) < 0) {
					break;
				}
			}
		}
	}
}

void string_sort(char *arr[], size_t lo, size_t hi, StringComparator compare) {
	sort((void **)arr, lo, hi, (Comparator)compare);
}

void sort(void *arr[], size_t lo, size_t hi, Comparator compare) {
	for (size_t i = lo; i < hi-1; i++) {
		if (compare(arr[i], arr[i+1]) > 0) {
			int j = i;
			while(1) {
				void *temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;

				j -= 1;
				if (j < 0 || compare(arr[j], arr[j+1]) < 0) {
					break;
				}
			}
		}
	}
}
