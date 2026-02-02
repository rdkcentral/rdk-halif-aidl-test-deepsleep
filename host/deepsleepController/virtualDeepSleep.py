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
import re
import yaml

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(dir_path)
sys.path.append(os.path.join(dir_path, "../"))

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utBaseUtils import utBaseUtils
#from raft.framework.plugins.ut_raft.utPlaneController import utPlaneController
from deepsleepController.DeepSleepControllerInterface import DeepSleepControllerInterface

class virtualDeepSleep(DeepSleepControllerInterface):
    """
    DEEPSLEEP related utility functions
    """
    def __init__(self, session, process_name: str="", bin_path: str="", vcomponentProfile:str="", prompt: str="~#", port:int=8080):
        """
        Initializes the class
        """
        self.port = port
        self.session = session
        self.commandPrompt = prompt
        self.utilities = utBaseUtils()

        self.deepsleepVcomponentConfigFile = os.path.normpath(os.path.join(dir_path, vcomponentProfile))
        self.deepsleepVcomponentConfigFile = self.deepsleepVcomponentConfigFile.replace("\\", "/")
        # Load the HDMI CEC device network configuration file
        with open(self.deepsleepVcomponentConfigFile, "r") as f:
            config_dict = yaml.safe_load(f)

        deepsleepVcomponentConfigString = yaml.dump(config_dict)
        deepsleepVcomponentConfigString = deepsleepVcomponentConfigString.replace('"', '\\"')
        #self.utPlaneController = utPlaneController(self.session, port=self.port)
        self.loadDeepsleepDeviceNetworkConfiguration(deepsleepVcomponentConfigString)

    def loadDeepsleepDeviceNetworkConfiguration(self, configString: str):
        """
        Loads the Deepsleep device network configuration file on to the vComponent.
        """

        #self.utPlaneController.sendMessage(configString)
        cmd = f'curl -X POST -H "Content-Type: application/x-yaml" --data-binary "{configString}" "http://localhost:{self.port}/api/postKVP"'
        self.session.write(cmd)

    def wakeOnTimer(self, timeOut:int):
        """
        Function to verify whether the device woke up after a timeout.

        Args:
            timeOut (int) : Time out value in seconds.
        """
        pass

    def wakeOnPowerkey(self, manual=False):
        """
        Function to verify whether the device woke up after pressing the PowerKey.

        Args:
            manual (bool, optional): Flag to indicate if manual verification should be used.
                                     Defaults to False for automation, True for manual.
        Return:
            bool: True if the device successfully wakes up, False otherwise.
        """
        pass

    def wakeOnIR(self, manual=False):
        """
        Function to verify whether the device woke up after receiving an IR signal.

        Args:
            manual (bool, optional): Flag to indicate if manual verification should be used.
                                     Defaults to False for automation, True for manual.
        Return:
            bool: True if the device successfully wakes up, False otherwise.
        """
        pass

    def wakeOnBT(self, manual=False):
        """
        Function to verify whether the device woke up after receiving an BT signal.

        Args:
            manual (bool, optional): Flag to indicate if manual verification should be used.
                                     Defaults to False for automation, True for manual.
        Return:
            bool: True if the device successfully wakes up, False otherwise.
        """
        pass

    def wakeOnCEC(self, manual=False):
        """
        Function to verify whether the device woke up after receiving a CEC signal.

        Args:
            manual (bool, optional): Flag to indicate if manual verification should be used.
                                     Defaults to False for automation, True for manual.
        Return:
            bool: True if the device successfully wakes up, False otherwise.
        """
        pass

    def wakeOnLAN(self, mac:str, manual=False):
        """
        Function to verify whether the device woke up after a LAN event.

        Args:
            mac (str) : MAC address of the device
            manual (bool, optional): Flag to indicate if manual verification should be used.
                                     Defaults to False for automation, True for manual.
        Return:
            bool: True if the device successfully wakes up, False otherwise.
        """
        pass

    def wakeOnWIFI(self, mac:str, manual=False):
        """
        Function to verify whether the device woke up after a Wi-Fi event.

        Args:
            mac (str) : MAC address of the device
            manual (bool, optional): Flag to indicate if manual verification should be used.
                                     Defaults to False for automation, True for manual.
        Return:
            bool: True if the device successfully wakes up, False otherwise.
        """
        pass

    def triggerWakeup(self, trigger: str) -> bool:
        """
        This function sends a specified trigger.

        Args:
          trigger (str): trigger to be sent for wakeup

        Returns:
          bool: True if the message was sent successfully, False otherwise.
        """
        yaml_content = ()
        # Not included TIMER as tested in L2
        if trigger == "RCU_IR":
            yaml_content = (
                "deepsleep:\n"
                "  command: \"DEEPSLEEP_WAKEUP\"\n"
                "  trigger: RCU_IR\n"
                "  keycode: 6\n"
            )
        elif trigger == "RCU_BT":
            yaml_content = (
                "deepsleep:\n"
                "  command: \"DEEPSLEEP_WAKEUP\"\n"
                "  trigger: RCU_BT\n"
                "  keycode: 7\n"
            )
        elif trigger == "RCU_RF4CE":
            yaml_content = (
                "deepsleep:\n"
                "  command: \"DEEPSLEEP_WAKEUP\"\n"
                "  trigger: RCU_RF4CE\n"
                "  keycode: 8\n"
            )
        elif trigger == "LAN":
            yaml_content = (
                "deepsleep:\n"
                "  command: \"DEEPSLEEP_WAKEUP\"\n"
                "  trigger: LAN\n"
            )
        elif trigger == "WLAN":
            yaml_content = (
                "deepsleep:\n"
                "  command: \"DEEPSLEEP_WAKEUP\"\n"
                "  trigger: WLAN\n"
            )
        elif trigger == "FRONT_PANEL":
            yaml_content = (
                "deepsleep:\n"
                "  command: \"DEEPSLEEP_WAKEUP\"\n"
                "  trigger: FRONT_PANEL\n"
            )
        elif trigger == "CEC":
            yaml_content = (
                "deepsleep:\n"
                "  command: \"DEEPSLEEP_WAKEUP\"\n"
                "  trigger: CEC\n"
            )
        elif trigger == "PRESENCE":
            yaml_content = (
                "deepsleep:\n"
                "  command: \"DEEPSLEEP_WAKEUP\"\n"
                "  trigger: PRESENCE\n"
            )
        elif trigger == "VOICE":
            yaml_content = (
                "deepsleep:\n"
                "  command: \"DEEPSLEEP_WAKEUP\"\n"
                "  trigger: VOICE\n"
            )
        else:
            print("Unknown trigger\n")

        # Send the command to ut-controller
        #self.utPlaneController.sendMessage(yaml_content)
        # Prepare the curl command
        cmd = f'curl -X POST -H "Content-Type: application/x-yaml" --data-binary "{yaml_content}" "http://localhost:{self.port}/api/postKVP"'
        self.session.write(cmd)

        return True
