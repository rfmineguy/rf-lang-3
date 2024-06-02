#include "malloc_trace.h"
#include <dlfcn.h>
#include <stdio.h>

void* my_malloc(size_t size, const char* file, int line) {
	static void* (*realmalloc)(size_t) = NULL;
	if (!realmalloc) {
		realmalloc = dlsym(RTLD_NEXT, "malloc");
	}

	void* p = realmalloc(size);
	printf("malloc [%s](%d)  %p\n", file, line, p);
	return p;
}

void* my_calloc(size_t count, size_t size, const char* file, int line) {
	static void* (*realcalloc)(size_t) = NULL;
	if (!realcalloc) {
		realcalloc = dlsym(RTLD_NEXT, "calloc");
	}

	void* p = realcalloc(size);
	printf("calloc [%s](%d)  %p\n", file, line, p);
	return p;
}

void my_free(const char* file, int line, void* ptr) {
	static void (*realfree)(void*) = NULL;
	char* error;
	if (!realfree) {
		realfree = dlsym(RTLD_NEXT, "free");
		if ((error = dlerror()) != NULL) {
			fputs(error, stderr);
			return;
		}
	}
	printf("free [%s](%d)  %p\n", file, line, ptr);
	realfree(ptr);
}
