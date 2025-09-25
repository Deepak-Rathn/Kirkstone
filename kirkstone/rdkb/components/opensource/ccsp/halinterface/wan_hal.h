/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2019 RDK Management
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

#ifndef __WAN_HAL_H__
#define __WAN_HAL_H__

#include <stdint.h>

/**
 * @defgroup WAN_HAL WAN HAL
 *
 * @defgroup WAN_HAL_TYPES  WAN HAL Data Types
 * @ingroup  WAN_HAL
 *
 * @defgroup WAN_HAL_APIS   WAN HAL APIs
 * @ingroup  WAN_HAL
 *
 **/

/**
 * @addtogroup WAN_HAL_TYPES
 * @{
 */

#define RETURN_ERROR 1
#define RETURN_OK 0

#define BUFLEN_8 8
#define BUFLEN_32 32
#define BUFLEN_64 64

typedef enum
{
    WAN_MODE_DISABLED = 1,
    WAN_MODE_DSL_ONLY = 2,
    WAN_MODE_ETH_ONLY = 3,
    WAN_MODE_AUTO = 4
} t_eWanMode;

typedef struct 
_WAN_IPV4_CFG 
{
    char ifname[64];         // interface name (erouter0)
    char subnetmask[64];     // subnet mask
    char ipaddress[64];      // ip address to assign (192.168.0.1)
    char dnsservers[256];    // new-line separated list of servers to be added
    char defaultgateway[64]; // default gateway address

} WAN_IPV4_CFG, *PWAN_IPV4_CFG;

typedef struct
_WAN_IPV6_CFG
{
    char ifname[64];         // interface name (erouter0)
    char ipaddress[128];      // ip address to assign 
    char dnsservers[512];    // new-line separated list of servers to be added
    int  preferredlifetime;  
    int  validlifetime;

} WAN_IPV6_CFG, *PWAN_IPV6_CFG;

/*
 * The WAN_QOS_QUEUE structure needed in ccsp-wanagent and wan-hal
  */
typedef  struct
_WAN_QOS_QUEUE
{
    unsigned long       InstanceNumber;
    char                Alias[64];
    unsigned long       queueKey;
    unsigned char       queueEnable;
    char                queueStatus[256];
    char                queueInterface[256];
    unsigned long       queueWeight;
    unsigned long       queuePrecedence;
    unsigned long       REDThreshold;
    char                dropAlgorithm[256];
    char                schedulerAlgorithm[256];
    signed long         shapingRate;
    unsigned long       shapingBurstSize;
    signed long         MinBitRate;
    char                QueueName[256];
    signed long         DslLatency;
    signed long         PtmPriority;
    unsigned long       QueueId;
    unsigned long       LowClassMaxThreshold;
    unsigned long       LowClassMinThreshold;
    unsigned long       HighClassMinThreshold;
    unsigned long       HighClassMaxThreshold;
    char                L2DeviceType[32];

}WAN_QOS_QUEUE,  *PWAN_QOS_QUEUE;

/* * MAPT */
typedef  struct
_WAN_MAPT_CFG
{
    char          ifName[64];
    char          brIPv6Prefix[128];
    char          ruleIPv4Prefix[128];
    char          ruleIPv6Prefix[128];
    unsigned int  psidOffset;
    unsigned int  ratio;
    char          pdIPv6Prefix[128];

} WAN_MAPT_CFG, *PWAN_MAPT_CFG;

/**
 * Structure for SELFHEAL configuration which is required in ccsp-wanagent
 * and wan hal. */
typedef struct
_SELFHEAL_CONFIG
{
    unsigned int rebootStatus;
}
SELFHEAL_CONFIG, *PSELFHEAL_CONFIG;

/** @} */  //END OF GROUP WAN_HAL_TYPES

/**
 * @addtogroup WAN_HAL_APIS
 * @{
 */

