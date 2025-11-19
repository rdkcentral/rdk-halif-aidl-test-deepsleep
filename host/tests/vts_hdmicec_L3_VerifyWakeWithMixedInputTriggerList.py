#!/usr/bin/env python3
#** *****************************************************************************
# *
# * If not stated otherwise in this file or this component's LICENSE file the
# * following copyright and licenses apply:
# *
# * Copyright 2024 RDK Management
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *
# http://www.apache.org/licenses/LICENSE-2.0
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
import time
import threading

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../"))

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.core.logModule import logModule
from raft.framework.plugins.ut_raft import utHelperClass
from raft.framework.plugins.ut_raft.utSuiteNavigator import UTSuiteNavigatorClass
from raft.framework.plugins.ut_raft.utBaseUtils import utBaseUtils
from vts_deepsleep_helper import deepsleepHelperClass

DEEPSLEEP_L3_TEST_SUITE_NAME = "VTS_L3_DEEPSLEEP"

class  VTS_DEEPSLEEP_L3_VerifyWakeWithMixedInputTriggerList(deepsleepHelperClass):
    """
    Runs L3 VerifyWakeWithMixedInputTriggerList test
    """
    def __init__(self, log:logModule=None):
        """
        Initializes the test class
        """
        self.testName = "DeepSleep_L3_VerifyWakeWithMixedInputTriggerList"
        qcId = "1"

        super().__init__(self.testName, qcId, log)

    def updateTestconfigurationFile(self, timer: int = 0):
        """
        Updates the DEEPSLEEP L3 ReceiveMessage YAML configuration file.
        """
        data = self.deepsleepReadYaml(self.l3TestConfigFile)

        data["DeepSleep"]["L3"]["EnterDeepSleep"]["Parameters"]["timer"] = timer
        data["DeepSleep"]["L3"]["EnterDeepSleep"]["Parameters"]["input_triggersToWakeUpon"] = [
            "LAN",
            "TIMER",
            "FRONT_PANEL"
        ]
        
        data["DeepSleep"]["L3"]["EnterDeepSleep"]["Expected_Results"]["output_wokeUpByTriggers"] = [
            "LAN",
            "TIMER",
            "FRONT_PANEL"
        ]

        data["DeepSleep"]["L3"]["EnterDeepSleep"]["Expected_Results"]["output_keycode"] = False
        # write to YAML
        self.deepsleepWriteYaml(self.l3TestConfigFile, data)

    def wakeuptriggerThread(self, trigger: str, threadwaittime: int=5):
        """
        Thread function to send messages to DUT
        """
        #self.power_level = self.getPowerLevel()
        time.sleep(threadwaittime)  # Wait for DUT to be ready to trigger wakeup
        self.deepsleepDevice.triggerWakeup(trigger)
        time.sleep(1)  # Small delay between messages

    def testFunction(self):
        """
        The main test function to run the L3 ReceiveMessage.

        Returns:
            bool: Final result of the test.
        """
        self.utilities.change_directory(self.hal_session, self.targetWorkspace)
        results = False
        self.testcase = "EnterDeepSleep"

        self.updateTestconfigurationFile(10)
        self.utilities.sftpCopy(self.hal_session, self.l3TestConfigFile, self.l3TestConfigFolder)
        self.wakeuptriggerThreadObj1 = threading.Thread(target=self.wakeuptriggerThread, args=("LAN", 2))
        self.utMenu.start()
        self.wakeuptriggerThreadObj1.start()
        self.log.stepStart(f'Test Suit: {DEEPSLEEP_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')
        output = self.utMenu.select(DEEPSLEEP_L3_TEST_SUITE_NAME, self.testcase, None, self.waitTime, False)
        self.wakeuptriggerThreadObj1.join()
        results = self.utMenu.collect_results(output, True)
        if results is None:
            results = False
        self.log.stepResult(results, f'Test Suit: {DEEPSLEEP_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')

        self.wakeuptriggerThreadObj1 = threading.Thread(target=self.wakeuptriggerThread, args=("FRONT_PANEL", 2))
        self.utMenu.start()
        self.wakeuptriggerThreadObj1.start()
        self.log.stepStart(f'Test Suit: {DEEPSLEEP_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')
        output = self.utMenu.select(DEEPSLEEP_L3_TEST_SUITE_NAME, self.testcase, None, self.waitTime, False)
        self.wakeuptriggerThreadObj1.join()
        results = self.utMenu.collect_results(output, True)
        if results is None:
            results = False
        self.log.stepResult(results, f'Test Suit: {DEEPSLEEP_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')

        self.log.stepStart(f'Test Suit: {DEEPSLEEP_L3_TEST_SUITE_NAME} Test Case: check_power_level')
        self.profileConfiguration = ConfigRead(self.moduleHFPFile, self.interfaceName)
        expected_power = self.profileConfiguration.expected_power_level
        if (self.power_level == expected_power):
            results = True
        else:
            results = False
        self.log.stepResult(results, f'Test Suit: {DEEPSLEEP_L3_TEST_SUITE_NAME} Test Case: check_power_level')

        return results

if __name__ == '__main__':
    summaryLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summaryLog  = logModule(summaryLogName, level=logModule.INFO)
    test = VTS_DEEPSLEEP_L3_VerifyWakeWithMixedInputTriggerList(summaryLog )
    test.run(False)