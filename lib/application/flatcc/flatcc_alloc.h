#ifndef FLATCC_ALLOC_H
#define FLATCC_ALLOC_H
#pragma once
/*
 * These allocation abstractions are __only__ for runtime libraries.
 *
 * The flatcc compiler uses Posix allocation routines regardless
 * of how this file is configured.
 *
 * This header makes it possible to use systems where malloc is not
 * valid to use. In this case the portable library will not help
 * because it implements Posix / C11 abstractions.
 *
 * Systems like FreeRTOS do not work with Posix memory calls and here it
 * can be helpful to override runtime allocation primitives.
 *
 * In general, it is better to customize the allocator and emitter via
 * flatcc_builder_custom_init and to avoid using the default emitter
 * specific high level calls the copy out a buffer that must later be
 * deallocated. This provides full control of allocation withou the need
 * for this file.
 *
 *
 * IMPORTANT
 *
 * If you override malloc, free, etc., make sure your applications
 * use the same allocation methods. For example, samples/monster.c
 * and several test cases are no longer guaranteed to work out of the
 * box.
 *
 * The changes must only affect target runtime compilation including the
 * the runtime library libflatccrt.
 *
 * The host system flatcc compiler and the compiler library libflatcc
 * should NOT be compiled with non-Posix allocation since the compiler
 * has a dependency on the runtime library and the wrong free operation
 * might be callled. The safest way to avoid this problem this is to
 * compile flatcc with the CMake script and the runtime files with a
 * dedicated build system for the target system.
 */

#include <stdlib.h>

#ifndef FLATCC_ALLOC
#define FLATCC_ALLOC(n) malloc(n)
#endif

#ifndef FLATCC_FREE
#define FLATCC_FREE(p) free(p)
#endif

#ifndef FLATCC_REALLOC
#define FLATCC_REALLOC(p, n) realloc(p, n)
#endif

#ifndef FLATCC_CALLOC
#define FLATCC_CALLOC(nm, n) calloc(nm, n)
#endif

void* __aligned_malloc(size_t alignment, size_t bytes);
void __aligned_free(void *p );

#define FLATCC_ALIGNED_ALLOC(alignment, size) __aligned_malloc(alignment, size)
#define FLATCC_ALIGNED_FREE(p) __aligned_free(p)
#define aligned_alloc(alignment, size) __aligned_malloc(alignment, size)
#define aligned_free(p) __aligned_free(p)
#define __aligned_alloc_is_defined 1
#define __aligned_free_is_defined 1

#endif /* FLATCC_ALLOC_H */
