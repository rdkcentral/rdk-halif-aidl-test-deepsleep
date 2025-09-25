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
 * @defgroup DEEPSLEEP_HALTEST_HEADER DEEPSLEEP HAL Tests helper functions Header File
 * @{
 * @parblock
 *
 * ### Common functions/macros required for the deepsleep :
 *
 * Common functions/macros required for the deepsleep .
 *
 * **Pre-Conditions:**  None @n
 * **Dependencies:** None @n
 *
 * @endparblock
 *
 */

/**
 * @file vts_deepsleep.h
 *
 */

#ifndef __VTS_DEEPSLEEP_H__
#define __VTS_DEEPSLEEP_H__

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include <ut.h>
#include <ut_log.h>
#include <ut_kvp_profile.h>
#include <ut_control_plane.h>
#include <chrono>
#include <thread>

#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <binder/Status.h>
#include <utils/String16.h>

#include "com/rdk/hal/deepsleep/IDeepSleep.h"

#include "vts_deepsleep_helper_functions.h"

using android::sp;
using android::String16;
using android::defaultServiceManager;
using android::binder::Status;
using namespace std;
using namespace std::chrono;

using namespace com::rdk::hal;
using namespace com::rdk::hal::deepsleep;

#define DEEPSLEEP_GET_CAPABILITY(deepsleepHandle, capabilities) \
    do { \
        ::android::binder::Status status; \
        UT_LOG_DEBUG("Calling getState(OUT:capabilities[]"); \
        status = (deepsleepHandle)->getCapabilities(&capabilities); \
        UT_LOG_DEBUG("Result getCapabilities(OUT:caps), status:[isOK:%s, exception:%s]", \
                UT_Control_GetMapString(bool_mapTable_deepsleep, status.isOk()), \
                UT_Control_GetMapString(statusDeepSleepException_mapTable, status.exceptionCode())); \
        UT_LOG_DEBUG("Capabilities supportedTriggers size: %zu, preconfiguredTriggers size:%zu", caps.supportedTriggers.size(), caps.preconfiguredTriggers.size()); \
        UT_ASSERT_TRUE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), Status::EX_NONE); \
    } while (0)
#if 0
#define DEEPSLEEP_OPEN_SUCCESS(deepsleepHandle, controllerListener, hdmiCecController, stateCheck) \
    do { \
        android::binder::Status status; \
        \
        /* Open the Decoder */ \
        UT_LOG_DEBUG("Calling open(IN:controllerListener[%p], OUT:hdmiCecController[])", controllerListener); \
        status = (deepsleepHandle)->open(controllerListener, &hdmiCecController); \
        UT_LOG_DEBUG("Result open(IN:IN:controllerListener[%p], OUT:hdmiCecController[0x%X]), status:[isOK:%s, exception:%s]", \
            controllerListener, hdmiCecController.get(), \
            UT_Control_GetMapString(bool_mapTable_cec, status.isOk()), \
            UT_Control_GetMapString(statusHdmiCecException_mapTable, status.exceptionCode())); \
        UT_ASSERT_TRUE_FATAL(status.isOk()); \
        UT_ASSERT_NOT_EQUAL_FATAL(hdmiCecController.get(), nullptr); \
        UT_ASSERT_EQUAL_FATAL(status.exceptionCode(), Status::EX_NONE); \
        \
        if (stateCheck) \
        { \
            com::rdk::hal::State lastState; \
            DEEPSLEEP_WAIT_FOR_STATE(deepsleepHandle, State::STARTED, lastState); \
            UT_ASSERT_EQUAL_FATAL(lastState, State::STARTED); \
        } \
    } while (0)

#define DEEPSLEEP_OPEN_FAILURE(deepsleepHandle, controllerListener, hdmiCecController, exception) \
    do { \
        android::binder::Status status; \
        \
        /* Open the Decoder */ \
        UT_LOG_DEBUG("Calling open(IN:controllerListener[%p], OUT:hdmiCecController[])", controllerListener); \
        status = (deepsleepHandle)->open(controllerListener, &hdmiCecController); \
        UT_LOG_DEBUG("Result open(IN:IN:controllerListener[%p], OUT:hdmiCecController[0x%X]), status:[isOK:%s, exception:%s]", \
            controllerListener, hdmiCecController.get(), \
            UT_Control_GetMapString(bool_mapTable_cec, status.isOk()), \
            UT_Control_GetMapString(statusHdmiCecException_mapTable, status.exceptionCode())); \
        UT_ASSERT_FALSE(status.isOk()); \
        UT_ASSERT_EQUAL(status.exceptionCode(), exception); \
        UT_ASSERT_EQUAL(hdmiCecController.get(), nullptr); \
    } while (0)
#endif

#endif // __VTS_DEEPSLEEP_H__

/** @} */ // End of DEEPSLEEP_HALTEST_HEADER
/** @} */ // End of DEEPSLEEP_HALTEST
/** @} */ // End of DEEPSLEEP Module
/** @} */ // End of HPK