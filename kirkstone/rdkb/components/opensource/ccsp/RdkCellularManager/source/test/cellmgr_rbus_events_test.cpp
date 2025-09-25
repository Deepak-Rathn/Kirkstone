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

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <experimental/filesystem>
#include "mocks/rbusevent_mock.h"
#include "cellmgr_mock.h"
#define MOCK_CELLULAR_MGR_RBUS_EVENT_MAIN_2

using namespace std;
using std::experimental::filesystem::exists;
using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

extern "C"
{
#include "cellularmgr_global.h"
#include "cellularmgr_cellular_apis.h"
#include "cellularmgr_cellular_internal.h"
#include <rbus.h>
#include <stdio.h>
#include "cellularmgr_rbus_events.h"
}

extern rbusEventMock * g_rbusEventMock;

extern CellularMGR_rbusSubListSt gRBUSSubListSt;

TEST_F (CellularManagerTestFixture, CellularMgr_RBUS_Events_PublishPhyConnectionStatus1)
{
        unsigned char bPrevPhyState = 1;
        unsigned char bCurrentPhyState = 0;
        gRBUSSubListSt.stInterface.PhyConnectedStatusSubFlag = 1;
        EXPECT_NE(bPrevPhyState, bCurrentPhyState);
        char pParamName[] = "Device.Cellular.Interface.1.X_RDK_PhyConnectedStatus";
        ON_CALL(*g_rbusEventMock, CellularMgr_Rbus_String_EventPublish_OnValueChange( StrEq(pParamName), _, _, RBUS_BOOLEAN))
        .WillByDefault(Return(ANSC_STATUS_FAILURE));
        int res = CellularMgr_RBUS_Events_PublishPhyConnectionStatus(bPrevPhyState,bCurrentPhyState);
        EXPECT_EQ(RETURN_OK, res);

}

TEST_F (CellularManagerTestFixture, CellularMgr_RBUS_Events_PublishPhyConnectionStatus2){
        unsigned char bPrevPhyState = 1;
        unsigned char bCurrentPhyState = 0;
        gRBUSSubListSt.stInterface.PhyConnectedStatusSubFlag = 1;
        EXPECT_NE(bPrevPhyState, bCurrentPhyState);
        char pParamName[] = "Device.Cellular.Interface.1.X_RDK_PhyConnectedStatus";
        ON_CALL(*g_rbusEventMock, CellularMgr_Rbus_String_EventPublish_OnValueChange( StrEq(pParamName), _, _, RBUS_BOOLEAN))
        .WillByDefault(Return(ANSC_STATUS_SUCCESS));

        int ret = CellularMgr_RBUS_Events_PublishPhyConnectionStatus(bPrevPhyState,bCurrentPhyState);
        EXPECT_EQ(RETURN_OK, ret) ;
}

TEST_F (CellularManagerTestFixture, CellularMgr_RBUS_Events_PublishPhyConnectionStatus3){
        unsigned char bPrevPhyState = 0;
        unsigned char bCurrentPhyState = 1;
        gRBUSSubListSt.stInterface.PhyConnectedStatusSubFlag = 1;
        EXPECT_NE(bPrevPhyState, bCurrentPhyState);
        char pParamName[] = "Device.Cellular.Interface.1.X_RDK_PhyConnectedStatus";
        ON_CALL(*g_rbusEventMock, CellularMgr_Rbus_String_EventPublish_OnValueChange( StrEq(pParamName), _, _, RBUS_BOOLEAN))
        .WillByDefault(Return(ANSC_STATUS_SUCCESS));

        int ret = CellularMgr_RBUS_Events_PublishPhyConnectionStatus(bPrevPhyState,bCurrentPhyState);
        EXPECT_EQ(RETURN_OK, ret) ;
}

TEST (RBUSEventPhy_1, CellularMgr_RBUS_Events_PublishPhyConnectionStatus4){
        unsigned char bPrevPhyState = 0;
        unsigned char bCurrentPhyState = 1;
        gRBUSSubListSt.stInterface.PhyConnectedStatusSubFlag = 0;
        EXPECT_NE(bPrevPhyState, bCurrentPhyState);

        int ret = CellularMgr_RBUS_Events_PublishPhyConnectionStatus(bPrevPhyState,bCurrentPhyState);
        EXPECT_EQ(RETURN_OK, ret) ;
}

