/*
 * flatcc_alloc.c
 *
 *  Created on: 2 дек. 2017 г.
 *      Author: shurik
 */
#include <flatcc_alloc.h>

void* __aligned_malloc(size_t alignment, size_t bytes)
{
	void *p1 ,*p2;
		if((p1 =(void *) malloc(bytes + alignment + sizeof(void*)))==NULL)
			return NULL;
		size_t addr=(size_t)p1+alignment+sizeof(size_t);
		p2=(void *)(addr - (addr%alignment));
		*((size_t *)p2-1)=(size_t)p1;
		return p2;
}

void __aligned_free(void *p )
{
	free((void *)(*((size_t *) p-1)));
}
