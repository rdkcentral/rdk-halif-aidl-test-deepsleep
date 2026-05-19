/**
 *  If not stated otherwise in this file or this component's LICENSE
 *  file the following copyright and licenses apply:
 *
 *  Copyright 2026 RDK Management
 *
 *  Licensed under the Apache License, Version 2.0 (the License);
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an AS IS BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
 * @defgroup DEEPSLEEP_HALTEST_PARSE_TESTCONFIG_HEADER parse test configuration file
 * @{
 * @parblock
 *
 * ### Parse test configuration functions for DEEPSLEEP :
 *
 * Parse configuration functions required for the module across all vendors.
 *
 * **Pre-Conditions:**  None @n
 * **Dependencies:** None @n
 *
 * @endparblock
 *
 */

/**
 * @file vts_deepsleep_parse_testconfig.h
 *
 */

#ifndef __VTS_DEEPSLEEP_PARSE_TESTCONFIG_H__
#define __VTS_DEEPSLEEP_PARSE_TESTCONFIG_H__

#include <ut.h>
#include <ut_log.h>
#include "ut_kvp_profile.h"
#include "vts_deepsleep.h"

using namespace std;

/**
 * @brief Structure to hold expected DeepSleep test results
 * Contains the expected wake-up triggers and keycode status
 */
typedef struct vts_deepsleep_result
{
    int32_t expectedwaittime;
    std::vector<::com::rdk::hal::deepsleep::WakeUpTrigger> output_wokeUpByTriggers;
    std::optional<int32_t> output_keycode;
}vts_deepsleep_result_t;

/**
 * @brief Configuration structure for EnterDeepSleep test
 * Contains test input parameters and expected output results
 */
typedef struct enterdeepsleepTestConfig
{
    int32_t timer;
    std::vector<::com::rdk::hal::deepsleep::WakeUpTrigger> input_triggersToWakeUpon;
    vts_deepsleep_result_t output;
}enterdeepsleepTestConfig_t;

#define VTS_DEEPSLEEP_TEST_CONFIG_PATH "./test_configuration/vts_l3_DeepSleep.yaml"

/**
 * @brief Parses the EnterDeepSleep test configuration from YAML file
 * 
 * @param[in] configurationFile Path to the YAML configuration file
 * @param[in] module Module name
 * @param[in] testGroup Test group name
 * @param[in] testName Test name
 * @param[out] testConfiguration Parsed test configuration
 * 
 * @return UT_STATUS_OK on success, appropriate error code otherwise
 */
UT_status_t vts_deepsleep_parse_enterdeepsleep_config(char *configurationFile, const char *module, const char *testGroup,
                                                 const char *testName, enterdeepsleepTestConfig_t& testConfiguration);
#endif //__VTS_DEEPSLEEP_PARSE_TESTCONFIG_H__

/** @} */ // End of DEEPSLEEP_HALTEST_PARSE_TESTCONFIG_HEADER
/** @} */ // End of DEEPSLEEP_HALTEST
/** @} */ // End of DEEPSLEEP
/** @} */ // End of HPK