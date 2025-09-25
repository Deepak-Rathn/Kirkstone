#ifndef CELLMGRFIXTURE_H
#define CELLMGRFIXTURE_H

#include "gtest/gtest.h"
#include "mocks/mock_SMPThread.h"
#include "mocks/psmHandler_mock.h"
#include "mocks/rbusevent_mock.h"

class CellularManagerTestFixture : public ::testing::Test {
  protected:
        SMPThreadMock mockedSMPThread;
        psmHandlerMock mockedpsmcfg;
        rbusEventMock mockedrbusevent;

        CellularManagerTestFixture();
        virtual ~CellularManagerTestFixture();
        virtual void SetUp() override;
        virtual void TearDown() override;

        void TestBody() override;
};

#endif

