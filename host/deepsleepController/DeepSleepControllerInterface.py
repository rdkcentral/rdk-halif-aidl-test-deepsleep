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


from abc import ABCMeta, abstractmethod

class DeepSleepControllerInterface(metaclass=ABCMeta):
    """
    Abstract base class defining DEEPSLEEP interface.
    Both virtual and actual DEEPSLEEP implementations must inherit this.
    """

    def __init__(self, session, process_name: str="", bin_path: str="", prompt: str = "~#", port: int = 8080):
        self.session = session
        self.bin_path = bin_path
        self.process_name = process_name
        self.prompt = prompt
        self.port = port

    @abstractmethod
    def wakeOnTimer(self, timeOut:int):
        """
        Function to verify whether the device woke up after a timeout.

        Args:
            timeOut (int) : Time out value in seconds.
        """
        pass

    @abstractmethod
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

    @abstractmethod
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

    @abstractmethod
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

    @abstractmethod
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

    @abstractmethod
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

    @abstractmethod
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