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
import tarfile
import yaml
import time
import threading

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../"))

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft import utHelperClass
from raft.framework.core.logModule import logModule
from raft.framework.plugins.ut_raft.utSuiteNavigator import UTSuiteNavigatorClass
from raft.framework.plugins.ut_raft.utBaseUtils import utBaseUtils
from deepsleepController.DeepSleepController import DeepSleepController

DEEPSLEEP_TEST_MENU_CONFIGURATION_FILE = os.path.join(dir_path, "vts_deepsleep_testMenuConfiguration.yaml")
DEEPSLEEP_TESTCASES_CONFIGURATION_FILE = os.path.join(os.path.join(dir_path, "../configs"), "vts_deepsleep_testcase_configuration.yaml")
DEEPSLEEP_L3_TEST_CONFIG_FILE = os.path.join(os.path.join(dir_path, "../../test_configuration"), "vts_l3_DeepSleep.yaml")

DEEPSLEEP_L3_TEST_SUITE_NAME = "VTS_L3_DEEPSLEEP"

class deepsleepHelperClass(utHelperClass):

    def __init__(self, testName:str, qcId:str, log:logModule=None ):
        """
        Initializes the test class
        """
        self.moduleName = "DeepSleep"
        self.interfaceName = "IDeepSleep"
        super().__init__(testName, qcId, log)

        self.log = log
        if log is None:
            self.log = logModule(self.__class__.__name__)
            self.log.setLevel(self.log.INFO)

        # Parse device configuration
        self.deviceTestSetup = self.cpe.get("test")
        self.targetWorkspace = os.path.join(self.cpe.get("target_directory"), self.moduleName)
        self.moduleHFPFile = os.path.join(dir_path, self.deviceTestSetup.get("profile"))
        profileOnTarget = os.path.join(self.targetWorkspace, "profiles")
        profileOnTarget = os.path.join(profileOnTarget, os.path.basename(self.moduleHFPFile))
        profileOnTarget = profileOnTarget.replace("\\", "/")
        self.waitTime = self.cpe.get("wait_time")
        self.device_type = self.cpe.get("device_type")
        self.platform = self.cpe.get("platform")

        # Load test menu and test case configuration files
        self.l3TestConfigFile = DEEPSLEEP_L3_TEST_CONFIG_FILE
        self.testMenuConfiguration = ConfigRead(DEEPSLEEP_TEST_MENU_CONFIGURATION_FILE, self.moduleName)
        self.testCaseConfiguration = ConfigRead(DEEPSLEEP_TESTCASES_CONFIGURATION_FILE, self.moduleName)
        self.testMenuConfiguration.test.execute = self.testMenuConfiguration.test.execute + f" -p {profileOnTarget}"
        self.l3TestConfigFolder = os.path.join(self.targetWorkspace, "test_configuration")
        self.l3TestConfigFolder = self.l3TestConfigFolder.replace("\\", "/")
        self.power_level = 0.0

        # Establish HAL session
        self.hal_session = self.dut.getConsoleSession("ssh_hal_test")
        self.hal_session_1 = self.dut.getConsoleSession("ssh_hal_test_1")

        # Initialize UT menu navigator and utilities
        self.utMenu = UTSuiteNavigatorClass(self.testMenuConfiguration, None, self.hal_session)
        self.utilities = utBaseUtils()

        # Copy required artifacts to target device
        self.utilities.change_directory(self.hal_session, self.targetWorkspace)
        for artifact in self.testMenuConfiguration.test.artifacts:
            filesPath = os.path.join(dir_path, artifact)
            self.utilities.sftpCopy(self.hal_session, filesPath, self.targetWorkspace)

            if tarfile.is_tarfile(filesPath):
                target_tarfile = os.path.join(self.targetWorkspace, os.path.basename(artifact))
                target_tarfile = target_tarfile.replace("\\", "/")
                self.utilities.untar(self.hal_session, target_tarfile, self.targetWorkspace)

        #TODO: remove once the vendor layer start handled by vDevice service
        self.utilities.change_directory(self.hal_session_1, self.targetWorkspace)
        self.hal_session_1.write(f"./{self.cpe.get('deepsleep_process_name')} &")
        time.sleep(2)

        self.deepsleepDevice = DeepSleepController(self.platform, self.device_type, self.hal_session_1,
                                        self.cpe.get("prompt"), self.deviceTestSetup.get("controller_port"))

    def getPowerLevel(self) -> float:
        if (self.platform == "vDevice"):
            power = 1.0
        else:
            power = self.powerControl.getPowerLevel()
        return power

    def updateTestconfigurationFile(self, intrigger: list, outtrigger: list, timer: int=0, threadwaittime: int=5, keycode: int=0):
        """
        Updates the DEEPSLEEP L3 ReceiveMessage YAML configuration file.
        """
        data = self.deepsleepReadYaml(self.l3TestConfigFile)

        node = data["DeepSleep"]["L3"]["EnterDeepSleep"]
        node["Parameters"]["timer"] = timer
        node["Parameters"]["input_triggersToWakeUpon"] = intrigger
        
        node["Expected_Results"]["expectedwaittime"] = threadwaittime
        node["Expected_Results"]["output_wokeUpByTriggers"] = outtrigger

        node["Expected_Results"]["output_keycode"] = keycode

        # write to YAML
        self.deepsleepWriteYaml(self.l3TestConfigFile, data)

    def executeAndVerifyResult(self, inputlist: list, outputlist: list, timer: int=0, threadwaittime: int=5, keycode: int=0) -> bool:
            results = False
            self.updateTestconfigurationFile(inputlist, outputlist, timer, threadwaittime, keycode)
            self.utilities.sftpCopy(self.hal_session, self.l3TestConfigFile, self.l3TestConfigFolder)
            self.wakeuptriggerThreadObj1 = threading.Thread(target=self.wakeuptriggerThread, args=(outputlist[0], threadwaittime))
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
            self.log.stepResult(results, f'Test Suit: {DEEPSLEEP_L3_TEST_SUITE_NAME} Test Case: check_power_level{self.power_level}')
            return results

    def wakeuptriggerThread(self, trigger: str, threadwaittime: int=5):
        """
        Thread function to send messages to DUT
        """
        time.sleep(threadwaittime)  # Wait for DUT to be ready to trigger wakeup
        self.power_level = self.getPowerLevel()
        self.deepsleepDevice.triggerWakeup(trigger)
        time.sleep(1)  # Small delay between messages

    def deepsleepReadYaml(self, yaml_path: str) -> dict:
        """
        Reads a YAML file and returns its content as a dictionary.
        Args:
            yaml_path (str): Path to the YAML file
        Returns:
            dict: Content of the YAML file as a dictionary
        """
        with open(yaml_path, "r") as f:
            return yaml.safe_load(f)


    def deepsleepWriteYaml(self, yaml_path: str, data: dict):
        """
        Writes a dictionary to a YAML file.
        Args:
            yaml_path (str): Path to the YAML file
            data (dict): Content to write to the YAML file
        """
        with open(yaml_path, "w") as f:
            yaml.dump(data, f, sort_keys=False, default_flow_style=None)

    def updateNestedDictionaries(self, original: dict, updates: dict):
        """
        Recursively updates nested dictionaries.
        Args:
            original (dict): Original dictionary to be updated
            updates (dict): Dictionary with updates
        """
        for key, value in updates.items():
            if isinstance(value, dict) and key in original:
                self.updateNestedDictionaries(original[key], value)
            else:
                original[key] = value

    def testEndFunction(self, powerOff=True):
        """
        Ends the test case.
        Args:
            powerOff (bool): Whether to power off the device
        """
        super().testEndFunction(powerOff)
