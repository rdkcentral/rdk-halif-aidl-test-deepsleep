/**
 *  If not stated otherwise in this file or this component's LICENSE
 *  file the following copyright and licenses apply:
 *
 *  Copyright 2024 RDK Management
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
 * @defgroup DEEPSLEEP_HALTEST_HELPER_HEADER DEEPSLEEP HAL Tests helper functions Header File
 * @{
 * @parblock
 *
 * ### Helper functions for VTS tests :
 *
 * Helper functions required for the module across all vendors.
 *
 * **Pre-Conditions:**  None @n
 * **Dependencies:** None @n
 *
 * @endparblock
 *
 */

/**
 * @file vts_deepsleep_helper_functions.h
 *
 */

#ifndef __VTS_DEEPSLEEP_HELPER_H__
#define __VTS_DEEPSLEEP_HELPER_H__

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include <ut_control_plane.h>

#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <binder/Status.h>
#include <utils/String16.h>

#include "com/rdk/hal/deepsleep/IDeepSleep.h"

using android::sp;
using android::String16;
using android::defaultServiceManager;
using android::binder::Status;
using namespace std;
using namespace std::chrono;

using namespace com::rdk::hal;
using namespace com::rdk::hal::deepsleep;

extern const ut_control_keyStringMapping_t bool_mapTable_deepsleep[];
extern const ut_control_keyStringMapping_t statusDeepSleepException_mapTable[];
extern WakeUpTrigger stringToWakeUpTrigger(const std::string& str);

#define MAX_LENGTH 256

std::string vecIntToString(std::vector<int32_t>& addrs);
std::string vecByteToHexString(const std::vector<uint8_t>& data);
bool isTriggerSupported (std::vector<WakeUpTrigger>& triggerlist, WakeUpTrigger trigger);
WakeUpTrigger fromString(const std::string& str);

#endif //__VTS_DEEPSLEEP_HELPER_H__

/** @} */ // End of DEEPSLEEP_HALTEST_HELPER_HEADER
/** @} */ // End of DEEPSLEEP_HALTEST
/** @} */ // End of DEEPSLEEP
/** @} */ // End of HPK
