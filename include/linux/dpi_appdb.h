#if defined(CONFIG_BCM_KF_DPI) && defined(CONFIG_BRCM_DPI)
/*!
 * \file appdb.h
 * \brief Application database (appdb) to save signature database.
 * \details Signature database is an input from Trend Micro NTD to
 * save application list which we can identify. Each applications is represented by
 * 3 numbers, i.e. application category id, application name id, and application behavoir id.
 * If you combine the 3 numbers to 1 integer, it will be unique. Therefore, it's
 * also used as table search key.
 *
 * \par app behavoir instance id (behinst) layout (combined with 3 numbers).
 * \code
//    rsv    cat      app    beh
// +-------------------------------+
// | 8 bit | 8 bit | 8 bit | 8 bit |
// +-------------------------------+
// 0        8                      31
 * \endcode
 *
 * \par app instance id (appinst) layout (only combined with cat & app id), and behavoir id = 0,
 * \code
//    rsv    cat      app    beh
// +-------------------------------+
// | 8 bit | 8 bit | 8 bit | 8 bit |
// +-------------------------------+
// 0        8                      31
 * \endcode
 */

#ifndef __DPI_APPDB_H__
#define __DPI_APPDB_H__

/*!
 * \brief Application identifier in signature database.
 *
 * \warning Do not change this. You have been warned.
 */
typedef struct appdb_entry
{
	uint8_t cat_id; //!< Default category id in database. Not user-defined category.
	uint8_t app_id;
	uint8_t beh_id;
} __attribute__((packed)) appdb_entry_t;


#define appdb_beh2appinst(_behinst_id) ((_behinst_id) & 0xFFFFFF00)

/*
 * "Behavior Instance" is an application instance with behavior input
 */
#define appdb_get_behinst_id(_cat_id, _app_id, _beh_id) \
	(uint32_t) ( \
		(uint32_t) (((uint8_t) _cat_id) << ((sizeof(((appdb_entry_t *) 0)->app_id) * 8) + (sizeof(((appdb_entry_t *) 0)->beh_id) * 8))) | \
		(uint32_t) (((uint8_t) _app_id) << (sizeof(((appdb_entry_t *) 0)->beh_id) * 8)) | \
		(uint32_t) ((uint8_t) _beh_id) \
	)

#define appdb_get_unknown_behinst_id() appdb_get_behinst_id(0, 0, 0)

/*
 * "Application Instance" is an application instance without behavior input
 */
#define appdb_get_appinst_id(__cat_id, __app_id) appdb_get_behinst_id(__cat_id, __app_id, 0)

#define appdb_get_cat(_behinst_id) ((uint8_t) (((_behinst_id) & 0x00FF0000) >> 16))
#define appdb_get_app(_behinst_id) ((uint8_t) (((_behinst_id) & 0x0000FF00) >> 8))
#define appdb_get_beh(_behinst_id) ((uint8_t) ((_behinst_id) & 0x000000FF))

#define appdb_entry_setup(_e, _cat, _app, _beh) \
	do { (_e)->cat_id = _cat; (_e)->app_id = _app; (_e)->beh_id = _beh; } while (0)

#endif /* __DPI_APPDB_H__ */
#endif
