/**
 *******************************************************************************
 * @file    LIS2MDL_MAG_driver_HL.c
 * @author  MEMS Application Team
 * @version V3.0.0
 * @date    12-August-2016
 * @brief   This file provides a set of high-level functions needed to manage
            the LIS2MDL sensor
 *******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "LIS2MDL_MAG_driver_HL.h"
#include <math.h>



/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup COMPONENTS COMPONENTS
 * @{
 */

/** @addtogroup LIS2MDL LIS2MDL
 * @{
 */

/** @addtogroup LIS2MDL_Private_FunctionPrototypes Private function prototypes
 * @{
 */

static DrvStatusTypeDef LIS2MDL_Get_Axes_Raw( DrvContextTypeDef *handle, int16_t* pData );

/**
 * @}
 */

/** @addtogroup LIS2MDL_Callable_Private_FunctionPrototypes Callable private function prototypes
 * @{
 */

static DrvStatusTypeDef LIS2MDL_Init( DrvContextTypeDef *handle );
static DrvStatusTypeDef LIS2MDL_DeInit( DrvContextTypeDef *handle );
static DrvStatusTypeDef LIS2MDL_Sensor_Enable( DrvContextTypeDef *handle );
static DrvStatusTypeDef LIS2MDL_Sensor_Disable( DrvContextTypeDef *handle );
static DrvStatusTypeDef LIS2MDL_Get_WhoAmI( DrvContextTypeDef *handle, uint8_t *who_am_i );
static DrvStatusTypeDef LIS2MDL_Check_WhoAmI( DrvContextTypeDef *handle );
static DrvStatusTypeDef LIS2MDL_Get_Axes( DrvContextTypeDef *handle, SensorAxes_t *magnetic_field );
static DrvStatusTypeDef LIS2MDL_Get_AxesRaw( DrvContextTypeDef *handle, SensorAxesRaw_t *value );
static DrvStatusTypeDef LIS2MDL_Get_Sensitivity( DrvContextTypeDef *handle, float *sensitivity );
static DrvStatusTypeDef LIS2MDL_Get_ODR( DrvContextTypeDef *handle, float *odr );
static DrvStatusTypeDef LIS2MDL_Set_ODR( DrvContextTypeDef *handle, SensorOdr_t odr );
static DrvStatusTypeDef LIS2MDL_Set_ODR_Value( DrvContextTypeDef *handle, float odr );
static DrvStatusTypeDef LIS2MDL_Get_FS( DrvContextTypeDef *handle, float *fullScale );
static DrvStatusTypeDef LIS2MDL_Set_FS( DrvContextTypeDef *handle, SensorFs_t fullScale );
static DrvStatusTypeDef LIS2MDL_Set_FS_Value( DrvContextTypeDef *handle, float fullScale );
static DrvStatusTypeDef LIS2MDL_Read_Reg( DrvContextTypeDef *handle, uint8_t reg, uint8_t *data );
static DrvStatusTypeDef LIS2MDL_Write_Reg( DrvContextTypeDef *handle, uint8_t reg, uint8_t data );
static DrvStatusTypeDef LIS2MDL_Get_DRDY_Status( DrvContextTypeDef *handle, uint8_t *status );

/**
 * @}
 */

/** @addtogroup LIS2MDL_Private_Variables Private variables
 * @{
 */

/**
 * @brief LIS2MDL driver structure
 */
MAGNETO_Drv_t LIS2MDLDrv =
{
  LIS2MDL_Init,
  LIS2MDL_DeInit,
  LIS2MDL_Sensor_Enable,
  LIS2MDL_Sensor_Disable,
  LIS2MDL_Get_WhoAmI,
  LIS2MDL_Check_WhoAmI,
  LIS2MDL_Get_Axes,
  LIS2MDL_Get_AxesRaw,
  LIS2MDL_Get_Sensitivity,
  LIS2MDL_Get_ODR,
  LIS2MDL_Set_ODR,
  LIS2MDL_Set_ODR_Value,
  LIS2MDL_Get_FS,
  LIS2MDL_Set_FS,
  LIS2MDL_Set_FS_Value,
  LIS2MDL_Read_Reg,
  LIS2MDL_Write_Reg,
  LIS2MDL_Get_DRDY_Status
};

