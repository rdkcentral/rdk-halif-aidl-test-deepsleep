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

class  VTS_DEEPSLEEP_L3_SimulateErrorScenario(deepsleepHelperClass):
    """
    Runs L3 SimulateErrorScenario test
    """
    def __init__(self, log:logModule=None, hfpfile:str=None, testMenuConfigFile=None, moduleName=None, artifactDirPath=None):
        """
        Initializes the test class
        """
        self.testName = "DeepSleep_L3_SimulateErrorScenario"
        qcId = "1"

        super().__init__(self.testName, qcId, log, hfpfile, testMenuConfigFile, moduleName, artifactDirPath)

    def testFunction(self):
        """
        The main test function to run the L3 SimulateErrorScenario test

        Returns:
            bool: Final result of the deep sleep test, including power-level validation.
        """
        self.utilities.change_directory(self.hal_session, self.targetWorkspace)
        results = True
        self.testcase = "EnterDeepSleep"

        self.profileConfiguration = ConfigRead(self.moduleHFPFile, self.interfaceName)
        inputlist = self.profileConfiguration.supportedTriggers
        outputlist = []
        threadwaittime = 2
        self.updateTestconfigurationFile(inputlist, outputlist, 5, threadwaittime)
        self.utilities.sftpCopy(self.hal_session, self.l3TestConfigFile, self.l3TestConfigFolder)

        for errortype in ["BEFORE_DEEPSLEEP", "AFTER_DEEPSLEEP"]:
            self.deepsleepDevice.simulateError(errortype, "EX_NONE")
            time.sleep(1)
            self.utMenu.start()
            self.log.stepStart(f'Test Suit: {DEEPSLEEP_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')
            output = self.utMenu.select(DEEPSLEEP_L3_TEST_SUITE_NAME, self.testcase, None, self.waitTime, False)
            group_results = self.utMenu.collect_results(output, True)
            if group_results is None:
                group_results = False
            self.log.stepResult(group_results, f'Test Suit: {DEEPSLEEP_L3_TEST_SUITE_NAME} Test Case: {self.testcase}')
            self.deepsleepDevice.simulateError("", "")
            results = results and group_results

        return results

if __name__ == '__main__':
    summaryLogName = os.path.splitext(os.path.basename(__file__))[0] + "_summery"
    summaryLog  = logModule(summaryLogName, level=logModule.INFO)
    test = VTS_DEEPSLEEP_L3_SimulateErrorScenario(summaryLog )
    test.run(False)