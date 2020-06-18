#ifndef __COMPARATOR_H
#define __COMPARATOR_H

#include <stddef.h>

typedef int (*IntComparator)(const int a, const int b);
int int_asc(const int a, const int b);
int int_desc(const int a, const int b);

typedef int (*StringComparator)(const char *a, const char *b);
/* We don't need to define a StringComparator because strcmp is one! */

typedef int (*Comparator)(const void *a, const void *b);
int int_p_asc(const int *a, const int *b);
int int_p_desc(const int *a, const int *b);

#endif /* __COMPARATOR_H */
