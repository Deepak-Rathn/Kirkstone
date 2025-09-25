/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
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
* @file bridge_util_hal.h
* @brief For Bridge Util Component: Bridge Utils OEM Layer
*
*/

/**
 * @defgroup BRIDGE_UTIL_OEM BRIDGE UTIL OEM
 *
 * BRIDGE UTIL OEM is used for the RDK-B Bridge Util OEM abstraction layer.
 *
 * @defgroup BRIDGE_UTIL_OEM_DATA_TYPES BRIDGE UTIL OEM Data Types
 * @ingroup  BRIDGE_UTIL_OEM
 *
 * @defgroup BRIDGE_UTIL_OEM_APIS BRIDGE UTIL OEM APIs
 * @ingroup  BRIDGE_UTIL_OEM
 *
 **/

#ifndef  _BRIDGE_UTIL_OEM_H
#define  _BRIDGE_UTIL_OEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

#include "OvsAgentApi.h"

/**
 * @addtogroup BRIDGE_UTIL_OEM_DATA_TYPES
 * @{
 */

#define BRIDGE_UTIL_LOG_FNAME "/rdklogs/logs/bridgeUtils.log"

#define GRE_HANDLER_SCRIPT "/etc/utopia/service.d/service_multinet/handle_gre.sh"

#define TOTAL_IFLIST_SIZE 	1024
#define BRIDGE_NAME_SIZE  	64
#define IFACE_NAME_SIZE  	64

#define IFLIST_SIZE		256
#define MAX_LOG_BUFF_SIZE	1024
#define TIMESTAMP           	64

#define  INTERFACE_EXIST       0
#define  INTERFACE_NOT_EXIST  -1

extern int DeviceMode; // router = 0, bridge = 2

extern int MocaIsolation_Val;

extern int need_wifi_gw_refresh;
extern int need_switch_gw_refresh;
extern int syncMembers;
extern int BridgeOprInPropgress;
extern FILE *logFp;
extern char log_buff[MAX_LOG_BUFF_SIZE];
extern char log_msg_wtime[MAX_LOG_BUFF_SIZE+TIMESTAMP];
extern char primaryBridgeName[64];
extern int PORT2ENABLE;
extern int ethWanEnabled;
extern char ethWanIfaceName[64];

extern struct tm *timeinfo;
extern time_t utc_time;

#define bridge_util_log(fmt ...)    {\
				    		snprintf(log_buff, MAX_LOG_BUFF_SIZE-1,fmt);\
				    		if(logFp != NULL){ \
                                                snprintf(log_buff, MAX_LOG_BUFF_SIZE-1,fmt);\
                                                utc_time = time(NULL);\
                                                timeinfo = gmtime(&utc_time);\
                                                snprintf(log_msg_wtime, MAX_LOG_BUFF_SIZE+TIMESTAMP-1,"%04d-%02d-%02d %02d:%02d:%02d ::: %s",timeinfo->tm_year+1900,timeinfo->tm_mon+1,timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec,log_buff);\
                                    		fprintf(logFp,"%s", log_msg_wtime);\
                                    		fflush(logFp);}\
                                 	}

/**
 * @enum Config
 *
 * @brief List of Configurations for BridgeUtils API's.
 */
enum Config {
	PRIVATE_LAN = 1,
	HOME_SECURITY = 2,
	HOTSPOT_2G = 3,
	HOTSPOT_5G = 4,
	LOST_N_FOUND = 6,
	HOTSPOT_SECURE_2G = 7,
	HOTSPOT_SECURE_5G = 8,
	MOCA_ISOLATION = 9,
	MESH_BACKHAUL = 10,
	ETH_BACKHAUL = 11,
	MESH = 12,
	MESH_WIFI_BACKHAUL_2G = 13,
	MESH_WIFI_BACKHAUL_5G = 14
#if defined  (WIFI_MANAGE_SUPPORTED)
        ,MANAGE_WIFI_BRIDGE = 17
#endif /* WIFI_MANAGE_SUPPORTED*/
};

/**
 * @enum INTERFACE_TYPE
 *
 * @brief List of Interface types for BridgeUtils API's.
 */
enum INTERFACE_TYPE {
    IF_BRIDGE_BRIDGEUTIL = 1,
    IF_VLAN_BRIDGEUTIL = 2,
    IF_GRE_BRIDGEUTIL = 3,
    IF_MOCA_BRIDGEUTIL = 4,
    IF_WIFI_BRIDGEUTIL = 5,
    IF_ETH_BRIDGEUTIL = 6,
    IF_OTHER_BRIDGEUTIL
};

/**
 * @enum BridgeOpr
 *
 * @brief BridgeUtils Operations for BridgeUtils API's.
 */
enum BridgeOpr {
	DELETE_BRIDGE = 0,
	CREATE_BRIDGE = 1
};

