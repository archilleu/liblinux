//---------------------------------------------------------------------------
#ifndef LINUX_NET_TEST_TCP_CONNECTION_H_
#define LINUX_NET_TEST_TCP_CONNECTION_H_
//---------------------------------------------------------------------------
#include "test_base.h"
#include "../callback.h"
#include "../buffer.h"
//---------------------------------------------------------------------------
namespace net
{

namespace test
{

class TestTCPConnection : public TestBase
{
public:
    TestTCPConnection()
    {
    }
    virtual ~TestTCPConnection()
    {
    }

    virtual bool DoTest();

public:
    bool Test_Illegal();
    bool Test_Normal();
    bool Test_MultiThread();

private:
    void OnConnection   (const TCPConnectionPtr& conn_ptr);
    void OnDisconnection(const TCPConnectionPtr& conn_ptr);
    void OnRead         (const TCPConnectionPtr& conn_ptr, Buffer& rbuffer);

private:
    void    ConnectionAdd           (const TCPConnectionPtr& conn_ptr);
    void    ConnectionDel           (const TCPConnectionPtr& conn_ptr);
    size_t  ConnectionNums          ();
    void    OnConnectionRandomDel   (const TCPConnectionPtr& conn_ptr);

    void    Notify();
    void    Close();

private:
    std::set<TCPConnectionPtr>  tcp_connection_set_;
    std::mutex                  mutex_;
};

}

}//namespace net
//---------------------------------------------------------------------------
#endif //LINUX_NET_TEST_TCP_CONNECTION_H_
