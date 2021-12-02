/**
 * Copyright 2004-2006 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Tracing
 */

#ifndef CABOT_UTILITIES_DEBUG_TRACE_H_INCLUDED
#define CABOT_UTILITIES_DEBUG_TRACE_H_INCLUDED

/***************************************************************************************

Here is some information on how you can use CABOT_TRACE in your code. If you want to
use it, you must make sure that you build with CABOT_TRACE defined.

Inserting tracing into your code:
=================================
At the top of your C or C++ source file, add this:

 #include "utilities_debug/trace.h"
 TRACE_IMPLEMENT("new_module");

The TRACE_IMPLEMENT statement does some necessary initialisation.  The
string passed is the name of the tracing module, which you can use to
control the trace level in a very fine-grained way.

Then, later in the same code file, you can achieve trace output like this:

 TRACE_ERROR(("This is an error\n"));
 TRACE_WARN(("The CPU temperature is %d degrees centigrade\n", temp));
 TRACE_INFO(("Today is %s\n", day_of_week));
 TRACE_VERBOSE(("Here's a lot of output!\n"));

Note that the arguments are not evaluated unless the message will be printed
(and the TRACE macros expand to nothing in non-trace builds) so avoid
function calls with side-effects in TRACE calls.

A note about C
==============
There are 2 extra steps to trace C code:

The trace module needs to be declared.  (Note that, in C++ code,
TRACE_IMPLEMENT will do this for you automatically - so if you use the
same module in C and C++, or just in C++, then you don't need to do anything
extra to declare it).  To declare a module that's only used in C, call
TRACE_DECLARE from any C++ source file - this just declares a global variable
that's needed by the trace implementation.

You also need to call TRACE_PREPARE() from an initialisation function,
within each file you're tracing.

If you omit either of the above steps, then all your trace gets put into a
"NonCppCode" module.

Controlling what gets printed
=============================
The trace levels can be set on a per module basis to the following values:

 TRACE_LEVEL_NONE      // defined as 0
 TRACE_LEVEL_ERROR     // defined as 1
 TRACE_LEVEL_WARNING   // defined as 2
 TRACE_LEVEL_INFO      // defined as 3
 TRACE_LEVEL_VERBOSE   // defined as 4

There are 4 ways to control the trace level for a module, in increasing
order of priority:

  1) The default trace level for modules is TRACE_LEVEL_ERROR - i.e.
     only TRACE_ERROR statements will be printed.
  2) You can statically change the default trace level for a particular
     module by using:
       TRACE_IMPLEMENT_LEVEL("new_module", TRACE_LEVEL_WARNING);
     rather than the normal TRACE_IMPLEMENT call.  If you have multiple
     TRACE_IMPLEMENT calls for the same module name, you only need to
     change one of them.
  3) There is a big table in trace_override.cpp.  Use of this to specify
     default values for modules is deprecated (use TRACE_IMPLEMENT_LEVEL),
     but it's perfectly reasonable to use this to locally change trace
     levels when debugging (i.e. if you're not going to check in the
     changed trace levels).
  4) You can change trace levels at runtime using an API (see later),
     or using the ST testtool or the Observatory GUI.

API for controlling what gets printed
=====================================
At runtime you can call these configuration functions:

 // returns number of module trace levels adjusted
 int trace_set_level(const char* module_name, int level);
 void trace_set_all_levels(int level);

There are macros defined for these if you prefer:

 TRACE_SET_LEVEL(module_name,level);
 TRACE_SET_ALL_LEVELS(level);

These do what they say on the tin. So you could do 'TRACE_SET_LEVEL("dsmcc",
TRACE_LEVEL_INFO)' to show all DSMCC tracing at TRACE_LEVEL_INFO priority and
above.  You can also adjust the trace levels for multiple modules in a single
TRACE_SET_LEVEL macro (or a single call to the trace_set_level() function).
Basically, when you use these features to adjust a module's trace level, it
will search for an exact match with the module name first. If no exact match
is found, it will search for any module starting with the characters you gave.

For example, if several modules were to start with the prefix "mheg" ( "mheg",
"mheg_action", "mheg_application", "mheg_asn1parser", etc) then calling:

 TRACE_SET_LEVEL("mheg",TRACE_LEVEL_INFO);

would adjust only the "mheg" module since that's an exact match. But if you
were to call:

 TRACE_SET_LEVEL("mheg_",TRACE_LEVEL_INFO);

then since there are no exact matches for "mheg_" it would adjust any module
that starts with "mheg_". So all the "mheg_action", "mheg_application",
"mheg_asn1parser", etc. would get adjusted.

Where does trace go?
====================
Tracing uses FROST for support functions on the platform.  As a result,
these functions must be implemented, built and linked in the image.

The functions used are:

 void FROST_DebugOut(const char *string, unsigned int length);
 unsigned int FROST_Time(void);

Note: if your platform needs asynchronous buffered output, or use of other
widgetry, then you should achieve this inside your platform's implementation
of FROST_DebugOut().

Thread safety
=============
The trace system is thread-safe (but not suitable for use in ISRs).
It's designed to be as fast as possible - no runtime string comparisons
etc.

The trace system calls new internally (which may be implemented to take a
mutex), so it's not safe to trace inside your implementation of these
primitives.  There are non-thread-safe TRACE_xxx_STATIC varients that
do not allocate memory.

The trace system calls the platform's vsprintf function to format the
string, and also uses strcpy/strcat/strlen.  It also uses FROST_DebugOut
to output the final string.  These functions must be thread-safe.

Static initialisers
===================
It is best to avoid calling trace from static initialisers.

It is safe to call trace from a static initialiser, so long as the TRACE call
and the static initialiser are in the same file, and the TRACE_IMPLEMENT
call preceeds the static initialiser.  Note that cross-file calls are not
allowed.

On platforms that initialise all static memory to zero before calling any
static initialisers, it is also safe to call trace from any static
initialiser, although you may not get any output.

***************************************************************************************/
/**
 * @def TRACE_IMPLEMENT
 * @brief Initialise tracing and declare the module name.
 * Must be used at file scope, cannot be used inside a function declaration.
 *
 * Must be called exactly once per file, and before any other TRACE_xxx
 * macros are used.
 *
 * If hard disk logging is supported then this trace will be logged.
 *
 * @param module_name the name of this module, as a string.
 */
