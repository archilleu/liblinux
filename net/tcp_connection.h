//---------------------------------------------------------------------------
#ifndef BASE_LINUX_NET_TCP_CONNECTION_H_
#define BASE_LINUX_NET_TCP_CONNECTION_H_
//---------------------------------------------------------------------------
#include "../base/share_inc.h"
#include "../base/memory_block.h"
#include "callback.h"
#include "inet_address.h"
#include "buffer.h"
//---------------------------------------------------------------------------
namespace net
{

class EventLoop;
class InetAddress;
class Socket;
class Channel;

class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
public:
    typedef std::function<void (const TCPConnectionPtr&)> CalllbackDestroy;

    TCPConnection(EventLoop* owner_loop, const std::string& name, int fd, const InetAddress& local_addr, const InetAddress& peer_addr);
    ~TCPConnection();

    //各种回调
    void set_callback_connection        (const CallbackConnection& callback)        { callback_connection_      = std::move(callback); }
    void set_callback_disconnection     (const CallbackDisconnection& callback)     { callback_disconnection_   = std::move(callback); }
    void set_callback_destroy           (const CalllbackDestroy& callback)          { callback_destroy_         = std::move(callback); }
    void set_callback_read              (const CallbackRead& callback)              { callback_read_            = std::move(callback); }
    void set_callback_write_complete    (const CallbackWriteComplete& callback)     { callback_write_complete_  = std::move(callback); }
    void set_callback_high_water_mark   (const CallbackWriteHighWaterMark& callback, size_t overstock_size)
    {
        callback_high_water_mark_   = std::move(callback); 
        overstock_size_             = overstock_size;
    }

    //初始化
    void Initialize();

    //发送数据,线程安全
    void Send(const char* dat, size_t len);
    void Send(const base::MemoryBlock& dat);

    //关闭链接
    void Shutdown();

    //强制断线
    void ForceClose();

    //连接就绪和销毁
    void ConnectionEstablished();
    void ConnectionDestroy();

    const std::string   name()          { return name_; }
    const InetAddress&  local_addr()    { return local_addr_; }
    const InetAddress&  peer_addr()     { return peer_addr_; }

    EventLoop*                      owner_loop()    { return owner_loop_; }
    const std::shared_ptr<Socket>   socket()        { return socket_; }

private:
    //如果上面的Send调用不在本io线程中调用,则转换到本线程发送数据,达到线程安全的目的
    void    SendInLoopA             (const base::MemoryBlock dat);
    void    SendInLoopB             (const char* dat, size_t len);
    ssize_t _SendMostPossible       (const char* dat, size_t len);      //尽可能的发送数据
    void    _SendDatQueueInBuffer   (const char* dat, size_t remain);   //未完成发送的数据放入缓存

    //关闭和断开连接都需要在本线程做
    void ShutdownInLoop();
    void ForceCloseInLoop();

    //事件处理
    void HandleRead(base::Timestamp rcv_time);
    void HandleWrite();
    void HandleError();
    void HandleClose();

private:
    EventLoop*  owner_loop_;
    std::string name_;
    InetAddress local_addr_;
    InetAddress peer_addr_;
    bool        connected_;

    Buffer buffer_input_;
    Buffer buffer_output_;

    std::shared_ptr<Socket>     socket_;
    std::shared_ptr<Channel>    channel_;

    CallbackConnection          callback_connection_;
    CallbackDisconnection       callback_disconnection_;
    CalllbackDestroy            callback_destroy_;
    CallbackRead                callback_read_;
    CallbackWriteComplete       callback_write_complete_;
    CallbackWriteHighWaterMark  callback_high_water_mark_;
    size_t                      overstock_size_;
};

}//namespace net
//---------------------------------------------------------------------------
#endif //BASE_LINUX_NET_TCP_CONNECTION_H_