TEST (RBUSEventPhy_2, CellularMgr_RBUS_Events_PublishPhyConnectionStatus5){
        unsigned char bPrevPhyState = 1;
        unsigned char bCurrentPhyState = 1;
        gRBUSSubListSt.stInterface.PhyConnectedStatusSubFlag = 1;
        EXPECT_EQ(bPrevPhyState, bCurrentPhyState);

        int ret = CellularMgr_RBUS_Events_PublishPhyConnectionStatus(bPrevPhyState,bCurrentPhyState);
        EXPECT_EQ(RETURN_OK, ret) ;
}

TEST_F (CellularManagerTestFixture, CellularMgr_RBUS_Events_PublishLinkAvailableStatus1)
{
        unsigned char bPrevLinkState = 1;
        unsigned char bCurrentLinkState = 0;
        gRBUSSubListSt.stInterface.LinkAvailableStatusSubFlag = 1;
        EXPECT_NE(bPrevLinkState, bCurrentLinkState);
        char pParamName[] = "Device.Cellular.Interface.1.X_RDK_LinkAvailableStatus";
        ON_CALL(*g_rbusEventMock, CellularMgr_Rbus_String_EventPublish_OnValueChange( StrEq(pParamName), _, _, RBUS_BOOLEAN))
        .WillByDefault(Return(ANSC_STATUS_FAILURE));
        int res = CellularMgr_RBUS_Events_PublishLinkAvailableStatus(bPrevLinkState,bCurrentLinkState);
        EXPECT_EQ(RETURN_OK, res);

}

TEST_F (CellularManagerTestFixture, CellularMgr_RBUS_Events_PublishLinkAvailableStatuss2){
        unsigned char bPrevLinkState = 1;
        unsigned char bCurrentLinkState = 0;
        gRBUSSubListSt.stInterface.LinkAvailableStatusSubFlag = 1;
        EXPECT_NE(bPrevLinkState, bCurrentLinkState);
        char pParamName[] = "Device.Cellular.Interface.1.X_RDK_LinkAvailableStatus";
        ON_CALL(*g_rbusEventMock, CellularMgr_Rbus_String_EventPublish_OnValueChange( StrEq(pParamName), _, _, RBUS_BOOLEAN))
        .WillByDefault(Return(ANSC_STATUS_SUCCESS));

        int ret = CellularMgr_RBUS_Events_PublishLinkAvailableStatus(bPrevLinkState,bCurrentLinkState);
        EXPECT_EQ(RETURN_OK, ret) ;
}

TEST_F (CellularManagerTestFixture, CellularMgr_RBUS_Events_PublishLinkAvailableStatus3){
        unsigned char bPrevLinkState = 0;
        unsigned char bCurrentLinkState = 1;
        gRBUSSubListSt.stInterface.LinkAvailableStatusSubFlag = 1;
        EXPECT_NE(bPrevLinkState, bCurrentLinkState);
        char pParamName[] = "Device.Cellular.Interface.1.X_RDK_LinkAvailableStatus";
        ON_CALL(*g_rbusEventMock, CellularMgr_Rbus_String_EventPublish_OnValueChange( StrEq(pParamName), _, _, RBUS_BOOLEAN))
        .WillByDefault(Return(ANSC_STATUS_SUCCESS));

        int ret = CellularMgr_RBUS_Events_PublishLinkAvailableStatus(bPrevLinkState,bCurrentLinkState);
        EXPECT_EQ(RETURN_OK, ret) ;
}

TEST (RBUSEventLink_1, CellularMgr_RBUS_Events_PublishLinkAvailableStatus4){
        unsigned char bPrevLinkState = 0;
        unsigned char bCurrentLinkState = 1;
        gRBUSSubListSt.stInterface.LinkAvailableStatusSubFlag = 0;
        EXPECT_NE(bPrevLinkState, bCurrentLinkState);

        int ret = CellularMgr_RBUS_Events_PublishLinkAvailableStatus(bPrevLinkState,bCurrentLinkState);
        EXPECT_EQ(RETURN_OK, ret) ;
}

TEST (RBUSEventLink_2, CellularMgr_RBUS_Events_PublishLinkAvailableStatus5){
        unsigned char bPrevLinkState = 1;
        unsigned char bCurrentLinkState = 1;
        gRBUSSubListSt.stInterface.LinkAvailableStatusSubFlag = 1;
        EXPECT_EQ(bPrevLinkState, bCurrentLinkState);

        int ret = CellularMgr_RBUS_Events_PublishLinkAvailableStatus(bPrevLinkState,bCurrentLinkState);
        EXPECT_EQ(RETURN_OK, ret) ;
}

