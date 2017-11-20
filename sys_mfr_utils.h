/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
void getCurrentFlashedFileName(void);
void getCurrentRunningFileName(void);

#if defined(YOCTO_BUILD)
    void getPDRIVersion(void);
#endif

#if defined(USE_SAM_MFR)
    #define IARM_BUS_SAMMFRUTIL_NAME "SamMfrUtil"
#endif

#if defined(USE_CISCOXID_MFR)
    VL_MFR_API_RESULT getFlashFileName(VL_NVRAM_DATA * pNvRamData);
#endif