/* wan_hal_Init() function */
/**
* @description Initialise the wan features.
*
* @param None.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int wan_hal_Init();

/* wan_hal_SetWanmode() function */
/**
* @description Set wanmode value.
*
* @param[in] mode It is of type enum, that represents WANMODE value to be set.
*                   \n The possible values of the WANMODE are:
*                   \n WAN_MODE_DISABLED = 1,
*                   \n WAN_MODE_DSL_ONLY = 2,
*                   \n WAN_MODE_ETH_ONLY = 3,
*                   \n WAN_MODE_AUTO = 4.
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int wan_hal_SetWanmode(t_eWanMode mode);

/* wan_hal_SetWanConnectionEnable() function */
/**
* @description Set wanconnection enable value.
*
* @param[in] enable It is an unsigned integer, that represents WanConnectionEnable value to be set.
*                   \n The possible values of enable are:
*                   \n ENABLE : 1, DISABLE : 0.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
*
*/
int wan_hal_SetWanConnectionEnable(unsigned int enable);
/**
* @description Set SelfHeal Configuration to wanmanager.
*
* @param[in] pSelfHealConfig Pointer to structure that contains the configuration.
*                              \n And the only structure member is rebootStatus.
*                              \n The possible values for rebootStatus is 0 or 1.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
* @remark The caller is responsible for providing a valid memory location for the function argument.
*
*/
int wan_hal_SetSelfHealConfig(PSELFHEAL_CONFIG pSelfHealConfig);

/* wanmgr_hal_GetWanOEUpstreamCurrRate() function */
/**
* @description Get the current payload bandwidth of the upstream WANoE Connection.
*
* @param[out] pValue A pointer to an unsigned integer, where the current payload bandwidth of the upstream WANoE Connection to be updated.
*                    \n The valid range of the upstream rate is 0 to 2,147,483,647.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
* @remark The caller is responsible for providing a valid memory location for the function argument.
*
*/
int wan_hal_GetWanOEUpstreamCurrRate(unsigned int *pValue);

/* wanmgr_hal_GetWanOEDownstreamCurrRate() function */
/**
* @description Get the current payload bandwidth of the downstream WANoE Connection.
*
* @param[out] pValue A pointer to an unsigned integer, where the current payload bandwidth of the downstream WANoE Connection to be updated.
*                    \n The valid range of the downstream rate is 0 to 2,147,483,647.
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
* @remark The caller is responsible for providing a valid memory location for the function argument.
*
*/
int wan_hal_GetWanOEDownstreamCurrRate(unsigned int *pValue);