TEST_F (CellularManagerTestFixture, CellularMgr_RBUS_Events_Publish_X_RDK_Enable1)
{
        unsigned char bPrevEnableState = 1;
        unsigned char bCurrentEnableState = 0;
        gRBUSSubListSt.X_RDK_EnableSubFlag = 1;
        char pParamName[] = "Device.Cellular.X_RDK_Enable";
        ON_CALL(*g_rbusEventMock, CellularMgr_Rbus_String_EventPublish_OnValueChange( StrEq(pParamName), _, _, RBUS_BOOLEAN))
        .WillByDefault(Return(ANSC_STATUS_FAILURE));
        int res = CellularMgr_RBUS_Events_Publish_X_RDK_Enable(bPrevEnableState,bCurrentEnableState);
        EXPECT_EQ(RETURN_OK, res);

}

TEST_F (CellularManagerTestFixture, CellularMgr_RBUS_Events_Publish_X_RDK_Enable2){
        unsigned char bPrevEnableState = 1;
        unsigned char bCurrentEnableState = 0;
        gRBUSSubListSt.stInterface.PhyConnectedStatusSubFlag = 1;
        char pParamName[] = "Device.Cellular.X_RDK_Enable";
        ON_CALL(*g_rbusEventMock, CellularMgr_Rbus_String_EventPublish_OnValueChange( StrEq(pParamName), _, _, RBUS_BOOLEAN))
        .WillByDefault(Return(ANSC_STATUS_SUCCESS));

        int ret = CellularMgr_RBUS_Events_Publish_X_RDK_Enable(bPrevEnableState,bCurrentEnableState);
        EXPECT_EQ(RETURN_OK, ret) ;
}

TEST_F (CellularManagerTestFixture, CellularMgr_RBUS_Events_Publish_X_RDK_Enable3){
        unsigned char bPrevEnableState = 0;
        unsigned char bCurrentEnableState = 1;
        gRBUSSubListSt.stInterface.PhyConnectedStatusSubFlag = 1;
        char pParamName[] = "Device.Cellular.X_RDK_Enable";
        ON_CALL(*g_rbusEventMock, CellularMgr_Rbus_String_EventPublish_OnValueChange( StrEq(pParamName), _, _, RBUS_BOOLEAN))
        .WillByDefault(Return(ANSC_STATUS_SUCCESS));

        int ret = CellularMgr_RBUS_Events_Publish_X_RDK_Enable(bPrevEnableState,bCurrentEnableState);
        EXPECT_EQ(RETURN_OK, ret) ;
}

TEST (RBUSEventEnable_1, CellularMgr_RBUS_Events_Publish_X_RDK_Enable4){
        unsigned char bPrevEnableState = 0;
        unsigned char bCurrentEnableState = 1;
        gRBUSSubListSt.stInterface.PhyConnectedStatusSubFlag = 0;

        int ret = CellularMgr_RBUS_Events_Publish_X_RDK_Enable(bPrevEnableState,bCurrentEnableState);
        EXPECT_EQ(RETURN_OK, ret) ;
}

TEST (RBUSEventEnable_2, CellularMgr_RBUS_Events_Publish_X_RDK_Enable5){
        unsigned char bPrevEnableState = 1;
        unsigned char bCurrentEnableState = 1;
        gRBUSSubListSt.stInterface.PhyConnectedStatusSubFlag = 1;

        int ret = CellularMgr_RBUS_Events_Publish_X_RDK_Enable(bPrevEnableState,bCurrentEnableState);
        EXPECT_EQ(RETURN_OK, ret) ;
}

