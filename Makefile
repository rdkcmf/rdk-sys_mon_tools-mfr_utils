##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2016 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################
BIN_FILE := mfr_util
CC      := $(CROSS_COMPILE)-gcc
CXX     := $(CROSS_COMPILE)-g++

#include $(NEXUS_BIN_DIR)/include/platform_app.inc

CFLAGS = -I$(MFR_DIR_PATH)/include -I$(MFR_DIR_PATH) -I$(MFR_DIR_PATH)/broadcomlib/include/

CFLAGS += $(filter-out  -pedantic -Wstrict-prototypes -std=c89, $(NEXUS_CFLAGS)) $(addprefix -I,$(NEXUS_APP_INCLUDE_PATHS)) $(addprefix -D,$(NEXUS_APP_DEFINES)) -I$(NEXUS_TOP)/nxclient/include -I$(FSROOT)/usr/include

LDFLAGS=-Wl,-rpath-link=$(RDK_FSROOT_PATH)/usr/local/lib -Wl,-rpath-link=$(FSROOT)/lib -L$(WORK_DIR)/$(MFR_LIB_PATH) -L${NEXUS_BIN_DIR} -L$(NEXUS_LIB) -lstdc++

GLIBS=-lglib-2.0 -lz 

all: clean sys_mfr_utils.c
	@echo "Building mfr utility ....."
	$(CC) $(CFLAGS) $(LDFLAGS) $(ADDED_LDFLAGS) $(GLIBS) $(MFR_LIB_NAME) -lpthread sys_mfr_utils.c -o $(BIN_FILE)

clean:
	@rm -rf sys_mfr_utils.o
	@rm -rf $(BIN_FILE)

install: all
	@echo "Copying $BIN_FILE to $(FSROOT)/usr/bin ..."
	@cp -f $(BIN_FILE) $(FSROOT)/usr/bin

uninstall:
	@echo "Remove manufacturer utility from $(FSROOT)/usr/bin..."
	@rm -f $(FSROOT)/usr/bin/$(BIN_FILE)
