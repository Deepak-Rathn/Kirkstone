/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**
* @file cellular_modem_hal_api.h
* @brief For Cellular Modem component
*
*/

#ifndef __CCSP_HAL_CELL_MAN_H__
#define __CCSP_HAL_CELL_MAN_H__

/**
* @defgroup CELLULAR_MODEM_HAL  Cellular Modem HAL
*
* @defgroup CELLULAR_MODEM_HAL_TYPES  Cellular Modem HAL Data Types
* @ingroup  CELLULAR_MODEM_HAL
*
* @defgroup CELLULAR_MODEM_HAL_APIS Cellular Modem HAL  APIs
* @ingroup  CELLULAR_MODEM_HAL
*
**/

/**
 * @addtogroup CELLULAR_MODEM_HAL_TYPES
 * @{
 */

#define STATUS_SUCCESS 0
#define STATUS_FAILED -1

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @}
 */


/**
 * @addtogroup CELLULAR_MODEM_HAL_APIS
 * @{
 */

/**
* @brief Perform a factory reset on the modem.
*
* This function is responsible for resetting the modem to its factory settings.
*
* @Param <None>
*
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILED if any error is detected.
*/
int Modem_FactoryReset(void);

/**
* @brief Reboot the modem.
*
* This function is responsible for rebooting the modem.
*
* @Param <None>
*
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILED if any error is detected.
*/
int Modem_Reboot(void);

/**
* @brief Retrieve the firmware version of the modem.
*
* This function is responsible for fetching the current firmware version of the modem.
*
* @param[out] firmware_version A pointer to a character buffer where the firmware version will be stored.
*                              \n The buffer size of firmware_version should be atleast 128 bytes long.
*                              \n The possible string is "EM06ALAR04A01M4G_01.003.01.003".
*
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILED if any error is detected.
*/
int Modem_Firmware_Version(char *firmware_version);

/**
 * @}
 */
 
#ifdef __cplusplus
}
#endif

#endif
