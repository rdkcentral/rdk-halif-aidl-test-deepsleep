#!/usr/bin/env python3
#** *****************************************************************************
# *
# * If not stated otherwise in this file or this component's LICENSE file the
# * following copyright and licenses apply:
# *
# * Copyright 2026 RDK Management
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# * http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *
#* ******************************************************************************


import os
import sys
import threading
import time

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../"))

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.core.logModule import logModule
from raft.framework.plugins.ut_raft import utHelperClass
from raft.framework.plugins.ut_raft.utSuiteNavigator import UTSuiteNavigatorClass
from raft.framework.plugins.ut_raft.utBaseUtils import utBaseUtils
from vts_deepsleep_helper import deepsleepHelperClass

DEEPSLEEP_L3_TEST_SUITE_NAME = "VTS_L3_DEEPSLEEP"

class  VTS_DEEPSLEEP_L3_NoWakeFromUnspecifiedOrUnsupportedTrigger(deepsleepHelperClass):
    """
    Runs L3 NoWakeFromUnspecifiedOrUnsupportedTrigger test
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test class
        """
        self.testName = "DeepSleep_L3_NoWakeFromUnspecifiedOrUnsupportedTrigger"
        qcId = "1"

        super().__init__(self.testName, qcId, log)

    def wakeuptriggerThreadLocal(self, trigger: str, wakeusingtrigger: str, threadwaittime: int=5):
        """
        Thread function to send messages to DUT
        """
        time.sleep(threadwaittime)  # Wait for DUT to be ready to trigger wakeup
        self.deepsleepDevice.triggerWakeup(trigger)
        buffertime = self.profileConfiguration.bufferduration
        time.sleep(buffertime)  # Small delay between messages
        self.log.stepStart(f'Test Suit: {DEEPSLEEP_L3_TEST_SUITE_NAME} Test Case: validate_no_wakeupfrom_{trigger}')
        result = True
        if (self.inDeepsleep == False):
            result = False #Device is not in deepsleep"
        else:
            self.deepsleepDevice.triggerWakeup(wakeusingtrigger)
            time.sleep(1)
        self.log.stepResult(result, f'Test Suit: {DEEPSLEEP_L3_TEST_SUITE_NAME} Test Case: validate_no_wakeupfrom_{trigger}')


    def testFunction(self):
        """
        The main test function to run the L3 to Verify the below scenerios
        1. No deepsleep wake up from triggers not specified in supported list.
        2. No deepsleep wake up from triggers not specified in input list.

        Returns:
            bool: Final result of the deep sleep test, including power-level validation.
        """
        self.utilities.change_directory(self.hal_session, self.targetWorkspace)
        results = False
        self.testcase = "EnterDeepSleep"

        # test for unsupported triggers
        self.profileConfiguration = ConfigRead(self.moduleHFPFile, self.interfaceName)
        supported_triggers = self.profileConfiguration.supportedTriggers

        # excluded TIMER
        availabletriggers = ["RCU_IR", "RCU_BT" , "RCU_RF4CE", "LAN", "WLAN", "FRONT_PANEL", "CEC", "PRESENCE", "VOICE"]
        outputlist = [supported_triggers[0]]
        threadwaittime = 5
        unsupportedtrigger = [x for x in availabletriggers if x not in supported_triggers]
        keycode = self.cpe.get("remote").get("deepsleep").get(supported_triggers[0])
        self.updateTestconfigurationFile(supported_triggers, outputlist, 10, threadwaittime, keycode)
        self.utilities.sftpCopy(self.hal_session, self.l3TestConfigFile, self.l3TestConfigFolder)
        for trigger in unsupportedtrigger:
            self.wakeuptriggerThreadObj1 = threading.Thread(target=self.wakeuptriggerThreadLocal, args=(trigger, supported_triggers[0], threadwaittime))
            self.utMenu.start()
            self.wakeuptriggerThreadObj1.start()
            self.inDeepsleep = True
            self.log.stepStart(f'Test Suit: {DEEPSLEEP_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')
            output = self.utMenu.select(DEEPSLEEP_L3_TEST_SUITE_NAME, self.testcase, None, self.waitTime, False)
            self.inDeepsleep = False
            self.wakeuptriggerThreadObj1.join()
            results = self.utMenu.collect_results(output, True)
            if results is None:
                results = False
            self.log.stepResult(results, f'Test Suit: {DEEPSLEEP_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')

        # test for unspecified triggers
        preconfigured_triggers = self.profileConfiguration.preconfiguredTriggers
        unspecifiedtrigger = []
        for x in supported_triggers:
            if (x == "TIMER"):
                supported_triggers.remove(x)
                continue
            if x not in preconfigured_triggers:
                unspecifiedtrigger.append(x)
                supported_triggers.remove(x)
                break
        outputlist = [supported_triggers[0]]
        keycode = self.cpe.get("remote").get("deepsleep").get(supported_triggers[0])
        self.updateTestconfigurationFile(supported_triggers, outputlist, 10, threadwaittime, keycode)
        self.utilities.sftpCopy(self.hal_session, self.l3TestConfigFile, self.l3TestConfigFolder)

        for trigger in unspecifiedtrigger:
            self.wakeuptriggerThreadObj1 = threading.Thread(target=self.wakeuptriggerThreadLocal, args=(trigger, supported_triggers[0], threadwaittime))
            self.utMenu.start()
            self.wakeuptriggerThreadObj1.start()
            self.inDeepsleep = True
            self.log.stepStart(f'Test Suit: {DEEPSLEEP_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')
            output = self.utMenu.select(DEEPSLEEP_L3_TEST_SUITE_NAME, self.testcase, None, self.waitTime, False)
            self.inDeepsleep = False
            self.wakeuptriggerThreadObj1.join()
            results = self.utMenu.collect_results(output, True)
            if results is None:
                results = False
            self.log.stepResult(results, f'Test Suit: {DEEPSLEEP_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')
            
        return results

if __name__ == '__main__':
    summaryLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summaryLog  = logModule(summaryLogName, level=logModule.INFO)
    test = VTS_DEEPSLEEP_L3_NoWakeFromUnspecifiedOrUnsupportedTrigger(summaryLog )
    test.run(False)