typedef struct bridgeDetails {
	char bridgeName[BRIDGE_NAME_SIZE];
	char vlan_name[BRIDGE_NAME_SIZE];
    	char VirtualParentIfname[BRIDGE_NAME_SIZE];
	int  vlanID;
	char ethIfList[IFLIST_SIZE];
	char MoCAIfList[IFLIST_SIZE];
	char GreIfList[IFLIST_SIZE];
	char WiFiIfList[IFLIST_SIZE];
}bridgeDetails;

/** @} */  //END OF GROUP BRIDGE_UTIL_OEM_DATA_TYPES

/**
 * @addtogroup BRIDGE_UTIL_OEM_APIS
 * @{
 */

/* updateBridgeInfo() function */
/**
* @brief Provides generic changes which needs to be configured after creating/updating/deleting bridge.
* @param[out] bridgeInfo - Pointer to bridgeDetails structure that will hold the complete bridge information.
* <pre>
* The structure members are defined as below:
*    bridgeName           -    It is a 64 bytes character array that represents the name of the bridge.
*                              It is vendor specific.
*    vlan_name            -    It is a 64 bytes character array that represents the virtual LAN name.
*                              It is vendor specific.
*    VirtualParentIfname  -    It is a 64 bytes character array that represents the virtual parent interface name.
*                              It is vendor specific.
*    vlanID               -    It is an integer that represents the vlanID.
*                              It is in range -2^31 to (2^31)-1.
*    ethIfList            -    It is a 256 bytes character array that represents the list of ethernet interfaces.
*                              It is vendor specific.
*    MoCAIfList           -    It is a 256 bytes character array that represents the list of MoCA interfaces.
*                              It is vendor specific.
*    GreIfList            -    It is a 256 bytes character array that represents the list of GRE interfaces.
*                              It is vendor specific.
*    WiFiIfList           -    It is a 256 bytes character array that represents the list of WiFi interfaces.
*                              It is vendor specific.
* </pre>
* @param[out] ifNameToBeUpdated - Is a 64 byte character array where the interface is to be deleted and updated, applicable only during sync. It is vendor specific.
* @param[in] Opr - It is an enumeration that defines the different network interface or bridge. It provides information about operations whether the request creating/updating/deleting bridge.
* \n The range of acceptable values is 1 to 4 based on OVS_CMD enum type.
* <pre>
*                  enum OVS_CMD is defined as below:
*                  OVS_OTHER_IF_TYPE =  0
*                  OVS_BRIDGE_IF_TYPE = 1
*                  OVS_ETH_IF_TYPE = 2
*                  OVS_GRE_IF_TYPE = 3
*                  OVS_VLAN_IF_TYPE = 4
* </pre>
* @param[in] type - It is an enumeration that defines the different types of interfaces and in case of sync delete the value is set to unknown/other.
* \n The range of acceptable values is 1 to 7 based on INTERFACE_TYPE enum type.
* <pre>
*                   enum INTERFACE_TYPE is defined as below:
*                   IF_BRIDGE_BRIDGEUTIL = 1
*                   IF_VLAN_BRIDGEUTIL = 2
*                   IF_GRE_BRIDGEUTIL = 3
*                   IF_MOCA_BRIDGEUTIL = 4
*                   IF_WIFI_BRIDGEUTIL = 5
*                   IF_ETH_BRIDGEUTIL = 6
*                   IF_OTHER_BRIDGEUTIL = 7
* </pre>
* @return The status of the operation.
* @retval 0 on success or -1 on negative error code.
*
* @remark The caller is responsible for providing a valid memory location for the function arguments.
*/
extern int updateBridgeInfo(bridgeDetails *bridgeInfo, char* ifNameToBeUpdated, int Opr , int type);
/* checkIfExists() function */
/**
* @brief Check if interface is created.
* @param[in] iface_name - It is a 64 byte character array which represents the name of the interface. It is vendor specific.
* @return The status of the operation.
* @retval 0 on success or -1 on negative error code.
*
*/
extern int checkIfExists(char* iface_name);
/* removeIfaceFromList() function */
/**
* @brief Remove interface from the list of interfaces.
* @param[out] str - It is a 64 byte character array which has the list of interfaces name. It is vendor specific.
* @param[in] sub - It is a 64 byte character array that represents the interface name that needs to be removed from the list. It is vendor specific.
* @return The status of the operation.
* @retval void
*
*/
extern void removeIfaceFromList(char *str, const char *sub);
/* checkIfExistsInBridge() function */
/**
* @brief Check if interface is attached to bridge.
* @param[in] iface_name - It is a 64 byte character array which represents the interface name. It is vendor specific.
* @param[out] bridge_name - It is a 64 byte character array which represents the bridge name.It is vendor specific.
* @return The status of the operation.
* @retval 0 on success or -1 on negative error code.
*
*/
extern int checkIfExistsInBridge(char* iface_name, char *bridge_name);
/* HandlePreConfigVendor() function */
/**
* @brief Provides OEM/SOC specific changes which needs to be configured before creating/updating/deleting bridge.
* @param[out] bridgeInfo - Pointer to bridgeDetails structure that will hold the complte bridge information.
* <pre>
* The structure members are defined as below:
*    bridgeName           -    It is a 64 bytes character array that represents the name of the bridge.
*                              It is vendor specific.
*    vlan_name            -    It is a 64 bytes character array that represents the virtual LAN name.
*                              It is vendor specific.
*    VirtualParentIfname  -    It is a 64 bytes character array that represents the virtual parent interface name.
*                              It is vendor specific.
*    vlanID               -    It is an integer that represents the vlanID.
*                              It is in range -2^31 to (2^31)-1.
*    ethIfList            -    It is a 256 bytes character array that represents the list of ethernet interfaces.
*                              It is vendor specific.
*    MoCAIfList           -    It is a 256 bytes character array that represents the list of MoCA interfaces.
*                              It is vendor specific.
*    GreIfList            -    It is a 256 bytes character array that represents the list of GRE interfaces.
*                              It is vendor specific.
*    WiFiIfList           -    It is a 256 bytes character array that represents the list of WiFi interfaces.
*                              It is vendor specific.
* </pre>
* @param[in] InstanceNumber - It is an enumeration that defines the instance number for configuration.
* \n The range of acceptable values is 1 to 14 based on Config enum type.
* <pre>
*              enum Config type is defined as below:
*	       PRIVATE_LAN = 1
*	       HOME_SECURITY = 2
*	       HOTSPOT_2G = 3
*	       HOTSPOT_5G = 4
*	       LOST_N_FOUND = 6
*	       HOTSPOT_SECURE_2G = 7
*	       HOTSPOT_SECURE_5G = 8
*	       MOCA_ISOLATION = 9
*	       MESH_BACKHAUL = 10
*	       ETH_BACKHAUL = 11
*	       MESH = 12
*	       MESH_WIFI_BACKHAUL_2G = 13
*	       MESH_WIFI_BACKHAUL_5G = 14
* @return The status of the operation.
* @retval 0 on success or -1 on negative error code.
*
* @remark The caller is responsible for providing a valid memory location for the function arguments.
*/
int HandlePreConfigVendor(bridgeDetails *bridgeInfo,int InstanceNumber);
/* HandlePostConfigVendor() function */
/**
* @brief Provides OEM/SOC specific changes which needs to be configured after creating/updating/deleting bridge.
* @param[out] bridgeInfo - Pointer to bridgeDetails structure that will hold the complete bridge information.
* <pre>
* The structure members are defined as below:
*    bridgeName           -    It is a 64 bytes character array that represents the name of the bridge.
*                              It is vendor specific.
*    vlan_name            -    It is a 64 bytes character array that represents the virtual LAN name.
*                              It is vendor specific.
*    VirtualParentIfname  -    It is a 64 bytes character array that represents the virtual parent interface name.
*                              It is vendor specific.
*    vlanID               -    It is an integer that represents the vlanID.
*                              It is in range -2^31 to (2^31)-1.
*    ethIfList            -    It is a 256 bytes character array that represents the list of ethernet interfaces.
*                              It is vendor specific.
*    MoCAIfList           -    It is a 256 bytes character array that represents the list of MoCA interfaces.
*                              It is vendor specific.
*    GreIfList            -    It is a 256 bytes character array that represents the list of GRE interfaces.
*                              It is vendor specific.
*    WiFiIfList           -    It is a 256 bytes character array that represents the list of WiFi interfaces.
*                              It is vendor specific.
* </pre>
* @param[in] Config - It is an enumeration that defines the instance number for configuration.
* \n The range of acceptable values is 1 to 14 based on Config enum type.
* <pre>
*              enum Config type is defined as below:
*	       PRIVATE_LAN = 1
*	       HOME_SECURITY = 2
*	       HOTSPOT_2G = 3
*	       HOTSPOT_5G = 4
*	       LOST_N_FOUND = 6
*	       HOTSPOT_SECURE_2G = 7
*	       HOTSPOT_SECURE_5G = 8
*	       MOCA_ISOLATION = 9
*	       MESH_BACKHAUL = 10
*	       ETH_BACKHAUL = 11
*	       MESH = 12
*	       MESH_WIFI_BACKHAUL_2G = 13
*	       MESH_WIFI_BACKHAUL_5G = 14
* @return The status of the operation.
* @retval 0 on success or -1 on negative error code.
*
* @remark The caller is responsible for providing a valid memory location for the function arguments.
*/
int HandlePostConfigVendor(bridgeDetails *bridgeInfo,int Config);
/* getVendorIfaces() function */
/**
* @brief Provides vendor interface information for creating/updating/deleting bridge.
* @param None
* @return The status of the operation.
* @retval vendor interface which is a 64 byte character array or NULL.
*
*/
char *getVendorIfaces();
/** @} */  //END OF BRIDGE_UTIL_OEM_APIS
#endif
