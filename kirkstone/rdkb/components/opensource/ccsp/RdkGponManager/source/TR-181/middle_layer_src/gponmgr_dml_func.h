/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
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

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/

#ifndef  _GPONMGR_DML_FUNC_H
#define  _GPONMGR_DML_FUNC_H

/***********************************************************************

 APIs for Object:
GponPhy_GetParamStringValue
GponPhy_GetParamUlongValue
GponPhyAlarm_GetParamUlongValue
GponPhyRxpwr_GetParamIntValue
GponPhyRxpwr_SetParamIntValue
GponPhyTxpwr_GetParamIntValue
GponPhyTxpwr_SetParamIntValue
GponPhyVoltage_GetParamIntValue
GponPhyTemperature_GetParamIntValue
GponPhyPerformanceThreshold_GetParamUlongValue

GponGtc_GetParamUlongValue

GponPloam_GetParamUlongValue
GponPloam_GetParamStringValue
GponPloamRegTmr_GetParamUlongValue

GponGem_GetParamUlongValue
GponGemEthFlowIngress_GetParamUlongValue
GponGemEthFlowIngressSvlan_GetParamUlongValue
GponGemEthFlowIngressCvlan_GetParamUlongValue
GponGemEthFlowEgress_GetParamUlongValue
GponGemEthFlowEgressSvlan_GetParamUlongValue
GponGemEthFlowEgressCvlan_GetParamUlongValue

GponOmci_GetParamIntValue
GponOmci_GetParamUlongValue

GponVeip_GetParamStringValue
GponVeip_GetParamUlongValue
GponVeipEthFlowIngress_GetParamUlongValue
GponVeipEthFlowIngressQlan_GetParamUlongValue
GponVeipEthFlowEgress_GetParamUlongValue
GponVeipEthFlowEgress_SetParamUlongValue
GponVeipEthFlowEgressQlan_GetParamUlongValue
***********************************************************************/


typedef struct
_DML_FUNC_GET_ENTRY_
{
    UINT index;
}
DML_FUNC_GET_ENTRY_T;





BOOL GponPhy_IsUpdated(ANSC_HANDLE hInsContext);

ULONG GponPhy_Synchronize(ANSC_HANDLE hInsContext);

ULONG GponPhy_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE GponPhy_GetEntry(ANSC_HANDLE hInsContext,ULONG nIndex,ULONG* pInsNumber);

BOOL GponPhy_Validate(ANSC_HANDLE hInsContext,char* pReturnParamName,ULONG* puLength);

ULONG GponPhy_Commit(ANSC_HANDLE  hInsContext);

ULONG GponPhy_Rollback(ANSC_HANDLE hInsContext);

ULONG GponPhy_GetParamStringValue(ANSC_HANDLE hInsContext,char* ParamName,char*   pValue,ULONG*  pUlSize);

#if defined(WAN_MANAGER_UNIFICATION_ENABLED)
BOOL GponPhy_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL GponPhy_GetParamBoolValue(ANSC_HANDLE hInsContext,char* ParamName,BOOL*   pBool);

BOOL GponPhy_SetParamBoolValue(ANSC_HANDLE hInsContext,char* ParamName,BOOL bValue);
#endif

BOOL GponPhy_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponPhyRxpwr_GetParamIntValue(ANSC_HANDLE hInsContext,char* ParamName,int* pInt);

BOOL GponPhyRxpwr_SetParamIntValue(ANSC_HANDLE hInsContext,char* ParamName,int iValue);

BOOL GponPhyTxpwr_GetParamIntValue(ANSC_HANDLE hInsContext,char* ParamName,int* pInt);

BOOL GponPhyTxpwr_SetParamIntValue(ANSC_HANDLE hInsContext,char* ParamName,int iValue);

BOOL GponPhyVoltage_GetParamIntValue(ANSC_HANDLE hInsContext,char* ParamName,int* pInt);

BOOL GponPhyBias_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponPhyTemperature_GetParamIntValue(ANSC_HANDLE hInsContext,char* ParamName,int* pInt);

BOOL GponPhyPerformanceThreshold_GetParamUlongValue   (ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponPhyAlarm_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponGtc_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponPloam_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

ULONG GponPloam_GetParamStringValue(ANSC_HANDLE hInsContext,char* ParamName,char*   pValue,ULONG*  pUlSize);

#if !defined(WAN_MANAGER_UNIFICATION_ENABLED)
BOOL GponPloamRegTmr_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);
#endif

BOOL GponGem_IsUpdated(ANSC_HANDLE hInsContext);

ULONG GponGem_Synchronize(ANSC_HANDLE hInsContext);

ULONG GponGem_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE GponGem_GetEntry(ANSC_HANDLE hInsContext,ULONG nIndex,ULONG* pInsNumber);

BOOL GponGem_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponGemEthFlowIngress_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponGemEthFlowIngressSvlan_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponGemEthFlowIngressCvlan_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponGemEthFlowEgress_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponGemEthFlowEgressSvlan_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponGemEthFlowEgressCvlan_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponGem_Validate(ANSC_HANDLE hInsContext,char* pReturnParamName,ULONG* puLength);

ULONG GponGem_Commit(ANSC_HANDLE  hInsContext);

ULONG GponGem_Rollback(ANSC_HANDLE hInsContext);

BOOL GponOmci_GetParamIntValue(ANSC_HANDLE hInsContext,char* ParamName,int* pInt);

BOOL GponOmci_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponVeip_IsUpdated(ANSC_HANDLE hInsContext);

ULONG GponVeip_Synchronize(ANSC_HANDLE hInsContext);

ULONG GponVeip_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE GponVeip_GetEntry(ANSC_HANDLE hInsContext,ULONG nIndex,ULONG* pInsNumber);

ULONG GponVeip_GetParamStringValue(ANSC_HANDLE hInsContext,char* ParamName,char*   pValue,ULONG*  pUlSize);

BOOL GponVeip_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponVeipEthFlowIngress_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponVeipEthFlowIngress_SetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG uValue);

BOOL GponVeipEthFlowIngressQlan_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponVeipEthFlowIngressQlan_SetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG uValue);

BOOL GponVeipEthFlowEgress_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponVeipEthFlowEgress_SetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG uValue);

BOOL GponVeipEthFlowEgressQlan_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

BOOL GponVeipEthFlowEgressQlan_SetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG uValue);

BOOL GponVeip_Validate(ANSC_HANDLE hInsContext,char* pReturnParamName,ULONG* puLength);

ULONG GponVeip_Commit(ANSC_HANDLE  hInsContext);

ULONG GponVeip_Rollback(ANSC_HANDLE hInsContext);

ULONG Gpontr69_GetParamStringValue(ANSC_HANDLE hInsContext,char* ParamName,char*   pValue,ULONG*  pUlSize);

BOOL Gpontr69_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName,ULONG* puLong);

#endif