/* wan_hal_SetQosConfiguration() function */
/**
* @description Sets the current QoS configuration.
* @param[in] pQueue A Pointer the structure which contains the QoS configurations to be set.
* <pre>
* The structure members are defined below:
*     InstanceNumber        - It is an unsigned long integer (8 bytes) that represents the instance number.
*                             The valid range of InstanceNumber is 1 to 128.
*     Alias                 - It is an array of characters (a string) with a size of 64. It stores an alias or name for the queue.
*                             The possible values of the Alias are :
*                             Queue_1, Queue_2, Queue_3, Queue_4, Queue_5, Queue_6, Queue_7, Queue_8, Queue_9, Queue_10.
*     queueKey              - It is an unsigned long integer (8 bytes) representing a key for the queue.
*                             The possible values of the queueKey is : 0 to Maxqueues-1, where Maxqueues is a vendor specific value.
*     queueEnable           - It is an unsigned char (1 byte) indicating whether the queue is enabled or disabled.
*                             The possible values of queueEnable are : TRUE, FALSE.
*     queueStatus           - It is an array of characters (a string) with a size of 256. It contains the status of the queue.
*                             The possibel values of the queueStatus are : Disabled, Enabled.
*     queueInterface        - It is an array of characters (a string) with a size of 256. It represents the associated interface for the queue.
*                             The possible values of queueInterface are : erouter0.
*     queueWeight           - It is an unsigned long integer (8 bytes) representing the weight of the queue.
*                             The range is usually between 0 (no weight) and 100 (maximum weight).
*                             It is a vendor specific value.
*     queuePrecedence       - It is an unsigned long integer (8 bytes) indicating the precedence of the queue.
*                             The range is [1:4294967295].It is a vendor specific value.
*     REDThreshold          - It is an unsigned long integer (8 bytes) representing the RED (Random Early Detection) threshold for the queue.
*                             Range: [1:100].It is a vendor specific value.
*     dropAlgorithm         - It is an array of characters (a string) with a size of 256. It specifies the drop algorithm used by the queue.
*                             The Possible Values: DT, RED, WRED, ARED, Blue, SFB, PIE, CoDel, FQ-CoDel.
*                             It is a vendor specific value.
*     schedulerAlgorithm    - It is an array of characters (a string) with a size of 256. It likely represents the scheduling algorithm used by the queue.
*                             The Possible Values: SP, WRR, WFQ, RR, PQ, CBWFQ, LLQ.
*                             It is a vendor specific value.
*     shapingRate           - It is a signed long integer (8 bytes) indicating the shaping rate for the queue.
*                             Range: [-1:2147483647].It is a vendor specific value.
*     shapingBurstSize      - It is an unsigned long integer (8 bytes) representing the burst size for the queue shaping.
*                             Range: 0 to the maximum burst size in bytes.
*     MinBitRate            - It is a signed long integer (8 bytes) indicating the minimum bit rate for the queue.
*                             Range: [-1:2147483647].It is a vendor specific value.
*     QueueName             - It is an array of characters (a string) with a size of 256. It represents the name of the queue.
*                             Range: Depends on character and length limitations.
*                             Possible values : WAN Q1, WAN Q2, WAN Q3, WAN Q4, WAN Q5, WAN Q6, WAN Q7, WAN Q8, WAN Q9, WAN Q10.
*     DslLatency            - It is a signed long integer (8 bytes) representing the DSL latency for the queue.
                              Range: [-1:1].It is a vendor specific value.
*     PtmPriority           - It is a signed long integer (8 bytes) indicating the Ptm priority for the queue.
*                             Range: [-1:1].It is a vendor specific value.
*     QueueId               - It is an unsigned long integer (8 bytes) representing the ID of the queue.
*                             Range: A unique identifier, typically between 0 and maxqueues, where maxqueues is a vendor specific value.
*                             Examples are : 1,2,3,4,5,6,7,8.
*     LowClassMaxThreshold  - It is a unsigned long integer (8 bytes) representing MaxThreshold for low class of the queue.
*                             Range: [0:100].It is a vendor specific value.
*     LowClassMinThreshold  - It is a unsigned long integer (8 bytes) representing MinThreshold for low class of the queue.
*                             Range: [0:100].It is a vendor specific value.
*     HighClassMinThreshold - It is a unsigned long integer (8 bytes) representing MinThreshold for high class of the queue.
*                             Range: [0:100].It is a vendor specific value.
*     HighClassMaxThreshold - It is a unsigned long integer (8 bytes) representing MaxThreshold for high class of the queue.
*                             Range: [0:100].It is a vendor specific value.
*     L2DeviceType          - It is an array of characters (a string) with a size of 32. It indicates the type of Layer 2 device associated with the queue.
*                             The possible values are : eth, ptm.
* </pre>
*
* @param[in] QueueNumberOfEntries The number of QoS profiles.
*            \n It is a vendor specific value.
* @param[in] baseifname A pointer to the character array that will hold the Base interface name.
*                         \n The buffer size should be atleast 32 bytes.
*                         \n The possible values are : erouter0.
* @param[in] wanifname A pointer to the character array that will hold the Wan interface name.
*                        \n The buffer size should be atleast 32 bytes.
*                        \n The possible values are : erouter0.
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
* @remark The caller is responsible for providing a valid memory location for the function argument.
*
*/
int wan_hal_SetQoSConfiguration(PWAN_QOS_QUEUE pQueue, unsigned int QueueNumberOfEntries, const char* baseifname, const char* wanifname);

/* wan_hal_ConfigureIpv4() function */
/**
* @description configure IPv4 dnsservers, netmask and ip address for the required interface
*
* @param[in] pWanIpv4Cfg Pointer to the structure.
* <pre>
* The structure members are defined below:
*     ifname                - interface name. It is a 64 byte character array.
*                             The possible values are : erouter0.
*     subnetmask            - subnetmask. It is a 64 byte character array.
*                             The valid range for IPv4 subnetmask spans from 0.0.0.0 to 255.255.255.255
*                             Example: 255.255.255.0
*     ipaddress             - ipaddress. It is a 64 byte character array.
*                             The valid ranges for IPv4 addresses are:  1.0.0.0 to 127.0.0.0, 128.0.0.0 to 191.255.0.0, 192.0.0.0 to 223.255.255.0
*     dnsservers            - list of servers ips to be added, separated by a new-line.
*                             It is a 256 byte character array.
*                             The valid ranges for IPv4 addresses are:  1.0.0.0 to 127.0.0.0, 128.0.0.0 to 191.255.0.0, 192.0.0.0 to 223.255.255.0
*     defaultgateway        - default gateway address.It is a 64 byte character array.
*                             The valid ranges for IPv4 addresses are:  1.0.0.0 to 127.0.0.0, 128.0.0.0 to 191.255.0.0, 192.0.0.0 to 223.255.255.0
*                             Example: 192.168.1.1
* </pre>
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
* @remark The caller is responsible for providing a valid memory location for the function argument.
*
*/
int wan_hal_ConfigureIpv4(PWAN_IPV4_CFG pWanIpv4Cfg);