/**
 * @def TRACE_IMPLEMENT_LEVEL
 * @brief Initialise tracing and declare the module name, specifying a default
 * trace level.
 *
 * Apart from the default trace level, this is identical to
 * TRACE_IMPLEMENT, and has the same usage & restrictions.
 * If hard disk logging is supported then this trace will be logged.
 *
 * The trace level is per-module, not per-file.
 *
 * If different files use the same module name, then you only need
 * one TRACE_IMPLEMENT_LEVEL and the other files can use TRACE_IMPLEMENT.
 *
 * Multiple uses of TRACE_IMPLEMENT_LEVEL (in different files) with the
 * same module name is not recommended as but is allowed - if all
 * TRACE_IMPLEMENT_LEVEL calls use the same level then this is well-defined,
 * if some use different levels then which one is used is compiler- and
 * linker-dependent.
 *
 * @param module_name   the name of this module, as a string.
 * @param initial_level the initial trace this module, as a TRACE_LEVEL_xxx
 *                      constant.
 */
/**
 * @def TRACE_IMPLEMENT_NO_HDD_LOG
 * @brief Initialise tracing and declare the module name.
 *
 * This is identical to TRACE_IMPLEMENT except trace is never logged
 * to hard disk - this is required within the filesystem tracing itself
 * to prevent infinite recursion.
 */
/**
 * @def TRACE_IMPLEMENT_LEVEL_NO_HDD_LOG
 * @brief Initialise tracing and declare the module name, specifying a default
 * trace level.
 *
 * This is identical to TRACE_IMPLEMENT_LEVEL except trace is never logged
 * to hard disk - this is required within the filesystem tracing itself
 * to prevent infinite recursion.
 */
/**
 * @def TRACE_ERROR
 * @brief Print an error.
 *
 * Usage:
 *    TRACE_ERROR((format, ...));
 * Where format is a printf-style format string and is followed by normal
 * printf arguments.  Note the double brackets.
 */
/**
 * @def TRACE_ERROR
 * @brief Print a warning.
 *
 * Usage:
 *    TRACE_WARN((format, ...));
 * Where format is a printf-style format string and is followed by normal
 * printf arguments.  Note the double brackets.
 */
/**
 * @def TRACE_INFO
 * @brief Print information.
 *
 * Usage:
 *    TRACE_INFO((format, ...));
 * Where format is a printf-style format string and is followed by normal
 * printf arguments.  Note the double brackets.
 */
/**
 * @def TRACE_VERBOSE
 * @brief Print verbose debugging.
 *
 * Usage:
 *    TRACE_VERBOSE((format, ...));
 * Where format is a printf-style format string and is followed by normal
 * printf arguments.  Note the double brackets.
 */
