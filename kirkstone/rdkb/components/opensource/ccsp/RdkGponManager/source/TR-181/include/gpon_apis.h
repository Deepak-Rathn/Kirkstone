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

#ifndef  _GPON_APIS_H
#define  _GPON_APIS_H

#include "ssp_global.h"

/***********************************
    Actual definition declaration
************************************/
/*
    GPON Part
*/

typedef struct
_DML_POWER
{
    INT     SignalLevel;
    INT     SignalLevelLowerThreshold;
    INT     SignalLevelUpperThreshold;
}
DML_POWER, *PDML_POWER;

typedef struct
_DML_VOLT
{
#if defined(WAN_MANAGER_UNIFICATION_ENABLED)
    INT     CurrentVoltage;
#else
    INT     VoltageLevel;
#endif
}
DML_VOLT, *PDML_VOLT;

typedef struct
_DML_BIAS
{
    UINT    CurrentBias;
}
DML_BIAS, *PDML_BIAS;

typedef struct
_DML_TEMPERATURE
{
    INT    CurrentTemp;
}
DML_TEMPERATURE, *PDML_TEMPERATURE;

typedef enum
_ALARM_ENUM
{
    ACTIVE,
    INACTIVE
} ALARM_ENUM;

typedef struct
_DML_ALARM
{
    ALARM_ENUM     RDI;
    ALARM_ENUM     PEE;
    ALARM_ENUM     LOS;
    ALARM_ENUM     LOF;
    ALARM_ENUM     DACT;
    ALARM_ENUM     DIS;
    ALARM_ENUM     MIS;
    ALARM_ENUM     MEM;
    ALARM_ENUM     SUF;
    ALARM_ENUM     SF;
    ALARM_ENUM     SD;
    ALARM_ENUM     LCDG;
    ALARM_ENUM     TF;
    ALARM_ENUM     ROGUE;
}
DML_ALARM, *PDML_ALARM;

typedef struct
_DML_GTC
{
    ULONG         CorrectedFecBytes;
    ULONG         CorrectedFecCodeWords;
    ULONG         UnCorrectedFecCodeWords;
#if defined(WAN_MANAGER_UNIFICATION_ENABLED)
    ULONG         GtcTotalFecCodeWords;
    ULONG         GtcHecErrorCount;
#else
    ULONG         TotalFecCodeWords;
    ULONG         HecErrorCount;
#endif
    ULONG         PSBdHecErrors;
    ULONG         FrameHecErrors;
    ULONG         FramesLost;
    ULONG         LastFetchedTime;
}
DML_GTC, *PDML_GTC;

typedef struct
_DML_PERFORM
{
    ULONG         SignalFail;
    ULONG         SignalDegrade;
}
DML_PERFORM, *PDML_PERFORM;

typedef enum
_DML_CAGE_ENUM
{
    BoB,
    SFP
}
DML_CAGE_ENUM;

typedef enum
_DML_PONMODE_ENUM
{
    GPON,
    XGPON,
    NGPON2,
    XGPON2
}
DML_PONMODE_ENUM;

typedef enum
_DML_CONNECTOR_ENUM
{
    LC,
    ST,
    FC,
    SC,
    MT_RJ
}
DML_CONNECTOR_ENUM;

typedef enum
_DML_LOCK_ENUM
{
    Lock,
    Unlock
}
DML_LOCK_ENUM;

typedef enum
_DML_PHYMEDIASTATUS_ENUM
{
    Up,
    Down,
    Unknown,
    Dormant,
    NotPresent,
    LowerLayerDown,
    Error
}
DML_PHYMEDIASTATUS_ENUM;

typedef enum
_DML_REDUNDANCYSTATE_ENUM
{
    Active,
    Standby
}
DML_REDUNDANCYSTATE_ENUM;

typedef enum
_DML_OPERSTATE_ENUM
{
    veip_Up,
    veip_Down,
    veip_Unknown,
    veip_Dormant,
    veip_NotPresent,
    veip_LowerLayerDown,
    veip_Error
}
DML_OPERSTATE_ENUM;

typedef  struct
_DML_PHY_MEDIA
{
    ULONG                       uInstanceNumber;
    DML_CAGE_ENUM               Cage;
    CHAR                        ModuleVendor[256];
    CHAR                        ModuleName[256];
    CHAR                        ModuleVersion[256];
    CHAR                        ModuleFirmwareVersion[256];
    DML_PONMODE_ENUM            PonMode;
    DML_CONNECTOR_ENUM          Connector;
    UINT                        NominalBitRateDownstream;
    UINT                        NominalBitRateUpstream;
#if defined(WAN_MANAGER_UNIFICATION_ENABLED)
    BOOLEAN 			Enable;
#endif
    DML_PHYMEDIASTATUS_ENUM     Status;
    DML_REDUNDANCYSTATE_ENUM    RedundancyState;
#if defined(WAN_MANAGER_UNIFICATION_ENABLED)
    CHAR                        Alias[64];
    UINT                        LastChange;
    CHAR                        LowerLayers[1024];
    BOOLEAN                     Upstream;
#endif
    DML_PERFORM                 PerformThreshold;
    DML_POWER                   RxPower;
    DML_POWER                   TxPower;
    DML_VOLT                    Voltage;
    DML_BIAS                    Bias;
    DML_TEMPERATURE             Temperature;
    DML_ALARM                   Alarm;
}
DML_PHY_MEDIA, *PDML_PHY_MEDIA;

