/*
 *
 * Copyright 2016 Comcast Cable Communications Management, LLC
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
 * SPDX-License-Identifier: Apache-2.0
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string>
#include <gtest/gtest.h>

extern "C" {
#include "SanityTest.h"
}

namespace{
  const char* cloud_cmd = "curl --interface erouter0 -LI https://agent.xais.apg.comcast.net:9443/environment/redirect/AABBCCDDEEFF -o /dev/null -w '%{http_code}\n' -s";
  const char* rabid_cmd = "pidof cujo-agent | wc -l";
  const char* advsec_cmd = "pidof CcspAdvSecuritySsp | wc -l";
  const char* DFP_cmd = "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_DeviceFingerPrint.Enable |  grep -i value | awk '{print $5}'";
  const char* SB_cmd = "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_AdvancedSecurity.SafeBrowsing.Enable |  grep -i value | awk '{print $5}'";
  const char* SF_cmd = "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_AdvancedSecurity.Softflowd.Enable |  grep -i value | awk '{print $5}'";
  const char* APC_cmd = "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_AdvancedParentalControl.Activate |  grep -i value | awk '{print $5}'";
  const char* RFC_APC_cmd = "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AdvancedParentalControl.Enable |  grep -i value | awk '{print $5}'";
  const char* PP_cmd = "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_PrivacyProtection.Activate |  grep -i value | awk '{print $5}'";
  const char* RFC_PP_cmd = "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.PrivacyProtection.Enable |  grep -i value | awk '{print $5}'";
  const char* RFC_WS_cmd = "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.WS-Discovery_Analysis.Enable |  grep -i value | awk '{print $5}'";
  const char* RFC_OTM_cmd = "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AdvancedSecurityOTM.Enable |  grep -i value | awk '{print $5}'";
  const char* RFC_ICMPv6_cmd = "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.DeviceFingerPrintICMPv6.Enable |  grep -i value | awk '{print $5}'";
  const char* RFC_raptr_cmd = "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AdvSecAgentRaptr.Enable |  grep -i value | awk '{print $5}'";
  const char* AdvSecAgentStatus_commands[] = {"dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AdvSecAgent.Enable | grep -i value | awk '{print $5}'", "cujo-agent-status breakers | grep -o '\"startup\":[^,}]*' | awk '{print $2}'"};
  const char* AgentSetToTrue_commands[] = {"dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AdvSecAgent.Enable bool true", "cujo-agent-status running;echo $?", "cujo-agent-status breakers | grep -o '\"startup\":[^,}]*' | awk '{print $2}'", "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AdvSecAgent.Enable"};
  const char* SafeBrowsingSetToTrue_commands[] = {"dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AdvSecSafeBrowsing.Enable bool true", "cujo-agent-status running;echo $?", "cujo-agent-status breakers | grep -o '\"safebro\":[^,}]*' | awk '{print $2}'", "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AdvSecSafeBrowsing.Enable", "iptables-save | grep -i SAFEBRO"};
  const char* CujoTelemetryWiFiFPSetToTrue_commands[] = {"dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AdvSecCujoTelemetryWiFiFP.Enable bool true", "cujo-agent-status running;echo $?", "cujo-agent-status breakers | grep -o '\"telemetry_wifi_fp_upload\":[^,}]*' | awk '{print $2}'", "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AdvSecCujoTelemetryWiFiFP.Enable"};
  const char* CujoTelemetryWiFiFPSetToFalse_commands[] = {"dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AdvSecCujoTelemetryWiFiFP.Enable bool false", "cujo-agent-status running;echo $?", "cujo-agent-status breakers | grep -o '\"telemetry_wifi_fp_upload\":[^,}]*' | awk '{print $2}'", "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AdvSecCujoTelemetryWiFiFP.Enable"};
  const char* SafeBrowsingSetToFalse_commands[] = {"dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AdvSecSafeBrowsing.Enable bool false", "cujo-agent-status running;echo $?", "cujo-agent-status breakers | grep -o '\"safebro\":[^,}]*' | awk '{print $2}'", "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AdvSecSafeBrowsing.Enable", "iptables-save | grep -i SAFEBRO | wc -l"};
  const char* AgentSetToFalse_commands[] = {"dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AdvSecAgent.Enable bool false", "cujo-agent-status running;echo $?", "dmcli eRT getv Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.AdvSecAgent.Enable"};
}

TEST(CcspAdvSecuritySanityFixture, CujoCloudConnectivityTestCase)
{
    EXPECT_EQ(0 , CujoCloudConnectivity(cloud_cmd));
}

TEST(CcspAdvSecuritySanityFixture, CujoAgentProcessStatusTestCase)
{
    EXPECT_EQ(0 , CujoAgentProcessStatus(rabid_cmd));
}

TEST(CcspAdvSecuritySanityFixture, CcspAdvsecProcessStatusTestCase)
{
    EXPECT_EQ(0 , CcspAdvsecProcessStatus(advsec_cmd));
}

TEST(CcspAdvSecuritySanityFixture, AdvsecDeviceFingerPrintEnableStatusTestCase)
{
    EXPECT_EQ(0 , CcspAdvsecFeatureTr181(DFP_cmd));
}

TEST(CcspAdvSecuritySanityFixture, AdvsecDeviceSafeBrowsingEnableStatusTestCase)
{
    EXPECT_EQ(0 , CcspAdvsecFeatureTr181(SB_cmd));
}

TEST(CcspAdvSecuritySanityFixture, AdvsecDeviceSoftflowdTestCase)
{
    EXPECT_EQ(0 , CcspAdvsecFeatureTr181(SF_cmd));
}

TEST(CcspAdvSecuritySanityFixture, AdvsecDeviceAdvancedParentalControlTestCase)
{
    EXPECT_EQ(0 , CcspAdvsecFeatureTr181(APC_cmd));
}

TEST(CcspAdvSecuritySanityFixture, AdvsecDeviceAdvancedParentalControlRFCTestCase)
{
    EXPECT_EQ(0 , CcspAdvsecFeatureTr181(RFC_APC_cmd));
}

TEST(CcspAdvSecuritySanityFixture, AdvsecDevicePrivacyProtectionTestCase)
{
    EXPECT_EQ(0 , CcspAdvsecFeatureTr181(PP_cmd));
}

TEST(CcspAdvSecuritySanityFixture, AdvsecDevicePrivacyProtectionRFCTestCase)
{
    EXPECT_EQ(0 , CcspAdvsecFeatureTr181(RFC_PP_cmd));
}

TEST(CcspAdvSecuritySanityFixture, AdvsecWSDiscoveryAnalysisRFCTestCase)
{
    EXPECT_EQ(0 , CcspAdvsecFeatureTr181(RFC_WS_cmd));
}

TEST(CcspAdvSecuritySanityFixture, AdvsecOTMTestRFCCase)
{
    EXPECT_EQ(0 , CcspAdvsecFeatureTr181(RFC_OTM_cmd));
}

TEST(CcspAdvSecuritySanityFixture, AdvsecFingerPrintICMPv6RFCTestCase)
{
    EXPECT_EQ(0 , CcspAdvsecFeatureTr181(RFC_ICMPv6_cmd));
}

TEST(CcspAdvSecuritySanityFixture, AdvsecRaptrRFCTestCase)
{
    EXPECT_EQ(0 , CcspAdvsecFeatureTr181(RFC_raptr_cmd));
}

TEST(CcspAdvSecuritySanityFixture, CcspAdvSecAgent_SetToTrueTestCase)
{
    EXPECT_EQ(0 , CcspAdvSecAgent_SetToTrue(AgentSetToTrue_commands));
}

TEST(CcspAdvSecuritySanityFixture, AdvSecSafeBrowsing_SetToTrueTestCase)
{
    EXPECT_EQ(0 , AdvSecSafeBrowsing_SetToTrue(SafeBrowsingSetToTrue_commands));
}

TEST(CcspAdvSecuritySanityFixture, AdvSecCujoTelemetryWiFiFP_SetToTrueTestCase)
{
    EXPECT_EQ(0 , AdvSecCujoTelemetryWiFiFP_SetToTrue(CujoTelemetryWiFiFPSetToTrue_commands));
}

TEST(CcspAdvSecuritySanityFixture, CcspAdvSecAgentStatusTestCase)
{
    EXPECT_EQ(0 , CcspAdvSecAgentStatus(AdvSecAgentStatus_commands, 2));
}

TEST(CcspAdvSecuritySanityFixture, AdvSecCujoTelemetryWiFiFP_SetToFalseTestCase)
{
    EXPECT_EQ(0 , AdvSecCujoTelemetryWiFiFP_SetToFalse(CujoTelemetryWiFiFPSetToFalse_commands));
}

TEST(CcspAdvSecuritySanityFixture, AdvSecSafeBrowsing_SetToFalseTestCase)
{
    EXPECT_EQ(0 , AdvSecSafeBrowsing_SetToFalse(SafeBrowsingSetToFalse_commands));
}

TEST(CcspAdvSecuritySanityFixture, CcspAdvSecAgent_SetToFalseTestCase)
{
    EXPECT_EQ(0 , CcspAdvSecAgent_SetToFalse(AgentSetToFalse_commands));
}