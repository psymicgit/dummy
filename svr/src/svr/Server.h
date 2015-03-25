//************************************  
// 函数名称: $SymbolName$  
// 函数说明：  
// 作 成 者：Mr.M  
// 作成日期：$DATE$  
// 返 回 值: $SymbolType$  
// 参    数: $MethodArg$  
//************************************  

// 服务器的基类，各类服务器均从此派生

#ifndef Server_h__
#define Server_h__


#include <vector>

using namespace std;

class Client;
typedef vector<Client*> ClientVec;

class Thread;

class Server
{
public:
    void Init();

    void StartUp();

    void Run(){}

    void Terminate();

private:
    ClientVec m_clients;

    Thread *m_pWorker;
};
#endif // Server_h__