/* wan_hal_UnConfigureIpv4() function */
/**
* @description unconfigure IPv4 dnsservers, netmask and ip address for the required interface
*
* @param[in] pWanIpv4Cfg Pointer to the structure.
* <pre>
* The structure members are defined below:
*     ifname                - interface name. It is a 64 byte character array.
*                             The possible values are : erouter0.
*     subnetmask            - subnetmask. It is a 64 byte character array.
*                             The valid range for IPv4 subnetmask spans from 0.0.0.0 to 255.255.255.255
*                             Example: 255.255.255.0
*     ipaddress             - ipaddress. It is a 64 byte character array.
*                             The valid ranges for IPv4 addresses are:  1.0.0.0 to 127.0.0.0, 128.0.0.0 to 191.255.0.0, 192.0.0.0 to 223.255.255.0
*     dnsservers            - list of servers ips to be added, separated by a new-line.
*                             It is a 256 byte character array.
*                             The valid ranges for IPv4 addresses are:  1.0.0.0 to 127.0.0.0, 128.0.0.0 to 191.255.0.0, 192.0.0.0 to 223.255.255.0
*     defaultgateway        - default gateway address.It is a 64 byte character array.
*                             The valid ranges for IPv4 addresses are:  1.0.0.0 to 127.0.0.0, 128.0.0.0 to 191.255.0.0, 192.0.0.0 to 223.255.255.0
*                             Example: 192.168.1.1
* </pre>
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
* @remark The caller is responsible for providing a valid memory location for the function argument.
*
*/
int wan_hal_UnConfigureIpv4(PWAN_IPV4_CFG pWanIpv4Cfg);

/* wan_hal_ConfigureIpv6() function */
/**
* @description Add IPv6 address for the required interface
*
* @param[in] pWanIpv6Cfg Pointer to the structure.
* <pre>
* The structure members are defined below:
*     ifname              -   interface name. It is a 64 byte character array.
*                             The possible values are : erouter0.
*     ipaddress           -   ipaddress. It is a 128 byte character array.
*                             The range 2000:: to 3FFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF
*                             Example: 2001:0db8:85a3:0000:0000:8a2e:0370:7334
*     dnsservers          -   list of servers ips to be added, separated by new-line.
*                             It is a 512 byte character array.
*                             The range 2000:: to 3FFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF
*     preferredlifetime   -   A preferred lifetime is the length of time an address is intended for full use on
*                             an interface, and must be less than or equal to the address's valid lifetime.
*                             And preferredlifetime is of int type.
*                             The valid range for the IPv6 "preferred lifetime" is from 0 to 4,294,967,295 seconds.
*                             Example: 604800 sec.
*     validlifetime       -   The valid lifetime, which is the total time the address is available,
*                             is equal to or greater than the preferred lifetime.
*                             And validlifetime is of int type.
*                             The valid range for the IPv6 "valid lifetime" is from 0 to 4,294,967,295 seconds.
*                             Example: 2592000 sec.
* </pre>
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
* @remark The caller is responsible for providing a valid memory location for the function argument.
*
*/
int wan_hal_ConfigureIpv6(PWAN_IPV6_CFG pWanIpv6Cfg);

/* wan_hal_UnConfigureIpv4() function */
/**
* @description Delete IPv6 address for the required interface
*
* @param[in] pWanIpv6Cfg Pointer to the structure.
* <pre>
* The structure members are defined below:
*     ifname              -   interface name. It is a 64 byte character array.
*                             The possible values are : erouter0.
*     ipaddress           -   ipaddress. It is a 128 byte character array.
*                             The range 2000:: to 3FFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF
*                             Example: 2001:0db8:85a3:0000:0000:8a2e:0370:7334
*     dnsservers          -   list of servers ips to be added, separated by new-line.
*                             It is a 512 byte character array.
*                             The range 2000:: to 3FFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF
*     preferredlifetime   -   A preferred lifetime is the length of time an address is intended for full use on
*                             an interface, and must be less than or equal to the address's valid lifetime.
*                             And preferredlifetime is of int type.
*                             The valid range for the IPv6 "preferred lifetime" is from 0 to 4,294,967,295 seconds.
*                             Example: 604800 sec.
*     validlifetime       -   The valid lifetime, which is the total time the address is available,
*                             is equal to or greater than the preferred lifetime.
*                             And validlifetime is of int type.
*                             The valid range for the IPv6 "valid lifetime" is from 0 to 4,294,967,295 seconds.
*                             Example: 2592000 sec.
* </pre>
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
* @remark The caller is responsible for providing a valid memory location for the function argument.
*
*/
int wan_hal_UnConfigureIpv6(PWAN_IPV6_CFG pWanIpv6Cfg);

