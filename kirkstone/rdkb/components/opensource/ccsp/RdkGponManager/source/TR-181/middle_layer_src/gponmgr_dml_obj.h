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

#ifndef  _GPONMGR_DML_OBJ_H
#define  _GPONMGR_DML_OBJ_H

#include <pthread.h>
#include "ansc_platform.h"
#include "ansc_string_util.h"
#include "gponmgr_dml_data.h"

typedef  ANSC_HANDLE
(*PFN_DM_CREATE)
    (
        VOID
    );

typedef  ANSC_STATUS
(*PFN_DM_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DM_INITIALIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * the main struct in cosa_xxx_apis.h need includes this struct and realize all functions.
 */
#define  COSA_BASE_CONTENT                                                              \
    /* start of object class content */                                                 \
    ULONG                           Oid;                                                \
    ANSC_HANDLE                     hSbContext;                                         \
                                                                                        \
    PFN_DM_CREATE               Create;                                                 \
    PFN_DM_REMOVE               Remove;                                                 \
    PFN_DM_INITIALIZE           Initialize;                                             \

typedef  struct
_COSA_BASE_OBJECT
{
    COSA_BASE_CONTENT
}
COSA_BASE_OBJECT,  *PCOSA_BASE_OBJECT;

/***********************************
    Actual definition declaration
************************************/
typedef  struct
_DATAMODEL_GPON_SERVICES
{
    COSA_BASE_CONTENT

    /* start of GPON object class content */
    GPON_DML_DATA*   data;

}
DATAMODEL_GPON_SERVICES,  *PDATAMODEL_GPON_SERVICES;

/**********************************
    Standard function declaration
***********************************/
ANSC_HANDLE GponMgrDml_create ( VOID );
ANSC_STATUS GponMgrDml_Initialize (ANSC_HANDLE hThisObject);
ANSC_STATUS GponMgrDml_Remove (ANSC_HANDLE hThisObject);



#endif
