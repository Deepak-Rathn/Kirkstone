#include "psmHandler_mock.h"
#include <stdbool.h>

using namespace std;

extern psmHandlerMock * g_psmHandlerMock;

extern "C" int PSM_Set_Record_Value2
(
        void*                       bus_handle,
    char const * const          pSubSystemPrefix,
    char const * const          pRecordName,
    unsigned int const          ulRecordType,
    char const * const          pVal

)
{
        if(!g_psmHandlerMock)
        {
                return false;
        }
        return g_psmHandlerMock->PSM_Set_Record_Value2(bus_handle,pSubSystemPrefix,pRecordName,ulRecordType,pVal);
}

extern "C" int PSM_Get_Record_Value2
(
        void*                       bus_handle,
    char const * const          pSubSystemPrefix,
    char const * const          pRecordName,
    unsigned int *              ulRecordType,
    char**                      pValue

)
{
        if(!g_psmHandlerMock)
        {
                return false;
        }
        return g_psmHandlerMock->PSM_Get_Record_Value2(bus_handle,pSubSystemPrefix,pRecordName,ulRecordType,pValue);
}
extern "C" int CcspBaseIf_getParameterValues
(
    void*       bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    char * parameterNames[],
    int size,
    int *val_size,
    parameterValStruct_t ***val

)
{
        if(!g_psmHandlerMock)
        {
                return false;
        }
        return g_psmHandlerMock->CcspBaseIf_getParameterValues(bus_handle,dst_component_id,dbus_path,parameterNames,size,val_size,val);
}
extern "C" int CcspBaseIf_setParameterValues
(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    unsigned int writeID,
    parameterValStruct_t *val,
    int size,
    dbus_bool commit,
    char ** invalidParameterName

)
{
        if(!g_psmHandlerMock)
        {
                return CCSP_FAILURE;
        }
        return g_psmHandlerMock->CcspBaseIf_setParameterValues(bus_handle,dst_component_id,dbus_path,sessionId,writeID,val,size,commit,invalidParameterName);
}

