/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2022 Sky
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * Copyright 2022 RDK Management
 * Licensed under the Apache License, Version 2.0
 */

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

extern "C"
{
#include "cellularmgr_sm.h"
#include "cellularmgr_rbus_events.h"
#include "cellularmgr_cellular_apis.h"
#include "cellularmgr_cellular_internal.h"
#include "cellularmgr_messagebus_interface.h"
#include "cellular_hal_utils.h"
#include "cellular_hal_qmi_apis.h"
#include "cellularmgr_rbus_dml.h"
#include "cellularmgr_cellular_dml.h"
#include "cellularmgr_ssp_internal.h"
#include "cellularmgr_cellular_webconfig_api.h"
#include "cellularmgr_cellular_param.h"
#include "cellularmgr_plugin_main.h"
#include "cellularmgr_plugin_main_apis.h"
#include "cellularmgr_utils.h"
#include "cellularmgr_cellular_helpers.h"
#include "cellularmgr_rbus_helpers.h"
#include "cellularmgr_bus_utils.h"
#include <rbus.h>
#include "cellular_hal.h"
}

#define GTEST_DEFAULT_RESULT_FILEPATH "/tmp/Gtest_Report/"
#define GTEST_DEFAULT_RESULT_FILENAME "RdkCellularManager_gtest_report.xml"
#define GTEST_REPORT_FILEPATH_SIZE 128

class CellularManagerEnvironment : public ::testing::Environment
{
    public:
        virtual ~CellularManagerEnvironment()
        {
        }
        virtual void SetUp()
        {
            printf("%s Environment\n", __func__);
        }
        virtual void TearDown()
        {
            printf("%s Environment\n", __func__);
        }
};

GTEST_API_ int main(int argc, char* argv[])
{
    char testresults_fullfilepath[GTEST_REPORT_FILEPATH_SIZE];
    char buffer[GTEST_REPORT_FILEPATH_SIZE];

    memset(testresults_fullfilepath, 0, GTEST_REPORT_FILEPATH_SIZE);
    memset(buffer, 0, GTEST_REPORT_FILEPATH_SIZE);

    snprintf(testresults_fullfilepath, GTEST_REPORT_FILEPATH_SIZE, "xml:%s%s", GTEST_DEFAULT_RESULT_FILEPATH, GTEST_DEFAULT_RESULT_FILENAME);
    ::testing::GTEST_FLAG(output) = testresults_fullfilepath;
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
