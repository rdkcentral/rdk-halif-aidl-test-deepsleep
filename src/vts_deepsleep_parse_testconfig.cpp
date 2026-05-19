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
 * @file vts_deepsleep_parse_testconfig.cpp
 *
 */

#include "vts_deepsleep_parse_testconfig.h"

static void* vcomponentkvp_create_instance(char *fileName)
{
    ut_kvp_instance_t* instance;
    ut_kvp_status_t result;

    instance = ut_kvp_createInstance();

    if(!instance)
    {
        UT_LOG_ERROR("[VCOMPONENT_DEEPSLEEP]Failed to create ut kvp instance\n");
        return nullptr;
    }

    result = ut_kvp_open(instance, fileName);

    if(result != UT_KVP_STATUS_SUCCESS)
    {
        UT_LOG_ERROR("[VCOMPONENT_DEEPSLEEP]Failed to open test configuration file\n");
        ut_kvp_destroyInstance(instance);
        return nullptr;
    }

    return (ut_kvp_instance_t*)instance;
}

static void vcomponentkvp_destroy_instance(void *instance)
{
    ut_kvp_destroyInstance((ut_kvp_instance_t*)instance);
}

UT_status_t vts_deepsleep_parse_enterdeepsleep_config(char *configurationFile, const char *module, const char *testGroup,
                                                 const char *testName, enterdeepsleepTestConfig_t& testConfiguration)
{
    void *testconfig = vcomponentkvp_create_instance(configurationFile);
    ut_kvp_instance_t* kvpTestInstance = (ut_kvp_instance_t*)testconfig;

    const std::string testNode = string(module) + "." + string(testGroup) + "." + string(testName);
    
    const std::string inputtriggerstr = testNode + ".Parameters" + ".input_triggersToWakeUpon";
    testConfiguration.timer = ut_kvp_getUInt32Field(kvpTestInstance, (testNode + ".Parameters" + ".timer").c_str());
    uint32_t inputtriggercount = ut_kvp_getListCount(kvpTestInstance, inputtriggerstr.c_str());
    if (inputtriggercount > 0)
    {
        for (uint32_t index = 0; index < inputtriggercount; index++)
        {
            char trigger[MAX_LENGTH] = {0};
            ut_kvp_status_t ret = ut_kvp_getStringField(kvpTestInstance, (inputtriggerstr + "." + std::to_string(index)).c_str(), trigger, MAX_LENGTH);
            if (ret == UT_KVP_STATUS_SUCCESS)
                testConfiguration.input_triggersToWakeUpon.push_back(fromString(trigger));
        }
    }
    else
    {
        UT_LOG_ERROR("Input trigger list is empty\n");
    }

    testConfiguration.output.expectedwaittime = ut_kvp_getUInt32Field(kvpTestInstance, (testNode + ".Expected_Results" + ".expectedwaittime").c_str());
    const std::string outputtriggerstr = testNode + ".Expected_Results" + ".output_wokeUpByTriggers";
    uint32_t outputtriggercount = ut_kvp_getListCount(kvpTestInstance, outputtriggerstr.c_str());
    if (outputtriggercount > 0)
    {
        for (uint32_t index = 0; index < outputtriggercount; index++)
        {
            char trigger[MAX_LENGTH] = {0};
            ut_kvp_status_t ret = ut_kvp_getStringField(kvpTestInstance, (outputtriggerstr + "." + std::to_string(index)).c_str(), trigger, MAX_LENGTH);
            if (ret == UT_KVP_STATUS_SUCCESS)
                testConfiguration.output.output_wokeUpByTriggers.push_back(fromString(trigger));
        }
    }
    else
    {
        UT_LOG_ERROR("Output trigger list is empty\n");
    }
    if (ut_kvp_fieldPresent(kvpTestInstance, (testNode + ".Expected_Results" + ".output_keycode").c_str())) {
        testConfiguration.output.output_keycode = ut_kvp_getUInt32Field(kvpTestInstance, (testNode + ".Expected_Results" + ".output_keycode").c_str());
    }

    vcomponentkvp_destroy_instance(kvpTestInstance);
    return UT_STATUS_OK;
}

/** @} */ // End of DEEPSLEEP_HALTEST_PARSE_TESTCONFIG_HEADER
/** @} */ // End of DEEPSLEEP_HALTEST
/** @} */ // End of DEEPSLEEP
/** @} */ // End of HPK