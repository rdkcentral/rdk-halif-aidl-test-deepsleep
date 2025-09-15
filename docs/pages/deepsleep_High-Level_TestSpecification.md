# Deepsleep High Level Test Specification Document

## Table of Contents

- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [Introduction](#introduction)
- [Module Description](#module-description)
- [L2 Test Specification](#l2-test-specification)
- [L3 Test Specification](#l3-test-specification)


## Acronyms, Terms and Abbreviations

- `HAL`    - Hardware Abstraction Layer
- `API`    - Application Programming Interface
- `AIDL`   - Android Interface Definition Language
- `L2`     - Level 2 Testing
- `L3`     - Level 3 Testing
- `NA`     - Not Applicable
- `Caller` - Any user of the interface via the `APIs`
- `CB`     - Call-back function (suffix)
- `YAML`   - YAML Ain’t Markup Language
- `RCU`    - Remote Control Unit 
- `CEC`    - Consumer Electronics Control
- `RTC`    - Real-Time Clock

## Introduction

This document provides an overview of the testing requirements for the Deepsleep module. It outlines the scope of testing, objectives, testing levels and approaches, specific test requirements, and expected deliverables.

- Deepsleep Interface Definitions - [Deepsleep HAL](https://github.com/rdkcentral/rdk-halif-aidl/tree/v0.6/deepsleep/current/com/rdk/hal/deepsleep)
-   Deeplslep Overview - [Deepsleep Overview](https://wiki.rdkcentral.com/display/RDKHAL/Audio+Sink)

## Module Description

The Deepsleep `HAL` provides an interface to enable devices to enter a low-power standby state with support for configurable wake-up triggers such as timers, RCU (IR/BT), motion detection, network activity, and voice commands.

The Deepsleep HAL also provides APIs to set and get wake-up timers.

## L2 Test Specification

|Test|Description|HAL APIs|
|----|-----------|--------|
|Set Deepsleep Capabilities|<ul><li>Read supported/prconfigured triggers of deepsleep using `IDeepSleep` class</li><li>Validate the list of `supportedTriggers[]` and `preconfiguredTriggers[]` to ensure all values match `hfp-deepsleep.yaml`.| `getCapabilities()`|
|Enter Deep Sleep with TIMER Triggers|Pass TIMER triggers to `enterDeepSleep()` and validate that device enters/exits deep sleep successfully.|`enterDeepSleep()`|
Set and Get Deepsleep wakeup timer|Set the timer with various values and validate the same using get calls|`setWakeUpTimer()` `getWakeUpTimer()`|

## L3 Test Specification

### Prerequisites

The platform should have all relevant hardware interfaces (IR, BT, CEC, LAN, etc.) functional and validated through L1 and L2 testing.

### Feature List

VTS L3 validates the deepsleep feature for all possible wakeup triggers and also combination of wakeup triggers.

The following table outlines the core features validated in the L3 test phase

|#|Testing Feature|Description|
|-|---------------|-----------|
|1|Verify the deepsleep wake up from IR| Trigger deepsleep, and trigger wake up from IR|
|2|Verify the deepsleep wake up from Bluetooth| Trigger deepsleep, and trigger wake up from bluetooth|
|3|Verify the deepsleep wake up from RF4CE| Trigger deepsleep, and trigger wake up from RF4CE|
|4|Verify the deepsleep wake up from LAN|depsleep, and trigger wake up from LAN|
|5|Verify the deepsleep wake up from WLAN| Trigger deepsleep, and trigger wake up from WLAN|
|6|Verify the deepsleep wake up from TIMER|deepsleep, and trigger wake up from TIMER|
|7|Verify the deepsleep wake up from FrontPanel| Trigger deepsleep, and trigger wake up from Power Key|
|8|Verify the deepsleep wake up from CEC| deepsleep, and trigger wake up from CEC|
|9|Verify the deepsleep wake up from Voice|Trigger deepsleep, and trigger wake up from voice up source| 
|10|Verify the deepsleep wake up from Presence Detection| Trigger deepsleep, and trigger wake up from presence detection|
|11|Verify the deepsleep wake up from per-configured triggers| Trigger deepsleep without secifying the trigger list, and trigger wake up from any of pre-configured trigger|
|12|Verify the deepsleep wake up from mixed triggers| Trigger deepsleep, and trigger wake up from multiple trigger| 

### Test Cases

The table below summarizes the L3 tests executed using various wakeup triggers:

|#|Test|TriggerUsed|Timer|KeycodeReturned|Expected Scenrio|
|-|--------------|------|------|---------|----------|
|1|RCU IR Wake|RCU_IR|✘|✔|Wake on IR press with `keyCode`|
|2|RCU Bluetooth Wake|RCU_BT|✘|✔|Wake on BT press with `keyCode`|
|3|RCU RF4CE Wake|RCU_RF4CE|✘|✔|Wake on RF4CE press with `keyCode`|
|4|LAN Wake|LAN|✘|✘|Wake on magic packet|
|5|WLAN Wake|WLAN|✘|✘|Wake on magic packet|
|6|TIMER Wake|TIMER|✔|✘|Wake on TIMER elapsed|
|7|FrontPanel Wake|FRONT_PANEL|✘|✘|Wake on Power key|
|8|CEC Wake|CEC|✘|✘|Wake on CEC signal|
|9|Voice source Wake|VOICE|✘|✘|Wake on voice detection|
|10|Presence detection Wake|PRESENCE|✘|✘|Wake on presence detection|
|11|Pre-configured trigger Wake|None|✘|✘|Wake on preconfigured trigger|
|12|Mixed trigger Wake|RCU_BT, TIMER, FRONT_PANEL|✔|✔|Wake on any specified|
