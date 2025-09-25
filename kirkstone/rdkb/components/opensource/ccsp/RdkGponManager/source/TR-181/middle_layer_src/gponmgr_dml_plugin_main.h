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

/**********************************************************************

    module: gponmgr_dml_plugin_main.h

        For COSA Library development.

    ---------------------------------------------------------------

    description:

        This header file defines the exported apis for COSA Library plugin.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    revision:

        09/12/2019    initial revision.

**********************************************************************/


#ifndef  _GPONMGR_DML_PLUGIN_MAIN_H
#define  _GPONMGR_DML_PLUGIN_MAIN_H

#include "gponmgr_dml_backendmgr.h"


#define ANSC_EXPORT_API

#ifdef __cplusplus
extern "C"{
#endif

int ANSC_EXPORT_API GponMgrDml_Init(ULONG uMaxVersionSupported, void* hCosaPlugInfo);

#ifdef __cplusplus
}
#endif

#endif
