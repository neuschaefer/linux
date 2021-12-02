#ifndef _MTK_UTIL_H_
#define _MTK_UTIL_H_
#if defined(__cplusplus)
extern "C" {
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef unsigned int   MTK_UInt32;
typedef int            MTK_Int32;
typedef unsigned short MTK_UInt16;
typedef short          MTK_Int16;
typedef unsigned char  MTK_UInt8;
typedef signed char    MTK_Int8;
typedef float          MTK_Float;

typedef enum {
    MTK_FALSE = 0, 
    MTK_TRUE  = 1
} MTK_Boolean;

/**
 * Signed integer value representing a function or method result (return value)
 *
 * When a function or method call succeeds, the return value is always 
 * MTK_SUCCESS unless otherwise documented. Error conditions are always
 * negative values, defined in AtxResults.h
 */
typedef int MTK_Result;

/**
 * Type used to represent a bit pattern signifying a combination of flags
 * that can be on or off. Bits set to 1 indicate that the corresponding flag
 * is on, bits set to 0 indicate that the corresponding flag is off. The
 * position and meaning of flags is specific to each method, function, variable
 * or data structure that uses this type, and the corresponding header file
 * will give symbolic constants to represent individual flag bits.
 */
typedef MTK_UInt32 MTK_Flags;

/**
 * An unsigned integer used to represent a bit mask.
 */
typedef MTK_UInt32 MTK_Mask;

/**
 * An unsigned integer used to represent a measurable quantity (eg. the 
 * size of a file)
 */
typedef MTK_UInt32 MTK_Size;

/**
 * A signed integer used to represent an offset from a base value.
 */
typedef MTK_Int32 MTK_Offset;

/**
 * An address as a generic pointer, that can be dereferenced as a byte address.
 */
typedef unsigned char* MTK_Address;

/**
 * An unsigned integer used to represent the difference between a matximum 
 * value and a minimum value.
 */
typedef MTK_UInt32 MTK_Range;

/**
 * An unsigned integer used to represent a quantity that can be counted (such
 * as an number of elements in a list).
 */
typedef MTK_UInt32 MTK_Cardinal;

/**
 * An unsigned integer that represents a position in a sequence (such as an
 * index into a list of elements).
 */
typedef MTK_UInt32  MTK_Ordinal;

/**
 * An unsigned integer used to represent a version Id. Version Ids are 
 * monotonic, so that a larger integer means a more recent version.
 */
typedef MTK_UInt32  MTK_VersionId;

/**
 * A const pointer to a NULL-terminated character array, used to represent 
 * strings. Strings that cannot be represented by characters in the ASCII set
 * are encoded as UTF-8 unless otherwise specified.
 */
typedef const char* MTK_CString;
typedef char*       MTK_CStringBuffer;

/**
 * Pointer to void, used to represent pointers to arbitrary untyped data 
 * buffers.
 */
typedef void*       MTK_Any;
typedef const void* MTK_AnyConst;

/**
 * 8-bit Byte
 */
typedef MTK_UInt8 MTK_Byte;

/**
 * Pointer to a byte buffer
 */
typedef MTK_UInt8* MTK_ByteBuffer;

/**
 * Timeout in milliseconds
 */
typedef int        MTK_Timeout;

typedef struct MTK_Mutex MTK_Mutex;
typedef unsigned long    MTK_ThreadId;

#define MTK_FAILED(result)              ((result) != MTK_SUCCESS)
#define MTK_SUCCEEDED(result)           ((result) == MTK_SUCCESS)

/*----------------------------------------------------------------------
|    result codes      
+---------------------------------------------------------------------*/
/** Result indicating that the operation or call succeeded */
#define MTK_SUCCESS                     0

/** Result indicating an unspecififed failure condition */
#define MTK_FAILURE                     (-1)

/* general error codes */
#ifndef MTK_ERROR_BASE               
#define MTK_ERROR_BASE                  (-10000)
#endif

#define MTK_ERROR_BASE_GENERAL          (MTK_ERROR_BASE-0)
#define MTK_ERROR_OUT_OF_MEMORY         (MTK_ERROR_BASE_GENERAL - 0)
#define MTK_ERROR_OUT_OF_RESOURCES      (MTK_ERROR_BASE_GENERAL - 1)
#define MTK_ERROR_INTERNAL              (MTK_ERROR_BASE_GENERAL - 2)
#define MTK_ERROR_INVALID_PARAMETERS    (MTK_ERROR_BASE_GENERAL - 3)
#define MTK_ERROR_INVALID_STATE         (MTK_ERROR_BASE_GENERAL - 4)
#define MTK_ERROR_NOT_IMPLEMENTED       (MTK_ERROR_BASE_GENERAL - 5)
#define MTK_ERROR_OUT_OF_RANGE          (MTK_ERROR_BASE_GENERAL - 6)
#define MTK_ERROR_ACCESS_DENIED         (MTK_ERROR_BASE_GENERAL - 7)
#define MTK_ERROR_INVALID_SYNTAX        (MTK_ERROR_BASE_GENERAL - 8)
#define MTK_ERROR_NOT_SUPPORTED         (MTK_ERROR_BASE_GENERAL -  9)
#define MTK_ERROR_INVALID_FORMAT        (MTK_ERROR_BASE_GENERAL - 10)
#define MTK_ERROR_NOT_ENOUGH_SPACE      (MTK_ERROR_BASE_GENERAL - 11)
#define MTK_ERROR_NO_SUCH_ITEM          (MTK_ERROR_BASE_GENERAL - 12)
#define MTK_ERROR_OVERFLOW              (MTK_ERROR_BASE_GENERAL - 13)

/* device and i/o errors */
#define MTK_ERROR_BASE_DEVICE           (MTK_ERROR_BASE-100)
#define MTK_ERROR_DEVICE_BUSY           (MTK_ERROR_BASE_DEVICE - 0)
#define MTK_ERROR_NO_SUCH_DEVICE        (MTK_ERROR_BASE_DEVICE - 1)
#define MTK_ERROR_OPEN_FAILED           (MTK_ERROR_BASE_DEVICE - 2)
#define MTK_ERROR_NO_MEDIUM             (MTK_ERROR_BASE_DEVICE - 3)

/* object model error codes */
#define MTK_ERROR_BASE_INTERFACES       (MTK_ERROR_BASE-200)

/* properties error codes */
#define MTK_ERROR_BASE_PROPERTIES       (MTK_ERROR_BASE-300)

/* iterator error codes */
#define MTK_ERROR_BASE_ITERATOR         (MTK_ERROR_BASE-400)

/* byte stream error codes */
#define MTK_ERROR_BASE_BYTE_STREAM      (MTK_ERROR_BASE-500)

/* socket error codes */
#define MTK_ERROR_BASE_SOCKETS          (MTK_ERROR_BASE-600)

/* file error codes */
#define MTK_ERROR_BASE_FILE             (MTK_ERROR_BASE-700)

/* standard error codes                                  */
/* these are special codes to convey an errno            */
/* the error code is (MTK_ERROR_BASE_ERRNO - errno)      */
/* where errno is the positive integer from errno.h      */
#define MTK_ERROR_BASE_ERRNO            (MTK_ERROR_BASE-2000)
#define MTK_ERROR_ERRNO(e)              (MTK_ERROR_BASE_ERRNO - (e))


#define MTK_ASSERT(x)
#define MTK_LOG_FATAL(_msg)
#define MTK_CHECK_FATAL(_result) do {\
    MTK_Result _x = (_result); \
    if (_x != MTK_SUCCESS) {   \
        return _x;             \
    }                          \
} while(0)

