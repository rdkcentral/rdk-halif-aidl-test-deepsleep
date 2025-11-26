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

#define SUPPORTEDLIST_NODE "IDeepSleep.supportedTriggers"
#define PRECONFIGUREDLIST_NODE "IDeepSleep.preconfiguredTriggers"
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

/**
 * @brief Test the positive and valid invocation of getCapabilities API
 *
 * This test verifies that the getCapabilities API, when invoked, returns the expected valid capabilities.
 * It also validates that the supportedTriggers and preconfiguredTriggers arrays are correctly populated and that 
 * every preconfigured trigger is a member of the supportedTriggers.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Invoke the getCapabilities API call on the service object to retrieve capabilities. | input: none; output: capabilities, status | API returns a valid status object with isOk() == true | Should Pass |@n
 * | 02 | Log the output of the getCapabilities API including supportedTriggers and preconfiguredTriggers counts. | input: capabilities (populated by API) | Log displays correct counts of supportedTriggers and preconfiguredTriggers | Should be successful |@n
 * | 03 | Assert that the API call was successful using status.isOk(). | status returned from API call | Assertion passes if status.isOk() is true | Should Pass |@n
 * | 04 | Validate that the output parameter 'capabilities' has non-empty supportedTriggers and preconfiguredTriggers arrays. | capabilities.supportedTriggers, capabilities.preconfiguredTriggers | Both arrays are non-empty | Should Pass |@n
 * | 05 | Verify that every trigger in preconfiguredTriggers is present in supportedTriggers. | capabilities.preconfiguredTriggers, capabilities.supportedTriggers | Every preconfigured trigger is also available in supportedTriggers | Should Pass |@n
 * | 06 | Retrieve the expected counts of supportedTriggers and preconfiguredTriggers from the YAML configuration. | expectedSupportedCount, expectedpreconfiguredCount from YAML macros | Expected counts are retrieved correctly | Should Pass |@n
 * | 07 | Iterate over the YAML supportedTriggers values and confirm that each expected trigger is present in supportedTriggers. | YAML input: deep_sleep.supportedTriggers values; triggerMap mapping; expectedEnum values | Each expected trigger is found in capabilities.supportedTriggers | Should Pass |@n
 * | 08 | Iterate over the YAML preconfiguredTriggers values and confirm that each expected trigger is present in preconfiguredTriggers. | YAML input: deep_sleep.preconfiguredTriggers values; triggerMap mapping; expectedEnum values | Each expected trigger is found in capabilities.preconfiguredTriggers | Should Pass |
 */