/**
 * @}
 */

/** @addtogroup LIS2MDL_Callable_Private_Functions Callable private functions
 * @{
 */

/**
 * @brief Initialize the LIS2MDL sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Init( DrvContextTypeDef *handle )
{

  if ( LIS2MDL_Check_WhoAmI( handle ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }

  /* Operating mode selection - power down */
  if ( LIS2MDL_MAG_W_SystemOperatingMode( (void *)handle, LIS2MDL_MAG_MD_POWER_DOWN ) == MEMS_ERROR )
  {
    return COMPONENT_ERROR;
  }

  /* Enable BDU */
  if ( LIS2MDL_MAG_W_BlockDataUpdate( (void *)handle, LIS2MDL_MAG_BDU_ENABLE ) == MEMS_ERROR )
  {
    return COMPONENT_ERROR;
  }

  if ( LIS2MDL_Set_ODR( handle, ODR_HIGH ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }

  if ( LIS2MDL_Set_FS( handle, FS_LOW ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }

  /* Temperature sensor disable - temp. sensor not used */
  if ( LIS2MDL_MAG_W_TemperatureSensor( (void *)handle, LIS2MDL_MAG_TEMP_EN_DISABLE ) == MEMS_ERROR )
  {
    return COMPONENT_ERROR;
  }

  handle->isInitialized = 1;

  return COMPONENT_OK;
}


/**
 * @brief Deinitialize the LIS2MDL sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_DeInit( DrvContextTypeDef *handle )
{

  if ( LIS2MDL_Check_WhoAmI( handle ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }

  /* Disable the component */
  if ( LIS2MDL_Sensor_Disable( handle ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }

  handle->isInitialized = 0;

  return COMPONENT_OK;
}



/**
 * @brief Enable the LIS2MDL sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Sensor_Enable( DrvContextTypeDef *handle )
{

  /* Check if the component is already enabled */
  if ( handle->isEnabled == 1 )
  {
    return COMPONENT_OK;
  }

  /* Operating mode selection - continuous */
  if ( LIS2MDL_MAG_W_SystemOperatingMode( (void *)handle, LIS2MDL_MAG_MD_CONTINUOUS ) == MEMS_ERROR )
  {
    return COMPONENT_ERROR;
  }

  handle->isEnabled = 1;

  return COMPONENT_OK;
}



/**
 * @brief Disable the LIS2MDL sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Sensor_Disable( DrvContextTypeDef *handle )
{

  /* Check if the component is already disabled */
  if ( handle->isEnabled == 0 )
  {
    return COMPONENT_OK;
  }

  /* Operating mode selection - power down */
  if ( LIS2MDL_MAG_W_SystemOperatingMode( (void *)handle, LIS2MDL_MAG_MD_POWER_DOWN ) == MEMS_ERROR )
  {
    return COMPONENT_ERROR;
  }

  handle->isEnabled = 0;

  return COMPONENT_OK;
}



/**
 * @brief Get the WHO_AM_I ID of the LIS2MDL sensor
 * @param handle the device handle
 * @param who_am_i pointer to the value of WHO_AM_I register
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Get_WhoAmI( DrvContextTypeDef *handle, uint8_t *who_am_i )
{

  /* Read WHO AM I register */
  if ( LIS2MDL_MAG_R_WHO_AM_I_( (void *)handle, ( uint8_t* )who_am_i ) == MEMS_ERROR )
  {
    return COMPONENT_ERROR;
  }

  return COMPONENT_OK;
}



