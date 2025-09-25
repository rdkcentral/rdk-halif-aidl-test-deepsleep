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
                    _aidl_return->supportedTriggers.push_back(::com::rdk::hal::deepsleep::WakeUpTrigger::RCU_IR);
                    _aidl_return->supportedTriggers.push_back(::com::rdk::hal::deepsleep::WakeUpTrigger::RCU_BT);
                    _aidl_return->supportedTriggers.push_back(::com::rdk::hal::deepsleep::WakeUpTrigger::LAN);
                    _aidl_return->supportedTriggers.push_back(::com::rdk::hal::deepsleep::WakeUpTrigger::WLAN);
                    _aidl_return->supportedTriggers.push_back(::com::rdk::hal::deepsleep::WakeUpTrigger::TIMER);
                    _aidl_return->supportedTriggers.push_back(::com::rdk::hal::deepsleep::WakeUpTrigger::FRONT_PANEL);
                    _aidl_return->supportedTriggers.push_back(::com::rdk::hal::deepsleep::WakeUpTrigger::CEC);
                    _aidl_return->supportedTriggers.push_back(::com::rdk::hal::deepsleep::WakeUpTrigger::PRESENCE);
                    _aidl_return->supportedTriggers.push_back(::com::rdk::hal::deepsleep::WakeUpTrigger::VOICE);
                    _aidl_return->preconfiguredTriggers.push_back(::com::rdk::hal::deepsleep::WakeUpTrigger::RCU_IR);
                    _aidl_return->preconfiguredTriggers.push_back(::com::rdk::hal::deepsleep::WakeUpTrigger::RCU_BT);
                    _aidl_return->preconfiguredTriggers.push_back(::com::rdk::hal::deepsleep::WakeUpTrigger::TIMER);
                    _aidl_return->preconfiguredTriggers.push_back(::com::rdk::hal::deepsleep::WakeUpTrigger::FRONT_PANEL);
                    return ::android::binder::Status::ok();
                }

                ::android::binder::Status DeepSleep::enterDeepSleep(const ::std::vector<WakeUpTrigger>& triggersToWakeUpon, ::std::vector<WakeUpTrigger>* wokeUpByTriggers, ::std::optional<KeyCode>* keyCode, bool* _aidl_return)
                {
                    *_aidl_return = true;
                    return ::android::binder::Status::ok();
                }
                ::android::binder::Status DeepSleep::setWakeUpTimer(int32_t seconds, bool* _aidl_return)
                {
                    *_aidl_return = true;
                    return ::android::binder::Status::ok();
                }
                ::android::binder::Status DeepSleep::getWakeUpTimer(int32_t* _aidl_return)
                {
                    *_aidl_return = 10;
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