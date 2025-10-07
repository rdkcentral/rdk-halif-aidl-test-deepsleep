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

/* 
* @brief 
* vcomponent_DeepSleep.cpp
* Implementation of the DeepSleep service implementation
*/

#include "vcomponent_DeepSleep.h"
#include "vcomponent_DeepSleep_parse_config.h"
#include <regex>

namespace com
{
    namespace rdk
    {
        namespace hal
        {
            namespace deepsleep
            {

                DeepSleep::DeepSleep()
                {
                }
                DeepSleep::~DeepSleep()
                {
                }

                ::android::binder::Status DeepSleep::getCapabilities(Capabilities *_aidl_return)
                {
                    if (!_aidl_return)
                    {
                        return android::binder::Status::fromExceptionCode(android::binder::Status::EX_ILLEGAL_ARGUMENT);
                    }
                    *_aidl_return = Capabilities();
                    _aidl_return->supportedTriggers = gDeepSleepConfig.supportedValues;
                    _aidl_return->preconfiguredTriggers = gDeepSleepConfig.preconfiguredValues;
                    return ::android::binder::Status::ok();
                }

                ::android::binder::Status DeepSleep::enterDeepSleep(const ::std::vector<WakeUpTrigger>& triggersToWakeUpon, ::std::vector<WakeUpTrigger>* wokeUpByTriggers, ::std::optional<KeyCode>* keyCode, bool* _aidl_return)
                {
                    UT_LOG_INFO("wakeup duration : %d\n", gTimer);
                    WakeUpTrigger trigger = WakeUpTrigger::ERROR_UNKNOWN;
                    if (triggersToWakeUpon.empty()) {
                        if (gDeepSleepConfig.preconfiguredValues.empty()) {
                            UT_LOG_INFO("Both triggersToWakeUpon and preconfiguredValues list are empty\n");
                            *_aidl_return = false;
                            return ::android::binder::Status::ok();
                        } else {
                            trigger = gDeepSleepConfig.preconfiguredValues[0];
                        }
                    } else {
                        trigger = triggersToWakeUpon[0];
                    }

                    bool result = false;
                    *keyCode = std::nullopt;
                    wokeUpByTriggers->clear();
                    KeyCode k;           // create a KeyCode instance
                    std::vector<WakeUpTrigger> triggerlist;

                    switch (trigger) {
                        case WakeUpTrigger::RCU_IR:
                            UT_LOG_INFO("Trigger: RCU_IR\n");
                            k.keyCode = 0;
                            triggerlist.push_back(WakeUpTrigger::RCU_IR);
                            *keyCode = k;
                            result = true;
                            break;
                        case WakeUpTrigger::RCU_BT:
                            UT_LOG_INFO("Trigger: RCU_BT\n");
                            k.keyCode = 1;
                            triggerlist.push_back(WakeUpTrigger::RCU_BT);
                            *keyCode = k;
                            result = true;
                            break;
                        case WakeUpTrigger::RCU_RF4CE:
                            UT_LOG_INFO("Trigger: RCU_RF4CE\n");
                            k.keyCode = 2;
                            triggerlist.push_back(WakeUpTrigger::RCU_RF4CE);
                            *keyCode = k;
                            result = true;
                            break;
                        case WakeUpTrigger::LAN:
                            UT_LOG_INFO("Trigger: LAN\n");
                            triggerlist.push_back(WakeUpTrigger::LAN);
                            result = true;
                            break;
                        case WakeUpTrigger::WLAN:
                            UT_LOG_INFO("Trigger: WLAN\n");
                            triggerlist.push_back(WakeUpTrigger::WLAN);
                            result = true;
                            break;
                        case WakeUpTrigger::TIMER:
                            if (gTimer > 0) {
                                sleep(gTimer);
                                UT_LOG_INFO("Trigger: TIMER\n");
                                triggerlist.push_back(WakeUpTrigger::TIMER);
                                result = true;
                            }
                            break;
                        case WakeUpTrigger::FRONT_PANEL:
                            UT_LOG_INFO("Trigger: FRONT_PANEL\n");
                            triggerlist.push_back(WakeUpTrigger::FRONT_PANEL);
                            result = true;
                            break;
                        case WakeUpTrigger::CEC:
                            UT_LOG_INFO("Trigger: CEC\n");
                            triggerlist.push_back(WakeUpTrigger::CEC);
                            result = true;
                            break;
                        case WakeUpTrigger::PRESENCE:
                            UT_LOG_INFO("Trigger: PRESENCE\n");
                            triggerlist.push_back(WakeUpTrigger::PRESENCE);
                            result = true;
                            break;
                        case WakeUpTrigger::VOICE:
                            UT_LOG_INFO("Trigger: VOICE\n");
                            triggerlist.push_back(WakeUpTrigger::VOICE);
                            result = true;
                            break;
                        case WakeUpTrigger::ERROR_UNKNOWN:
                        default:
                            UT_LOG_INFO("Trigger: UNKNOWN or invalid (%d)\n", static_cast<int>(trigger));
                            triggerlist.push_back(WakeUpTrigger::ERROR_UNKNOWN);
                            result = false;
                            break;
                    }
                    *wokeUpByTriggers = triggerlist;
                    *_aidl_return = result;
                    return ::android::binder::Status::ok();
                }

                ::android::binder::Status DeepSleep::setWakeUpTimer(int32_t seconds, bool* _aidl_return)
                {
                    bool result = false;
                    if ((seconds > 0) && (seconds < 604801)) {
                        gTimer = seconds;
                        result = true;
                    }
                    *_aidl_return = result;
                    return ::android::binder::Status::ok();
                }
                ::android::binder::Status DeepSleep::getWakeUpTimer(int32_t* _aidl_return)
                {
                    *_aidl_return = gTimer;
                    return ::android::binder::Status::ok();
                }

                void DeepSleep::binderDied(const ::android::wp<::android::IBinder> &who)
                {
                    // Handle binder death if necessary
                }

            } // namespace deepsleep
        } // namespace hal
    } // namespace rdk
} // namespace com