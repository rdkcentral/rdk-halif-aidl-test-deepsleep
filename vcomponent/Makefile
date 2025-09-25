#* ******************************************************************************
#*  If not stated otherwise in this file or this component's LICENSE
#*  file the following copyright and licenses apply:
#*
#*  Copyright 2025 RDK Management
#*
#*  Licensed under the Apache License, Version 2.0 (the License);
#*  you may not use this file except in compliance with the License.
#*  You may obtain a copy of the License at
#*
#*  http://www.apache.org/licenses/LICENSE-2.0
#*
#*  Unless required by applicable law or agreed to in writing, software
#*  distributed under the License is distributed on an AS IS BASIS,
#*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#*  See the License for the specific language governing permissions and
#*  limitations under the License.
#*
#* ******************************************************************************

.PHONY: all

AIDL_LIB_NAME := aidl_client
UT_LIB_NAME := ut_control
VCOMPONENT_DEEPSLEEP := VComponentDeepSleepService

BUILD_DIR ?= $(CURDIR)/../build
AIDL_GEN_DIR ?= $(BUILD_DIR)/current
UT_CORE_DIR ?= $(CURDIR)/../ut_core
GTEST_INC_DIR ?= $(UT_CORE_DIR)/framework/gtest/linux/googletest-1.15.2/googletest/include

VCOMPONENT_DEEPSLEEP_DIR ?= $(CURDIR)/src

VCOMPONENT_DEEPSLEEP_SRC := $(VCOMPONENT_DEEPSLEEP_DIR)/vcomponent_DeepSleep_main.cpp
VCOMPONENT_DEEPSLEEP_SRC += $(VCOMPONENT_DEEPSLEEP_DIR)/vcomponent_DeepSleep.cpp
VCOMPONENT_DEEPSLEEP_SRC += $(VCOMPONENT_DEEPSLEEP_DIR)/vcomponent_DeepSleep_init.cpp
VCOMPONENT_DEEPSLEEP_SRC += $(VCOMPONENT_DEEPSLEEP_DIR)/vcomponent_DeepSleep_parse_config.cpp

VCOMPONENT_DEEPSLEEP_INC_FLAGS := -I$(AIDL_GEN_DIR)/h
VCOMPONENT_DEEPSLEEP_INC_FLAGS += -I$(BUILD_DIR)/usr/include
VCOMPONENT_DEEPSLEEP_INC_FLAGS += -I$(UT_CORE_DIR)/include
VCOMPONENT_DEEPSLEEP_INC_FLAGS += -I$(UT_CORE_DIR)/include -I$(UT_CORE_DIR)/framework/ut-control/include -I$(GTEST_INC_DIR)

VCOMPONENT_DEEPSLEEP_LDFLAGS := -L. -L$(BUILD_DIR)/usr/lib -L$(BUILD_DIR)/bin
VCOMPONENT_DEEPSLEEP_LD_LIBS += -lbinder -lutils -lpthread -l$(AIDL_LIB_NAME) -l$(UT_LIB_NAME)
VCOMPONENT_DEEPSLEEP_LDFLAGS += -Wl,-rpath-link=$(BUILD_DIR)/usr/lib -Wl,-rpath,.

VCOMPONENT_DEEPSLEEP_CFLAGS := -Wall -Wextra -std=c++17 -ggdb

VCOMPONENT_DEEPSLEEP := $(BUILD_DIR)/bin/$(VCOMPONENT_DEEPSLEEP)

all: $(VCOMPONENT_DEEPSLEEP)

$(VCOMPONENT_DEEPSLEEP): $(VCOMPONENT_DEEPSLEEP_SRC:%.cpp=%.o)
	g++ $(VCOMPONENT_DEEPSLEEP_LDFLAGS) \
		$(VCOMPONENT_DEEPSLEEP_SRC:%.cpp=%.o) -o $@ \
		$(VCOMPONENT_DEEPSLEEP_LD_LIBS)

%.o: %.cpp
	g++ $(VCOMPONENT_DEEPSLEEP_CFLAGS) $(VCOMPONENT_DEEPSLEEP_INC_FLAGS) -fPIC -c $< -o $@