# DeepSleep L2 Low Level Test Specification and Procedure Documentation

## Table of Contents

- [Overview](#overview)
- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [Definitions](#definitions)
- [References](#references)
- [Level 2 Test Procedure](#level-2-test-procedure)

## Overview

This document describes the level 2 testing suite for the DeepSleep module.

### Acronyms, Terms and Abbreviations

- `HAL` \- Hardware Abstraction Layer, may include some common components
- `UT`  \- Unit Test(s)
- `OEM`  \- Original Equipment Manufacture
- `SoC`  \- System on a Chip

### Definitions

- `ut-core` \- Common Testing Framework <https://github.com/rdkcentral/ut-core>, which wraps a open-source framework that can be expanded to the requirements for future framework.

### References

- `High Level Test Specification` - [deepsleep_High-Level_TestSpecification.md](deepsleep_High-Level_TestSpecification.md)

## Level 2 Test Procedure

The following functions are planned as part of L2 testing.

### Test 1

| Title | Details |
| -- | -- |
| Function Name | `test_l2_DeepSleep_getCapabilities` |
| Description | Read supported capabilities using getCapabilities function. |
| Test Group | Module : 03 |
| Test Case ID | 001 |
| Priority | High |

**Pre-Conditions :**
None

**Dependencies :**
None

**User Interaction :**
If user chose to run the test in interactive mode, then the test case has to be selected via console.

**Test Procedure :**

| Variation / Steps     | Description                          | Test Data                                  | Expected Result                               | Notes                          |
|-----------------------|--------------------------------------|--------------------------------------------|-----------------------------------------------|--------------------------------|
| 1.| Obtain IDeepSleep handle            | None                           | Valid handle acquired                | Ensure HAL is initialized      |
| 2.| Get the capabilities using getCapabilities() | caps = valid buffer           | Function returns successfully     | -     |
| 3.| Compare the capabilities with the value in the configuration file      | caps.supportedTriggers = DseepSleep/supportedTriggers caps.preconfiguredTriggers = DeepSleep/preconfiguredTriggers            | Values should match          | -       |

### Test 2

| Title | Details |
| -- | -- |
| Function Name | `test_l2_DeepSleep_setgettimer` |
| Description | Set and get the wakeup time for duration|
| Test Group | Module : 03 |
| Test Case ID | 002 |
| Priority | High |

**Pre-Conditions :**
None

**Dependencies :**
None

**User Interaction :**
If user chose to run the test in interactive mode, then the test case has to be selected via console.

**Test Procedure :**

| Variation / Steps | Description | Test Data | Expected Result | Notes |
|-------------------|-------------|-----------|----------------|-------|
| 1 | Obtain IDeepSleep handle | None | Valid handle acquired | Ensure HAL is initialized  |
| 2 | Set the deep sleep wakeup duration of one second using setWakeUpTimer() | seconds = 1 | Function returns successfully | - |
| 3 | Get the deep sleep wakeup duration of one second using getWakeUpTimer() | None | Timed value set has to be returned | - |
| 4 | Verify the timer values | seconds = getWakeUpTimer() | Timer values should match  | - |
| 5 | Repeat step 2,3,4 for deep sleep wakeup duration of 10 sec | seconds = 10 | Timer values should match | Verify proper set/get of wakeup duration value |

### Test 3

| Title | Details |
| -- | -- |
| Function Name | `test_l2_DeepSleep_enter deepsleep` |
| Description | Set the deep sleep with a duration of ten seconds and verify the wake-up trigger|
| Test Group | Module : 03 |
| Test Case ID | 003 |
| Priority | High |

**Pre-Conditions :**
None

**Dependencies :**
None

**User Interaction :**
If user chose to run the test in interactive mode, then the test case has to be selected via console.

**Test Procedure :**

| Variation / Steps | Description | Test Data | Expected Result | Notes |
|-------------------|-------------|-----------|----------------|-------|
| 1 | Obtain IDeepSleep handle | None | Valid handle acquired | Ensure HAL is initialized  |
| 2 | Set the deep sleep wakeup duration of one second using setWakeUpTimer() | seconds = 10 | Function returns successfully | - |
| 4 | Enter deepsleep with trigger TIMER in triggersToWakeUpon using enterDeepSleep() | triggersToWakeUpon[TIMER] | Function should return in 10 seconds  | DUT should wake from TIMER trigger |
| 5 | Verify the wokeUpByTriggers | wokeUpByTriggers[TIMER] | Value in wokeUpByTriggers should be TIMER | - |
