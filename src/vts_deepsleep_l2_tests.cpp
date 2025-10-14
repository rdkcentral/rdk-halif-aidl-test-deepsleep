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
 * @defgroup DEEPSLEEP_HALTEST_CLASS DEEPSLEEP HAL Tests L2 Test Cases
 * @{
 * @parblock
 *
 * ### Common functions/macros required for the DEEPSLEEP module :
 *
 * Common functions/macros required for the DEEPSLEEP module .
 *
 * **Pre-Conditions:**  None @n
 * **Dependencies:** None @n
 *
 * @endparblock
 *
 */

/**
 * @file vts_deepsleep_l2_tests.cpp
 *
 */

#include "vts_deepsleep.h"
#include "com/rdk/hal/deepsleep/IDeepSleep.h"
#include "ut_log.h"
#include "vts_deepsleep_helper_functions.h"

extern sp<IDeepSleep> gDEEPSLEEPHandle;
#define TEST_TRIGGER_KVP_SIZE      128
#define TEST_TRIGGER_TYPE_SIZE     32

/* Test class registration with UT_Core */
UT_ADD_TEST_TO_GROUP(VTS_L2_DEEPSLEEP, UT_TESTS_L2)

class VTS_L2_DEEPSLEEP : public UTCore
{
public:
    VTS_L2_DEEPSLEEP() : UTCore() {}

    ~VTS_L2_DEEPSLEEP() override = default;

    void SetUp() override
    {
        // android::ProcessState::self()->startThreadPool();
    }

    void TearDown() override
    {
    }
};

/**
 * @brief Test for getCapabilities() function of IDeepSleep
 *
 * This test verifies that the getCapabilities() method of the IDeepSleep interface
 * gives the Capabilities structure with the supported triggers and preconfigured triggers.
 *
 * **Test Group ID:** 02@n
 * **Test Case ID:** 001@n
 *
 * **Test Procedure:**
 * Refer to Deepsleep specification documentation [deepsleep_L2_Low-Level_TestSpecification](../docs/pages/)
 */
UT_ADD_TEST(VTS_L2_DEEPSLEEP, test_getCapablities)
{
    com::rdk::hal::deepsleep::Capabilities caps;
    DEEPSLEEP_GET_CAPABILITY(gDEEPSLEEPHandle, caps);
    size_t supportedcount = UT_KVP_PROFILE_GET_LIST_COUNT("IDeepSleep.supportedTriggers");
    size_t preconfiguredcount = UT_KVP_PROFILE_GET_LIST_COUNT("IDeepSleep.preconfiguredTriggers");
    UT_ASSERT_EQUAL(caps.supportedTriggers.size(), supportedcount);
    bool result = true;
    for (size_t i = 0; i < supportedcount; i++) {
        std::string key_string = "IDeepSleep.supportedTriggers." + std::to_string(i);
        char trigger[256] = {0};
        UT_KVP_PROFILE_GET_STRING(key_string.c_str(), trigger);
        if (trigger[0] != '\0') {
            WakeUpTrigger triggerenum = stringToWakeUpTrigger(trigger);
            if (!(std::find(caps.supportedTriggers.begin(), caps.supportedTriggers.end(), triggerenum) != caps.supportedTriggers.end())) {
                UT_LOG_INFO("Trigger %s mismatch in supported list\n", trigger);
                result = false;
                break;
            }
        }
    }

    UT_ASSERT_EQUAL(caps.preconfiguredTriggers.size(), preconfiguredcount);
    for (size_t i = 0; i < preconfiguredcount; i++) {
        //char key_string[TEST_TRIGGER_KVP_SIZE] = {0};
        //char trigger[TEST_TRIGGER_TYPE_SIZE] = {0};
        //snprintf(key_string, TEST_TRIGGER_KVP_SIZE, "IDeepSleep.preconfiguredTriggers.%ld" , i);
        //UT_KVP_PROFILE_GET_STRING(key_string, trigger);
        std::string key_string = "IDeepSleep.preconfiguredTriggers." + std::to_string(i);
        char trigger[256] = {0};
        UT_KVP_PROFILE_GET_STRING(key_string.c_str(), trigger);
        if (trigger[0] != '\0') {
            WakeUpTrigger triggerenum = stringToWakeUpTrigger(trigger);
            if (!(std::find(caps.preconfiguredTriggers.begin(), caps.preconfiguredTriggers.end(), triggerenum) != caps.preconfiguredTriggers.end())) {
                UT_LOG_INFO("Trigger %s mismatch in preconfigured list\n", trigger);
                result = false;
                break;
            }
        }
    }
    UT_ASSERT_TRUE(result);
    return;
}

/**
 * @brief Test for get/set Timer function of IDeepSleep
 *
 * This test verifies that the get/set timer method of the IDeepSleep interface
 * set and get the expected wakeup duration values.
 *
 * **Test Group ID:** 02@n
 * **Test Case ID:** 002@n
 *
 * **Test Procedure:**
 * Refer to Deepsleep specification documentation [deepsleep_L2_Low-Level_TestSpecification](../docs/pages/)
 */
UT_ADD_TEST(VTS_L2_DEEPSLEEP, test_setgettimer)
{
    com::rdk::hal::deepsleep::Capabilities caps;
    DEEPSLEEP_GET_CAPABILITY(gDEEPSLEEPHandle, caps);
    if (!(std::find(caps.supportedTriggers.begin(), caps.supportedTriggers.end(), WakeUpTrigger::TIMER) != caps.supportedTriggers.end())) {
        if (!(std::find(caps.preconfiguredTriggers.begin(), caps.preconfiguredTriggers.end(), WakeUpTrigger::TIMER) != caps.preconfiguredTriggers.end())) {
            UT_LOG_INFO("TIMER trigger is not supported");
            return;
        }
    }
    int32_t time = 604800; //7 days max value
    int32_t gettime = -1;
    bool result = false;
    DEEPSLEEP_SET_TIMER(gDEEPSLEEPHandle, time, result);
    UT_ASSERT_TRUE(result);
    if (!result) {
        UT_LOG_ERROR("DEEPSLEEP_SET_TIMER Failed");
        return;
    }
    DEEPSLEEP_GET_TIMER(gDEEPSLEEPHandle, gettime);
    UT_ASSERT_EQUAL(gettime, time);
    time = 1;
    DEEPSLEEP_SET_TIMER(gDEEPSLEEPHandle, time, result);
    UT_ASSERT_TRUE(result);
    if (!result) {
        UT_LOG_ERROR("DEEPSLEEP_SET_TIMER Failed");
        return;
    }
    DEEPSLEEP_GET_TIMER(gDEEPSLEEPHandle, gettime);
    UT_ASSERT_EQUAL(gettime, time);
    return;
}

UT_ADD_TEST(VTS_L2_DEEPSLEEP, test_enterdeepsleep)
{
    com::rdk::hal::deepsleep::Capabilities caps;
    DEEPSLEEP_GET_CAPABILITY(gDEEPSLEEPHandle, caps);
    if (!(std::find(caps.supportedTriggers.begin(), caps.supportedTriggers.end(), WakeUpTrigger::TIMER) != caps.supportedTriggers.end())) {
        if (!(std::find(caps.preconfiguredTriggers.begin(), caps.preconfiguredTriggers.end(), WakeUpTrigger::TIMER) != caps.preconfiguredTriggers.end())){
            UT_LOG_INFO("TIMER trigger is not supported");
            return;
        }
    }
    std::vector<::com::rdk::hal::deepsleep::WakeUpTrigger> triggersToWakeUpon;
    std::vector<::com::rdk::hal::deepsleep::WakeUpTrigger> wokeUpByTriggers;
    std::optional<com::rdk::hal::deepsleep::KeyCode> kCode;
    bool result = false;
    int32_t time = 10;
    DEEPSLEEP_SET_TIMER(gDEEPSLEEPHandle, time, result);
    UT_ASSERT_TRUE(result);
    if (!result) {
        UT_LOG_ERROR("DEEPSLEEP_SET_TIMER Failed");
        return;
    }
    triggersToWakeUpon.push_back(com::rdk::hal::deepsleep::WakeUpTrigger::TIMER);
    auto timeBeforeSleep = std::chrono::system_clock::now();
    DEEPSLEEP_ENTERDEEPSLEEP(gDEEPSLEEPHandle, triggersToWakeUpon, wokeUpByTriggers, kCode, result);
    auto timeAfterSleep = std::chrono::system_clock::now();
    UT_ASSERT_TRUE(result);
    UT_ASSERT_FALSE(kCode.has_value());
    UT_ASSERT_TRUE(std::find(triggersToWakeUpon.begin(), triggersToWakeUpon.end(), WakeUpTrigger::TIMER) != triggersToWakeUpon.end());
    auto sleepDuration = std::chrono::duration_cast<std::chrono::seconds>(timeAfterSleep - timeBeforeSleep).count();
    UT_LOG_INFO("Slept for %lld seconds (timervalue ~%d)", sleepDuration, time);
    uint32_t buffertime = UT_KVP_PROFILE_GET_UINT32("IDeepSleep.bufferduration");
    UT_ASSERT_TRUE(std::abs(sleepDuration - time) <= buffertime);
    return;
}

/**
* @} // End of DEEPSLEEP_HALTEST_CLASS
* @} // End of DEEPSLEEP_HALTEST
* @} // End of DEEPSLEEP
* @} // End of HPK
*/
/* End of vts_deepsleep_l2_tests.cpp */
