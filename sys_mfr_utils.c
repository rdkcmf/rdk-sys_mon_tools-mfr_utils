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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mfr_types.h"

#if defined(YOCTO_BUILD)
    #include "mfrTypes.h"
#endif

#if defined(USE_PACE_MFR)
    #include "pace_mfr_api.h"
    #include "pace_mfr_nvram_api.h"
    #include "pace_DFAST2_prod_gfe_ext_v3.2.h"
    #include "pace_DFAST2_prod_header_v3.2.h"
#endif

#include "sys_mfr_utils.h"

#define NVRAM_TEST_SIZE     128

#if defined(YOCTO_BUILD)
    const char* validParams[] = {"--CurrentImageFilename", "--FlashedFilename","--PDRIVersion"};
    const int numberOfParams = 3;
#else
    const char* validParams[] = {"--CurrentImageFilename", "--FlashedFilename"};
    const int numberOfParams = 2;
#endif

void displayHelp() {
     printf("Usage : mfr_util [CMD] \n");
     printf("CMDs are \n" );
     printf("%5s -> %s \n","--help", "print this help.");
     printf("%5s -> %s \n","--CurrentImageFilename", "Get current running imagename ");
     printf("%5s -> %s \n","--FlashedFilename", "Get current flashed imagename ");
#if defined(YOCTO_BUILD)
     printf("%5s -> %s \n","--PDRIVersion", "Get current PDRIVersion ");
#endif

}

/**
   Return the index of parameter to be retrived if valid.
   If not valid return -1 and display the help screen
**/
int validateParams(const char* param) {
    int paramIndex = -1 ;
    int i = 0 ;
    for ( i=0; i < numberOfParams; i++ ) {
        if (strcmp(param, validParams[i]) == 0 ) {
            paramIndex = i ;
            break ;
        }
    }
    return paramIndex;
}

void getCurrentRunningFileName() {

    VL_NORMAL_NVRAM_DATA_TYPE eType = VL_NORMAL_NVRAM_DATA_BOOT_IMAGE_NAME;
    VL_MFR_API_RESULT result = VL_MFR_API_RESULT_FAILED;
    VL_NVRAM_DATA pNvRamData ;
    FILE fp_old = *stdout;  // preserve the original stdout
    // redirect stdout to null to avoid printing debug prints from other modules
    *stdout = *fopen("/dev/null","w");
    #if defined(USE_PACE_MFR)
        pace_mfr_init();
        result = pace_mfr_read_normal_nvram(eType, &pNvRamData);
    #elif defined(USE_INTEL_MFR)
        result = intel_mfr_read_normal_nvram(eType, &pNvRamData);
    #elif defined(USE_CISCO_MFR)
        result = cisco_mfr_read_normal_nvram(eType, &pNvRamData);
    #elif defined(USE_VIVID_MFR)
        result = vivid_mfr_read_normal_nvram(eType, &pNvRamData);
    #elif defined(USE_ARRIS_MFR)
        result = motorola_mfr_read_normal_nvram(eType, &pNvRamData);
    #else
        result = vl_mfr_read_normal_nvram(eType, &pNvRamData);
    #endif

    *stdout=fp_old;  // restore stdout 

    if ( VL_MFR_API_RESULT_SUCCESS == result ) {
         printf("%s\n" , pNvRamData.pData);
    } 

    return ;
}

void getCurrentFlashedFileName() {

    VL_NORMAL_NVRAM_DATA_TYPE eType = VL_NORMAL_NVRAM_DATA_UPGRADE_IMAGE_NAME;
    VL_MFR_API_RESULT result = VL_MFR_API_RESULT_FAILED;
    VL_NVRAM_DATA pNvRamData ;
    FILE fp_old = *stdout;  // preserve the original stdout
    // redirect stdout to null to avoid printing debug prints from other modules
    *stdout = *fopen("/dev/null","w");
    #if defined(USE_PACE_MFR)
        pace_mfr_init();
        result = pace_mfr_read_normal_nvram(eType, &pNvRamData);
    #elif defined(USE_INTEL_MFR)
        result = intel_mfr_read_normal_nvram(eType, &pNvRamData);
    #elif defined(USE_CISCO_MFR)
        result = cisco_mfr_read_normal_nvram(eType, &pNvRamData);
    #elif defined(USE_VIVID_MFR)
        result = vivid_mfr_read_normal_nvram(eType, &pNvRamData);
    #elif defined(USE_ARRIS_MFR)
        result = motorola_mfr_read_normal_nvram(eType, &pNvRamData);
    #else
        result = vl_mfr_read_normal_nvram(eType, &pNvRamData);
    #endif

    *stdout=fp_old;  // restore stdout 

    if ( VL_MFR_API_RESULT_SUCCESS == result ) {
         if ( pNvRamData.pData[0] != '\0' )
             printf("%s\n" , pNvRamData.pData);
         else
             getCurrentRunningFileName();
    }
    return ;
}

#if defined(YOCTO_BUILD)
void getPDRIVersion(){

    mfrSerializedType_t mfr_type;
    mfrError_t status = mfrERR_NONE;
    mfrSerializedData_t data;
    int i;

    mfr_type = mfrSERIALIZED_TYPE_PDRIVERSION;
    status = mfrGetSerializedData(mfr_type,&data);
    if( status != mfrERR_NONE ){
        printf("\n FAILED :%x ",status);
        return;
    }

    if(data.bufLen != 0)
    {
        for(i=0;i < data.bufLen;i++)
            printf("%c",data.buf[i]);
        printf("\n");
    }
}
#endif

int main(int argc, char *argv[])
{

    int paramIndex = 0;

    if (argc != 2) {
        displayHelp();
        return -1;
    }

    paramIndex = validateParams(argv[1]);

    if( validateParams(argv[1]) == -1 ){
        displayHelp();
        return -1;
    }

    switch(paramIndex) {
        /*Check for validParams array for parameter name mapping*/
        case 0 :
            getCurrentRunningFileName();
            break;
        case 1 :
            getCurrentFlashedFileName();
            break;
#if defined(YOCTO_BUILD)            
        case 2 :
            getPDRIVersion();
            break;
#endif
        default :
            displayHelp();
            break;

    }
    return 0;
}

