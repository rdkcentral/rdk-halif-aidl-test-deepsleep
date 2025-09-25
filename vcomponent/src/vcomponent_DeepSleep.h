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

 /*
 @brief Header file for the DeepSleep service implementation.
 */

#pragma once // Include guard

#include <com/rdk/hal/deepsleep/IDeepSleep.h>
#include <com/rdk/hal/deepsleep/BnDeepSleep.h>
#include <binder/BinderService.h>
#include <binder/IBinder.h>
#include <ut_kvp.h>

namespace com
{
    namespace rdk
    {
        namespace hal
        {
            namespace deepsleep
            {

                class DeepSleep : public android::BinderService<DeepSleep>, public android::IBinder::DeathRecipient, public BnDeepSleep
                {
                public:
                    static char const* getServiceName() { return IDeepSleep::serviceName().c_str(); }

                    DeepSleep();
                    virtual ~DeepSleep();

                    //::android::IBinder *onAsBinder() override;

                    ::android::binder::Status getCapabilities(Capabilities *_aidl_return) override;
                    ::android::binder::Status enterDeepSleep(const ::std::vector<WakeUpTrigger>& triggersToWakeUpon, ::std::vector<WakeUpTrigger>* wokeUpByTriggers, ::std::optional<KeyCode>* keyCode, bool* _aidl_return) override;
                    ::android::binder::Status setWakeUpTimer(int32_t seconds, bool* _aidl_return) override;
                    ::android::binder::Status getWakeUpTimer(int32_t* _aidl_return) override;

                private:
                    void binderDied(const ::android::wp<::android::IBinder> &who) override;
                };

            } // namespace deepsleep
        } // namespace hal
    } // namespace rdk
} // namespace com
extern ut_kvp_instance_t* ginstance;