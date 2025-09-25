#ifndef MOCK_RBUS_H
#define MOCK_RBUS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <rbus.h>
#include "ansc_platform.h"

class rbusEventInterface {
public:
        virtual ~rbusEventInterface() {}
        virtual ANSC_STATUS CellularMgr_Rbus_String_EventPublish_OnValueChange(char *, void *, void *, rbusValueType_t) = 0;
};

class rbusEventMock: public rbusEventInterface {
public:
        virtual ~rbusEventMock() {}
        MOCK_METHOD(ANSC_STATUS, CellularMgr_Rbus_String_EventPublish_OnValueChange, (char *, void *, void *, rbusValueType_t));
};
#endif

