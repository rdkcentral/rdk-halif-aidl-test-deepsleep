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
 * @defgroup DEEPSLEEP_HALTEST_CLASS DEEPSLEEP HAL Tests L1 Test Cases
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
 * @file vts_deepsleep_l1_tests.cpp
 *
 */

#include "vts_deepsleep.h"
#include "com/rdk/hal/deepsleep/IDeepSleep.h"
#include "ut_log.h"

extern sp<IDeepSleep> gDEEPSLEEPHandle;

/* Test class registration with UT_Core */
UT_ADD_TEST_TO_GROUP(VTS_L1_DEEPSLEEP, UT_TESTS_L1)

class VTS_L1_DEEPSLEEP : public UTCore
{
public:
    VTS_L1_DEEPSLEEP() : UTCore() {}

    ~VTS_L1_DEEPSLEEP() override = default;

    void SetUp() override
    {
        android::ProcessState::self()->startThreadPool();
    }

    void TearDown() override
    {
    }
};

UT_ADD_TEST(VTS_L1_DEEPSLEEP, getCaps_pos)
{
    com::rdk::hal::deepsleep::Capabilities caps;
    DEEPSLEEP_GET_CAPABILITY(gDEEPSLEEPHandle, caps);
    // Validate that the output parameter has been populated.
    UT_ASSERT_TRUE(!caps.supportedTriggers.empty());
    UT_ASSERT_TRUE(!caps.preconfiguredTriggers.empty());
    size_t supportedcount = UT_KVP_PROFILE_GET_LIST_COUNT("IDeepSleep.supportedTriggers");
    size_t preconfiguredcount = UT_KVP_PROFILE_GET_LIST_COUNT("IDeepSleep.preconfiguredTriggers");
    // Verify that every preconfigured trigger is a member of supportedTriggers.
    for (auto mandTrigger : caps.preconfiguredTriggers) {
        UT_ASSERT_NOT_EQUAL(std::find(caps.supportedTriggers.begin(), caps.supportedTriggers.end(), mandTrigger),
                  caps.supportedTriggers.end());
    }

    UT_ASSERT_EQUAL(caps.supportedTriggers.size(), supportedcount);
    for (size_t i = 0; i < supportedcount; i++) {
        bool result = false;
        std::string key_string = "IDeepSleep.supportedTriggers." + std::to_string(i);
        char trigger[256] = {0};
        UT_KVP_PROFILE_GET_STRING(key_string.c_str(), trigger);
        if (trigger[0] != '\0') {
            for (auto x : caps.supportedTriggers) {
                std::string triggerstr = toString(x);
                if (trigger == triggerstr) {
                    result = true;
                    break;
                }
            }
            if (!result) {
                UT_LOG_DEBUG("Trigger %s mismatch in supported list\n", trigger);
            }
            UT_ASSERT_TRUE(result);
        }
    }

    UT_ASSERT_EQUAL(caps.preconfiguredTriggers.size(), preconfiguredcount);
    for (size_t i = 0; i < preconfiguredcount; i++) {
        bool result = false;
        std::string key_string = "IDeepSleep.preconfiguredTriggers." + std::to_string(i);
        char trigger[256] = {0};
        UT_KVP_PROFILE_GET_STRING(key_string.c_str(), trigger);
        if (trigger[0] != '\0') {
            for (auto x : caps.preconfiguredTriggers) {
                std::string triggerstr = toString(x);
                if (trigger == triggerstr) {
                    result = true;
                    break;
                }
            }
            if (!result) {
                UT_LOG_DEBUG("Trigger %s mismatch in preconfigured list\n", trigger);
            }
            UT_ASSERT_TRUE(result);
        }
    }
    return;
}

UT_ADD_TEST(VTS_L1_DEEPSLEEP, gettimer_pos)
{
    int32_t time;
    DEEPSLEEP_GET_TIMER(gDEEPSLEEPHandle, time);
    UT_ASSERT_EQUAL(time, -1);
    return;
}

UT_ADD_TEST(VTS_L1_DEEPSLEEP, settimer_pos)
{
    com::rdk::hal::deepsleep::Capabilities caps;
    bool result;
    bool supported = true;
    DEEPSLEEP_GET_CAPABILITY(gDEEPSLEEPHandle, caps);
    if (!(std::find(caps.supportedTriggers.begin(), caps.supportedTriggers.end(), WakeUpTrigger::TIMER) != caps.supportedTriggers.end())) {
        UT_LOG_INFO("TIMER trigger is not supported");
        supported = false;
    }
    std::vector<int32_t> secondsValues = {604800, 3600, 1};
    for (const auto& time : secondsValues) {
        DEEPSLEEP_SET_TIMER(gDEEPSLEEPHandle, time, result);
        if (supported) {
            UT_ASSERT_TRUE(result);
        } else {
            UT_ASSERT_FALSE(result);
        }
    }
    return;
}

UT_ADD_TEST(VTS_L1_DEEPSLEEP, settimer_neg)
{
    bool result;
    std::vector<int32_t> secondsValues = {0, -1, -10};
    for (const auto& time : secondsValues) {
        DEEPSLEEP_SET_TIMER(gDEEPSLEEPHandle, time, result);
        UT_ASSERT_FALSE(result);
    }
    return;
}

UT_ADD_TEST(VTS_L1_DEEPSLEEP, enterdeepsleep_neg)
{
    com::rdk::hal::deepsleep::Capabilities caps;
    DEEPSLEEP_GET_CAPABILITY(gDEEPSLEEPHandle, caps);
    std::vector<::com::rdk::hal::deepsleep::WakeUpTrigger> triggersToWakeUpon;
    std::vector<::com::rdk::hal::deepsleep::WakeUpTrigger> wokeUpByTriggers;
    std::optional<com::rdk::hal::deepsleep::KeyCode> kCode;
    bool result;
    DEEPSLEEP_ENTERDEEPSLEEP(gDEEPSLEEPHandle, triggersToWakeUpon, wokeUpByTriggers, kCode, result);
    if (caps.preconfiguredTriggers.empty()) {
        // when both input list and preconfigured list is empty
        UT_ASSERT_FALSE(result);
    } else {
        UT_ASSERT_TRUE(result);
    }
    UT_ASSERT_FALSE(kCode.has_value());
    triggersToWakeUpon.push_back(com::rdk::hal::deepsleep::WakeUpTrigger::ERROR_UNKNOWN);
    DEEPSLEEP_ENTERDEEPSLEEP(gDEEPSLEEPHandle, triggersToWakeUpon, wokeUpByTriggers, kCode, result);
    UT_ASSERT_FALSE(result);
    triggersToWakeUpon.clear();
    triggersToWakeUpon.push_back(static_cast<::com::rdk::hal::deepsleep::WakeUpTrigger>(999));
    DEEPSLEEP_ENTERDEEPSLEEP(gDEEPSLEEPHandle, triggersToWakeUpon, wokeUpByTriggers, kCode, result);
    UT_ASSERT_FALSE(result);
    return;
}

/**
* @} // End of DEEPSLEEP_HALTEST_CLASS
* @} // End of DEEPSLEEP_HALTEST
* @} // End of DEEPSLEEP
* @} // End of HPK
*/
/* End of vts_deepsleep_l1_tests.cpp */