#define MTK_COMPILER_UNUSED(p) (void)p
#define MTK_LOG_SEVERE(_msg)
#define MTK_LOG_SEVERE_1(_msg,_arg1)

#define MTK_SET_LOCAL_LOGGER(_name)
#define MTK_LOG_WARNING(_msg)
#define MTK_LOG_WARNING_1(_msg,_arg1)

#define MTK_CHECK(_result) do {\
    MTK_Result _x = (_result); \
    if (_x != MTK_SUCCESS) {   \
        return _x;             \
    }                          \
} while(0)
#define MTK_CHECK_LABEL(_result, _label) do {\
    MTK_Result _x = (_result);               \
    if (_x != MTK_SUCCESS) {                 \
        goto _label;                         \
    }                                        \
} while(0)

#define MTK_CHECK_LABEL_SEVERE(_result, _label)do {                                                                     \
    MTK_Result _x = (_result);                                                                                                                    \
    if (_x != MTK_SUCCESS) {                                                                                                                      \
        goto _label;                                                                                                                              \
    }                                                                                                                                             \
} while(0)


#define MTK_CHECK_LABEL_WARNING(_result, _label) do {                                                                     \
    MTK_Result _x = (_result);                                                                                                                    \
    if (_x != MTK_SUCCESS) {                                                                                                                      \
        goto _label;                                                                                                                              \
    }                                                                                                                                             \
} while(0)

