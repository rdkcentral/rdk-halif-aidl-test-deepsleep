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
import socket

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(dir_path)
sys.path.append(os.path.join(dir_path, "../"))

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utBaseUtils import utBaseUtils
from raft.framework.plugins.ut_raft.utPlaneController import utPlaneController
from deepsleepController.DeepSleepControllerInterface import DeepSleepControllerInterface

class actualDeepSleep(DeepSleepControllerInterface):
    """
    DEEPSLEEP related utility functions
    """
    def __init__(self, device_type:str, session, prompt: str="~#", port:int=8080):
        """
        Initializes the class
        """
        self.port = port
        self.session = session
        self.commandPrompt = prompt
        self.utilities = utBaseUtils()

        self.utPlaneController = utPlaneController(self.session, port=self.port)

    def getDeviceNetworkMACDetails(self):
        """
        Function to get the device network interface details.

        Args:
            None
        Return:
            dict - dictionary containing network interface details
        """
        # Execute the command to list network interfaces and IP addresses
        self.session.write("ip addr show")

        # Fetch the output and errors
        result = self.session.read_until("ip")

        # Parse the output for interface names, IPs, and MAC addresses
        interfaces = {}
        current_interface = None

        for line in result.splitlines():
            if line.startswith(" "):  # Details of the current interface
                if "link/ether" in line:  # MAC address
                    parts = line.split()
                    mac_address = parts[1]
                    if current_interface:
                        interfaces[current_interface]["MAC"] = mac_address
                elif "inet" in line:  # IPv4 address
                    parts = line.split()
                    ip_address = parts[1].split('/')[0]
                    if current_interface:
                        interfaces[current_interface]["IPs"].append(ip_address)
            else:  # Interface line
                parts = line.split(": ")
                if len(parts) > 1:
                    current_interface = parts[1].split("@")[0]
                    interfaces[current_interface] = {"MAC": None, "IPs": []}

        return interfaces

    def wolwowlSendMagicPacket(self, mac_address):
        """
        This function sends a Wake-on-LAN magic packet to a device with the given MAC address

        Args:
            mac_address (str): MAC address of the device.
        Return:
            None
        """
        # Format MAC address (remove colons or dashes)
        mac_address = mac_address.replace(":", "").replace("-", "")

        if len(mac_address) != 12:
            self.log.fatal("Invalid mac address")

        # Create the magic packet (6 x FF followed by 16 x MAC address)
        magic_packet = bytes.fromhex("FF" * 6 + mac_address * 16)

        # Send the packet to the broadcast address
        broadcast_address = ("255.255.255.255", 9)
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
            sock.sendto(magic_packet, broadcast_address)

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
        if manual == True:
            self.testUserResponse.getUserYN(f"Please trigger wake up via PowerKey and Press Enter:")
        else:
            #TODO: Add automation wakeup and verification methods
            return False
        
        return True

    def wakeOnIR(self, manual=False):
        """
        Function to verify whether the device woke up after receiving an IR signal.

        Args:
            manual (bool, optional): Flag to indicate if manual verification should be used.
                                     Defaults to False for automation, True for manual.
        Return:
            bool: True if the device successfully wakes up, False otherwise.
        """
        if manual == True:
            self.testUserResponse.getUserYN(f"Please trigger wake up via IR and Press Enter:")
        else:
            #TODO: Add automation wakeup and verification methods
            return False

        return True

    def wakeOnBT(self, manual=False):
        """
        Function to verify whether the device woke up after receiving an BT signal.

        Args:
            manual (bool, optional): Flag to indicate if manual verification should be used.
                                     Defaults to False for automation, True for manual.
        Return:
            bool: True if the device successfully wakes up, False otherwise.
        """
        if manual == True:
            self.testUserResponse.getUserYN(f"Please trigger wake up via BT and Press Enter:")
        else:
            #TODO: Add automation wakeup and verification methods
            return False

        return True

    def wakeOnCEC(self, manual=False):
        """
        Function to verify whether the device woke up after receiving a CEC signal.

        Args:
            manual (bool, optional): Flag to indicate if manual verification should be used.
                                     Defaults to False for automation, True for manual.
        Return:
            bool: True if the device successfully wakes up, False otherwise.
        """
        if manual == True:
            self.testUserResponse.getUserYN(f"Please trigger wake up via CEC and Press Enter:")
        else:
            #TODO: Add automation wakeup and verification methods
            return False

        return self.isDeviceAwake()

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
        if manual == True:
            self.testUserResponse.getUserYN(f"Please trigger wake up via LAN and Press Enter:")
        else:
            # Wait for the device to go to deep sleep
            time.sleep(30)

            self.log.step(f"Send magic packet to the device to wakeup")

            self.wolwowlSendMagicPacket(mac)

            # Wait for the device to go to wakeup
            time.sleep(5)

        return self.isDeviceAwake()

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
        if manual == True:
            self.testUserResponse.getUserYN(f"Please trigger wake up via WIFI and Press Enter:")
        else:
            # Wait for the device to go to deep sleep
            time.sleep(30)

            self.log.step(f"Send magic packet to the device to wakeup")

            self.wolwowlSendMagicPacket(mac)

            # Wait for the device to go to wakeup
            time.sleep(5)

        return True

    def triggerWakeup(self, trigger: str) -> bool:
        """
        This function sends a specified trigger.

        Args:
          trigger (str): trigger to be sent for wakeup

        Returns:
          bool: True if the message was sent successfully, False otherwise.
        """
        interfaces = self.getDeviceNetworkMACDetails()
        # Not included TIMER as tested in L2
        if trigger == "RCU_IR":
            self.wakeOnIR(True)
        elif trigger == "RCU_BT":
            self.wakeOnBT(True)
        elif trigger == "RCU_RF4CE":
            # Wake on RCU_RF4CE
            # Not supported in this version
            pass
        elif trigger == "LAN":
            # Wake on LAN
                eth0Interface = interfaces.get("eth0")
                if eth0Interface:
                    eth0MAC = eth0Interface.get("MAC")
                    if eth0MAC:
                        result = self.wakeOnLAN(eth0MAC)
        elif trigger == "WLAN":
            # Wake on Wifi
                wlan0Interface = interfaces.get("wlan0")
                if wlan0Interface:
                    wlan0MAC = wlan0Interface.get("MAC")
                    if wlan0MAC:
                        result = self.wakeOnWIFI(wlan0MAC)
        elif trigger == "FRONT_PANEL":
            result = self.wakeOnPowerkey(True)
        elif trigger == "CEC":
            result = self.wakeOnCEC(True)
        elif trigger == "PRESENCE":
            # Wake on Presence detection
            # Not supported in this version
            pass
        elif trigger == "VOICE":
            # Wake on VOICE
            # Not supported in this version
            pass
        else:
            print("Unknown trigger\n")

        return True