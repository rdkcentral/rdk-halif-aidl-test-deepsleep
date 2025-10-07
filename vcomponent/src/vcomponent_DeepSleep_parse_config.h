/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2024 RDK Management
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

/** @brief
* vcomponent_DeepSleep_parse_config.h
*
* Header file for vcomponent DeepSleep driver
*/

#pragma once

#include <string>
#include <ut.h>
#include <ut_log.h>
#include <ut_kvp.h>
#include <ut_control_plane.h>
#include "vcomponent_DeepSleep.h"

typedef struct deepSleepConfig
{
    std::vector<::com::rdk::hal::deepsleep::WakeUpTrigger> supportedValues;
    std::vector<::com::rdk::hal::deepsleep::WakeUpTrigger> preconfiguredValues;
} deepSleepConfig_t;

extern deepSleepConfig_t gDeepSleepConfig;
static uint32_t gTimer = -1;

void vcomponent_DeepSleep_parse_config(char *configurationFile);
void vcomponent_DeepSleep_initialize_ControlPlane();
void vcomponent_DeepSleep_register_and_start_ControlPlane();
void vcomponent_DeepSleep_destroy_ControlPlane();
// void* vcomponent_DeepSleep_kvp_create_instance(char *fileName);
// void vcomponent_DeepSleep_kvp_destroy_instance(void *instance);
// ut_kvp_instance_t* vcomponent_DeepSleep_get_kvp_instance();