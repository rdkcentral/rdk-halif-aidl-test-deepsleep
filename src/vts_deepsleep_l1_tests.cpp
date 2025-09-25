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
        // android::ProcessState::self()->startThreadPool();
    }

    void TearDown() override
    {
    }
};

UT_ADD_TEST(VTS_L1_DEEPSLEEP, getCaps_pos)
{
    com::rdk::hal::deepsleep::Capabilities caps;
    DEEPSLEEP_GET_CAPABILITY(gDEEPSLEEPHandle, caps);
    UT_ASSERT_EQUAL(caps.supportedTriggers.size(), UT_KVP_PROFILE_GET_LIST_COUNT("deep_sleep.supportedTriggers"));
    UT_ASSERT_EQUAL(caps.preconfiguredTriggers.size(), UT_KVP_PROFILE_GET_LIST_COUNT("deep_sleep.preconfiguredTriggers"));
    return;
}

UT_ADD_TEST(VTS_L1_DEEPSLEEP, getCaps_neg)
{
    com::rdk::hal::deepsleep::Capabilities caps;
    DEEPSLEEP_GET_CAPABILITY(gDEEPSLEEPHandle, nullptr);
    return;
}

/**
* @} // End of DEEPSLEEP_HALTEST_CLASS
* @} // End of DEEPSLEEP_HALTEST
* @} // End of DEEPSLEEP
* @} // End of HPK
*/
/* End of vts_deepsleep_l1_tests.cpp */