UT_ADD_TEST(VTS_L1_DEEPSLEEP, getCapabilities_pos)
{
    com::rdk::hal::deepsleep::Capabilities caps;
    DEEPSLEEP_GET_CAPABILITY(gDEEPSLEEPHandle, caps);
    // Validate that the output parameter has been populated.
    UT_ASSERT_TRUE(!caps.supportedTriggers.empty());
    UT_ASSERT_TRUE(!caps.preconfiguredTriggers.empty());
    size_t supportedcount = UT_KVP_PROFILE_GET_LIST_COUNT(SUPPORTEDLIST_NODE);
    size_t preconfiguredcount = UT_KVP_PROFILE_GET_LIST_COUNT(PRECONFIGUREDLIST_NODE);
    // Verify that every preconfigured trigger is a member of supportedTriggers.
    for (auto mandTrigger : caps.preconfiguredTriggers) {
        UT_ASSERT_NOT_EQUAL(std::find(caps.supportedTriggers.begin(), caps.supportedTriggers.end(), mandTrigger),
                  caps.supportedTriggers.end());
    }

    UT_ASSERT_EQUAL(caps.supportedTriggers.size(), supportedcount);
    for (size_t i = 0; i < supportedcount; i++) {
        bool result = false;
        std::string key_string = std::string(SUPPORTEDLIST_NODE) + "." + std::to_string(i);
        char trigger[MAX_LENGTH] = {0};
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
        std::string key_string = std::string(PRECONFIGUREDLIST_NODE) + "." + std::to_string(i);
        char trigger[MAX_LENGTH] = {0};
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

/**
 * @brief Validate the getWakeUpTimer API functionality.
 *
 * This test validates that getWakeUpTimer returns -1.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Call getWakeUpTimer to retrieve the timer value where no timer period is set. | getWakeUpTimer output: timer | getWakeUpTimer returns -1| Should Pass |
 * | 02 | Verify timer value received is -1 | getWakeUpTimer output: timer , -1 | passes the assertion check.  | Should Pass |@n
 */
UT_ADD_TEST(VTS_L1_DEEPSLEEP, gettimer_pos)
{
    int32_t time;
    DEEPSLEEP_GET_TIMER(gDEEPSLEEPHandle, time);
    UT_ASSERT_EQUAL(time, -1);
    return;
}

/**
* @brief Verify that setWakeUpTimer works correctly for positive scenarios
*
* It verifies that the API call returns a successful status and that the timer is set successfully for each input. 
* The test ensures that the wake-up timer function behaves as expected in a positive scenario.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 003@n
* **Priority:** High@n
*
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
*
* **Test Procedure:**
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | For each seconds value in the vector {1, 604800, 3600}, log the input parameters and the expected pre-call state. | seconds = 1, 604800, 3600 | Pre-call logging should indicate the input seconds value for each iteration | Should be successful |
* | 02 | Call the setWakeUpTimer API with the provided seconds value and capture the output in timerSuccess, while logging the result. | input: seconds = current value; output: timerSuccess | API should return status.isOk() as true and timerSuccess as true | Should Pass |
* | 03 | Assert that the status returned by setWakeUpTimer is ok and that the timerSuccess flag is true. | input: status returned; output: status.isOk() = true, timerSuccess = true | Both assertions should pass, indicating correct API functionality | Should Pass |
*/
UT_ADD_TEST(VTS_L1_DEEPSLEEP, settimer_pos)
{
    com::rdk::hal::deepsleep::Capabilities caps;
    bool result;
    bool supported = true;
    DEEPSLEEP_GET_CAPABILITY(gDEEPSLEEPHandle, caps);
    if (!(isTriggerSupported(caps.supportedTriggers, WakeUpTrigger::TIMER))) {
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

/**
* @brief Validate the negative scenarios for the setWakeUpTimer API.
*
* This test validates the behavior of the setWakeUpTimer API when provided with invalid timer periods.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 004@n
* **Priority:** High@n
*
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
*
* **Test Procedure:**
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | For each seconds value in the vector {0, -1, -10}, log the input parameters | seconds = 0, -1, -10 | Pre-call logging should indicate the input seconds value for each iteration | Should be successful |
* | 02 | Call the setWakeUpTimer API with the provided seconds value and capture the output in timerSuccess, while logging the result. | input: seconds = current value; output: timerSuccess | API should return status.isOk() as true and timerSuccess as false | Should Pass |
* | 03 | Assert that the status returned by setWakeUpTimer is ok and that the timerSuccess flag is false. | input: status returned; output: status.isOk() = true, timerSuccess = false | Both assertions should pass, indicating correct API functionality | Should Pass |
*/
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

/**
 * @brief Validate negative scenarios for the enterDeepSleep API.
 *
 * This test verifies the enterDeepSleep API by performing  negative test scenarios.
 * 
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 005@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | call enterDeepSleep when both triggersToWakeUpon list and preconfigured list is empty. verify that the returned _aidl_return is false and the wokeUpByTriggers vector is empty. | triggersToWakeUpon as empty, output: wokeUpByTriggers (vector), keyCode (optional) | API should return _aidl_return as false and wokeUpByTriggers as empty, confirming proper handling of empty trigger input | Should Pass |
 * | 02 | Call enterDeepSleep with invalid trigger ERROR_UNKNOWN. verify that the returned _aidl_return is false and the wokeUpByTriggers vector is empty. | triggersToWakeUpon = ERROR_UNKNOWN, output: wokeUpByTriggers (vector), keyCode (optional) | API should return _aidl_return as false and wokeUpByTriggers as empty, confirming proper handling of invalid trigger input | Should Pass |
 * | 03 | Call enterDeepSleep with invalid trigger 999. verify that the returned _aidl_return is false and the wokeUpByTriggers vector is empty. | triggersToWakeUpon = 999, output: wokeUpByTriggers (vector), keyCode (optional) | API should return _aidl_return as false and wokeUpByTriggers as empty, confirming proper handling of invalid trigger input | Should Pass |
 */
UT_ADD_TEST(VTS_L1_DEEPSLEEP, enterdeepsleep_neg)
{
    com::rdk::hal::deepsleep::Capabilities caps;
    DEEPSLEEP_GET_CAPABILITY(gDEEPSLEEPHandle, caps);
    std::vector<::com::rdk::hal::deepsleep::WakeUpTrigger> triggersToWakeUpon;
    std::vector<::com::rdk::hal::deepsleep::WakeUpTrigger> wokeUpByTriggers;
    std::optional<com::rdk::hal::deepsleep::KeyCode> kCode;
    bool result;
    if (caps.preconfiguredTriggers.empty()) {
        // when both input list and preconfigured list is empty
        DEEPSLEEP_ENTERDEEPSLEEP(gDEEPSLEEPHandle, triggersToWakeUpon, wokeUpByTriggers, kCode, result);
        UT_ASSERT_FALSE(result);
    }
    UT_ASSERT_TRUE(wokeUpByTriggers.empty());
    UT_ASSERT_FALSE(kCode.has_value());
    triggersToWakeUpon.push_back(com::rdk::hal::deepsleep::WakeUpTrigger::ERROR_UNKNOWN);
    DEEPSLEEP_ENTERDEEPSLEEP(gDEEPSLEEPHandle, triggersToWakeUpon, wokeUpByTriggers, kCode, result);
    UT_ASSERT_FALSE(result);
    UT_ASSERT_TRUE(wokeUpByTriggers.empty());
    UT_ASSERT_FALSE(kCode.has_value());
    triggersToWakeUpon.clear();
    triggersToWakeUpon.push_back(static_cast<::com::rdk::hal::deepsleep::WakeUpTrigger>(999));
    DEEPSLEEP_ENTERDEEPSLEEP(gDEEPSLEEPHandle, triggersToWakeUpon, wokeUpByTriggers, kCode, result);
    UT_ASSERT_FALSE(result);
    UT_ASSERT_TRUE(wokeUpByTriggers.empty());
    UT_ASSERT_FALSE(kCode.has_value());
    return;
}

/**
* @} // End of DEEPSLEEP_HALTEST_CLASS
* @} // End of DEEPSLEEP_HALTEST
* @} // End of DEEPSLEEP
* @} // End of HPK
*/
/* End of vts_deepsleep_l1_tests.cpp */