typedef enum
_REG_STATE_ENUM
{
        O1,
        O2,
        O3,
        O4,
        O5,
        O6,
        O7,
        O8,
        O9
} REG_STATE_ENUM;

#if !defined(WAN_MANAGER_UNIFICATION_ENABLED)
typedef struct
_DML_REG_TIMER
{
    ULONG         TO1;
    ULONG         TO2;
}
DML_REG_TIMER, *PDML_REG_TIMER;
#endif

typedef struct
_DML_PLOAM
{
    UINT                OnuId;
    CHAR                VendorId[128];
    CHAR                SerialNumber[128];
    REG_STATE_ENUM      RegistrationState;
    ULONG               ActivationCounter;
    ULONG               TxMessageCount;
    ULONG               RxMessageCount;
    ULONG               MicErrors;
#if defined(WAN_MANAGER_UNIFICATION_ENABLED)
    ULONG         	TO1Timer;
    ULONG         	TO2Timer;
#endif
    ULONG               LastFetchedTime;
#if !defined(WAN_MANAGER_UNIFICATION_ENABLED)
    DML_REG_TIMER       RegistrationTimers;
#endif
}
DML_PLOAM, *PDML_PLOAM;

typedef struct
_DML_OMCI
{
#if defined(WAN_MANAGER_UNIFICATION_ENABLED)
    UINT        BaselineMessageCount;
    UINT        ExtendedMessageCount;
#else
    UINT        RxBaseLineMessageCountValid;
    UINT        RxExtendedMessageCountValid;
#endif
    ULONG       MicErrors;
    ULONG       LastFetchedTime;
}
DML_OMCI, *PDML_OMCI;

typedef enum
_DML_CAST_ENUM
{
    Unidirectional,
    Bidirectional
}
DML_CAST_ENUM;

typedef enum
_DML_TAGGED_ENUM
{
    Single,
    Double
}
DML_TAGGED_ENUM;

typedef enum
_DML_VEIP_TAGGED_ENUM
{
    veip_Untagged,
    veip_Single,
    veip_Double
}
DML_VEIP_TAGGED_ENUM;

typedef struct
_DML_GRESS_VLAN
{
    UINT    Vid;
    UINT    Pcp;
    UINT    Dei;
}
DML_GRESS_VLAN, *PDML_GRESS_VLAN;

typedef struct
_DML_GEM_GRESS
{
    DML_TAGGED_ENUM Tagged;
    DML_GRESS_VLAN  SVLAN;
    DML_GRESS_VLAN  CVLAN;
}
DML_GEM_GRESS, *PDML_GEM_GRESS;

typedef struct
_DML_VEIP_GRESS
{
    DML_VEIP_TAGGED_ENUM    Tagged;
    DML_GRESS_VLAN          QVLAN;
}
DML_VEIP_GRESS, *PDML_VEIP_GRESS;


typedef struct
_DML_GEM_ETHFLOW
{
    DML_GEM_GRESS     Ingress;
    DML_GEM_GRESS     Egress;
}
DML_GEM_ETHFLOW, *PDML_GEM_ETHFLOW;

typedef struct
_DML_VEIP_ETHFLOW
{
    DML_VEIP_GRESS     Ingress;
    DML_VEIP_GRESS     Egress;
}
DML_VEIP_ETHFLOW, *PDML_VEIP_ETHFLOW;

typedef struct
_DML_GEM
{
    ULONG             uInstanceNumber;
    ULONG             PortId;
    DML_CAST_ENUM     TrafficType;
    ULONG             TransmittedFrames;
    ULONG             ReceivedFrames;
    DML_GEM_ETHFLOW   EthernetFlow;
}
DML_GEM, *PDML_GEM;

typedef struct
_DML_VEIP
{
    ULONG              uInstanceNumber;
#if defined(WAN_MANAGER_UNIFICATION_ENABLED)
    ULONG              ManagedEntityId;
#else
    ULONG              MeId;
#endif
    DML_LOCK_ENUM      AdministrativeState;
    DML_OPERSTATE_ENUM OperationalState;
    CHAR               InterDomainName[25];
    CHAR               InterfaceName[256];
    DML_VEIP_ETHFLOW   EthernetFlow;
}
DML_VEIP, *PDML_VEIP;

typedef struct
_DML_TR69
{
    CHAR               url[256];
    ULONG              AssociatedTag;
    ULONG              LastFetchedTime;
}
DML_TR69, *PDML_TR69;


typedef  struct
_DML_X_RDK_GPON_DEVICE
{
    CHAR    RootDataModelVersion[32];
    UINT    InterfaceStackNumberOfEntries;
}
DML_X_RDK_GPON_DEVICE,  *PDML_X_RDK_GPON_DEVICE;

#endif
