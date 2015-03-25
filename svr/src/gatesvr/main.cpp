#include "GateSvr.h"
#include <net/SocketsOps.h>

int main(int argc, char **argv)
{
    logging::Init(argv[0], "../../log/gatesvr_");

    LOG_INFO << "this thread's tid = " << this_thread::get_id();

#ifdef WIN32    //windows
    if(sockets::SocketStartUp() != 0){  
        LOG_SOCKET_FATAL << "Ì×½Ó×Ö³õÊ¼»¯Ê§°Ü!";
        return -1;
    }
#endif

    const int port = 24;

    EventLoop loop;
    InetAddress listenAddr(port);
    InetAddress peerAddr("127.0.0.1", 23);

    BaseSvr basesvr(&loop, listenAddr);
    basesvr.Start();
    basesvr.ConnectToPeer(peerAddr);
    basesvr.ConnectToDB("tcp://127.0.0.1:3306", "root", "123456", "svr", 5);

    loop.loop();

    system("pause");

    return 0;
}