#ifndef __cplusplus
/**
 * @def TRACE_PREPARE
 * @brief Prepare the trace subsystem (only required in C code, not C++).
 *
 * Usage:
 *    TRACE_PREPARE()
 *
 * Call inside your initialisation function.  May be called multiple times,
 * but excessive calls should be avoided for speed reasons.
 *
 * This function is not defined in C++.
 */
#else
/**
 * @def TRACE_DECLARE
 * @brief Declare a module for the trace subsystem (only required for trace
 * modules that are only used in C code).
 *
 * Usage:
 *    TRACE_DECLARE("module_name")
 *
 * Call at file scope inside a C++ source file.
 *
 * This function is not defined in C.
 */
#endif


/**
 * @def TRACE_VAR
 * @brief A statement that is only needed and compiled when TRACE is used, so that trace code less visible.
 *
 * Usage:
 *    TRACE_VAR(statement)
 *    e.g.
 *    TRACE_VAR(USting buff)
 *    ...
 *    ...
 *    if ...
 *        TRACE_VAR(uicontrolinterface->getsomestring(buff))
 *        if  ...
 *        else
 *             TRACE_ERROR(("The %s was invalid\n", buff))
 */

#ifdef CABOT_TRACE

/*
 * define the trace levels - think of these as priorities, so
 * an error message is top priority (1).  None is the exception.
 */
#define TRACE_LEVEL_USE_DEFAULT -1
#define TRACE_LEVEL_NONE        0x00
#define TRACE_LEVEL_ERROR       0x01
#define TRACE_LEVEL_WARNING     0x02
#define TRACE_LEVEL_INFO        0x03
#define TRACE_LEVEL_VERBOSE     0x04

/*
 * prototypes for tracing functions
 */