/**
 * @brief Check the WHO_AM_I ID of the LIS2MDL sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Check_WhoAmI( DrvContextTypeDef *handle )
{

  uint8_t who_am_i = 0x00;

  if ( LIS2MDL_Get_WhoAmI( handle, &who_am_i ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  if ( who_am_i != handle->who_am_i )
  {
    return COMPONENT_ERROR;
  }

  return COMPONENT_OK;
}



/**
 * @brief Get the LIS2MDL sensor axes
 * @param handle the device handle
 * @param magnetic_field pointer where the values of the axes are written
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Get_Axes( DrvContextTypeDef *handle, SensorAxes_t *magnetic_field )
{

  int16_t pDataRaw[3];
  float sensitivity = 0;

  /* Read raw data from LIS2MDL output register. */
  if ( LIS2MDL_Get_Axes_Raw( handle, pDataRaw ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }

  /* Get LIS2MDL actual sensitivity. */
  if ( LIS2MDL_Get_Sensitivity( handle, &sensitivity ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }

  /* Calculate the data. */
  magnetic_field->AXIS_X = ( int32_t )( pDataRaw[0] * sensitivity );
  magnetic_field->AXIS_Y = ( int32_t )( pDataRaw[1] * sensitivity );
  magnetic_field->AXIS_Z = ( int32_t )( pDataRaw[2] * sensitivity );

  return COMPONENT_OK;
}



/**
 * @brief Get the LIS2MDL sensor raw axes
 * @param handle the device handle
 * @param value pointer where the raw values of the axes are written
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Get_AxesRaw( DrvContextTypeDef *handle, SensorAxesRaw_t *value )
{

  int16_t pDataRaw[3];

  /* Read raw data from LIS2MDL output register. */
  if ( LIS2MDL_Get_Axes_Raw( handle, pDataRaw ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }

  /* Set the raw data. */
  value->AXIS_X = pDataRaw[0];
  value->AXIS_Y = pDataRaw[1];
  value->AXIS_Z = pDataRaw[2];

  return COMPONENT_OK;
}



/**
 * @brief Get the LIS2MDL sensor sensitivity
 * @param handle the device handle
 * @param sensitivity pointer where the sensitivity value is written [LSB/gauss]
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Get_Sensitivity( DrvContextTypeDef *handle, float *sensitivity )
{
  *sensitivity = 0.66667f; // 1.5 mgauss/LSB

  return COMPONENT_OK;
}



/**
 * @brief Get the LIS2MDL sensor output data rate
 * @param handle the device handle
 * @param odr pointer where the output data rate is written
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Get_ODR( DrvContextTypeDef *handle, float *odr )
{

  LIS2MDL_MAG_DO_t odr_low_level;

  if ( LIS2MDL_MAG_R_OutputDataRate( (void *)handle, &odr_low_level ) == MEMS_ERROR )
  {
    return COMPONENT_ERROR;
  }

  switch( odr_low_level )
  {
    case LIS2MDL_MAG_DO_10Hz:
      *odr = 10.000f;
      break;
    case LIS2MDL_MAG_DO_20Hz:
      *odr = 20.000f;
      break;
    case LIS2MDL_MAG_DO_50Hz:
      *odr = 50.000f;
      break;
    case LIS2MDL_MAG_DO_100Hz:
      *odr = 100.000f;
      break;
    default:
      *odr = -1.000f;
      return COMPONENT_ERROR;
  }

  return COMPONENT_OK;
}



/**
 * @brief Set the LIS2MDL sensor output data rate
 * @param handle the device handle
 * @param odr the functional output data rate to be set
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Set_ODR( DrvContextTypeDef *handle, SensorOdr_t odr )
{

  LIS2MDL_MAG_DO_t new_odr;

  switch( odr )
  {
    case ODR_LOW:
      new_odr = LIS2MDL_MAG_DO_10Hz;
      break;
    case ODR_MID_LOW:
      new_odr = LIS2MDL_MAG_DO_10Hz;
      break;
    case ODR_MID:
      new_odr = LIS2MDL_MAG_DO_20Hz;
      break;
    case ODR_MID_HIGH:
      new_odr = LIS2MDL_MAG_DO_50Hz;
      break;
    case ODR_HIGH:
      new_odr = LIS2MDL_MAG_DO_100Hz;
      break;
    default:
      return COMPONENT_ERROR;
  }

  if ( LIS2MDL_MAG_W_OutputDataRate( (void *)handle, new_odr ) == MEMS_ERROR )
  {
    return COMPONENT_ERROR;
  }

  return COMPONENT_OK;
}



/**
 * @brief Set the LIS2MDL sensor output data rate
 * @param handle the device handle
 * @param odr the output data rate value to be set
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Set_ODR_Value( DrvContextTypeDef *handle, float odr )
{

  LIS2MDL_MAG_DO_t new_odr;

  new_odr = ( odr <= 10.000f ) ? LIS2MDL_MAG_DO_10Hz
            : ( odr <= 20.000f ) ? LIS2MDL_MAG_DO_20Hz
            : ( odr <= 50.000f ) ? LIS2MDL_MAG_DO_50Hz
            :                     LIS2MDL_MAG_DO_100Hz;

  if ( LIS2MDL_MAG_W_OutputDataRate( (void *)handle, new_odr ) == MEMS_ERROR )
  {
    return COMPONENT_ERROR;
  }

  return COMPONENT_OK;
}



/**
 * @brief Get the LIS2MDL sensor full scale
 * @param handle the device handle
 * @param fullScale pointer where the full scale is written
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Get_FS( DrvContextTypeDef *handle, float *fullScale )
{
  *fullScale =  49.152f;

  return COMPONENT_OK;
}



/**
 * @brief Set the LIS2MDL sensor full scale
 * @param handle the device handle
 * @param fullScale the functional full scale to be set
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Set_FS( DrvContextTypeDef *handle, SensorFs_t fullScale )
{
  return COMPONENT_OK;
}



/**
 * @brief Set the LIS2MDL sensor full scale
 * @param handle the device handle
 * @param fullScale the full scale value to be set
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Set_FS_Value( DrvContextTypeDef *handle, float fullScale )
{
  return COMPONENT_OK;
}

/**
 * @brief Read the data from register
 * @param handle the device handle
 * @param reg register address
 * @param data register data
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Read_Reg( DrvContextTypeDef *handle, uint8_t reg, uint8_t *data )
{

  if ( LIS2MDL_MAG_ReadReg( (void *)handle, reg, data, 1 ) == MEMS_ERROR )
  {
    return COMPONENT_ERROR;
  }

  return COMPONENT_OK;
}

/**
 * @brief Write the data to register
 * @param handle the device handle
 * @param reg register address
 * @param data register data
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Write_Reg( DrvContextTypeDef *handle, uint8_t reg, uint8_t data )
{

  if ( LIS2MDL_MAG_WriteReg( (void *)handle, reg, &data, 1 ) == MEMS_ERROR )
  {
    return COMPONENT_ERROR;
  }

  return COMPONENT_OK;
}

/**
 * @brief Get magnetometer data ready status
 * @param handle the device handle
 * @param status the data ready status
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Get_DRDY_Status( DrvContextTypeDef *handle, uint8_t *status )
{

  LIS2MDL_MAG_ZYXDA_t status_raw;

  if ( LIS2MDL_MAG_R_NewXYZData( (void *)handle, &status_raw ) == MEMS_ERROR )
  {
    return COMPONENT_ERROR;
  }

  switch( status_raw )
  {
    case LIS2MDL_MAG_ZYXDA_AVAILABLE:
      *status = 1;
      break;
    case LIS2MDL_MAG_ZYXDA_NOT_AVAILABLE:
      *status = 0;
      break;
    default:
      return COMPONENT_ERROR;
  }

  return COMPONENT_OK;
}

/**
 * @}
 */

/** @addtogroup LIS2MDL_Private_Functions Private functions
 * @{
 */

/**
 * @brief Get the LIS2MDL sensor raw axes
 * @param handle the device handle
 * @param pData pointer where the raw values of the axes are written
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef LIS2MDL_Get_Axes_Raw( DrvContextTypeDef *handle, int16_t* pData )
{

  uint8_t regValue[6] = {0, 0, 0, 0, 0, 0};

  /* Read output registers from LIS2MDL_MAG_OUTX_L to LIS2MDL_MAG_OUTZ_H. */
  if ( LIS2MDL_MAG_Get_Magnetic( (void *)handle, ( uint8_t* )regValue ) == MEMS_ERROR )
  {
    return COMPONENT_ERROR;
  }

  /* Format the data. */
  pData[0] = ( ( ( ( int16_t )regValue[1] ) << 8 ) + ( int16_t )regValue[0] );
  pData[1] = ( ( ( ( int16_t )regValue[3] ) << 8 ) + ( int16_t )regValue[2] );
  pData[2] = ( ( ( ( int16_t )regValue[5] ) << 8 ) + ( int16_t )regValue[4] );

  return COMPONENT_OK;
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
