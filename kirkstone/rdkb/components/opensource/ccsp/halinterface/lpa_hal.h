/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2022 RDK Management
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
* @file lpa_hal.h
* @brief For LPA Component
*
*/

#ifndef _LPA_HAL_H_
#define _LPA_HAL_H_
/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/
/**
* @defgroup lpa_hal LPA HAL
*
* This module provides the function call prototypes and structure definitions used for the  LPA hardware abstraction layer.
*
* @defgroup LPA_HAL_TYPES  LPA HAL Data Types
* @ingroup  lpa_hal
*
* @defgroup LPA_HAL_APIS   LPA HAL  APIs
* @ingroup  lpa_hal
*
**/

/**
* @addtogroup LPA_HAL_TYPES
* @{
*/

#define RETURN_ERROR        (-1)
#define RETURN_OK           (0)
#ifndef TRUE
#define TRUE                (1)
#endif
#ifndef FALSE
#define FALSE               (0)
#endif
typedef enum _Cellular_PreferredUICC_t
{
    CELLULAR_UICC_USIM     = 0,
    CELLULAR_UICC_ESIM,
} CellularPreferredUICC_t;
typedef enum _CellularDeviceSimStatus_t
{
   SIM_PREPARATION_PENDING = 1,
   SIM_PREPARATION_INPROGRESS,
   SIM_PREPARATION_DOWNLOADED,
   SIM_PREPARATION_DONE,
} CellularDeviceSimStatus_t;
/*
*  This struct is for esim profile object.
*/
typedef  struct
{
    char iccid[32];
    int  profileState;
    char profileName[16];
} eSIMProfileStruct;

/**
* @}
*/

/**
* @addtogroup LPA_HAL_APIS
* @{
*/

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/
/* cellular_sim_download_progress_callback  function */
/**
* @description - This callback sends to upper layer about the download progress.
*
* @param[in] progress - It is a character pointer which represents the download progress either have download completed progress or download in progress.
*                       \n The string values are:- "download successful", "Activation Code decoded", "Initiate authentication done", "SM-DP+ address checking done", "Authenticate server done", "Authenticate client done", "PPR conditions check done", "Get Bound Profile Package done",
*                       \n "Send PIR notification to server done", "Clear PIR notification done", "Read pending notifications...", "<1> pending notification(s) detected", "Processing notification #1 <33>", "Notification <33> successfully sent", "Load Bound Profile Package done - Profile download successful".
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
typedef int (*cellular_sim_download_progress_callback)( char *progress );
/* cellular_esim_download_profile_with_activationcode() function */
/**
* @description - Downloads the profile with activation code.
*
* @param[out] ActivationCodeStr - It is a 255 bytes character pointer that provides the activation code for the profile.
*                                 \n The example values is: "1$sm-v4-059-ppa-gtm.pr.go-esim.com$50949C4E5F62378896B6BE0EBEF554E6"
* @param[in] download_progress  - The variable is the function pointer to the call back function cellular_sim_download_progress_callback which receives information about the download progress.
*            progress - It is a character pointer which represents the download progress either have download completed progress or download in progress.
*                       \n The string values are:- "download successful", "Activation Code decoded", "Initiate authentication done", "SM-DP+ address checking done", "Authenticate server done", "Authenticate client done", "PPR conditions check done", "Get Bound Profile Package done",
*                       \n "Send PIR notification to server done", "Clear PIR notification done", "Read pending notifications...", "<1> pending notification(s) detected", "Processing notification #1 <33>", "Notification <33> successfully sent", "Load Bound Profile Package done - Profile download successful".
*
* @return The status of the operation.
* @retval TRUE if modem device presents.
* @retval FALSE if modem device not presents.
*
*/
int cellular_esim_download_profile_with_activationcode(char* ActivationCodeStr, cellular_sim_download_progress_callback download_progress);
/*  cellular_esim_download_profile_from_smds() function */
/**
* @description - Downloads the profile from SMDS.
*
* @param[in] smds - It is a 128 bytes character pointer that represents the SMDS address.
*                   \n The example value for smds is: "oem-smds-json.demo.gemalto.com"
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_download_profile_from_smds(char* smds);
/*  cellular_esim_download_profile_from_defaultsmdp() function */
/**
* @description - Downloads the profile from default SMDP.
*
* @param[in] smdp   - It is a 128 bytes character pointer that represents the SMDP address.
*                       \n The example values for smdp is: "deviceDefaultSMDPAddress=smdp-plus.test.gsma.com"
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_download_profile_from_defaultsmdp(char* smdp);
/*  cellular_esim_get_profile_info() function */
/**
* @description - Gets the list of MNO profiles on eSIM.
*
* @param[out] profile_list - Pointer to eSIMProfileStruct structure that contains the list of MNO profiles.
*                   <pre>
*                   The structure members are defined as below:
*                      iccid           -    It is a 32 bytes character array that represents the ICCID.
*                                           The example values for iccid are 98410800004860024951,98109909002143658739,
*                                           98414102915071000054.
*                      profileState    -    It is an integer which represents the state of the profile.
*                                           The values are 00 which represents disabled state and 01 which represents enabled state.
*                      profileName     -    It is a 16 bytes character array that represents the profile name.
*                                           The values are Xfinity Mobile, Comcast and CRTC.
*                   </pre>
* @param[out] nb_profiles - It is an integer pointer that provids the number of profiles.
*                           \n The valid range is 0 to 2,147,483,647.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_get_profile_info(eSIMProfileStruct **profile_list, int *nb_profiles);
/*  cellular_esim_enable_profile() function */
/**
* @description - Enable MNO profile with matching ICCID.
*
* @param[in] iccid - It is a 10 bytes character pointer which represents the ICCID.
*                    \nThe example values for iccid are 98410800004860024951,98109909002143658739,98414102915071000054.
* @param[out] iccid_size - It is an integer which represents the size of the ICCID.
*                          \n The value is 10.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_enable_profile(char* iccid, int iccid_size);
/*  cellular_esim_disable_profile() function */
/**
* @description - Disable MNO profile with matching ICCID.
*
* @param[in] iccid - It is a 10 bytes character pointer which represents the ICCID.
*                    \nThe example values for iccid are 98410800004860024951,98109909002143658739,98414102915071000054.
* @param[out] iccid_size - It is an integer which represents the size of the ICCID.
*                          \n The value is 10.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_disable_profile(char* iccid, int iccid_size);
/*  cellular_esim_delete_profile() function */
/**
* @description - Delete MNO profile with matching ICCID.
*
* @param[in] iccid - It is a 10 bytes character pointer which represents the ICCID.
*                    \nThe example values for iccid are 98410800004860024951,98109909002143658739,98414102915071000054.
* @param[out] iccid_size - It is an integer which represents the size of the ICCID.
*                          \n The value is 10.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_delete_profile(char* iccid, int iccid_size);
/*  cellular_esim_lpa_init() function */
/**
* @description - Initialize the LPA module that is to setup LPA SDK parameters.
* @param None.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_lpa_init(void);
/*  cellular_esim_lpa_exit function */
/**
* @description - Cleanup the LPA module.
* @param None.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_lpa_exit(void);
/*  cellular_esim_get_eid function */
/**
* @description - Get EID info.
* @param None.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_get_eid();
/*  cellular_esim_get_euicc function */
/**
* @description - Get eUicc info.
* @param None.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int cellular_esim_get_euicc();
#endif //_LPA_HAL_H_

/**
* @}
*/