#ifdef __cplusplus
extern "C" {
#endif

void            trace_enable_timing(int);
void            trace_list_modules(void);
void            trace_set_line_length(int length);
int             trace_set_level(const char *module_name, int level);
void            trace_set_all_levels(int level);
unsigned short  trace_get_number_of_modules(void);
void            trace_get_module_name(unsigned short module_index, char *module_name);
int             trace_get_level_from_index(unsigned short module_index);

/* Following functions should not be called directly - use the macros. */
struct CabotTracer;
int             trace_test_level(const struct CabotTracer * module_index, unsigned short level);
struct CabotTracer * trace_get_index(const char* module_name);
void            trace_output(struct CabotTracer * module, unsigned short level, char *string);
/*lint -function(printf, trace_string) */
char*           trace_string(const char* fmt, ...)
#ifdef __GNUC__
    __attribute__ ((format(printf, 1, 2))) /* For useful compiler warnings */
#endif
    ;

/*
 * Static char[] versions
 */
void            trace_output_static(struct CabotTracer * module, unsigned short level, char *string);
/*lint -function(printf, trace_string_static) */
char*           trace_string_static(const char* fmt, ...)
#ifdef __GNUC__
    __attribute__ ((format(printf, 1, 2))) /* For useful compiler warnings */
#endif
    ;

/*
 * Hex dump function
 */
void trace_hexdump( const unsigned char* buf, unsigned int len );

/* Used in trace_override.cpp */
struct trace_info_t
{
    const char* name;
    short level;
};
extern const struct trace_info_t override_trace_levels[];
extern const int override_trace_levels_count;

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
/* Following class should not be used directly - use the macros! */
/* NOTE: Never ever deallocate the memory used by a CabotTracer.
 * They must always be allocated statically.
 */
struct CabotTracer
{
public:
    CabotTracer(const char * moduleName, bool file_logging);
    CabotTracer(const char * moduleName, short initialLevel, bool file_logging);
    #ifdef NO_GLOBAL_CONSTRUCTION
      CabotTracer* selfCheck(const char* module_name);
    #endif

/* Following are only used inside trace.cpp: */
    int testLevel(unsigned short level) const;
    inline const char * getName() const { return m_name; }
    inline short getLevel() const { return m_level; }
    inline short getFileLogging() const { return m_file_logging; }

private:
    CabotTracer &operator=(const CabotTracer &); /* Not implemented */
    CabotTracer(const CabotTracer &);    /* Not implemented */

    /* The next trace module in the list (with a different name). */
    CabotTracer * m_next;

    /* The next trace module with the same name, if any. */
    CabotTracer * m_same;

    /* The trace level for this module. */
    short m_level;

    /* Should this trace ever be logged to a file
    ** nb: filesystem trace can't be or we get infinite recursion
    */
    bool m_file_logging;

    /* The name of this module */
    char m_name[64];

    /* Set the trace level for this module and all following modules with
     * the same name.  Should only be called on the first entry in the m_same
     * list. */
    void setLevel(short level);

    /* CabotTracerList is part of the implementation in trace.cpp */
    friend class CabotTracerList;
};
/* Next 3 are internal macros - do not use! */
#define TRACE__CAT1(a,b) a##b
#define TRACE__CAT2(a,b) TRACE__CAT1(a,b)
#define TRACE__UNIQUEID(prefix) TRACE__CAT2(prefix,__LINE__)
#endif /* !__cplusplus */

/*
 * define the macros
 */
#ifdef __cplusplus
    #ifdef NO_GLOBAL_CONSTRUCTION
        #define MODULE_GLOBAL_NAME(name)  static char _module_name[] = name;
        #define SELF_CHECK(x)  x->selfCheck(_module_name)
    #else
        #define MODULE_GLOBAL_NAME(name)
        #define SELF_CHECK(x) x
    #endif
    #define TRACE_IMPLEMENT(module_name) \
        MODULE_GLOBAL_NAME(module_name) \
        static struct CabotTracer _module_impl(module_name, true); \
        static struct CabotTracer * _module = &_module_impl
    #define TRACE_IMPLEMENT_LEVEL(module_name,initial_level) \
        MODULE_GLOBAL_NAME(module_name) \
        static struct CabotTracer _module_impl(module_name, initial_level, true); \
        static struct CabotTracer * _module = &_module_impl
    #define TRACE_IMPLEMENT_NO_HDD_LOG(module_name) \
        MODULE_GLOBAL_NAME(module_name) \
        static struct CabotTracer _module_impl(module_name, false); \
        static struct CabotTracer * _module = &_module_impl
    #define TRACE_IMPLEMENT_LEVEL_NO_HDD_LOG(module_name,initial_level) \
        MODULE_GLOBAL_NAME(module_name) \
        static struct CabotTracer _module_impl(module_name, initial_level, false); \
        static struct CabotTracer * _module = &_module_impl
    /* TRACE_PREPARE() has to be defined in case C source is compiled with a C++ compiler */
    #define TRACE_PREPARE()
    #define TRACE_DECLARE(moduleName) \
        static CabotTracer TRACE__UNIQUEID(_module_) (moduleName, true)
    #define TRACE_DECLARE_LEVEL(moduleName, initial_level) \
        static CabotTracer TRACE__UNIQUEID(_module_) (moduleName, initial_level, true)

    /* lint -esym(528, _module) */
#else
    #define SELF_CHECK(x) x
    #define TRACE_IMPLEMENT(x) \
        static char _module_name[] = x; \
        static struct CabotTracer * _module = 0
    #define TRACE_PREPARE() \
        _module = trace_get_index(_module_name)
    /* We cannot define TRACE_DECLARE() as it requires C++. */
#endif

#define TRACE_TEST_LEVEL(level)     trace_test_level(SELF_CHECK(_module), level)

/*
 * Trace macros for ouputting printf-style strings - NB: these use 'new'
 */
#define TRACE_ALWAYS(x)             trace_output(_module, TRACE_LEVEL_NONE, trace_string x)

#define TRACE_ERROR(x)              if (TRACE_TEST_LEVEL(TRACE_LEVEL_ERROR))   \
                                        trace_output(_module, TRACE_LEVEL_ERROR, trace_string x)
#define TRACE_WARN(x)               if (TRACE_TEST_LEVEL(TRACE_LEVEL_WARNING)) \
                                        trace_output(_module, TRACE_LEVEL_WARNING, trace_string x)
#define TRACE_INFO(x)               if (TRACE_TEST_LEVEL(TRACE_LEVEL_INFO))    \
                                        trace_output(_module, TRACE_LEVEL_INFO, trace_string x)
#define TRACE_VERBOSE(x)            if (TRACE_TEST_LEVEL(TRACE_LEVEL_VERBOSE)) \
                                        trace_output(_module, TRACE_LEVEL_VERBOSE, trace_string x)
/*
 * Trace macros for outputting a null-terminated string - these do not use 'new'
 */

#define TRACE_ALWAYS_STATIC(x)      trace_output_static(_module, TRACE_LEVEL_NONE, trace_string_static x)

#define TRACE_ERROR_STATIC(x)       if (TRACE_TEST_LEVEL(TRACE_LEVEL_ERROR)) \
                                        trace_output_static(_module, TRACE_LEVEL_ERROR, trace_string_static x)
#define TRACE_WARN_STATIC(x)        if (TRACE_TEST_LEVEL(TRACE_LEVEL_WARNING)) \
                                        trace_output_static(_module, TRACE_LEVEL_WARNING, trace_string_static x)
#define TRACE_INFO_STATIC(x)        if (TRACE_TEST_LEVEL(TRACE_LEVEL_INFO))    \
                                        trace_output_static(_module, TRACE_LEVEL_INFO, trace_string_static x)
#define TRACE_VERBOSE_STATIC(x)     if (TRACE_TEST_LEVEL(TRACE_LEVEL_VERBOSE)) \
                                        trace_output_static(_module, TRACE_LEVEL_VERBOSE, trace_string_static x)
/*
 * Trace macros for outputting a hex dump of a buffer
 */
#define TRACE_HEXDUMP_ERROR(b,l)     if (TRACE_TEST_LEVEL(TRACE_LEVEL_ERROR)) \
                                        trace_hexdump(b,l)
#define TRACE_HEXDUMP_WARN(b,l)     if (TRACE_TEST_LEVEL(TRACE_LEVEL_WARNING)) \
                                        trace_hexdump(b,l)
#define TRACE_HEXDUMP_INFO(b,l)     if (TRACE_TEST_LEVEL(TRACE_LEVEL_INFO)) \
                                        trace_hexdump(b,l)
#define TRACE_HEXDUMP_VERB(b,l)     if (TRACE_TEST_LEVEL(TRACE_LEVEL_VERBOSE)) \
                                        trace_hexdump(b,l)

#define TRACE_SET_LEVEL(x,y)        trace_set_level((x),(y))
#define TRACE_SET_ALL_LEVELS(x)     trace_set_all_levels((x))
#define TRACE_ENABLE_TIMING(x)      trace_enable_timing((x))
#define TRACE_VAR(x)                x;

#else /* CABOT_TRACE not defined */

/*
** Null macro defines for release builds where CABOT_TRACE is not defined
**
** Need a valid definition of TRACE_IMPLEMENT since some compilers (eg ST)
** treat a lone ';' as a declaration with no effect and give an error.
*/
#define TRACE_IMPLEMENT(module_name) extern void not_used(void)
#define TRACE_IMPLEMENT_LEVEL(module_name,initial_level) extern void not_used(void)
#define TRACE_IMPLEMENT_NO_HDD_LOG(module_name) extern void not_used(void)
#define TRACE_IMPLEMENT_LEVEL_NO_HDD_LOG(module_name,initial_level) extern void not_used(void)
#ifdef __cplusplus
/* Next 3 are internal macros - do not use! */
#define TRACE__CAT1(a,b) a##b
#define TRACE__CAT2(a,b) TRACE__CAT1(a,b)
#define TRACE__UNIQUEID(prefix) TRACE__CAT2(prefix,__LINE__)

#define TRACE_DECLARE(moduleName) extern void TRACE__UNIQUEID(_not_used)(void)
#define TRACE_DECLARE_LEVEL(moduleName, initial_level) extern void TRACE__UNIQUEID(_not_used)(void)
/* TRACE_PREPARE() has to be defined in case a C source file is compiled with a C++ compiler */
#define TRACE_PREPARE()
#else
#define TRACE_PREPARE() (void)0
#endif
#define TRACE_TEST_LEVEL(level) 0

#define TRACE_ALWAYS(x)
#define TRACE_ERROR(x)
#define TRACE_WARN(x)
#define TRACE_INFO(x)
#define TRACE_VERBOSE(x)

#define TRACE_ALWAYS_STATIC(x)
#define TRACE_ERROR_STATIC(x)
#define TRACE_WARN_STATIC(x)
#define TRACE_INFO_STATIC(x)
#define TRACE_VERBOSE_STATIC(x)

#define TRACE_HEXDUMP_ERROR(b,l)
#define TRACE_HEXDUMP_WARN(b,l)
#define TRACE_HEXDUMP_INFO(b,l)
#define TRACE_HEXDUMP_VERB(b,l)

#define TRACE_SET_LEVEL(x,y)
#define TRACE_SET_ALL_LEVELS(x)
#define TRACE_ENABLE_TIMING(x)
#define TRACE_VAR(x)

#endif /* CABOT_TRACE */

#endif /* CABOT_UTILITIES_DEBUG_TRACE_H_INCLUDED*/

