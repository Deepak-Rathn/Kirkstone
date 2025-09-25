/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
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
* @file dhcp6cApi.h
* @brief For CCSP Component: DHCP6-Client Status
*
* @description This header file gives the function call prototypes and structure definitions used for the RDK-Broadband DHCP6Client Status abstraction layer.
*/

#ifndef _DHCP6_CLIENT_API_
#define _DHCP6_CLIENT_API_

/**
 * @addtogroup DHCPV4C_HAL_APIS
 * @{
 */

/* ert_dhcp6c_get_info() function */
/**
* @description Gets the E-Router Info.
* @param[out] pInfo - Pointer to dhcp6cInfo_t structure that will hold the complete E-Router information.
* <pre>
*                  The structure members are defined as below:
*                  ifname            -    It is a 32 bytes character array that represents the interface name.
*                                         The possible value is "erouter0"
*                  smState           -    It is an integer that represents the DHCP state.
*                                         The range of acceptable values are 0 to n, where n is an integer. The possible value is 6. (RENEW)
*                  configAttemps     -    It is an integer that provides the number of attempts made to configure DHCPv6.
*                                         The range of acceptable values are 0 to n, where n is an integer. The possible value is 3.
*                  ipAddr            -    It is a 16 bytes unsigned character array that represents the IPv6 address.
*                                         The possible value is {0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x0d, 0x3a, 0xe1, 0x12, 0xd5, 0x4e, 0x84, 0x34, 0x3b, 0xff}.
*                  prefix            -    It is a 16 bytes unsigned character array that represents the IPv6 prefix information.
*                                         The possible value is {0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x0d, 0x3a, 0xe1, 0x12, 0xd5, 0x4e, 0x84, 0x34, 0x3b, 0x00}.
*                  gw                -    It is a 16 bytes unsigned character array that represents the gateway
*                                         The possible value is {0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x0d, 0x3a, 0xe1, 0x12, 0xd5, 0x4e, 0x84, 0x34, 0x3b, 0x01}.
*                  dhcpServer        -    It is a 16 bytes unsigned character array that stores the IPv6 address of the DHCPv6 server.
*                                         The possible value is {0xfd, 0x07, 0xc6, 0xc1, 0x8a, 0x0b, 0x00, 0x00, 0x02, 0x1e, 0xff, 0xfe, 0xd4, 0x80, 0x00, 0x00}
*                  t1                -    It is an unsigned integer representing the timer.
*                                         The range of acceptable values are 0 to n, where n is a positive integer. The possible value is 3600.
*                  t2                -    It is an unsigned integer representing the timer.
*                                         The range of acceptable values are 0 to n, where n is a positive integer. The possible value is 7200.
*                  leaseTime         -    It is an unsigned integer that provides the offered lease time.
*                                         The range of acceptable values are 0 to n, where n is a positive integer. The possible value is 14400.
*                  leaseGotTime      -    It is an unsigned integer that provides the time when the lease was obtained from the DHCPv6 server.
*                                         The range of acceptable values are 0 to n, where n is a positive integer. The possible value is 1673210200.
*                  remainLeaseTime   -    It is an unsigned integer that provides the remaining lease time.
*                                         The range of acceptable values are 0 to n, where n is a positive integer. The possible value is 7200.
*                  remainRenewTime   -    It is an unsigned integer that provides the remaining time to renew.
*                                         The range of acceptable values are 0 to n, where n is a positive integer. The possible value is 3600.
*                  remainRebindTime  -    It is an unsigned integer that provides the remaining time to rebind.
*                                         The range of acceptable values are 0 to n, where n is a positive integer. The possible value is 600.
*                  numOfDns          -    It is an integer representing the number of DNS server addresses provided by the DHCPv6 server.
*                                         The range of acceptable values are 0 to n, where n is an integer. The possible value is 2.
*                  dnsAddrList       -    It is a two dimensional unsigned character array having a size of 3 bytes and 16 bytes respectively which provides the DNS IP address list.
*                                         It is a Vendor Specific value.
*  </pre>
*
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @remark The caller is responsible for providing a valid memory location for the function arguments.
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
int ert_dhcp6c_get_info(dhcp6cInfo_t *pInfo);
/* ecm_dhcp6c_get_info() function */
/**
* @description Gets the ECM Info.
* @param[out] pInfo - Pointer to dhcp6cInfo_t structure that will hold the complete ECM information.
* <pre>
*                  The structure members are defined as below:
*                  ifname            -    It is a 32 bytes character array that represents the interface name.
*                                         The possible value is "wan0"
*                  smState           -    It is an integer that represents the DHCP state.
*                                         The range of acceptable values are 0 to n, where n is an integer. The possible value is 6. (RENEW)
*                  configAttemps     -    It is an integer that provides the number of attempts made to configure DHCPv6.
*                                         The range of acceptable values are 0 to n, where n is an integer. The possible value is 3.
*                  ipAddr            -    It is a 16 bytes unsigned character array that represents the IPv6 address.
*                                         The possible value is {0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x0d, 0x3a, 0xe1, 0x12, 0xd5, 0x4e, 0x84, 0x34, 0x3b, 0xff}.
*                  prefix            -    It is a 16 bytes unsigned character array that represents the IPv6 prefix information.
*                                         The possible value is {0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x0d, 0x3a, 0xe1, 0x12, 0xd5, 0x4e, 0x84, 0x34, 0x3b, 0x00}.
*                  gw                -    It is a 16 bytes unsigned character array that represents the gateway
*                                         The possible value is {0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x0d, 0x3a, 0xe1, 0x12, 0xd5, 0x4e, 0x84, 0x34, 0x3b, 0x01}.
*                  dhcpServer        -    It is a 16 bytes unsigned character array that stores the IPv6 address of the DHCPv6 server.
*                                         The possible value is {0xfd, 0x07, 0xc6, 0xc1, 0x8a, 0x0b, 0x00, 0x00, 0x02, 0x1e, 0xff, 0xfe, 0xd4, 0x80, 0x00, 0x00}
*                  t1                -    It is an unsigned integer representing the timer.
*                                         The range of acceptable values are 0 to n, where n is a positive integer. The possible value is 3600.
*                  t2                -    It is an unsigned integer representing the timer.
*                                         The range of acceptable values are 0 to n, where n is a positive integer. The possible value is 7200.
*                  leaseTime         -    It is an unsigned integer that provides the offered lease time.
*                                         The range of acceptable values are 0 to n, where n is a positive integer. The possible value is 14400.
*                  leaseGotTime      -    It is an unsigned integer that provides the time when the lease was obtained from the DHCPv6 server.
*                                         The range of acceptable values are 0 to n, where n is a positive integer. The possible value is 1673210200.
*                  remainLeaseTime   -    It is an unsigned integer that provides the remaining lease time.
*                                         The range of acceptable values are 0 to n, where n is a positive integer. The possible value is 7200.
*                  remainRenewTime   -    It is an unsigned integer that provides the remaining time to renew.
*                                         The range of acceptable values are 0 to n, where n is a positive integer. The possible value is 3600.
*                  remainRebindTime  -    It is an unsigned integer that provides the remaining time to rebind.
*                                         The range of acceptable values are 0 to n, where n is a positive integer. The possible value is 600.
*                  numOfDns          -    It is an integer representing the number of DNS server addresses provided by the DHCPv6 server.
*                                         The range of acceptable values are 0 to n, where n is an integer. The possible value is 2.
*                  dnsAddrList       -    It is a two dimensional unsigned character array having a size of 3 bytes and 16 bytes respectively which provides the DNS IP address list.
*                                         It is a Vendor Specific value.
*  </pre>
*
* @return The status of the operation.
* @retval STATUS_SUCCESS if successful.
* @retval STATUS_FAILURE if any error is detected
*
* @remark The caller is responsible for providing a valid memory location for the function arguments.
*
* @execution Synchronous.
* @sideeffect None.
*
* @note This function must not suspend and must not invoke any blocking system
* calls. It should probably just send a message to a driver event handler task.
*
*/
int ecm_dhcp6c_get_info(dhcp6cInfo_t *pInfo);
/** @} */  //END OF GROUP DHCPV4C_HAL_APIS
#endif
