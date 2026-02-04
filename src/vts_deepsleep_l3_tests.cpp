/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
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
 * @addtogroup HPK Hardware Porting Kit
 * @{
 * @par The Hardware Porting Kit
 * HPK is the next evolution of the well-defined Hardware Abstraction Layer
 * (HAL), but augmented with more comprehensive documentation and test suites
 * that OEM or SOC vendors can use to self-certify their ports before taking
 * them to RDKM for validation or to an operator for final integration and
 * deployment. The Hardware Porting Kit effectively enables an OEM and/or SOC
 * vendor to self-certify their own Video Accelerator devices, with minimal RDKM
 * assistance.
 *
 */

/**
 * @addtogroup DEEPSLEEP DEEPSLEEP Module
 * @{
 */

/**
 * @addtogroup DEEPSLEEP_HALTEST DEEPSLEEP HAL Tests
 * @{
 */

/**
 * @defgroup DEEPSLEEP_HALTEST_CLASS DEEPSLEEP HAL Tests L3 Test Cases
 * @{
 * @parblock
 *
 * ### L3 Test Cases for DEEPSLEEP HAL :
 *
 * This module includes Level 3 functional tests (success and failure scenarios).
 * This is to ensure that the API meets the operational requirements of the module across all vendors.
 *
 * **Pre-Conditions:**  None @n
 * **Dependencies:** None @n
 *
 * @endparblock
 *
 */

/**
 * @file vts_deepsleep_l3_tests.cpp
 *
 */

#include "vts_deepsleep.h"
#include "com/rdk/hal/deepsleep/IDeepSleep.h"
#include "ut_log.h"
#include "vts_deepsleep_parse_testconfig.h"

extern sp<IDeepSleep> gDEEPSLEEPHandle;
static const char *gModuleName = "DeepSleep";
static const char *gTestGroup = "L3";

/* Test class registration with UT_Core */
UT_ADD_TEST_TO_GROUP(VTS_L3_DEEPSLEEP, UT_TESTS_L3)

class VTS_L3_DEEPSLEEP : public UTCore
{
public:
    VTS_L3_DEEPSLEEP() : UTCore() {}

    ~VTS_L3_DEEPSLEEP() override = default;

    void SetUp() override
    {
        android::ProcessState::self()->startThreadPool();
    }

    void TearDown() override
    {
    }
};

/**
 * @brief Test to verify the EnterDeepSleep functionality with configured wake-up triggers.
 *
 *
 * **Test Group ID:** Basic: 03@n
 * **Test Case ID:** 001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:**
 *
 * **Dependencies:**
 * - A valid gDEEPSLEEPHandle must be available
 *
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * Refer to Deepsleep specification documentation [deepsleep_High-Level_TestSpecification.md](../docs/pages/deepsleep_High-Level_TestSpecification.md)
 *
 */
UT_ADD_TEST(VTS_L3_DEEPSLEEP, EnterDeepSleep)
{
    const char *testName = "EnterDeepSleep";
    enterdeepsleepTestConfig_t testConfiguration;
    std::string configFile = VTS_DEEPSLEEP_TEST_CONFIG_PATH;

    UT_status_t status = UT_STATUS_OK;

    status = vts_deepsleep_parse_enterdeepsleep_config((char *)configFile.c_str(), gModuleName, gTestGroup, testName, testConfiguration);
    UT_ASSERT_EQUAL_FATAL(status, UT_STATUS_OK);

    bool result;
    com::rdk::hal::deepsleep::Capabilities caps;
    DEEPSLEEP_GET_CAPABILITY(gDEEPSLEEPHandle, caps);
    if ((isTriggerSupported(testConfiguration.input_triggersToWakeUpon, WakeUpTrigger::TIMER))) {
            UT_LOG_INFO("TIMER trigger is in inputtrigger/preconfigure list\n");
            DEEPSLEEP_SET_TIMER(gDEEPSLEEPHandle, testConfiguration.timer, result);
            UT_ASSERT_TRUE(result);
            int32_t gettime;
            DEEPSLEEP_GET_TIMER(gDEEPSLEEPHandle, gettime);
            UT_ASSERT_EQUAL(testConfiguration.timer, gettime);
    }

    std::vector<::com::rdk::hal::deepsleep::WakeUpTrigger> wokeUpByTriggers;
    std::optional<com::rdk::hal::deepsleep::KeyCode> kCode;
    auto timeBeforeSleep = std::chrono::system_clock::now();
    DEEPSLEEP_ENTERDEEPSLEEP(gDEEPSLEEPHandle, testConfiguration.input_triggersToWakeUpon, wokeUpByTriggers, kCode, result);
    auto timeAfterSleep = std::chrono::system_clock::now();
    UT_ASSERT_TRUE(result);
    for (size_t i = 0; i < wokeUpByTriggers.size(); i++) {
        UT_LOG_INFO("wokeUpByTriggers(%d): %d", i, wokeUpByTriggers[i]);
        UT_ASSERT_TRUE(isTriggerSupported(testConfiguration.output.output_wokeUpByTriggers, wokeUpByTriggers[i]));
        if (wokeUpByTriggers[i] == WakeUpTrigger::RCU_IR ||  wokeUpByTriggers[i] == WakeUpTrigger::RCU_BT || wokeUpByTriggers[i] == WakeUpTrigger::RCU_RF4CE) {
            UT_ASSERT_TRUE(kCode.has_value());
            UT_ASSERT_EQUAL(testConfiguration.output.output_keycode, kCode->keyCode);
        } else {
            UT_ASSERT_FALSE(kCode.has_value());
        }
    }
    auto sleepDuration = std::chrono::duration_cast<std::chrono::seconds>(timeAfterSleep - timeBeforeSleep).count();
    UT_LOG_INFO("Time taken by deepsleep %lld seconds (expectedwaittime ~%d)", sleepDuration, testConfiguration.output.expectedwaittime);
    uint32_t buffertime = UT_KVP_PROFILE_GET_UINT32("deepsleep.bufferduration");
    UT_ASSERT_TRUE(std::abs(sleepDuration - testConfiguration.output.expectedwaittime) <= buffertime);
    return;
}

/**
* @} // End of DEEPSLEEP_HALTEST_CLASS
* @} // End of DEEPSLEEP_HALTEST
* @} // End of DEEPSLEEP
* @} // End of HPK
*/
/* End of vts_deepsleep_l3_tests.cpp */
