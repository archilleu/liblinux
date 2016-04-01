//---------------------------------------------------------------------------
#ifndef LINUX_BASE_TEST_TEST_JSON_H_
#define LINUX_BASE_TEST_TEST_JSON_H_
//---------------------------------------------------------------------------
#include "test_base.h"
//---------------------------------------------------------------------------
namespace base
{

namespace test
{

class TestJson : public TestBase
{
public:
    TestJson()
    {
    }
    virtual ~TestJson()
    {
    }

    virtual bool DoTest();

public:
    bool Test_Value();
    bool Test_CharReader();
    bool Test_TokenReader();
    bool Test_Json();
    bool Test_Normal();
};

}//namespace test

}//namespace base
//---------------------------------------------------------------------------
#endif //LINUX_BASE_TEST_TEST_JSON_H_
