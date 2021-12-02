/******************************************************************************
* %Id: by PSL $
*
* FileName   :dd_i2c.h
*
* Description  : dd_i2c.c header
*
*
* Copyright  :PSL/SC-SSDC
*
*****************************************************************************/

#ifndef __DD_I2C_H__
#define __DD_I2C_H__
/*-------------------------------------------------------------------------
INCLUDES
-------------------------------------------------------------------------*/
#include "dd_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /*-------------------------------------------------------------------------
    TYPEDEFS
    -------------------------------------------------------------------------*/
    /**
    * \typedef I2Caddr_t
    * \brief I2C device address (7-bit)
    */
    typedef u8_t    I2Caddr_t;

    /**
    * \typedef I2CdevId_t
    * \brief Device identifier.
    *
    * The device ID can be useful if several devices share an I2C address,
    * or if multiple I2C busses are used.
    * It can be used to control a "switch" selecting the correct device and/or
    * I2C bus.
    *
    */
    typedef u16_t   I2CdevId_t;

    /**
    * \struct _I2CDeviceAddr_t
    * \brief I2C device parameters.
    *
    * This structure contains the I2C address and the device ID.
    * This structure is used to reduce the drivers footprint.
    * It reduces the number of parameters in frequently called functions.
    *
    */
    struct _I2CDeviceAddr_t
    {
        I2Caddr_t                       i2cAddr;        /**< The I2C address ofthe device. */
        I2CdevId_t                      i2cDevId;       /**< The devive identiefier. */
    };

    /**
    * \typedef I2CDeviceAddr_t
    * \brief I2C device parameters.
    *
    * This structure contains the I2C address and the device ID.
    * This structure is used to reduce the drivers footprint.
    * It reduces the number of parameters in frequently called functions.
    *
    */
    typedef struct _I2CDeviceAddr_t I2CDeviceAddr_t;

    /**
    * \typedef pI2CDeviceAddr_t
    * \brief Pointer to I2C device parameters.
    */
    typedef I2CDeviceAddr_t         *pI2CDeviceAddr_t;

    /*-------------------------------------------------------------------------
    DEFINES
    -------------------------------------------------------------------------*/
#define I2C_BUS_ID 0
    /*-------------------------------------------------------------------------
    MACROS
    -------------------------------------------------------------------------*/

    /**
    * \def IS_I2C_10BIT( addr )
    * \brief Determine if I2C address 'addr' is a 10 bits address or not.
    * \param addr The I2C address.
    * \return int.
    * \retval 0 if address is not a 10 bits I2C address.
    * \retval 1 if address is a 10 bits I2C address.
    */
#define IS_I2C_10BIT(addr) \
(((addr) & 0xF8) == 0xF0)

    /*-------------------------------------------------------------------------
    ENUM
    -------------------------------------------------------------------------*/

    /*-------------------------------------------------------------------------
    STRUCTS
    -------------------------------------------------------------------------*/


    /*-------------------------------------------------------------------------
    THE END
    -------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif /* __DD_I2C_H__ */

