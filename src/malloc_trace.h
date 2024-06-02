#ifndef MYMALLOC
#define MYMALLOC

#include <stddef.h>
#define malloc(size) my_malloc(size, __FILE__, __LINE__)
#define calloc(count, size) my_calloc(count, size, __FILE__, __LINE__)
#define free(x) my_free(__FILE__, __LINE__, x)

void* my_malloc(size_t size, const char* file, int line);
void* my_calloc(size_t count, size_t size, const char* file, int line);
void  my_free(const char* file, int line, void* ptr);

#endif 
