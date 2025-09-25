#ifndef MOCK_PSM_H
#define MOCK_PSM_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C"
{
#include "ccsp_base_api.h"
}

class psmHandlerInterface {
public:
        virtual ~psmHandlerInterface() {}
        virtual int PSM_Set_Record_Value2(void* ,char const * const ,char const * const ,unsigned int const ,char const * const ) = 0;
        virtual int PSM_Get_Record_Value2(void* ,char const * const ,char const * const ,unsigned int * ,char** ) = 0;
        virtual int CcspBaseIf_getParameterValues(void*, const char*, char*, char **, int, int *, parameterValStruct_t ***) = 0;
        virtual int CcspBaseIf_setParameterValues(void* ,const char* ,char* ,int ,unsigned int, parameterValStruct_t*,int ,dbus_bool,char **) = 0;
};

class psmHandlerMock: public psmHandlerInterface {
public:
        virtual ~psmHandlerMock() {}
        MOCK_METHOD5(PSM_Set_Record_Value2 , int(void* ,char const * const ,char const * const ,unsigned int const ,char const * const));
        MOCK_METHOD5(PSM_Get_Record_Value2 , int(void* ,char const * const ,char const * const ,unsigned int * ,char** ));
        MOCK_METHOD7(CcspBaseIf_getParameterValues , int(void*, const char*, char*, char **, int, int *, parameterValStruct_t ***));
        MOCK_METHOD9(CcspBaseIf_setParameterValues , int(void* ,const char* ,char* ,int ,unsigned int, parameterValStruct_t*,int ,dbus_bool,char **));
};

#endif

