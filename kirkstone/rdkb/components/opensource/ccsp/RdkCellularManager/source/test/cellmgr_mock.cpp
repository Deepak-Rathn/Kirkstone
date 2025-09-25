#include <gmock/gmock.h>
#include "cellmgr_mock.h"

#ifndef CELLMGRFIXTURE_G_MOCK
#define CELLMGRFIXTURE_G_MOCK

SMPThreadMock * g_SMPThreadMock = nullptr;
psmHandlerMock * g_psmHandlerMock = nullptr;
rbusEventMock * g_rbusEventMock = nullptr;

CellularManagerTestFixture::CellularManagerTestFixture()
{
    g_SMPThreadMock = new SMPThreadMock;
    g_psmHandlerMock = new psmHandlerMock;
    g_rbusEventMock = new rbusEventMock;
}

CellularManagerTestFixture::~CellularManagerTestFixture()
{
    delete g_SMPThreadMock;
    delete g_psmHandlerMock;
    delete g_rbusEventMock;

    g_SMPThreadMock = nullptr;
    g_psmHandlerMock = nullptr;
    g_rbusEventMock = nullptr;
}

void CellularManagerTestFixture::SetUp()
{
}
void CellularManagerTestFixture::TearDown() {}
void CellularManagerTestFixture::TestBody() {}

#endif