TEST_F (CellularManagerTestFixture, CellularMgr_RBUS_Events_PublishInterfaceStatus1)
{
        char bPrevValue[] = {0};
        char bCurrentValue[] = {0};
        CellularInterfaceStatus_t PrevState = IF_UP;
        CellularInterfaceStatus_t CurrentState = IF_DOWN;
        char pParamName[] = "Device.Cellular.Interface.1.Status";

        snprintf(bPrevValue, sizeof(bPrevValue), "%s", (PrevState == IF_UP) ? "Up": "Down");
        snprintf(bCurrentValue, sizeof(bCurrentValue), "%s", (CurrentState == IF_UP) ? "Up": "Down");

        gRBUSSubListSt.stInterface.InterfaceStatusSubFlag = 1;
        EXPECT_NE(PrevState, CurrentState);

        ON_CALL(*g_rbusEventMock, CellularMgr_Rbus_String_EventPublish_OnValueChange( StrEq(pParamName), _, _, RBUS_STRING))
        .WillByDefault(Return(ANSC_STATUS_FAILURE));
        int res = CellularMgr_RBUS_Events_PublishInterfaceStatus(PrevState,CurrentState);
        EXPECT_EQ(RETURN_OK, res);

}

TEST_F (CellularManagerTestFixture, CellularMgr_RBUS_Events_PublishInterfaceStatus2){
        char bPrevValue[] = {0};
        char bCurrentValue[] = {0};
        CellularInterfaceStatus_t PrevState = IF_UP;
        CellularInterfaceStatus_t CurrentState = IF_DOWN;
        char pParamName[] = "Device.Cellular.Interface.1.Status";

        snprintf(bPrevValue, sizeof(bPrevValue), "%s", (PrevState == IF_UP) ? "Up": "Down");
        snprintf(bCurrentValue, sizeof(bCurrentValue), "%s", (CurrentState == IF_UP) ? "Up": "Down");

        gRBUSSubListSt.stInterface.InterfaceStatusSubFlag = 1;
        EXPECT_NE(PrevState, CurrentState);

        ON_CALL(*g_rbusEventMock, CellularMgr_Rbus_String_EventPublish_OnValueChange( StrEq(pParamName), _, _, RBUS_BOOLEAN))
        .WillByDefault(Return(ANSC_STATUS_SUCCESS));

        int ret = CellularMgr_RBUS_Events_PublishInterfaceStatus(PrevState,CurrentState);
        EXPECT_EQ(RETURN_OK, ret) ;
}

TEST_F (CellularManagerTestFixture, CellularMgr_RBUS_Events_PublishInterfaceStatus3){
        char bPrevValue[] = {0};
        char bCurrentValue[] = {0};
        CellularInterfaceStatus_t PrevState = IF_DOWN;
        CellularInterfaceStatus_t CurrentState = IF_UP;
        char pParamName[] = "Device.Cellular.Interface.1.Status";

        snprintf(bPrevValue, sizeof(bPrevValue), "%s", (PrevState == IF_UP) ? "Up": "Down");
        snprintf(bCurrentValue, sizeof(bCurrentValue), "%s", (CurrentState == IF_UP) ? "Up": "Down");

        gRBUSSubListSt.stInterface.InterfaceStatusSubFlag = 1;
        EXPECT_NE(PrevState, CurrentState);

        ON_CALL(*g_rbusEventMock, CellularMgr_Rbus_String_EventPublish_OnValueChange( StrEq(pParamName), _, _, RBUS_BOOLEAN))
        .WillByDefault(Return(ANSC_STATUS_SUCCESS));

        int ret = CellularMgr_RBUS_Events_PublishInterfaceStatus(PrevState,CurrentState);
        EXPECT_EQ(RETURN_OK, ret) ;
}

TEST (RBUSEventInterface_1, CellularMgr_RBUS_Events_PublishInterfaceStatus4){
        CellularInterfaceStatus_t PrevState = IF_DOWN;
        CellularInterfaceStatus_t CurrentState = IF_UP;

        gRBUSSubListSt.stInterface.InterfaceStatusSubFlag = 0;
        EXPECT_NE(PrevState, CurrentState);

        int ret = CellularMgr_RBUS_Events_PublishInterfaceStatus(PrevState,CurrentState);
        EXPECT_EQ(RETURN_OK, ret) ;
}

TEST (RBUSEventInterface_2, CellularMgr_RBUS_Events_PublishInterfaceStatus5){
        CellularInterfaceStatus_t PrevState = IF_UP;
        CellularInterfaceStatus_t CurrentState = IF_UP;
        gRBUSSubListSt.stInterface.InterfaceStatusSubFlag = 1;
        EXPECT_EQ(PrevState, CurrentState);

        int ret = CellularMgr_RBUS_Events_PublishInterfaceStatus(PrevState,CurrentState);
        EXPECT_EQ(RETURN_OK, ret) ;
}

