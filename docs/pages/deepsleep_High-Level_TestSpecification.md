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
- `RTC`    - Real-Time Clock

## Introduction

This document provides an overview of the testing requirements for the Deepsleep module. It outlines the scope of testing, objectives, testing levels and approaches, specific test requirements, and expected deliverables.

- Deepsleep Interface Definitions - [Deepsleep HAL](https://github.com/rdkcentral/rdk-halif-aidl/tree/main/deepsleep/current/com/rdk/hal/deepsleep)
- Deepsleep `HAL` Feature Profile - [hfp-deepsleep.yaml](https://github.com/rdkcentral/rdk-halif-aidl/blob/main/deepsleep/current/hfp-deepsleep.yaml)

## Module Description

The Deepsleep `HAL` provides an interface to enable devices to enter a low-power standby state with support for configurable wake-up triggers such as timers, RCU (IR/BT/RF4CE), frontpanel, motion detection, network activity, and voice commands.

The Deepsleep HAL also provides APIs to set and get wake-up timers.

## L2 Test Specification

|Test|Description|HAL APIs|
|----|-----------|--------|
|Enter Deep Sleep with TIMER Triggers|<ul><li>Pass TIMER triggers to `enterDeepSleep()` and validate that device returns the TIMER in `wokeUpByTriggers` param</li><li>Validate if the system clock has been adjusted to closely represent the actual elapsed time spent in deep sleep</li>|`enterDeepSleep()`|
|Set and Get Deepsleep wakeup timer|Set the timer with various values and validate the same using get calls|`setWakeUpTimer()` `getWakeUpTimer()`|

## L3 Test Specification

### Prerequisites

The platform should have all relevant hardware interfaces (IR, BT, CEC, LAN, etc.) functional and validated through L1 and L2 testing.

### Feature List

VTS L3 validates the deepsleep feature for all supported wakeup triggers and also combination of wakeup triggers.

The following table outlines the core features validated in the L3 test phase

|#|Testing Feature|Description|Expected Scenrio|
|-|---------------|-----------|----------------|
|1|Verify the deepsleep wake up from supported triggers| <ul><li>Get the supported wakeup triggers</li><li>Initiate deepsleep, and trigger wake up from available `supportedTriggers[]` excluding TIMER trigger. </li><li>Enum `WakeUpTrigger` represents the possible wakeup triggers</li><li>validate the wake up trigger received in `wokeUpByTriggers`</li><li>Validate power consuption when device is in deepsleep against the value in yaml file</li>|Device should wake up from trigger specified in `triggersToWakeUpon[]`|
|2|Verify no deepsleep wake up when triggered from unspecified triggers| Initiate deepsleep from a trigger which is not specified in `triggersToWakeUpon[]` param and not available in pre-configured triggers |Device should not wake up|
|3|Verify the deepsleep wake up from pre-configured triggers| <ul><li>Initiate deepsleep without specifying the triggers in `triggersToWakeUpon[]`, and trigger wake up from any of `preconfiguredTriggers[]`</li><li>validate the wake up trigger received in `wokeUpByTriggers`</li><li>Validate power consuption when device is in deepsleep against the value in yaml file</li>|Wake on any of `preconfiguredTriggers[]`|
|4|Verify the deepsleep wake up with mixed triggers| <ul><li>Initiate deepsleep by specifying multiple triggers in `triggersToWakeUpon[]`, and trigger wake up from any of the specified trigger</li><li>validate the wake up trigger received in `wokeUpByTriggers`</li><li>Validate power consuption when device is in deepsleep against the value in yaml file</li>| Device should wake up on any of the specified triggers|