/* wan_hal_EnableMapt() function */
/**
* @description Enable MAPT for the required interface
*
* @param[in] pMAPTCfg Pointer to the structure.
* <pre>
* The structure members are defined below:
*     ifname              -   interface name. It is a 64 byte character array.
*                             The possible values are : erouter0.
*     brIPv6Prefix        -   Is an array of characters (a string) with a size of 128. It is a valid IPv6 address in string format.
*                             The range 2000:: to 3FFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF
*                             Example: 2001:0db8:85a3:0000:0000:8a2e:0370:7334
*                             The buffer size should be at least 128 bytes long.
*     ruleIPv4Prefix      -   Is an array of characters (a string) with a size of 128. It is a valid IPv4 address in string format.
*                             The valid ranges for IPv4 addresses are:  1.0.0.0 to 127.0.0.0, 128.0.0.0 to 191.255.0.0, 192.0.0.0 to 223.255.255.0
*                             The buffer size should be at least 128 bytes long.
*     ruleIPv6Prefix      -   Is an array of characters (a string) with a size of 128. It is a valid IPv6 address in string format.
*                             The range 2000:: to 3FFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF
*                             Example: 2001:0db8:85a3:0000:0000:8a2e:0370:7334
*                             The buffer size should be at least 128 bytes long.
*     psidOffset          -   Is an unsigned integer (4 bytes) that represents the offset value for a Port Set Identifier (PSID).
*                             Range: 0 to 15. It is a vendor specific value.
*     ratio               -   Is an unsigned integer (4 bytes) representing a ratio.
*                             Range: 1 to 65,536. It is a vendor specific value.
*     pdIPv6Prefix        -   Is an array of characters (a string) with a size of 128. It stores an IPv6 prefix for prefix delegation.
*                             Range: Strings representing valid IPv6 prefixes with a maximum length of 127 characters (keeping the last character for a null terminator).
*                             The format of an IPv6 prefix typically looks like: 2001:0db8:85a3::/64, where 2001:0db8:85a3:: is the prefix and /64 denotes the prefix length (in this example, 64 bits).
*                             The prefix length can vary, commonly seen values include /48, /56, /60, /64, etc., but the exact valid range of prefix lengths will depend on the vendor.
* </pre>
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
* @remark The caller is responsible for providing a valid memory location for the function argument.
*
*/
int wan_hal_EnableMapt( PWAN_MAPT_CFG pMAPTCfg );

/* wan_hal_DisableMapt() function */
/**
* @description Disable MAPT for the required interface
*
* @param[in] ifName It is a pointer to a constant character array (string) named ifName.
*                  \n The possible values are : erouter0.
*                  \n This parameter represents the name of the interface on which the MAP-T feature is to be disabled.
*                  \n The buffer size should be at least 64 bytes long.
*
* @return The status of the operation.
* @retval RETURN_OK if successful.
* @retval RETURN_ERR if any error is detected.
* @remark The caller is responsible for providing a valid memory location for the function argument.
*
*/
int wan_hal_DisableMapt(const char* ifName);

/**
 * @description Set/Reset WanOE mode based on the enable flag. If it is
 * true, update the boardparam to set WAN_ETH_MODE in the CPE. Else disabled
 * the WAN_ETH_MODE and reclaim ethernet port.
 * @param[in] enable It is an unsigned char type named enable, used to control the enabling or disabling of the WAN OE mode.
 *                 \n The use of the const qualifier indicates that the parameter value will not be modified within the function.
 *                 \n The possible values are : Enable, Disable.
 */
int wan_hal_enableWanOEMode(const unsigned char enable);

/**
 * @description Get authentication information like ADSL username and password.
 * @param[out] autInfo to hold the authentication data.
 *          \n It is a pointer to a character array (string) named authInfo.
 *          \n This parameter is used to pass a buffer or memory location where the authentication information will be stored.
 *          \n The buffer size should be at least 256 bytes long and the maximum buffer size is 16384.
 *          \n It is a vendor specific value.
 * @retval RETURN_OK if successful else RETURN_ERR.
 * @remark The caller is responsible for providing a valid memory location for the function argument.
 */
int wan_hal_getAuthInfo(char *authInfo);

/** @} */  //END OF GROUP WAN_HAL_APIS
#endif /* __WAN_HAL_H__ */
