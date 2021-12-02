#ifndef _X_OS_H_
#define _X_OS_H_

#ifndef u_int32_t
typedef unsigned int u_int32_t;
//#define u_int32_t unsigned int
#endif

#ifndef int32_t
#define int32_t int
#endif

#ifndef u_int16_t
typedef unsigned int u_int16_t;
#endif

#ifndef u_int8_t
#define u_int8_t unsigned char
#endif

#ifndef u_int8_t
#define u_int8_t unsigned char
#endif

//typedef unsigned int size_t;
//#ifndef _SIZE_T
//#define _SIZE_T
typedef unsigned int size_t;
//#define unsigned int size_t
//#endif

#ifndef UINT_MAX
#define UINT_MAX    ((sizeof(int) == 2) ? 65535 : 4294967295UL)
#endif  /* UINT_MAX */

#ifndef INT_MAX
#define INT_MAX     ((sizeof(int) == 2) ? 32767 : 2147483647UL)
#endif /* INT_MAX */

#define x_mem_alloc     malloc
#define x_mem_calloc    calloc
#define x_mem_free      free
#define x_mem_realloc   realloc
#define x_memcpy        memcpy
#define x_memset        memset
#define x_memcmp        memcmp
#define x_strcmp        strcmp
#define x_strlen        strlen

void *tom_malloc(size_t size);
void *tom_realloc(void *ptr, size_t size);
void *tom_calloc(size_t nelem, size_t size);
void tom_free(void *ptr);
void *tom_memset(void *s, int c, size_t n);
void *tom_memcpy(void *s1, const void *s2, size_t n);
int tom_memcmp(const void *s1, const void *s2, size_t n);
int tom_strcmp(const char *s1, const char *s2);
void tom_memscrub( void *io_buf, size_t in_sz );
void * tom_memmove(void *pv_to, const void *pv_from, size_t z_l);

#endif //_X_OS_H_
