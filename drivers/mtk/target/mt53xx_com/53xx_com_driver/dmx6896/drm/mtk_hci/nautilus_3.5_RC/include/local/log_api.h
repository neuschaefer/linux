/** @addtogroup base base
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2007,2008,2009,2010 Sony Corporation, All Rights Reserved.
 */


#ifndef LOG_API_H
#define LOG_API_H


/**
 * Max. length of the configuration string understood by the logging system.
 * Everything what doesn't fit, will be ignored.
 */
#define LOG_MAX_CONFIG_LEN (1024)


/**
 * This constant represents a name of the default module, which can be used
 * to set a default debug level for all unspecified modules.
 */
#define LOG_DEFAULT_MODULE "*"


/**
 * This type represents default debug levels which can be used in
 * logging requests. Note that you can still use your own ones.
 */
typedef enum {
    LOG_OFF      = 0,
    LOG_DEBUG    = 10,
    LOG_INFO     = 20,
    LOG_WARN     = 30,
    LOG_ERROR    = 40,
    LOG_CRITICAL = 50
} log_dbg_level_t;


/**
 * This macro generates an ID of a module having a given name.
 */
#undef  LOG_ADD_MODULE
#define LOG_ADD_MODULE(m) LOG_##m

/**
 * This type represents IDs of modules which can make logging requests.
 *
 * \note IDs which are not listed here are considered as invalid.
 */
typedef enum {
    #include <module_defs.h>
} log_module_id_t;


/**
 * This array contains names of modules which are registered in
 * the logging system.
 */
extern const char* log_module_names[];

/**
 * This constant contains a number of modules registered in
 * the logging system.
 */
extern const u_int32_t LOG_NUMBER_MODULES;


/**
 * @brief This function initializes the logging system by parsing a given 
 * configuration string.
 *
 * \note Every configuration consists of a list of module configurations.
 *       A module configuration defines a debug level for a given
 *       module. If a module configuration is invalid (e.g. name is
 *       wrong or debug level is invalid), such configuration will be
 *       ignored. Note that names of modules and predefined debug levels
 *       are case-sensitive, so my_module and My_Module are different
 *       modules.
 *
 * \param[in] in_config A configuration string containing information about
 *                      logging capabilites of every module. It uses the 
 *                      following format: "module_name:debug_level", where
 *                      module_name - a name of a module
 *                      debug_level - a debug level for the specified module.
 *                      Note that if it is NULL, the logging will be 
 *                      switched off.
 * @return none
 */
void
log_Init(const char* in_config);


/**
 * @brief This function finalizes the logging system. It is guaranteed that after
 * this call logging will be switched off for every module.
 * @return none 
 */
void
log_Fin(void);


typedef enum {
    LOG_L_FALSE = 0,
    LOG_L_TRUE  = 1
} log_l_bool_t;


/*
 * This function checks if a logging request having a given debug level and
 * module ID is accepted by the system or not.
 */
log_l_bool_t
log_l_IsAccepted(log_dbg_level_t in_dbg_level,
                 log_module_id_t in_mod_id);


#if defined(NS_DEBUG)

/**
 * This is a way how a general logging request is made. This macro assumes
 * that the folloing macro is defined by the user before the call:
 * LOG_MODULE_ID - contains an ID of a module making the request. Must be
 *                 one of the IDs of log_module_id_t type. If a given ID
 *                 is invalid, the request is rejected.
 *
 * \param[in] func      A name of the function which will handle the request.
 * \param[in] dbg_level A debug level of the request.
 * \param[in] msg       A printf-like parameters surrounded by brackets.
 *
 * Example:
 *
 * #define LOG_MODULE_ID LOG_my_module
 * LOG_REQ(printf, LOG_INFO, ("input parameters: %d, %s, %f\n", p1, p2, p3));
 * If my_module is allowed to generate INFO logging, the following string
 * will be sent to stdout:
 * "input parameters: 10, "foo", 3.14"
 *
 * \note You can switch the logging off by defining NO_LOGGING. In this
 *       case no logging code will be included.
 */
#define LOG_REQ(func, dbg_level, msg) \
    do { \
        if (log_l_IsAccepted((dbg_level), LOG_MODULE_ID)) { \
            func msg; \
        } \
    } \
    while (LOG_L_FALSE)

/**
 * This is a way how you can run a given code depending on the current layer of
 * the module making the call.
 * LOG_MODULE_ID - contains an ID of a module making the request. Must be
 *                 one of the IDs of log_module_id_t type. If a given ID
 *                 is invalid, the request is rejected.
 *
 * \param[in] dbg_level A debug level of the request.
 * \param[in] code      A code to be executed.
 */
#define RUN_CODE(dbg_level, code) \
    do { \
        if (log_l_IsAccepted((dbg_level), LOG_MODULE_ID)) { code } \
    } \
    while (LOG_L_FALSE)

#else

#define LOG_REQ(func, dbg_level, msg)
#define RUN_CODE(dbg_level, code)

#endif /* NS_DEBUG */

#endif /* LOG_API_H */