#define MTK_CHECK_WARNING(_result) do {                                                                                   \
    MTK_Result _x = (_result);                                                                                                                    \
    if (_x != MTK_SUCCESS) {                                                                                                                      \
        return _x;                                                                                                                                \
    }                                                                                                                                             \
} while(0)

#define MTK_CHECK_SEVERE(_result) do {                                                                                   \
    MTK_Result _x = (_result);                                                                                                                    \
    if (_x != MTK_SUCCESS) {                                                                                                                      \
        return _x;                                                                                                                                \
    }                                                                                                                                             \
} while(0)

typedef struct MTK_DataBuffer MTK_DataBuffer;

MTK_Result MTK_DataBuffer_Create(MTK_Size size, MTK_DataBuffer** buffer);
MTK_Result MTK_DataBuffer_Clone(const MTK_DataBuffer* self,
                                MTK_DataBuffer**      clone);
MTK_Result MTK_DataBuffer_Destroy(MTK_DataBuffer* self);
MTK_Result MTK_DataBuffer_SetBuffer(MTK_DataBuffer* self,
                                    MTK_Byte*       buffer_memory, 
                                    MTK_Size        buffer_size);
MTK_Result MTK_DataBuffer_SetBufferSize(MTK_DataBuffer* self,
                                        MTK_Size        buffer_size);
MTK_Size   MTK_DataBuffer_GetBufferSize(const MTK_DataBuffer* self);
MTK_Result MTK_DataBuffer_Reserve(MTK_DataBuffer* self,
                                  MTK_Size        buffer_size);
const MTK_Byte*  MTK_DataBuffer_GetData(const MTK_DataBuffer* self);
MTK_Byte*  MTK_DataBuffer_UseData(MTK_DataBuffer* self);
MTK_Size   MTK_DataBuffer_GetDataSize(const MTK_DataBuffer* self);
MTK_Result MTK_DataBuffer_SetDataSize(MTK_DataBuffer* self, MTK_Size size);
MTK_Result MTK_DataBuffer_SetData(MTK_DataBuffer* self, 
                                  const MTK_Byte* data,
                                  MTK_Size        data_size);
MTK_Boolean MTK_DataBuffer_Equals(const MTK_DataBuffer* self,
                                  const MTK_DataBuffer* other);
MTK_Result  MTK_DataBuffer_AppendData(MTK_DataBuffer* self,
                                      const MTK_Byte* data,
                                      MTK_Size        data_size);



void MTK_BytesFromInt32Be(unsigned char* buffer, MTK_UInt32 value);
MTK_UInt32 MTKBytesToInt32Be(const unsigned char* buffer);
MTK_UInt32 MTK_BytesToInt32Be(const unsigned char* buffer);


/*need porting*/
void* MTK_AllocateMemory(unsigned int);
void* MTK_AllocateZeroMemory(unsigned int);
void MTK_FreeMemory(void* pointer);
int MTK_MemoryEqual(const void* s1, const void* s2, unsigned long n);
void MTK_CopyMemory(void* dest, const void* src, MTK_Size size);

MTK_ThreadId MTK_GetCurrentThreadId(void);
MTK_Result MTK_Mutex_Create(MTK_Mutex** mutex);
MTK_Result MTK_Mutex_LockAutoCreate(MTK_Mutex** mutex);
MTK_Result MTK_Mutex_Lock(MTK_Mutex* mutex);
MTK_Result MTK_Mutex_Unlock(MTK_Mutex* mutex);
MTK_Result MTK_Mutex_Destroy(MTK_Mutex* mutex);


#if defined(__cplusplus)
}
#endif

#endif